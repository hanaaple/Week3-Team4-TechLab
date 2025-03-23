#include "UGeometryGenerator.h"
#include <cmath>

void UGeometryGenerator::CreateCube(float Size, TArray<FVertexSimple>& OutVertices, TArray<uint32>& OutIndices)
{
    // 정육면체의 반 크기
    float halfSize = Size / 2.0f;

    // 정육면체의 8개 정점 정의
	OutVertices.Emplace(-halfSize, -halfSize, -halfSize, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f); // 0
	OutVertices.Emplace(-halfSize, -halfSize, halfSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f); // 1
	OutVertices.Emplace(-halfSize, halfSize, -halfSize, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f); // 2
	OutVertices.Emplace(-halfSize, halfSize, halfSize, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f); // 3
	OutVertices.Emplace(halfSize, -halfSize, -halfSize, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f); // 4
	OutVertices.Emplace(halfSize, -halfSize, halfSize, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f); // 5
	OutVertices.Emplace(halfSize, halfSize, -halfSize, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f); // 6
	OutVertices.Emplace(halfSize, halfSize, halfSize, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f); // 7

    // 각 면의 인덱스 정의 (각 면은 2개의 삼각형으로 구성됨)
    // 앞면
    OutIndices.Add(0); OutIndices.Add(1); OutIndices.Add(2);
    OutIndices.Add(2); OutIndices.Add(1); OutIndices.Add(3);

    // 뒷면
    OutIndices.Add(4); OutIndices.Add(6); OutIndices.Add(5);
    OutIndices.Add(5); OutIndices.Add(6); OutIndices.Add(7);

    // 왼쪽면
    OutIndices.Add(0); OutIndices.Add(2); OutIndices.Add(4);
    OutIndices.Add(4); OutIndices.Add(2); OutIndices.Add(6);

    // 오른쪽면
    OutIndices.Add(1); OutIndices.Add(5); OutIndices.Add(3);
    OutIndices.Add(3); OutIndices.Add(5); OutIndices.Add(7);

    // 윗면
    OutIndices.Add(2); OutIndices.Add(3); OutIndices.Add(6);
    OutIndices.Add(6); OutIndices.Add(3); OutIndices.Add(7);

    // 아랫면
    OutIndices.Add(0); OutIndices.Add(4); OutIndices.Add(1);
    OutIndices.Add(1); OutIndices.Add(4); OutIndices.Add(5);
}

void UGeometryGenerator::CreateSphere(float Radius, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>& OutVertices, TArray<uint32>& OutIndices)
{
    OutVertices.Emplace(0.0f, 0.0f, Radius, 1.0f, 1.0f, 1.0f, 1.0f);

    for (uint32 stack = 1; stack <= StackCount - 1; ++stack)
    {
        float phi = PI * stack / StackCount;
        for (uint32 slice = 0; slice <= SliceCount; ++slice)
        {
            float theta = 2 * PI * slice / SliceCount;

            OutVertices.Emplace(
                Radius * sinf(phi) * sinf(theta), Radius * sinf(phi) * cosf(theta), Radius * cosf(phi),
                static_cast<float>(stack) / StackCount, static_cast<float>(stack) / StackCount, 1.0f - static_cast<float>(stack) / StackCount, 1.0f
            );
        }
    }

	uint32 bottomIndex = OutVertices.Num();
    OutVertices.Emplace(0.0f, 0.0f, -Radius, 1.0f, 1.0f, 1.0f, 1.0f);
  
    for (uint32 slice = 0; slice < SliceCount; ++slice)
    {
        OutIndices.Add(0);
        OutIndices.Add((slice + 1) % SliceCount + 1);
        OutIndices.Add(slice + 1);
    }

    for (uint32 stack = 0; stack < StackCount - 2; ++stack)
    {
        uint32 baseIndex = 1 + stack * (SliceCount + 1);
        for (uint32 slice = 0; slice < SliceCount; ++slice)
        {
            OutIndices.Add(baseIndex + slice);
            OutIndices.Add(baseIndex + slice + 1);
            OutIndices.Add(baseIndex + slice + (SliceCount + 1));

            OutIndices.Add(baseIndex + slice + 1);
            OutIndices.Add(baseIndex + slice + (SliceCount + 1) + 1);
            OutIndices.Add(baseIndex + slice + (SliceCount + 1));
        }
    }

    uint32 lastStackBaseIndex = bottomIndex - (SliceCount + 1);
    for (uint32 slice = 0; slice < SliceCount; ++slice)
    {
        OutIndices.Add(bottomIndex);
        OutIndices.Add(lastStackBaseIndex + slice);
        OutIndices.Add(lastStackBaseIndex + slice + 1);
    }
}

void UGeometryGenerator::CreateCylinder(float BottomRadius, float TopRadius, float Height, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>&
                                        OutVertices, TArray<uint32>& OutIndices
)
{

    float stackHeight = Height / StackCount;
    float radiusStep = (TopRadius - BottomRadius) / StackCount;

    // Vertex data
    for (uint32 stack = 0; stack <= StackCount; ++stack)
    {
        float y = -0.5f * Height + stack * stackHeight;
        float r = BottomRadius + stack * radiusStep;

        for (uint32 slice = 0; slice <= SliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / SliceCount;

            OutVertices.Emplace(
                r * sinf(theta),
                r * cosf(theta),
                y,
                static_cast<float>(stack) / StackCount,
                static_cast<float>(slice) / SliceCount,
                1.0f - static_cast<float>(stack) / StackCount,
                1.0f
            );
        }
    }

    // Index data
    for (uint32 stack = 0; stack < StackCount; ++stack)
    {
        for (uint32 slice = 0; slice < SliceCount; ++slice)
        {
            uint32 baseIndex = stack * (SliceCount + 1) + slice;
            OutIndices.Add(baseIndex);
            OutIndices.Add(baseIndex + (SliceCount + 1));
            OutIndices.Add(baseIndex + 1);
                   
            OutIndices.Add(baseIndex + 1);
            OutIndices.Add(baseIndex + (SliceCount + 1));
            OutIndices.Add(baseIndex + (SliceCount + 1) + 1);
        }
    }

    // Top side
    uint32 topCenterIndex = OutVertices.Num();
    OutVertices.Emplace(
        0.0f, 0.0f, Height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );

    for (uint32 slice = 0; slice <= SliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / SliceCount;

        OutVertices.Emplace(
            TopRadius * sinf(theta),
            TopRadius * cosf(theta),
            Height / 2.0f,
            static_cast<float>(slice) / SliceCount,
            1.0f - static_cast<float>(slice) / SliceCount,
            0.0f,
            1.0f
        );
    }

    for (uint32 slice = 0; slice < SliceCount; ++slice)
    {
        OutIndices.Add(topCenterIndex);
        OutIndices.Add(topCenterIndex + (slice + 1) % SliceCount + 1);
        OutIndices.Add(topCenterIndex + slice + 1);
    }

    // Bottom side
    uint32 bottomCenterIndex = OutVertices.Num();
    OutVertices.Emplace(
        0.0f, 0.0f, -Height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );

    for (uint32 slice = 0; slice <= SliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / SliceCount;

        OutVertices.Emplace(
            BottomRadius * sinf(theta),
            BottomRadius * cosf(theta),
            -Height / 2.0f,
            static_cast<float>(slice) / SliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / SliceCount,
            1.0f
        );
    }

    for (uint32 slice = 0; slice < SliceCount; ++slice)
    {
        OutIndices.Add(bottomCenterIndex);
        OutIndices.Add(bottomCenterIndex + slice + 1);
        OutIndices.Add(bottomCenterIndex + (slice + 1) % SliceCount + 1);
    }
}

void UGeometryGenerator::CreateCone(float BottomRadius, float Height, uint32 SliceCount, uint32 StackCount, TArray<FVertexSimple>& OutVertices, TArray<
                                    uint32>& OutIndices)
{
    OutVertices.Emplace(0.0f, 0.0f, -Height / 2.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    for (uint32 slice = 0; slice <= SliceCount; ++slice)
    {
        float theta = 2.0f * PI * slice / SliceCount;

        OutVertices.Emplace(
            BottomRadius * sinf(theta),
            BottomRadius * cosf(theta),
            - Height / 2.0f,
            static_cast<float>(slice) / SliceCount,
            0.0f,
            1.0f - static_cast<float>(slice) / SliceCount,
            1.0f
        );
    }

    for (uint32 slice = 0; slice < SliceCount; ++slice)
    {
        OutIndices.Add(0);
        OutIndices.Add(slice + 1);
        OutIndices.Add((slice + 1) % SliceCount + 1);
    }

    uint32 topIndex = OutVertices.Num();
    OutVertices.Emplace(
        0.0f, 0.0f, Height / 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    );

    for (uint32 stack = 0; stack <= StackCount; ++stack)
    {
        float y = -Height / 2.0f + (Height * stack / StackCount);
        float r = BottomRadius * (1.0f - static_cast<float>(stack) / StackCount);

        for (uint32 slice = 0; slice <= SliceCount; ++slice)
        {
            float theta = 2.0f * PI * slice / SliceCount;

            OutVertices.Emplace(
                r * sinf(theta),
                r * cosf(theta),
                y,
                static_cast<float>(stack) / StackCount,
                static_cast<float>(slice) / SliceCount,
                1.0f - static_cast<float>(stack) / StackCount,
                1.0f
            );
        }
    }

    for (uint32 stack = 0; stack < StackCount; ++stack)
    {
        for (uint32 slice = 0; slice < SliceCount; ++slice)
        {
            uint32 baseIndex = topIndex + 1 + stack * (SliceCount + 1) + slice;

            OutIndices.Add(topIndex);
            OutIndices.Add(baseIndex + 1);
            OutIndices.Add(baseIndex);

            if (stack < StackCount - 1)
            {
                OutIndices.Add(baseIndex);
                OutIndices.Add(baseIndex + 1);
                OutIndices.Add(baseIndex + (SliceCount + 1));

                OutIndices.Add(baseIndex + 1);
                OutIndices.Add(baseIndex + (SliceCount + 1) + 1);
                OutIndices.Add(baseIndex + (SliceCount + 1));
            }
        }
    }
}

void UGeometryGenerator::CreateRadialCone(float Height, float Angle, uint32 SliceCount, TArray<FVertexSimple>* Vertices, TArray<uint32>* Indices)
{
	float radius = Height * tanf(Angle * 0.5f);

	float r = 1.0f;
	float g = 1.0f;
	float b = 0.0f;
	float a = 1.0f;

	// 꼭대기 정점
	uint32 apexIndex = Vertices->Num();
	Vertices->Add(FVertexSimple(
		0.0f, 0.0f, 0.0f,
		r, g, b, a
	));

	// 원뿔 바닥의 원 정점들 생성
	uint32 baseStartIndex = Vertices->Num();
	for (uint32 slice = 0; slice <= SliceCount; ++slice)
	{
		float theta = 2.0f * PI * slice / SliceCount;

		Vertices->Add(FVertexSimple(
			radius * sinf(theta),
			radius * cosf(theta),
			Height,
			r, g, b, a
		));
	}

	// 원뿔의 옆면 인덱스 (삼각형)
	for (uint32 slice = 0; slice < SliceCount; ++slice)
	{
		Indices->Add(apexIndex);
		Indices->Add(baseStartIndex + slice);
		Indices->Add(baseStartIndex + slice + 1);
	}
}