#include "World.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Input/PlayerInput.h"
#include "Core/Math/Ray.h"
#include "Debug/DebugDrawManager.h"
#include "Object/Actor/Arrow.h"
#include "Object/Actor/Camera.h"
#include "Object/Actor/OrthoGraphicActor.h"
#include "Object/Actor/Cone.h"
#include "Object/Actor/Cube.h"
#include "Object/Actor/Cylinder.h"
#include "Object/Actor/Sphere.h"
#include "Object/Gizmo/GizmoActor.h"
#include "Static/FEditorManager.h"
#include "Static/FLineBatchManager.h"
#include "Static/FUUIDBillBoard.h"
#include "Core/Rendering/FViewport.h"
#include "Object/Actor/StaticMeshObj.h"


void UWorld::InitWorld()
{
	GridSize = FString::ToFloat(UConfigManager::Get().GetValue(TEXT("World"), TEXT("GridSize")));
	ViewportManager = new FViewportManager();

	SSplitterV* TopSplitter = UEngine::Get().GetTopSplitter();
	SSplitterV* BottomSplitter = UEngine::Get().GetBottomSplitter();
	TArray<FRect> Rects;
	Rects.Add(TopSplitter->GetSideLT()->GetRect());
	Rects.Add(TopSplitter->GetSideRB()->GetRect());
	Rects.Add(BottomSplitter->GetSideLT()->GetRect());
	Rects.Add(BottomSplitter->GetSideRB()->GetRect());

	for (FRect Rect : Rects)
	{
		ACamera* orthocamera = SpawnActor<ACamera>();
		orthocamera->ProjectionMode = ECameraProjectionMode::Orthographic;
		ViewportManager->RegisterViewport(new FViewport(new FViewportClient(SpawnActor<ACamera>(), orthocamera) , Rect));
	}

	TArray<FViewport*> Viewports = ViewportManager->GetViewports();

	ViewportManager->SetActiveIndex(std::stoi((UConfigManager::Get().GetValue(TEXT("Viewports"), TEXT("ActiveScreen"))).c_char()));
	ViewportManager->SetActiveViewport(Viewports[ViewportManager->GetActiveIndex()]);

	if (UConfigManager::Get().GetValue(TEXT("Viewports"), TEXT("FullScreen")) == TEXT("true"))
	{
		ViewportManager->SetFullScreenViewport(Viewports[ViewportManager->GetActiveIndex()]);
	}
	
	Viewports[0]->GetClient()->SetLevelViewportType(ELevelViewportType::Top);
	Viewports[1]->GetClient()->SetLevelViewportType(ELevelViewportType::Front);
	Viewports[2]->GetClient()->SetLevelViewportType(ELevelViewportType::Perspective);
	Viewports[3]->GetClient()->SetLevelViewportType(ELevelViewportType::Right);

	Viewports[2]->GetClient()->SetViewMode(EViewModeIndex::VMI_Default);
}

void UWorld::BeginPlay()
{
	for (const auto& Actor : Actors)
	{
		Actor->BeginPlay();
	}

	APlayerInput::Get().RegisterMouseDownCallback(EKeyCode::LButton, [this](const FVector& MouseNDCPos)
	{
		RayCasting(MouseNDCPos);
	}, GetUUID());
}

void UWorld::Tick(float DeltaTime)
{
	for (const auto& Actor : ActorsToSpawn)
	{
		Actor->BeginPlay();
	}
	ActorsToSpawn.Empty();

	const auto CopyActors = Actors;
	for (const auto& Actor : CopyActors)
	{
		if (Actor->CanEverTick())
		{
			Actor->Tick(DeltaTime);
		}
	}

	AActor* PickedActor = FEditorManager::Get().GetSelectedActor();
	AGizmoActor* Gizmo = FEditorManager::Get().GetGizmo();
	if (PickedActor && Gizmo)
		Gizmo->SetActorPosition(PickedActor->GetActorPosition());
}

void UWorld::LateTick(float DeltaTime)
{
	const auto CopyActors = Actors;
	for (const auto& Actor : CopyActors)
	{
		if (Actor->CanEverTick())
		{
			Actor->LateTick(DeltaTime);
		}
	}

	for (const auto& PendingActor : PendingDestroyActors)
	{
		// Engine에서 제거
		UEngine::Get().GObjects.Remove(PendingActor->GetUUID());
	}
	PendingDestroyActors.Empty();

	FViewportManager* ViewportManager = UEngine::Get().GetWorld()->GetViewportManager();
	TArray<FViewport*> Viewports = ViewportManager->GetViewports();

	SSplitterV* TopSplitter = UEngine::Get().GetTopSplitter();
	SSplitterV* BottomSplitter = UEngine::Get().GetBottomSplitter();

	FViewport* FullScreenViewport = ViewportManager->GetFullScreenViewport();
	FViewport* ActiveViewport = ViewportManager->GetActiveViewport();

	Viewports[0]->SetRect(TopSplitter->GetSideLT()->GetRect());
	Viewports[1]->SetRect(TopSplitter->GetSideRB()->GetRect());
	Viewports[2]->SetRect(BottomSplitter->GetSideLT()->GetRect());
	Viewports[3]->SetRect(BottomSplitter->GetSideRB()->GetRect());

	FPoint MousePos = FPoint(APlayerInput::Get().GetMousePos().X, APlayerInput::Get().GetMousePos().Y);
	FVector MouseDeltaPos = APlayerInput::Get().GetMouseDeltaPos();

	int32 index = -1;
	for (FViewport* vp : Viewports)
	{
		index++;
		if (FullScreenViewport && FullScreenViewport != vp)
			continue;

		if (vp->GetRect().Contains(MousePos) &&
			(APlayerInput::Get().GetKeyDown(EKeyCode::LButton) || APlayerInput::Get().GetKeyDown(EKeyCode::RButton)))
		{
			ViewportManager->SetActiveViewport(vp);
			ViewportManager->SetActiveIndex(index);
		}

		ELevelViewportType LevelViewportType = vp->GetClient()->GetLevelViewportType();
		if(LevelViewportType != ELevelViewportType::Perspective)
			vp->GetClient()->GetOrthographicCamera()->SetActorTransform(UEngine::Get().GetWorld()->GetOrthoGraphicActor()->GetTransform(LevelViewportType));

		if (ActiveViewport && ActiveViewport != vp)
			continue;

		if (vp == ViewportManager->GetActiveViewport())
		{
			if (LevelViewportType == ELevelViewportType::Perspective)
			{
				FEditorManager::Get().SetCamera(vp->GetClient()->GetPerspectiveCamera());
				ACamera* PerspectiveCam = vp->GetClient()->GetPerspectiveCamera();
				if (APlayerInput::Get().GetKeyPress(EKeyCode::W)) { PerspectiveCam->MoveForward(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::S)) { PerspectiveCam->MoveBackward(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::A)) { PerspectiveCam->MoveLeft(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::D)) { PerspectiveCam->MoveRight(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::Q)) { PerspectiveCam->MoveDown(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::E)) { PerspectiveCam->MoveUp(); }
				if (APlayerInput::Get().GetKeyPress(EKeyCode::RButton)) { PerspectiveCam->Rotate(MouseDeltaPos); }
			}
			else
			{
				FEditorManager::Get().SetCamera(vp->GetClient()->GetOrthographicCamera());
				if (APlayerInput::Get().GetKeyPress(EKeyCode::RButton))
				{
					AOrthoGraphicActor* Actor = UEngine::Get().GetWorld()->GetOrthoGraphicActor();
					switch (LevelViewportType)
					{
					case ELevelViewportType::Top:
						Actor->MoveTop(MouseDeltaPos);
						break;
					case ELevelViewportType::Bottom:
						Actor->MoveBottom(MouseDeltaPos);
						break;
					case ELevelViewportType::Left:
						Actor->MoveLeft(MouseDeltaPos);
						break;
					case ELevelViewportType::Right:
						Actor->MoveRight(MouseDeltaPos);
						break;
					case ELevelViewportType::Front:
						Actor->MoveFront(MouseDeltaPos);
						break;
					case ELevelViewportType::Back:
						Actor->MoveBack(MouseDeltaPos);
					}
				}
			}
		}
	}
}

void UWorld::OnDestroy()
{
	UConfigManager::Get().SaveConfig("editor.ini");
}

void UWorld::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();

	if (Renderer == nullptr)
	{
		return;
	}

	//if (APlayerInput::Get().GetKeyDown(EKeyCode::LButton))
	//{
	//	RenderPickingTexture(*Renderer);
	//}

	AActor* SelectedActor = FEditorManager::Get().GetSelectedActor();
	if (SelectedActor != nullptr)
	{
		const FVector LocalMax = SelectedActor->GetActorLocalBoundsMax();
		const FVector LocalMin = SelectedActor->GetActorLocalBoundsMin();

		[[maybe_unused]] FVector WorldMax = SelectedActor->GetActorWorldBoundsMax();
		[[maybe_unused]] FVector WorldMin = SelectedActor->GetActorWorldBoundsMin();

		UDebugDrawManager::Get().DrawBoundingBox(LocalMin, LocalMax, SelectedActor->GetActorTransform(), FVector4::RED);
	}
	UDebugDrawManager::Get().Render();

	FLineBatchManager::Get().Render();

	FUUIDBillBoard::Get().Render();

	RenderMainTexture(*Renderer);
	//DisplayPickingTexture(*Renderer);
}

void UWorld::RenderPickingTexture(URenderer& Renderer)
{
	// Renderer.PreparePicking();
	// Renderer.PreparePickingShader();

	for (auto& RenderComponent : RenderComponents)
	{
		if (RenderComponent->GetOwner()->GetDepth() > 0)
		{
			continue;
		}
		// uint32 UUID = RenderComponent->GetUUID();
		// RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
		RenderComponent->Render();
	}

	// Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		
		RenderComponent->Render();
		//MsgBoxAssert("없어진 기능입니다");
		// uint32 UUID = RenderComponent->GetUUID();
		// RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
		// uint32 depth = RenderComponent->GetOwner()->GetDepth();
		// RenderComponent->Render();
	}
}

void UWorld::RenderMainTexture(URenderer& Renderer)
{
	// Renderer.Prepare();
	// Renderer.PrepareShader();
	// Renderer.PrepareMain();

	//Renderer.PrepareMainShader();
	for (auto& RenderComponent : RenderComponents)
	{
		if (RenderComponent->GetOwner()->GetDepth() > 0)
		{
			continue;
		}
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		// RenderComponent->UpdateConstantDepth(Renderer, depth);
		RenderComponent->Render();
	}

	FDevice::Get().PickingPrepare();

	//Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		RenderComponent->RenderGizmo();
	}

	FDevice::Get().SetRenderTargetOnly();
}

// void UWorld::DisplayPickingTexture(URenderer& Renderer)
// {
// 	Renderer.RenderPickingTexture();
// }

void UWorld::ClearWorld()
{
	TArray CopyActors = Actors;
	for (AActor* Actor : CopyActors)
	{
		// if (!Actor->Implements<IGizmoInterface>()) // TODO: RTTI 개선하면 사용
		if (!dynamic_cast<IGizmoInterface*>(Actor))
		{
			DestroyActor(Actor);
		}
	}

	UE_LOG("Clear World");
}

bool UWorld::DestroyActor(AActor* InActor)
{
	// 나중에 Destroy가 실패할 일이 있다면 return false; 하기
	assert(InActor);

	if (InActor->GetWorld() == nullptr)
	{
		return false;
	}

	if (PendingDestroyActors.Find(InActor) != -1)
	{
		return true;
	}

	// 삭제될 때 Destroyed 호출
	InActor->Destroyed();

	// World에서 제거
	Actors.Remove(InActor);

	// 제거 대기열에 추가
	PendingDestroyActors.Add(InActor);
	return true;
}

void UWorld::SaveWorld() const
{
	JsonSaveHelper::SaveScene(GetWorldInfo());
}

void UWorld::AddZIgnoreComponent(UPrimitiveComponent* InComponent)
{
	ZIgnoreRenderComponents.Add(InComponent);
	//InComponent->SetIsOrthoGraphic(true);
}

void UWorld::LoadWorld(const char* InSceneName)
{
	if (InSceneName == nullptr || strcmp(InSceneName, "") == 0){
		return;
	}

	ClearWorld();

	std::unique_ptr<UWorldInfo> WorldInfo = JsonSaveHelper::LoadScene(InSceneName);
	if (WorldInfo == nullptr)
	{
		if (InSceneName == "Default")
		{
			// Create New WorldInfo
			WorldInfo = std::make_unique<UWorldInfo>();
			WorldInfo->Version = 1.0;
			WorldInfo->SceneName = "Default";
			WorldInfo->CameraInfo.Location = FVector(-10.0f, 3.0f, 10.0f);
			WorldInfo->CameraInfo.Rotation = FVector(0.0f, 30.0f, 0.0f);
			WorldInfo->CameraInfo.FieldOfView = 60.0f;
			WorldInfo->CameraInfo.NearClip = 0.1f;
			WorldInfo->CameraInfo.FarClip = 1000.0f;
		}
		else
		{
			return;
		}
	}


	Version = WorldInfo->Version;
	this->SceneName = WorldInfo->SceneName;

	// Type 확인
	while (!WorldInfo->ObjectInfos.empty())
	{
		const std::unique_ptr<UObjectInfo> ObjectInfo = std::move(WorldInfo->ObjectInfos.front());
		WorldInfo->ObjectInfos.pop();

		FTransform Transform = FTransform(ObjectInfo->Location, FQuat(), ObjectInfo->Scale);
		Transform.Rotate(ObjectInfo->Rotation);

		AActor* Actor = nullptr;
		
		if (ObjectInfo->ObjectType == "Actor")
		{
			Actor = SpawnActor<AActor>();
		}
		else if (ObjectInfo->ObjectType == "Sphere")
		{
			Actor = SpawnActor<ASphere>();
		}
		else if (ObjectInfo->ObjectType == "Cube")
		{
			Actor = SpawnActor<ACube>();
		}
		else if (ObjectInfo->ObjectType == "Arrow")
		{
			Actor = SpawnActor<AArrow>();
		}
		else if (ObjectInfo->ObjectType == "Cylinder")
		{
			Actor = SpawnActor<ACylinder>();
		}
		else if (ObjectInfo->ObjectType == "Cone")
		{
			Actor = SpawnActor<ACone>();
		}
		else if (ObjectInfo->ObjectType == "StaticMeshObj")
		{
			Actor = SpawnActor<AStaticMeshObj>();
			dynamic_cast<UStaticMeshComponent*>(Actor->GetRootComponent())->SetStaticMesh(ObjectInfo->ObjStaticMeshName);
		}

		if (Actor)
			Actor->SetActorTransform(Transform);
	}
	Camera = UEngine::Get().GetWorld()->GetViewportManager()->GetActiveViewport()->GetClient()->GetPerspectiveCamera();
	Camera->SetActorTransform(FTransform(WorldInfo->CameraInfo.Location, FQuat(WorldInfo->CameraInfo.Rotation), FVector(1.0f)));
	Camera->SetFieldOfVew(WorldInfo->CameraInfo.FieldOfView);
	Camera->SetNear(WorldInfo->CameraInfo.NearClip);
	Camera->SetFar(WorldInfo->CameraInfo.FarClip);
}

void UWorld::RayCasting(const FVector& MouseNDCPos)
{
	FMatrix ProjMatrix = Camera->GetProjectionMatrix(); 
	FRay worldRay = FRay(Camera->GetViewMatrix(), ProjMatrix, MouseNDCPos.X, MouseNDCPos.Y);

	FLineBatchManager::Get().AddLine(worldRay.GetOrigin(), worldRay.GetDirection() * Camera->GetFar(), FVector4::CYAN);

	AActor* SelectedActor = nullptr;
	float minDistance = FLT_MAX;

	for (auto& Actor : Actors)
	{
		UPrimitiveComponent* PrimitiveComponent = Actor->GetComponentByClass<UPrimitiveComponent>();
		if (PrimitiveComponent == nullptr)
		{
			continue;
		}

		FMatrix primWorldMat = PrimitiveComponent->GetComponentTransform().GetMatrix();
		FRay localRay = FRay::TransformRayToLocal(worldRay, primWorldMat.Inverse());

		//std::shared_ptr<UStaticMesh> CurMesh = PrimitiveComponent->GetMesh();
		//CurMesh->GetVertexBuffer();

		float outT = 0.0f;
		bool bHit = false;

		switch (PrimitiveComponent->GetType())
		{
		case EPrimitiveType::EPT_Cube:
		{
			bHit = FRayCast::IntersectRayAABB(localRay, FVector(-0.5f, -0.5f, -0.5f), FVector(0.5f, 0.5f, 0.5f), outT);
			if (bHit)
			{
				UE_LOG("Cube Hit");
			}
			break;
		}
		case EPrimitiveType::EPT_Sphere:
		{
			bHit = FRayCast::InsertSectRaySphere(localRay, PrimitiveComponent->GetActorPosition(), 0.5f, outT);
			if (bHit)
			{
				UE_LOG("Sphere Hit");
			}
			break;
		}
		case EPrimitiveType::EPT_Cylinder:
		{
			bHit = FRayCast::IntersectRayAABB(localRay, FVector(0, -0.5f, 0), FVector(0, 0.5f, 0), outT);
			if (bHit)
			{
				UE_LOG("Cylinder Hit");
			}
			break;
		}
		case EPrimitiveType::EPT_Cone:
		{
			break;
		}
		case EPrimitiveType::EPT_Triangle:
		{
			break; 
		}
		default:
			break;
		}
		// 4. 교차가 발생했다면, 월드 좌표에서의 거리를 비교하여 최소 거리를 가진 액터 선택
		if (bHit)
		{
			float distance = worldRay.GetPoint(outT).Length();
			if (distance < minDistance)
			{
				minDistance = distance;
				SelectedActor = Actor;
				FUUIDBillBoard::Get().SetTarget(SelectedActor);
			}
		}
	}

	if (SelectedActor)
	{
		FEditorManager::Get().SelectActor(SelectedActor);
	}
}

void UWorld::PickByPixel(const FVector& MousePos)
{

}

void UWorld::OnChangedGridSize() const
{
	UConfigManager::Get().SetValue(TEXT("World"), TEXT("GridSize"), FString::SanitizeFloat(GridSize));
	FLineBatchManager::Get().DrawWorldGrid(GridSize, GridSize/100.f);
}

UWorldInfo UWorld::GetWorldInfo() const
{
	UWorldInfo WorldInfo;
	WorldInfo.ActorCount = Actors.Num();
	WorldInfo.SceneName = std::string(SceneName.c_char());
	WorldInfo.Version = 1;

	uint32 i = 0;
	for (auto& actor : Actors)
	{
		// if (actor->Implements<IGizmoInterface>()) // TODO: RTTI 개선하면 사용
		if (dynamic_cast<IGizmoInterface*>(actor))
		{
			WorldInfo.ActorCount--;
			continue;
		}
		WorldInfo.ObjectInfos.push(std::make_unique<UObjectInfo>(
			UObjectInfo{
				.Location = actor->GetActorPosition(),
				.Rotation = actor->GetActorRotation(),
				.Scale = actor->GetActorScale(),
				.ObjStaticMeshName = dynamic_cast<AStaticMeshObj*>(actor) ? dynamic_cast<UStaticMeshComponent*>(dynamic_cast<AStaticMeshObj*>(actor)->GetRootComponent())->GetStaticMesh()->GetStaticMeshAsset()->Name : "",
				//.ObjStaticMeshAssetPath = dynamic_cast<AStaticMeshObj*>(actor) ? dynamic_cast<UStaticMeshComponent*>(dynamic_cast<AStaticMeshObj*>(actor)->GetRootComponent())->GetStaticMesh()->GetStaticMeshAsset()->PathFileName : "",
				.ObjectType = actor->GetTypeName().c_char(),
				.UUID = actor->GetUUID()
			}
		));
		i++;
	}

	WorldInfo.CameraInfo.Location = Camera->GetActorPosition();
	WorldInfo.CameraInfo.Rotation = Camera->GetActorRotation();
	WorldInfo.CameraInfo.FieldOfView = Camera->GetFieldOfView();
	WorldInfo.CameraInfo.NearClip = Camera->GetNear();
	WorldInfo.CameraInfo.FarClip = Camera->GetFar();

	return WorldInfo;
}