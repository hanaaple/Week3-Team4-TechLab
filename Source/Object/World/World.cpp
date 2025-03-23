#include "World.h"

#include "Core/Config/ConfigManager.h"
#include "Core/Input/PlayerInput.h"
#include "Core/Math/Ray.h"
#include "Debug/DebugDrawManager.h"
#include "Object/Actor/Arrow.h"
#include "Object/Actor/Camera.h"
#include "Object/Actor/Cone.h"
#include "Object/Actor/Cube.h"
#include "Object/Actor/Cylinder.h"
#include "Object/Actor/Sphere.h"
#include "Object/Gizmo/GizmoActor.h"
#include "Static/FEditorManager.h"
#include "Static/FLineBatchManager.h"
#include "Static/FUUIDBillBoard.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

void UWorld::InitWorld()
{
	GridSize = FString::ToFloat(UConfigManager::Get().GetValue(TEXT("World"), TEXT("GridSize")));
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

	ACamera* cam = FEditorManager::Get().GetCamera();
	cam->UpdateCameraMatrix();

	//if (APlayerInput::Get().GetKeyDown(EKeyCode::LButton))
	//{
	//	RenderPickingTexture(*Renderer);
	//}

	RenderMainTexture(*Renderer);

	FLineBatchManager::Get().Render();

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
		AActor* Owner = RenderComponent->GetOwner();
		if (Owner->GetDepth() > 0 or Owner->IsHidden() == true)
		//if (RenderComponent->GetOwner()->GetDepth() > 0)
		{
			continue;
		}
		//uint32 depth = RenderComponent->GetOwner()->GetDepth();
		// RenderComponent->UpdateConstantDepth(Renderer, depth);
		RenderComponent->Render();
	}

	FDevice::Get().PickingPrepare();

	//Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		if (RenderComponent->GetOwner()->IsHidden() == true)
		{
			continue;
		}
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		RenderComponent->Render();
	}

	FDevice::Get().SetRenderTarget();
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

		if (Actor)
			Actor->SetActorTransform(Transform);
	}

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
		float outT = 0.0f;
		bool bHit = false;

		TSet<UActorComponent*> components = Actor->GetComponents();
		for (auto component : components)
		{
			if (component->IsA<UStaticMeshComponent>())
			{
				UStaticMeshComponent* StaticMeshComponent = dynamic_cast<UStaticMeshComponent*>(component);
				if (StaticMeshComponent != nullptr)
				{
					FTransform primTransform = StaticMeshComponent->GetComponentTransform();
					FMatrix primWorldMat = primTransform.GetMatrix();

					std::shared_ptr<UStaticMesh> CurMesh = StaticMeshComponent->GetStaticMesh();
					FVector vertexMin = CurMesh->GetVertexBuffer()->GetMin();
					FVector vertexMax = CurMesh->GetVertexBuffer()->GetMax();

					EPrimitiveType PrimitiveType = StaticMeshComponent->GetType();
					switch (PrimitiveType)
					{
					case EPrimitiveType::EPT_Sphere:
						{
							if (ABS(primTransform.GetScale().X - primTransform.GetScale().Y)
								+ ABS(primTransform.GetScale().Y - primTransform.GetScale().Z)
								+ ABS(primTransform.GetScale().X - primTransform.GetScale().Z)
								< 0.1f) {
								bHit = FRayCast::InsertSectRaySphere(worldRay, primTransform.GetPosition(), 1.0f * primTransform.GetScale().X, outT);
								if (bHit)
								{
									UE_LOG("%s : Sphere Hit", *Actor->GetFName().ToString());
								}
								break;
								}
							else {
								bHit = FRayCast::IntersectRayAABB(worldRay,
									primTransform.GetPosition() + primTransform.GetScale() * vertexMin,
									primTransform.GetPosition() + primTransform.GetScale() * vertexMax,
									outT);

								if (bHit)
								{
									UE_LOG("%s : AABB Hit form Vertex Min Max", *Actor->GetFName().ToString());
								}
								break;

								break;
							}

						}

					default:
						{
							bHit = FRayCast::IntersectRayAABB(worldRay,
								primTransform.GetPosition() + primTransform.GetScale() * vertexMin,
								primTransform.GetPosition() + primTransform.GetScale() * vertexMax,
								outT);

							if (bHit)
							{
								UE_LOG("%s : AABB Hit form Vertex Min Max", *Actor->GetFName().ToString());
							}
							break;
						}
					}
				}

				if (bHit)
				{
					float distance = worldRay.GetPoint(outT).Length();
					if (distance < minDistance)
					{
						minDistance = distance;
						SelectedActor = Actor;
					}
				}
		
				if (SelectedActor)
				{
					FEditorManager::Get().SelectActor(SelectedActor);
					FUUIDBillBoard::Get().SetTarget(SelectedActor);
				}
			}
		}
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