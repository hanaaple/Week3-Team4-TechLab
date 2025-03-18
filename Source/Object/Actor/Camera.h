#pragma once
#include "Core/HAL/PlatformType.h"
#include "Object/Actor/Actor.h"

namespace ECameraProjectionMode
{
    enum Type : uint8
    {
        Perspective,
        Orthographic
    };
}

class ACamera : public AActor
{
	DECLARE_CLASS(ACamera, AActor)
    
public:
    ACamera();

private:    
    float Near;
    float Far;
    // 화면각
    float FieldOfView;

	
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;


public:
    const float MaxYDegree = 89.8f;
    //카메라 스피드 IMGui용 나중에 Velocity로 관리하면 없어질애라 편하게 public에서 관리
    float CameraSpeed = 1.0f;
	float Sensitivity = 1.0f;
    
    // 투영 타입 - Perspective, Orthographic
    ECameraProjectionMode::Type ProjectionMode;
    // float AspectRatio;	// 카메라 비율 (이번 프로젝트에서는 사용 안할듯) 

	virtual void BeginPlay() override;

    void SetFieldOfVew(float Fov);
    void SetFar(float Far);
    void SetNear(float Near);
    
    float GetFieldOfView() const;
    float GetNear() const;
    float GetFar() const;


	
	const FMatrix& GetProjectionMatrix() const { return ProjectionMatrix; }
	const FMatrix& GetViewProjectionMatrix() const { return ViewProjectionMatrix; }
	const FMatrix& GetViewMatrix() const { return ViewMatrix; }
	
	
	void InitMatrix();

        
    FVector GetForward() const
    {
        return GetActorTransform().GetForward();
    }
    
    FVector GetRight() const
    {
        return GetActorTransform().GetRight();
    }
    
    FVector GetUp() const
    {
        return GetActorTransform().GetUp();
    }

	/** Projection 변환 Matrix를 업데이트 합니다. */
	void UpdateCameraMatrix();
	
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void Rotate(const FVector& mouseDelta);
};