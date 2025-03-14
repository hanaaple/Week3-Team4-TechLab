#pragma once
#include "MathUtility.h"
#include "Vector.h"

struct FMatrix;

class FRay
{
private:
	/// <summary>
	/// Ray의 시작점
	/// </summary>
	FVector Origin;

	/// <summary>
	/// Ray의 방향
	/// </summary>
	FVector Direction;
public:
	FRay() : Origin(FVector::Zero()), Direction(FVector::Zero()) {}
	FRay(const FVector& Origin, const FVector& Direction) : Origin(Origin), Direction(Direction) {}
	FRay(const FMatrix& ViewMatrix, const FMatrix& ProjMatrix, float MouseNDCX, float MouseNDCY, float ScreenWidth, float ScreenHeight);

	FVector GetPoint(float T) const
	{
		return Origin + Direction * T;
	}

	inline bool operator==(const FRay& Other)
	{
		return Origin == Other.Origin && Direction == Other.Direction;
	}

	inline bool operator!=(const FRay& Other)
	{
		return Origin != Other.Origin || Direction != Other.Direction;
	}

	inline const FVector& GetOrigin() const { return Origin; }
	inline const FVector& GetDirection() const { return Direction; }

	inline void SetOrigin(const FVector& Origin) { this->Origin = Origin; }
	inline void SetDirection(const FVector& Direction) { this->Direction = Direction; }

	inline void Normalize()
	{
		Direction.Normalize();
	}

	inline FRay GetNormalized() const
	{
		FRay Result = *this;
		Result.Normalize();
		return Result;
	}

	inline float Distance(const FVector& Point) const
	{
		return FVector::CrossProduct(Direction, Point - Origin).Length();
	}

	inline float DistanceSquared(const FVector& Point) const
	{
		return FVector::CrossProduct(Direction, Point - Origin).LengthSquared();
	}
};

struct FRayCast
{
public:
	/// <summary>
	/// Ray와 구(Sphere)와의 교차를 검사
	/// </summary>
	/// <param name="ray">교차 검사를 수행할 Ray</param>
	/// <param name="sphereCenter">구의 중심</param>
	/// <param name="sphereRadius">구의 반지름</param>
	/// <param name="outT">교차 시 Ray 상의 t 값</param>
	/// <returns>교차가 있으면 true, 없으면 false</returns>
	static bool InserSectRaySphere(const FRay& Ray, const FVector& SphereCenter, float SphereRadius, OUT float& OutT);

	/// <summary>
	/// Ray와 축에 정렬된 경계 상자(AABB)와의 교차를 슬랩(Slab) 기법을 이용해 검사
	/// </summary>
	/// <param name="ray">교차 검사를 수행할 Ray</param>
	/// <param name="boxMin">AABB의 최소 점 (x, y, z 최소값)</param>
	/// <param name="boxMax">AABB의 최대 점 (x, y, z 최대값)</param>
	/// <param name="outT">Ray와 상자와의 첫 번째 교차 시 Ray 상의 t 값</param>
	/// <returns>교차가 있으면 true, 없으면 false</returns>
	static bool IntersectRayAABB(const FRay& Ray, const FVector& BoxMin, const FVector& BoxMax, OUT float& OutT);

	/// <summary>
	/// <para>Ray와 삼각형 평면(삼각형을 이루는 v0, v1, v2가 있는 평면)과의 교차를 검사</para>
	/// 먼저, 삼각형의 평면에 대한 교차를 평면 방정식을 이용해 계산 후, 그 교차점이 삼각형 내부에 있는지를 바리센트릭 좌표(barycentric coordinates)를 이용해 판단
	/// </summary>
	/// <param name="ray">교차 검사를 수행할 Ray</param>
	/// <param name="v0">삼각형의 첫 번째 꼭짓점</param>
	/// <param name="v1">삼각형의 두 번째 꼭짓점</param>
	/// <param name="v2">삼각형의 세 번째 꼭짓점</param>
	/// <param name="outT">교차 시 Ray 상의 t 값</param>
	/// <returns>교차가 있고 삼각형 내부에 있으면 true, 아니면 false</returns>
	static bool IntersectRayTrianglePlane(const FRay& Ray, const FVector& V0, const FVector& V1, const FVector& V2, OUT float& OutT);

	/// <summary>
	/// <para>주어진 Ray와 평면(planePoint와 planeNormal으로 정의)과의 단순 교차를 검사</para>
	/// 평면 방정식을 이용해 교차 파라미터 t를 이용해 교차 검사
	/// </summary>
	/// <param name="ray">교차 검사를 수행할 Ray</param>
	/// <param name="planePoint">평면 위의 한 점</param>
	/// <param name="planeNormal">평면의 법선 벡터</param>
	/// <param name="outT">교차 시 Ray 상의 t 값</param>
	/// <returns>교차가 있으면 true, 없으면 false</returns>
	static bool IntersectRayPlane(const FRay& Ray, const FVector& PlanePoint, const FVector& PlaneNormal, OUT float& OutT);
};
