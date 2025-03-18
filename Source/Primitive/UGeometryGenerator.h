#pragma once
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"

class UGeometryGenerator
{
public:
    static void CreateCube(float size, TArray<FVertexSimple>& OutVertices, TArray<uint32>& OutIndices);
    static void CreateSphere(int32 Radius, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>& OutVertices, TArray<uint32>& OutIndices);
    static void CreateCylinder(float BottomRadius, float TopRadius, float Height, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>&
                               OutVertices, TArray<uint32>& OutIndices
    );
    static void CreateCone(float BottomRadius, float Height, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>& OutVertices, TArray<
                           uint32>& OutIndices);
};