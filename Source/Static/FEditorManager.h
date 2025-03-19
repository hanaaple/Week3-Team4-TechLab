#pragma once
#include "Object/Actor/Actor.h"

class AGizmoActor;

class FEditorManager : public TSingleton<FEditorManager>
{
	friend class URenderer;
	friend class FDevice;;
public:
	void Init();
    
    inline AActor* GetSelectedActor() const {return SelectedActor;}
    
    void SelectActor(AActor* NewActor);

    inline ACamera* GetCamera() const {return Camera;}

    void SetCamera(ACamera* NewCamera);

	AGizmoActor* GetGizmo() const {return Gizmo;}

	static FVector4 EncodeUUID(unsigned int UUID);
	static int DecodeUUID(FVector4 color);

	void LateTick(float DeltaTime);

	void OnUpdateWindowSize(uint32 Width, uint32 Height);

	void OnResizeComplete();

	FVector4 GetPixel(FVector MPos);
    
private:
    ACamera* Camera = nullptr;
    AActor* SelectedActor = nullptr;
	AGizmoActor* Gizmo = nullptr;

	

	std::shared_ptr<class FTexture> UUIDTexture = nullptr;
	//std::shared_ptr<class FTexture> UUIDTextureDepthStecil = nullptr;
};




