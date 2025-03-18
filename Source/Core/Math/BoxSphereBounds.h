#pragma once
#include "Vector.h"

struct FBox
{
public:
	FVector Min;
	FVector Max;

	FBox()
		: Min(FVector::ZeroVector)
		, Max(FVector::ZeroVector)
	{
	}

	FBox(FVector InMin, FVector InMax)
		: Min(InMin)
		, Max(InMax)
	{
	}

public:
	FORCEINLINE FBox& operator+=(const FBox& Other)
	{
		Min.X = FMath::Min(Min.X, Other.Min.X);
		Min.Y = FMath::Min(Min.Y, Other.Min.Y);
		Min.Z = FMath::Min(Min.Z, Other.Min.Z);

		Max.X = FMath::Max(Max.X, Other.Max.X);
		Max.Y = FMath::Max(Max.Y, Other.Max.Y);
		Max.Z = FMath::Max(Max.Z, Other.Max.Z);

		return *this;
	}

	FBox operator+(const FBox& Other) const
	{
		return FBox(*this) += Other;
	}

	FBox operator+=(const FVector& Other)
	{
		Min.X = FMath::Min(Min.X, Other.X);
		Min.Y = FMath::Min(Min.Y, Other.Y);
		Min.Z = FMath::Min(Min.Z, Other.Z);

		Max.X = FMath::Max(Max.X, Other.X);
		Max.Y = FMath::Max(Max.Y, Other.Y);
		Max.Z = FMath::Max(Max.Z, Other.Z);

		return *this;
	}

	FBox operator+(const FVector& Other) const
	{
		return FBox(*this) += Other;
	}

	float GetWidth() const { return Max.X - Min.X; }
	float GetHeight() const { return Max.Y - Min.Y; }
	float GetDepth() const { return Max.Z - Min.Z; }

	float ComputeSquaredDIstanceToPoint(FVector Point) const
	{
		float DistSqaured = 0.0f;

		if (Point.X < Min.X)
		{
			DistSqaured += FMath::Square(Point.X - Min.X);
		}
		else if (Point.X > Max.X)
		{
			DistSqaured += FMath::Square(Point.X - Max.X);
		}

		if (Point.Y < Min.Y)
		{
			DistSqaured += FMath::Square(Point.Y - Min.Y);
		}
		else if (Point.Y > Max.Y)
		{
			DistSqaured += FMath::Square(Point.Y - Max.Y);
		}

		if (Point.Z < Min.Z)
		{
			DistSqaured += FMath::Square(Point.Z - Min.Z);
		}
		else if (Point.Z > Max.Z)
		{
			DistSqaured += FMath::Square(Point.Z - Max.Z);
		}

		return DistSqaured;
	}

	float ComputeSquaredDistanceToBox(FBox Other) const
	{
		FVector AxisDist = FVector(FMath::Abs(GetCenter().X - Other.GetCenter().X), FMath::Abs(GetCenter().Y - Other.GetCenter().Y), FMath::Abs(GetCenter().Z - Other.GetCenter().Z)) - (GetExtent() + Other.GetExtent());
		AxisDist = (AxisDist > FVector::ZeroVector) ? AxisDist : FVector::ZeroVector;
		return AxisDist.Dot(AxisDist);
	}

	FBox ExpandBy(float W) const
	{
		return FBox(Min - FVector(W, W, W), Max + FVector(W, W, W));
	}

	FBox ExpandBy(FVector V) const
	{
		return FBox(Min - V, Max + V);
	}

	FBox ShiftBy(FVector Offet) const
	{
		return FBox(Min + Offet, Max + Offet);
	}

	FBox MoveTo(FVector Position) const
	{
		FVector Offset = Position - GetCenter();
		return ShiftBy(Offset);
	}

	FVector GetCenter() const
	{
		return (Min + Max) * 0.5f;
	}

	FVector GetExtent() const
	{
		return (Max - Min) * 0.5f;
	}

	FVector GetSize() const
	{
		return Max - Min;
	}

	float GetVolume() const
	{
		return  GetSize().X * GetSize().Y * GetSize().Z;
	}
};

struct FSphere
{
public:
	FVector Center;
	float Radius;
	FSphere()
		: Center(FVector::ZeroVector)
		, Radius(0.0f)
	{
	}
	FSphere(FVector InCenter, float InRadius)
		: Center(InCenter)
		, Radius(InRadius)
	{
	};

	float GetVolume() const
	{
		return 4.0f / 3.0f * PI * (Radius * Radius * Radius);
	}

};

struct FBoxSphereBounds
{
public:
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;

	FBoxSphereBounds()
		: Origin(FVector::ZeroVector)
		, BoxExtent(FVector::ZeroVector)
		, SphereRadius(0.0f)
	{
	}

	FBoxSphereBounds(FVector InOrigin, FVector InBoxExtent, float InSphereRadius)
		: Origin(InOrigin)
		, BoxExtent(InBoxExtent)
		, SphereRadius(InSphereRadius)
	{
	}

	FBoxSphereBounds(FVector InOrigin, float InSphereRadius)
		: Origin(InOrigin)
		, BoxExtent(FVector::ZeroVector)
		, SphereRadius(InSphereRadius)
	{
	}

	FBoxSphereBounds(FVector InOrigin, FVector InBoxExtent)
		: Origin(InOrigin)
		, BoxExtent(InBoxExtent)
		, SphereRadius(0.0f)
	{
	}

	FBoxSphereBounds(FBox InBox)
	{
		FVector LocalExtent;
		Origin = InBox.GetCenter();
		BoxExtent = InBox.GetExtent();
		SphereRadius = BoxExtent.Length();
	}

	FBox GetBox() const
	{
		return FBox(Origin - BoxExtent, Origin + BoxExtent);
	}

	FVector GetCenter() const
	{
		return Origin;
	}

	FSphere GetSphere() const
	{
		return FSphere(Origin, SphereRadius);
	}

	FBoxSphereBounds ExpandBy(float V) const
	{
		return FBoxSphereBounds(Origin, { BoxExtent.X + V, BoxExtent.Y + V, BoxExtent.Z + V }, SphereRadius + V);
	}

	FBoxSphereBounds TransformBy(const FMatrix& InMatrix) const
	{
		FBoxSphereBounds NewBounds;

		FVector4 m0 = FVector4(InMatrix.M[0][0], InMatrix.M[0][1], InMatrix.M[0][2], InMatrix.M[0][3]);
		FVector4 m1 = FVector4(InMatrix.M[1][0], InMatrix.M[1][1], InMatrix.M[1][2], InMatrix.M[1][3]);
		FVector4 m2 = FVector4(InMatrix.M[2][0], InMatrix.M[2][1], InMatrix.M[2][2], InMatrix.M[2][3]);
		FVector4 m3 = FVector4(InMatrix.M[3][0], InMatrix.M[3][1], InMatrix.M[3][2], InMatrix.M[3][3]);

		FVector4 NewOrigin = FVector4::MultiplyVector4(FVector4(Origin.X, Origin.X, Origin.X, Origin.X), m0);
		NewOrigin = FVector4::VectorMultiplyAdd(FVector4(Origin.Y, Origin.Y, Origin.Y, Origin.Y), m1, NewOrigin);
		NewOrigin = FVector4::VectorMultiplyAdd(FVector4(Origin.Z, Origin.Z, Origin.Z, Origin.Z), m2, NewOrigin);
		NewOrigin = FVector4(NewOrigin.X + m3.X, NewOrigin.Y + m3.Y, NewOrigin.Z + m3.Z, NewOrigin.W + m3.W);

		FVector4 MultiplyExXMO = FVector4::MultiplyVector4(FVector4(BoxExtent.X, BoxExtent.X, BoxExtent.X, BoxExtent.X), m0);
		FVector4 NewExtent = FVector4(FMath::Abs(MultiplyExXMO.X), FMath::Abs(MultiplyExXMO.Y), FMath::Abs(MultiplyExXMO.Z), FMath::Abs(MultiplyExXMO.W));
		FVector4 MultiplyNewExYM1 = FVector4::VectorMultiplyAdd(FVector4(BoxExtent.Y, BoxExtent.Y, BoxExtent.Y, BoxExtent.Y), m1, NewExtent);
		NewExtent = FVector4(FMath::Abs(MultiplyNewExYM1.X), FMath::Abs(MultiplyNewExYM1.Y), FMath::Abs(MultiplyNewExYM1.Z), FMath::Abs(MultiplyNewExYM1.W));
		FVector4 MultiplyNewExZM2 = FVector4::VectorMultiplyAdd(FVector4(BoxExtent.Z, BoxExtent.Z, BoxExtent.Z, BoxExtent.Z), m2, NewExtent);
		NewExtent = FVector4(FMath::Abs(MultiplyNewExZM2.X), FMath::Abs(MultiplyNewExZM2.Y), FMath::Abs(MultiplyNewExZM2.Z), FMath::Abs(MultiplyNewExZM2.W));

		NewBounds.Origin = FVector(NewOrigin.X, NewOrigin.Y, NewOrigin.Z);
		NewBounds.BoxExtent = FVector(NewExtent.X, NewExtent.Y, NewExtent.Z);

		FVector4 MaxRadius = FVector4::MultiplyVector4(m0, m0);
		MaxRadius = FVector4::VectorMultiplyAdd(m1, m1, MaxRadius);
		MaxRadius = FVector4::VectorMultiplyAdd(m2, m2, MaxRadius);
		MaxRadius = FVector4::VectorMax(MaxRadius, FVector4::VectorMax(FVector4(MaxRadius.Y, MaxRadius.Y, MaxRadius.Y, MaxRadius.Y), FVector4(MaxRadius.Z, MaxRadius.Z, MaxRadius.Z, MaxRadius.Z)));
		
		NewBounds.SphereRadius = FMath::Sqrt(MaxRadius.X) * SphereRadius;

		float const BoxExtentMagnitude = FMath::Sqrt(BoxExtent.X * BoxExtent.X + BoxExtent.Y * BoxExtent.Y + BoxExtent.Z * BoxExtent.Z);
		NewBounds.SphereRadius += BoxExtentMagnitude * FMath::Sqrt(MaxRadius.W);

		return NewBounds;
	}

	FBoxSphereBounds TransformBy(const FTransform& InTransform) const
	{
		return TransformBy(InTransform.ToMatrixWithScale());
	}
};

