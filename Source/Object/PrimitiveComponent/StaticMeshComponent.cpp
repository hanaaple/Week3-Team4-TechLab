#include "StaticMeshComponent.h"

UCubeComp::UCubeComp()
{
	SetStaticMesh("Cube");
	bCanBeRendered = true;
	Max = GetStaticMesh()->GetVertexBuffer()->GetMax();
	Min = GetStaticMesh()->GetVertexBuffer()->GetMin();

	FVector extent = (Max - Min) / 2;
	SetBoxExtent(extent);
}

USphereComp::USphereComp()
{
	SetStaticMesh("Sphere");
	bCanBeRendered = true;
	Max = GetStaticMesh()->GetVertexBuffer()->GetMax();
	Min = GetStaticMesh()->GetVertexBuffer()->GetMin();

	FVector extent = (Max - Min) / 2;
	float radius = extent.Length();

	SetSphereRadius(radius);
}

void USphereComp::SetSphereRadius(float InSphereRadius)
{
	Radius = InSphereRadius;
	UpdateBounds();
}

FBoxSphereBounds USphereComp::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FVector::ZeroVector, FVector(Radius, Radius, Radius), Radius).TransformBy(LocalToWorld);
}

float USphereComp::GetShapeScale() const
{
	FTransform LocalToWorld = GetComponentTransform();

	// Scale3DAbsXYZ1 = { Abs(X), Abs(Y)), Abs(Z), 0 }
	FVector4 ScaleAbsXYYZ0 = FVector4(FMath::Abs(LocalToWorld.GetScale().X), FMath::Abs(LocalToWorld.GetScale().Y), FMath::Abs(LocalToWorld.GetScale().Z), 0.0f);
	// Scale3DAbsYZX1 = { Abs(Y),Abs(Z)),Abs(X), 0 }
	FVector4 ScaledAbsYZX0 = FVector4(ScaleAbsXYYZ0.Y, ScaleAbsXYYZ0.Z, ScaleAbsXYYZ0.X, 0.0f);
	// Scale3DAbsZXY1 = { Abs(Z),Abs(X)),Abs(Y), 0 }
	FVector4 ScaledAbsZXY0 = FVector4(ScaleAbsXYYZ0.Z, ScaleAbsXYYZ0.X, ScaleAbsXYYZ0.Y, 0.0f);

	// t0 = { Min(Abs(X), Abs(Y)),  Min(Abs(Y), Abs(Z)), Min(Abs(Z), Abs(X)), 0 }
	FVector4 t0 = FVector4(FMath::Min(ScaleAbsXYYZ0.X, ScaledAbsYZX0.X), FMath::Min(ScaleAbsXYYZ0.Y, ScaleAbsXYYZ0.Y), FMath::Min(ScaleAbsXYYZ0.Z, ScaleAbsXYYZ0.Z), 0.0f);
	// t1 = { Min(Abs(X), Abs(Y), Abs(Z)), Min(Abs(Y), Abs(Z), Abs(X)), Min(Abs(Z), Abs(X), Abs(Y)), 0 }
	FVector4 t2 = FVector4(FMath::Max(t0.X, ScaledAbsZXY0.X), FMath::Max(t0.Y, ScaledAbsZXY0.Y), FMath::Max(t0.Z, ScaledAbsZXY0.Z), 0.0f);
	// Scale3DAbsMax = Min(Abs(X), Abs(Y), Abs(Z));
	float scaleAbsMin = FMath::Min(t2.X, FMath::Min(t2.Y, t2.Z));

	return scaleAbsMin;
}

UTriangleComp::UTriangleComp()
{
	SetStaticMesh("Triangle");
	bCanBeRendered = true;
}

UQuadComp::UQuadComp()
{
	SetStaticMesh("Quad");
}

ULineComp::ULineComp()
{
	SetStaticMesh("Line");
}

UCylinderComp::UCylinderComp()
{
	SetStaticMesh("Cylinder");
}

UConeComp::UConeComp()
{
	SetStaticMesh("Cone");
}
