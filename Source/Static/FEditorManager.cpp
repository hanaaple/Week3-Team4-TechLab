#include "FEditorManager.h"
#include "Core/Engine.h"
#include "Object/World/World.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Transform.h"
#include <Object/Gizmo/GizmoActor.h>

void FEditorManager::SelectActor(AActor* NewActor)
{
    if (Gizmo == nullptr)
    {
		Gizmo = UEngine::Get().GetWorld()->SpawnActor<AGizmoActor>();
		Gizmo->SetDepth(1);
    }

	if (SelectedActor == NewActor)
		return;
	
    if (SelectedActor != nullptr && SelectedActor != NewActor)
    {
        SelectedActor->UnPick();
    }

	SelectedActor = NewActor;
	
    if (SelectedActor != nullptr)
    {
        SelectedActor->Pick();
		FTransform newActorTransform = NewActor->GetActorTransform();
		Gizmo->SetActorTransform(newActorTransform);
	}
}

void FEditorManager::SetCamera(ACamera* NewCamera)
{
    Camera = NewCamera;
}
