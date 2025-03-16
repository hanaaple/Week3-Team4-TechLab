#pragma once
#include "Object/Actor/Actor.h"

class AGizmoActor;

class FEditorManager : public TSingleton<FEditorManager>
{
public:
    
    inline AActor* GetSelectedActor() const {return SelectedActor;}
    
    void SelectActor(AActor* NewActor);

    inline ACamera* GetCamera() const {return Camera;}

    void SetCamera(ACamera* NewCamera);

	AGizmoActor* GetGizmo() const {return Gizmo;}
    
private:
    ACamera* Camera = nullptr;
    AActor* SelectedActor = nullptr;
	AGizmoActor* Gizmo = nullptr;
};
