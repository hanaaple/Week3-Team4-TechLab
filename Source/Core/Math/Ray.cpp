#include "Ray.h"

#include "Core/Math/Matrix.h"
#include "DirectXTK/DirectXHelpers.h"
#include "Static/FLineBatchManager.h"


FRay::FRay(const FMatrix& ViewMatrix, const FMatrix& ProjMatrix, float MouseNDCX, float MouseNDCY)
{
	FMatrix viewProj = ViewMatrix * ProjMatrix;
	FMatrix invViewProj = viewProj.Inverse();
	
	FVector4 NDCStart = FVector4(MouseNDCX, MouseNDCY, 0.0f, 1.0f);
	FVector4 NDCEnd = FVector4(MouseNDCX, MouseNDCY, 1.0f, 1.0f);
	
	FVector4 ViewToWorldStart = invViewProj.TransformVector4(NDCStart);
	FVector4 ViewToWorldEnd = invViewProj.TransformVector4(NDCEnd);

	FVector RayStartWorldSpace{ViewToWorldStart};
	FVector RayEndWorldSpace{ViewToWorldEnd};

	if (ViewToWorldStart.W != 0.0f)
	{
		RayStartWorldSpace /= ViewToWorldStart.W;
	}
	if (ViewToWorldEnd.W != 0.0f)
	{
		RayEndWorldSpace /= ViewToWorldEnd.W;
	}
	
	Origin = RayStartWorldSpace;
	Direction = (RayEndWorldSpace - RayStartWorldSpace).GetSafeNormal();
}

FRay FRay::TransformRayToLocal(const FRay& worldRay, const FMatrix& primWorldMat)
{
	// 로컬 좌표로 변환: 원점 (w=1)
	FVector4 localOrigin = primWorldMat.TransformVector4(FVector4(worldRay.GetOrigin(), 1.0f));
	localOrigin /= localOrigin.W;

	// 방향 변환 (w=0)
	FVector4 localDir = primWorldMat.TransformVector4(FVector4(worldRay.GetDirection(), 0.0f));

	return FRay(FVector(localOrigin.X, localOrigin.Y, localOrigin.Z),
		FVector(localDir.X, localDir.Y, localDir.Z));
}

bool FRayCast::InserSectRaySphere(const FRay& Ray, const FVector& SphereCenter, float SphereRadius, OUT float& OutT)
{
	// Ray : Origin + tD
	// Sphere : (X - Center) · (X - Center) = r^2

	// 교차점을 구하기 위한 방정식
	// (Origin + tD - Center) · (Origin + tD - Center) = r^2
	// (Origin - Center + tD) · (Origin - Center + tD) = r^2
	// (Origin - Center) · (Origin - Center) + 2t(Origin - Center) · D + t^2(D · D) = r^2
	// t^2 + 2(Origin - Center)·t + (Origin - Center)·(Origin - Center) - r^2 = 0

	// 위의 방정식은 a*t^2 + b*t + c = 0 형태로 변환 가능
	// 2차 방정식의 해를 구하기 위해 판별식을 계산
	// D = b^2 - 4ac

	//Ray의 원점에서 구의 중심까지의 벡터
	FVector L = SphereCenter - Ray.GetOrigin();

	// b = 2(L · D)
	float b = 2 * FVector::DotProduct(Ray.GetDirection(), L);

	// c = L · L - r^2
	float c = L.Dot(L) - SphereRadius * SphereRadius;

	float d = b * b - 4 * c;

	// 판별식이 음수이면 교차점이 없음
	if (d < SMALL_NUMBER)
	{
		return false;
	}

	float t0 = (-b - sqrtf(d)) / 2;
	float t1 = (-b + sqrtf(d)) / 2;

	// 두 해 중에서 작은 값이 교차점
	if (t0 >= 0)
	{
		OutT = t0;
	}
	else if (t1 >= 0)
	{
		OutT = t1;
	}
	else
	{
		return false;
	}

	return true;
}

bool FRayCast::IntersectRayAABB(const FRay& Ray, const FVector& BoxMin, const FVector& BoxMax, OUT float& OutT)
{
	float tMin = 0;
	float tMax = 10000000.f;

	{// X축에 대한 슬랩 테스트
		float invDx = 1.0f / Ray.GetDirection().X;
		float t1 = (BoxMin.X - Ray.GetOrigin().X) * invDx;
		float t2 = (BoxMax.X - Ray.GetOrigin().X) * invDx;
		if (t1 > t2)
			std::swap(t1, t2);

		tMin = FMath::Max(tMin, t1);
		tMax = FMath::Min(tMax, t2);

		if (tMin > tMax)
			return false;
	}

	{// Y축에 대한 슬랩 테스트
		float invDy = 1.0f / Ray.GetDirection().Y;
		float t1 = (BoxMin.Y - Ray.GetOrigin().Y) * invDy;
		float t2 = (BoxMax.Y - Ray.GetOrigin().Y) * invDy;
		if (t1 > t2)
			std::swap(t1, t2);

		tMin = FMath::Max(tMin, t1);
		tMax = FMath::Min(tMax, t2);

		if (tMin > tMax)
			return false;
	}

	{// Z축에 대한 슬랩	테스트
		float invDz = 1.0f / Ray.GetDirection().Z;
		float t1 = (BoxMin.Z - Ray.GetOrigin().Z) * invDz;
		float t2 = (BoxMax.Z - Ray.GetOrigin().Z) * invDz;
		if (t1 > t2)
			std::swap(t1, t2);

		tMin = FMath::Max(tMin, t1);
		tMax = FMath::Min(tMax, t2);

		if (tMin > tMax)
			return false;
	}

	// Ray의 원점이 AABB 내부에 있거나 tMin이 음수인 경우에는 tMax가 사용
	if (tMin >= 0)
	{
		OutT = tMin;
		return true;
	}
	else if (tMax >= 0)
	{
		OutT = tMax;
		return true;
	}
	else
	{
		return false;
	}
}

bool FRayCast::IntersectRayTrianglePlane(const FRay& Ray, const FVector& V0, const FVector& V1, const FVector& V2, OUT float& OutT)
{
	FVector edge0 = V1 - V0;
	FVector edge1 = V2 - V0;
	FVector normal = FVector::CrossProduct(edge0, edge1).GetSafeNormal();

	float t;
	if (!IntersectRayPlane(Ray, V0, normal, t))
	{
		return false;
	}

	// 교차점 P
	FVector P = Ray.GetOrigin() + Ray.GetDirection() * t;

	// 바리센트릭 좌표 계산으로 P가 삼각형 내부에 있는지 판별
	// 바리센트릭 좌표(barycentric coordinates)는 삼각형 내부의 임의의 점을 삼각형의 세 꼭짓점에 대한 가중치의 조합으로 표현하는 좌표계
	// 삼각형의 꼭짓점을 v0, v1, v2 라고 할 때, P = u*v0 + v*v1 + (1-u-v)*v2 로 표현
	// u, v, 1-u-v이 모두 0 이상이고, u+v가 1 이하이면 삼각형 내부에 있다고 판단

	FVector vp0 = P - V0;
	float dot00 = FVector::DotProduct(edge0, edge0);
	float dot01 = FVector::DotProduct(edge0, edge1);
	float dot02 = FVector::DotProduct(edge0, vp0);
	float dot11 = FVector::DotProduct(edge1, edge1);
	float dot12 = FVector::DotProduct(edge1, vp0);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	if (u < SMALL_NUMBER || v < SMALL_NUMBER || u + v > 1 + SMALL_NUMBER)
	{
		return false;
	}

	OutT = t;
	return true;
}

bool FRayCast::IntersectRayPlane(const FRay& Ray, const FVector& PlanePoint, const FVector& PlaneNormal, OUT float& OutT)
{
	// Ray 방향과 평면 법선의 내적 계산
	float denom = Ray.GetDirection().Dot(PlaneNormal);

	// 내적 계산 결과가 0이면 Ray와 평면이 평행
	if (fabs(denom) < SMALL_NUMBER)
	{
		return false;
	}

	// 교차점이 평면 위에 있는지 확인
	float t = (PlanePoint - Ray.GetOrigin()).Dot(PlaneNormal) / denom;
	if (t < SMALL_NUMBER)
	{
		return false;
	}

	OutT = t;
	return true;
}
