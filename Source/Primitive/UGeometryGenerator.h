#pragma once
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"

class UGeometryGenerator
{
public:
    static void CreateCube(float size, TArray<FVertexSimple>* vertices, TArray<uint32>* indices);
    static void CreateSphere(int32 radius, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices);
    static void CreateCylinder(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices);
    static void CreateCone(float bottomRadius, float height, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices);
};