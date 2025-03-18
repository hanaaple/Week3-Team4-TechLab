#include "UParticleSubUVComponent.h"
#include "Core/Engine.h"
#include "Core/Rendering/FDevice.h"
#include "Core/Rendering/URenderer.h"
#include "Object/Actor/Actor.h"
#include "Object/World/World.h"
#include "Object/Actor/Camera.h"

struct alignas(16) FSubUVConstantsBufferData
{
	FMatrix MVP;
	FVector4 Color;
	// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
	uint32 bUseVertexColor;
	FVector Padding;
};

UParticleSubUVComponent::UParticleSubUVComponent()
{
	bLoop = true;
	bIsPlaying = false;
	bIsPicked = false;
	PlayRate = 1.0f;
	CurrentTime = 0.0f;
	CurrentFrame = 0;
	TotalFrames = 0;
	//TextureInfo = Texture;

	VertexBuffer = FVertexBuffer::Find("TextureQuad");
	IndexBuffer = FIndexBuffer::Find("TextureQuad");
	GetRenderResourceCollection().SetConstantBufferBinding("FSubUVVertexConstantsData", &GetVertexConstantsData(), 3, true, false);
	GetRenderResourceCollection().SetConstantBufferBinding("FSubUVPixelConstantsData", &GetPixelConstantsData(), 4, true, false);

	if (VertexBuffer == nullptr)
	{
		FVertexTexture tempArray[] =
		{
			{  0.0f, -1.0f, 1.0f, 0.0f, 0.0f },
			{  0.0f, 1.0f, 1.0f, 0.0f, 1.0f },
			{  0.0f, 1.0f, -1.0f, 1.0f, 1.0f },
			{  0.0f, -1.0f, -1.0f, 1.0f, 0.0f }
		};

		TArray<FVertexTexture> vertices;

		vertices.Add(tempArray[0]);
		vertices.Add(tempArray[1]);
		vertices.Add(tempArray[2]);
		vertices.Add(tempArray[3]);

		uint32 QuadIndices[6] =
		{
			0, 1, 2,
			0, 2, 3
		};

		TArray<uint32> indices;
		indices.Add(QuadIndices[0]);
		indices.Add(QuadIndices[1]);
		indices.Add(QuadIndices[2]);
		indices.Add(QuadIndices[3]);
		indices.Add(QuadIndices[4]);
		indices.Add(QuadIndices[5]);

		VertexBuffer = FVertexBuffer::Create(FString("TextureQuad"), vertices);
		IndexBuffer = FIndexBuffer::Create(FString("TextureQuad"), indices);
	}

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// 버텍스 쉐이더 컴파일
	D3DCompileFromFile(L"Shaders/Font_VS.hlsl", nullptr, nullptr, "Font_VS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

	// 픽셀 쉐이더 컴파일
	D3DCompileFromFile(L"Shaders/SubUV_PS.hlsl", nullptr, nullptr, "SubUV_PS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

	// 쉐이더 생성
	FDevice::Get().GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &VertexShader);
	FDevice::Get().GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &PixelShader);

	// 입력 레이아웃 설정
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	FDevice::Get().GetDevice()->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &InputLayout);

	BlendState = FBlendState::Find("DefaultBlendState");
	DepthStencilStat = FDepthStencilState::Find("DefaultDepthStencilState");
	Rasterizer = FRasterizer::Find("DefaultRasterizer");
	ConstantBuffer = FConstantBuffer::Find("DefaultConstantBuffer");
}

//UParticleSubUVComponent::UParticleSubUVComponent(FSubUVTextureInfo& Texture)
//{
//	bLoop = true;
//	bIsPlaying = false;
//	bIsPicked = false;
//	PlayRate = 1.0f;
//	CurrentTime = 0.0f;
//	CurrentFrame = 0;
//	TotalFrames = 0;
//	TextureInfo = Texture;
//
//	VertexBuffer = FVertexBuffer::Find("Quad");
//	IndexBuffer = FIndexBuffer::Find("Quad");
//
//	if (VertexBuffer == nullptr)
//	{
//		FVertexSimple tempArray[] =
//		{
//			{  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
//			{  0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
//			{  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
//			{  0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
//		};
//
//		TArray<FVertexSimple> vertices;
//
//		vertices.Add(tempArray[0]);
//		vertices.Add(tempArray[1]);
//		vertices.Add(tempArray[2]);
//		vertices.Add(tempArray[3]);
//
//		uint32 QuadIndices[6] =
//		{
//			0, 1, 2,
//			0, 2, 3
//		};
//
//		TArray<uint32> indices;
//		indices.Add(QuadIndices[0]);
//		indices.Add(QuadIndices[1]);
//		indices.Add(QuadIndices[2]);
//		indices.Add(QuadIndices[3]);
//		indices.Add(QuadIndices[4]);
//		indices.Add(QuadIndices[5]);
//
//		VertexBuffer = FVertexBuffer::Create(FString("Quad"), vertices);
//		IndexBuffer = FIndexBuffer::Create(FString("Quad"), indices);
//	}
//
//	ID3DBlob* vsBlob = nullptr;
//	ID3DBlob* psBlob = nullptr;
//	ID3DBlob* errorBlob = nullptr;
//
//	// 버텍스 쉐이더 컴파일
//	D3DCompileFromFile(L"Shaders/Font_VS.hlsl", nullptr, nullptr, "Font_VS", "vs_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);
//
//	// 픽셀 쉐이더 컴파일
//	D3DCompileFromFile(L"Shaders/SubUV_PS.hlsl", nullptr, nullptr, "SubUV_PS", "ps_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);
//
//	// 쉐이더 생성
//	FDevice::Get().GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &VertexShader);
//	FDevice::Get().GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &PixelShader);
//
//	// 입력 레이아웃 설정
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	UINT numElements = ARRAYSIZE(layout);
//
//	FDevice::Get().GetDevice()->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &InputLayout);
//
//	BlendState = FBlendState::Find("DefaultBlendState");
//	DepthStencilStat = FDepthStencilState::Find("DefaultDepthStencilState");
//	Rasterizer = FRasterizer::Find("DefaultRasterizer");
//	//ConstantBuffer = FConstantBuffer::Find("DefaultConstantBuffer");
//}

void UParticleSubUVComponent::BeginPlay()
{
	//USceneComponent::BeginPlay();

	if (TextureInfo.NumRows > 0 && TextureInfo.NumColumns > 0)
	{
		TextureInfo.FrameWidth = 1.0f / TextureInfo.NumColumns;
		TextureInfo.FrameHeight = 1.0f / TextureInfo.NumRows;
		TotalFrames = TextureInfo.NumRows * TextureInfo.NumColumns;
	}
}

void UParticleSubUVComponent::Tick(float DeltaTime)
{
	USceneComponent::Tick(DeltaTime);

	if (bIsPlaying && TotalFrames > 0)
	{
		CurrentTime += DeltaTime * PlayRate;

		float frameCount = CurrentTime * TotalFrames;

		if (bLoop)
		{
			// 루핑 애니메이션
			CurrentFrame = static_cast<uint32>(frameCount) % TotalFrames;
		}
		else
		{
			CurrentFrame = static_cast<uint32>(frameCount);
			if (CurrentFrame >= TotalFrames)
			{
				CurrentFrame = TotalFrames - 1;
				bIsPlaying = false;
			}
		}
	}

	Render();
}

void UParticleSubUVComponent::CalculateModelMatrix(FMatrix& OutMatrix)
{
	ACamera* cam = UEngine::Get().GetWorld()->GetCamera();

	FVector cameraPosition = cam->GetActorTransform().GetPosition();


	FVector objectPosition = GetWorldTransform().GetPosition();
	FVector objectScale = GetWorldTransform().GetScale();

	FVector lookDir = (cameraPosition - objectPosition).GetSafeNormal();
	FVector right = FVector(0, 0, 1).Cross(lookDir).GetSafeNormal();

	FVector up = lookDir.Cross(right).GetSafeNormal();
	FMatrix rotationMatrix;

	rotationMatrix.X = FVector4(lookDir.X, lookDir.Y, lookDir.Z, 0);
	rotationMatrix.Y = FVector4(right.X, right.Y, right.Z, 0);
	rotationMatrix.Z = FVector4(up.X, up.Y, up.Z, 0);
	rotationMatrix.W = FVector4(0, 0, 0, 1);

	FMatrix positionMatrix = FMatrix::GetTranslateMatrix(objectPosition);
	FMatrix scaleMatrix = FMatrix::GetScaleMatrix(objectScale);

	OutMatrix = scaleMatrix * rotationMatrix * positionMatrix;

	return;
}

void UParticleSubUVComponent::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer == nullptr)
	{
		return;
	}

	FMatrix ModelMatrix;
	CalculateModelMatrix(ModelMatrix);

	const FMatrix& ViewProjectionMatrix = UEngine::Get().GetWorld()->GetCamera()->GetViewProjectionMatrix();

	FMatrix MVP = FMatrix::Transpose(
		ModelMatrix *
		ViewProjectionMatrix
	);

	FSubUVVertexConstantsData& Data = GetVertexConstantsData();

	Data = {
		MVP,
	};

	VertexBuffer->Setting();
	IndexBuffer->Setting();
	FDevice::Get().GetDeviceContext()->VSSetShader(VertexShader, nullptr, 0);
	FDevice::Get().GetDeviceContext()->PSSetShader(PixelShader, nullptr, 0);
	FDevice::Get().GetDeviceContext()->IASetInputLayout(InputLayout);
	DepthStencilStat->Setting();
	Rasterizer->Setting();
	BlendState->Setting();
	//PrimitiveComp.ConstantBufferBinding->Setting();
	FDevice::Get().GetDeviceContext()->IASetPrimitiveTopology(Topology);
	FDevice::Get().GetDeviceContext()->DrawIndexed(IndexBuffer->GetIndexCount(), 0, 0);
}

void UParticleSubUVComponent::Play()
{
	bIsPlaying = true;
}

void UParticleSubUVComponent::Pause()
{
	bIsPlaying = false;
}

void UParticleSubUVComponent::Stop()
{
	bIsPlaying = false;
	CurrentTime = 0.0f;
	CurrentFrame = 0;
}

void UParticleSubUVComponent::SetPlayRate(float NewRate)
{
	PlayRate = NewRate;
}

void UParticleSubUVComponent::SetLooping(bool bShouldLoop)
{
	bLoop = bShouldLoop;
}

FVector4 UParticleSubUVComponent::GetCurrentFrameUV() const
{
	if (TotalFrames == 0)
	{
		return FVector4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	// 현재 프레임의 행과 열 계산
	uint32 row = CurrentFrame / TextureInfo.NumColumns;
	uint32 col = CurrentFrame % TextureInfo.NumColumns;

	float u1 = col * TextureInfo.FrameWidth;
	float v1 = row * TextureInfo.FrameHeight;
	float u2 = u1 + TextureInfo.FrameWidth;
	float v2 = v1 + TextureInfo.FrameHeight;

	return FVector4(u1, v1, u2, v2);
}
