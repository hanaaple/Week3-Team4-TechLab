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
};

