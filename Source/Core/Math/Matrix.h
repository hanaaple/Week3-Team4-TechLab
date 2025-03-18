#pragma once
#include "Vector.h"

struct FVector4;
struct FVector;
struct FQuat;
struct FRotator;
struct FTransform;

struct alignas(16) FMatrix
{
	union {

		float M[4][4];
		//FVector4[4];

		struct
		{
			FVector4 X;
			FVector4 Y;
			FVector4 Z;
			FVector4 W;
		};
	};

	FMatrix();
	FMatrix(const FVector4& InX, const FVector4& InY, const FVector4& InZ, const FVector4& InW);
	FMatrix(const FRotator& Rotation);

	static FMatrix Identity();
	static FMatrix Transpose(const FMatrix& Matrix);
	static FMatrix GetTranslateMatrix(float X, float Y, float Z);
	static FMatrix GetTranslateMatrix(FVector Translation);
	static FMatrix GetScaleMatrix(float X, float Y, float Z);
	static FMatrix GetScaleMatrix(const FVector& InScale);
	static FMatrix GetRotateMatrix(const FQuat& Q);
	static FMatrix GetQuatToRotationMatrixScaleMatrix(const FQuat& Q, const FVector& Scale);
	static FMatrix LookAtLH(const FVector& EyePosition, const FVector& FocusPoint, const FVector& cameraUp);
	static FMatrix PerspectiveFovLH(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane);
	static FMatrix OrthographicLH(float ViewWidth, float ViewHeight, float NearPlane, float FarPlane);
	static FMatrix OrthoForLH(float ViewWidth, float VeiwHeight, float NearPlane, float FarPlane);

	static FMatrix InverseGaussJordan(FMatrix& mat);

	FMatrix operator+(const FMatrix& Other) const;
	FMatrix operator+=(const FMatrix& Other);
	FMatrix operator-(const FMatrix& Other) const;
	FMatrix operator-=(const FMatrix& Other);
	FMatrix operator*(const FMatrix& Other) const;
	FMatrix operator*=(const FMatrix& Other);
	FMatrix operator*(float Other) const;
	FMatrix operator*=(float Other);
	bool operator==(const FMatrix& Other) const;
	bool operator!=(const FMatrix& Other) const;

	FMatrix GetTransposed() const;
	float Determinant() const;
	FMatrix Inverse() const;

	FVector GetTranslation() const;
	FVector GetScale() const;
	FVector GetRotation() const;

	FVector TransformVector(const FVector& Vector) const;
	FVector4 TransformVector(const FVector4& Vector) const;
	FVector4 TransformVector4(const FVector4& Vector) const;

	FTransform GetTransform() const;

	static FMatrix RotateRoll(float Angle);

	static FMatrix RotatePitch(float Angle);

	static FMatrix RotateYaw(float Angle);

	static FMatrix RotateToMatrix(float X, float Y, float Z);

	FVector ExtractScale(float Tolerance = 1e-8f);

	void RemoveScaling(float Tolerance = 1e-8f);

	FTransform ConstructTransformFromMatrixWithDesiredScale(FMatrix& AMatrix, FMatrix& BMatrix, FVector DesiredScale);

	void SetAxis(int32 i, const FVector& Axis);

	FVector GetAxis(int32 i) const;
};


