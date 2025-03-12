#include "UGeometryGenerator.h"
#include "Core/Math/Vector.h"
#include <cmath>

void UGeometryGenerator::CreateCube(float size, TArray<FVertexSimple>* vertices, TArray<uint32>* indices)
{
    // 정육면체의 반 크기
    float halfSize = size / 2.0f;

    // 정육면체의 8개 정점 정의
    vertices->Add(FVertexSimple(-halfSize, -halfSize, -halfSize, 0.0f, 0.0f, 0.0f, 1.0f)); // 0
    vertices->Add(FVertexSimple(-halfSize, -halfSize, halfSize, 0.0f, 1.0f, 0.0f, 1.0f)); // 1
    vertices->Add(FVertexSimple(-halfSize, halfSize, -halfSize, 1.0f, 0.0f, 0.0f, 1.0f)); // 2
    vertices->Add(FVertexSimple(-halfSize, halfSize, halfSize, 1.0f, 1.0f, 0.0f, 1.0f)); // 3
    vertices->Add(FVertexSimple(halfSize, -halfSize, -halfSize, 0.0f, 0.0f, 1.0f, 1.0f)); // 4
    vertices->Add(FVertexSimple(halfSize, -halfSize, halfSize, 0.0f, 1.0f, 1.0f, 1.0f)); // 5
    vertices->Add(FVertexSimple(halfSize, halfSize, -halfSize, 1.0f, 0.0f, 1.0f, 1.0f)); // 6
    vertices->Add(FVertexSimple(halfSize, halfSize, halfSize, 1.0f, 1.0f, 1.0f, 1.0f)); // 7

    // 각 면의 인덱스 정의 (각 면은 2개의 삼각형으로 구성됨)
    // 앞면
    indices->Add(0); indices->Add(1); indices->Add(2);
    indices->Add(2); indices->Add(1); indices->Add(3);

    // 뒷면
    indices->Add(4); indices->Add(6); indices->Add(5);
    indices->Add(5); indices->Add(6); indices->Add(7);

    // 왼쪽면
    indices->Add(0); indices->Add(2); indices->Add(4);
    indices->Add(4); indices->Add(2); indices->Add(6);

    // 오른쪽면
    indices->Add(1); indices->Add(5); indices->Add(3);
    indices->Add(3); indices->Add(5); indices->Add(7);

    // 윗면
    indices->Add(2); indices->Add(3); indices->Add(6);
    indices->Add(6); indices->Add(3); indices->Add(7);

    // 아랫면
    indices->Add(0); indices->Add(4); indices->Add(1);
    indices->Add(1); indices->Add(4); indices->Add(5);
}

void UGeometryGenerator::CreateSphere(int32 radius, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices)
{
    vertices->Add(FVertexSimple(0.0f, 0.0f, radius, 1.0f, 1.0f, 1.0f, 1.0f));

    for (uint32 stack = 1; stack <= stackCount - 1; ++stack)
    {
        float phi = PI * stack / stackCount;
        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2 * PI * slice / sliceCount;

            vertices->Add(FVertexSimple(
                radius * sinf(phi) * sinf(theta), radius * sinf(phi) * cosf(theta), radius * cosf(phi),
                static_cast<float>(stack) / stackCount, static_cast<float>(stack) / stackCount, 1.0f - static_cast<float>(stack) / stackCount, 1.0f)
            );
        }
    }

    vertices->Add(FVertexSimple(0.0f, 0.0f, -radius, 1.0f, 1.0f, 1.0f, 1.0f));
  
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        indices->Add(0);
        indices->Add((slice + 1) % sliceCount + 1);
        indices->Add(slice + 1);
    }

    for (uint32 stack = 0; stack < stackCount - 1; ++stack)
    {
        uint32 baseIndex = 1 + stack * (sliceCount + 1);
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            indices->Add(baseIndex + slice);
            indices->Add(baseIndex + slice + 1);
            indices->Add(baseIndex + slice + (sliceCount + 1));

            indices->Add(baseIndex + slice + 1);
            indices->Add(baseIndex + slice + (sliceCount + 1) + 1);
            indices->Add(baseIndex + slice + (sliceCount + 1));
        }
    }

    uint32 bottomIndex = vertices->Num() - 1;
    uint32 lastStackBaseIndex = bottomIndex - (sliceCount + 1);
    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        indices->Add(bottomIndex);
        indices->Add(lastStackBaseIndex + slice);
        indices->Add(lastStackBaseIndex + slice + 1);
    }
}

void UGeometryGenerator::CreateCylinder(float bRadius, float tRadius, float height, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices)
{

    float stackHeight = height / stackCount;
    float radiusStep = (tRadius - bRadius) / stackCount;

    // Vertex data
    for (uint32 stack = 0; stack <= stackCount; ++stack)
    {
        float y = -0.5f * height + stack * stackHeight;
        float r = bRadius + stack * radiusStep;

        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / sliceCount;

            vertices->Add(
                FVertexSimple(
                    r * sinf(theta),
                    r * cosf(theta),
                    y,
                    static_cast<float>(stack) / stackCount,
                    static_cast<float>(slice) / sliceCount,
                    1.0f - static_cast<float>(stack) / stackCount,
                    1.0f
                )
            );
        }
    }

    // Index data
    for (uint32 stack = 0; stack < stackCount; ++stack)
    {
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            uint32 baseIndex = stack * (sliceCount + 1) + slice;
            indices->Add(baseIndex);
            indices->Add(baseIndex + (sliceCount + 1));
            indices->Add(baseIndex + 1);
                   
            indices->Add(baseIndex + 1);
            indices->Add(baseIndex + (sliceCount + 1));
            indices->Add(baseIndex + (sliceCount + 1) + 1);
        }
    }

    // Top side
    uint32 topCenterIndex = vertices->Num();
    vertices->Add(FVertexSimple(
        0.0f, 0.0f, height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    ));

    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount;

        vertices->Add(FVertexSimple(
            tRadius * sinf(theta),
            tRadius * cosf(theta),
            height / 2.0f,
            static_cast<float>(slice) / sliceCount,
            1.0f - static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f
        ));
    }

    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        indices->Add(topCenterIndex);
        indices->Add(topCenterIndex + (slice + 1) % sliceCount + 1);
        indices->Add(topCenterIndex + slice + 1);
    }

    // Bottom side
    uint32 bottomCenterIndex = vertices->Num();
    vertices->Add(FVertexSimple(
        0.0f, 0.0f, -height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    ));

    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount;

        vertices->Add(FVertexSimple(
            bRadius * sinf(theta),
            bRadius * cosf(theta),
            -height / 2.0f,
            static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / sliceCount,
            1.0f
        ));
    }

    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        indices->Add(bottomCenterIndex);
        indices->Add(bottomCenterIndex + slice + 1);
        indices->Add(bottomCenterIndex + (slice + 1) % sliceCount + 1);
    }
}

void UGeometryGenerator::CreateCone(float bottomRadius, float height, uint32 sliceCount, uint32 stackCount, TArray<FVertexSimple>* vertices, TArray<uint32>* indices)
{
    vertices->Add(FVertexSimple(0.0f, 0.0f, -height / 2.0f, 1.0f, 1.0f, 1.0f, 1.0f));

    for (uint32 slice = 0; slice <= sliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / sliceCount;

        vertices->Add(FVertexSimple(
            bottomRadius * sinf(theta),
            bottomRadius * cosf(theta),
            - height / 2.0f,
            static_cast<float>(slice) / sliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / sliceCount,
            1.0f
        ));
    }

    for (uint32 slice = 0; slice < sliceCount; ++slice)
    {
        indices->Add(0);
        indices->Add(slice + 1);
        indices->Add((slice + 1) % sliceCount + 1);
    }

    uint32 topIndex = vertices->Num();
    vertices->Add(FVertexSimple(
        0.0f, 0.0f, height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    ));

    for (uint32 stack = 0; stack <= stackCount; ++stack)
    {
        float y = -height / 2.0f + (height * stack / stackCount);
        float r = bottomRadius * (1.0f - static_cast<float>(stack) / stackCount);

        for (uint32 slice = 0; slice <= sliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / sliceCount;

            vertices->Add(FVertexSimple(
                r * sinf(theta),
                r * cosf(theta),
                y,
                static_cast<float>(stack) / stackCount,
                static_cast<float>(slice) / sliceCount,
                1.0f - static_cast<float>(stack) / stackCount,
                1.0f
            ));
        }
    }

    for (uint32 stack = 0; stack < stackCount; ++stack)
    {
        for (uint32 slice = 0; slice < sliceCount; ++slice)
        {
            uint32 baseIndex = topIndex + 1 + stack * (sliceCount + 1) + slice;

            indices->Add(topIndex);
            indices->Add(baseIndex + 1);
            indices->Add(baseIndex);

            if (stack < stackCount - 1)
            {
                indices->Add(baseIndex);
                indices->Add(baseIndex + 1);
                indices->Add(baseIndex + (sliceCount + 1));

                indices->Add(baseIndex + 1);
                indices->Add(baseIndex + (sliceCount + 1) + 1);
                indices->Add(baseIndex + (sliceCount + 1));
            }
        }
    }
}