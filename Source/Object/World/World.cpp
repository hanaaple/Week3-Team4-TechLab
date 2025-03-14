#include "World.h"
#include <cassert>
#include "JsonSavehelper.h"

#include "Core/Container/Map.h"
#include "Core/Input/PlayerInput.h"
#include "Object/Actor/Camera.h"
#include <Object/Gizmo/GizmoHandle.h>

#include "Object/Actor/Cone.h"
#include "Object/Actor/Cube.h"
#include "Object/Actor/Cylinder.h"
#include "Object/Actor/Sphere.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"
#include "Static/FLineBatchManager.h"
#include <Core/Math/Ray.h>


void UWorld::BeginPlay()
{
	for (const auto& Actor : Actors)
	{
		Actor->BeginPlay();
	}

	APlayerInput::Get().RegisterMouseDownCallback(EKeyCode::LButton, std::bind(&UWorld::RayCasting, this, std::placeholders::_1), GetUUID());
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

void UWorld::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();

	//라인 렌더링 임시



	if (Renderer == nullptr)
	{
		return;
	}

	ACamera* cam = FEditorManager::Get().GetCamera();
	Renderer->UpdateViewMatrix(cam->GetActorTransform());
	Renderer->UpdateProjectionMatrix(cam);
	
	if (APlayerInput::Get().GetKeyDown(EKeyCode::LButton))
	{
		RenderPickingTexture(*Renderer);
	}
	
	RenderMainTexture(*Renderer);


	FLineBatchManager::Get().Render();
	// DisplayPickingTexture(*Renderer);

}

void UWorld::RenderPickingTexture(URenderer& Renderer)
{
	Renderer.PreparePicking();
	Renderer.PreparePickingShader();

	for (auto& RenderComponent : RenderComponents)
	{
		if (RenderComponent->GetOwner()->GetDepth() > 0)
		{
			continue;
		}
		uint32 UUID = RenderComponent->GetUUID();
		RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
		RenderComponent->Render();
	}

	Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		uint32 UUID = RenderComponent->GetUUID();
		RenderComponent->UpdateConstantPicking(Renderer, APicker::EncodeUUID(UUID));
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		RenderComponent->Render();
	}
}

void UWorld::RenderMainTexture(URenderer& Renderer)
{
	Renderer.PrepareMain();
	Renderer.PrepareMainShader();
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

	Renderer.PrepareZIgnore();
	for (auto& RenderComponent: ZIgnoreRenderComponents)
	{
		uint32 depth = RenderComponent->GetOwner()->GetDepth();
		RenderComponent->Render();
	}
}

void UWorld::DisplayPickingTexture(URenderer& Renderer)
{
	Renderer.RenderPickingTexture();
}

void UWorld::ClearWorld()
{
	TArray CopyActors = Actors;
	for (AActor* Actor : CopyActors)
	{
		if (!Actor->IsGizmoActor())
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

void UWorld::SaveWorld()
{
	const UWorldInfo& WorldInfo = GetWorldInfo();
	JsonSaveHelper::SaveScene(WorldInfo);
}

void UWorld::AddZIgnoreComponent(UPrimitiveComponent* InComponent)
{
	ZIgnoreRenderComponents.Add(InComponent);
	InComponent->SetIsOrthoGraphic(true);
}

void UWorld::LoadWorld(const char* SceneName)
{
	if (SceneName == nullptr || strcmp(SceneName, "") == 0){
		return;
	}
	
	UWorldInfo* WorldInfo = JsonSaveHelper::LoadScene(SceneName);
	if (WorldInfo == nullptr) return;

	ClearWorld();

	Version = WorldInfo->Version;
	this->SceneName = WorldInfo->SceneName;
	uint32 ActorCount = WorldInfo->ActorCount;

	// Type 확인
	for (uint32 i = 0; i < ActorCount; i++)
	{
		UObjectInfo* ObjectInfo = WorldInfo->ObjctInfos[i];
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
		
		Actor->SetActorTransform(Transform);
	}
}

void UWorld::RayCasting(const FVector& MouseNDCPos)
{
	float screenWidth = UEngine::Get().GetScreenWidth();
	float screenHeight = UEngine::Get().GetScreenHeight();

	FMatrix ProjMatrix = Camera->GetProjectionMatrix();

	FRay ray = FRay(Camera->GetViewMatrix(), ProjMatrix, MouseNDCPos.X, MouseNDCPos.Y, screenWidth, screenHeight);

	AActor* debugActor = SpawnActor<AActor>();
	debugActor->SetRootComponent(debugActor->AddComponent<USphereComp>());
	debugActor->SetActorTransform(FTransform(ray.GetOrigin(), FQuat(), FVector(0.01f, 0.01f, 0.01f)));
	AddRenderComponent(debugActor->GetComponentByClass<USphereComp>());

	for (auto& Actor : Actors)
	{
		UCubeComp* PrimitiveComponent = Actor->GetComponentByClass<UCubeComp>();

		if (PrimitiveComponent == nullptr)
		{
			continue;
		}

		FMatrix primWorldMat = PrimitiveComponent->GetComponentTransform().GetMatrix();

		FVector4 localOrigin = primWorldMat.TransformVector4(FVector4(ray.GetOrigin(), 1));
		localOrigin /= localOrigin.W;

		FVector4 localDir = primWorldMat.TransformVector4(FVector4(ray.GetDirection(), 0));

		FRay localRay = FRay(FVector(localOrigin.X, localOrigin.Y, localOrigin.Z), FVector(localDir.X, localDir.Y, localDir.Z));

		switch (PrimitiveComponent->GetType())
		{
		case EPrimitiveType::EPT_Cube:
		{
			float outT = 0;
			if (FRayCast::IntersectRayAABB(localRay, FVector(-0.5f, -0.5f, -0.5f), FVector(0.5f, 0.5f, 0.5f), outT))
			{
				UE_LOG("Hit Cube");
			}
			break;
		}
		}
	}
}

UWorldInfo UWorld::GetWorldInfo() const
{
	UWorldInfo WorldInfo;
	WorldInfo.ActorCount = Actors.Num();
	WorldInfo.ObjctInfos = new UObjectInfo*[WorldInfo.ActorCount];
	WorldInfo.SceneName = *SceneName;
	WorldInfo.Version = 1;
	uint32 i = 0;
	for (auto& actor : Actors)
	{
		if (actor->IsGizmoActor())
		{
			WorldInfo.ActorCount--;
			continue;
		}
		WorldInfo.ObjctInfos[i] = new UObjectInfo();
		const FTransform& Transform = actor->GetActorTransform();
		WorldInfo.ObjctInfos[i]->Location = Transform.GetPosition();
		WorldInfo.ObjctInfos[i]->Rotation = Transform.GetRotation();
		WorldInfo.ObjctInfos[i]->Scale = Transform.GetScale();
		WorldInfo.ObjctInfos[i]->ObjectType = actor->GetTypeName();

		WorldInfo.ObjctInfos[i]->UUID = actor->GetUUID();
		i++;
	}
	return WorldInfo;
}