#include "FLineBatchManager.h"
#include "Core/Engine.h"
#include "Core/Rendering/URenderer.h"
#include <d3dcompiler.h>

#include "Object/Actor/Camera.h"
#include "Object/World/World.h"

void FLineBatchManager::AddLine(const FVector& Start, const FVector& End, const FVector4& Color, float Thickness)
{
	// 버텍스 버퍼에 두 정점 추가
	uint32 Index = VertexBuffer.Num();

	VertexBuffer.Add({ (Start), Color });
	VertexBuffer.Add({ (End) , Color });

	// 인덱스 버퍼에 두 인덱스 추가
	IndexBuffer.Add(Index);
	IndexBuffer.Add(Index + 1);

	// 버퍼가 가득 차서 플러시가 필요한지 확인
	if (VertexBuffer.Num() >= MaxVerticesPerBatch)
	{
		Flush();
	}
}

void FLineBatchManager::Flush()
{
}

void FLineBatchManager::DrawWorldGrid(float GridSize, float GridSpacing, const FVector4& GridColor, bool bCenterGrid)
{

	VertexBuffer.Empty();
	IndexBuffer.Empty();

	// 그리드 크기 및 라인 개수 계산
	int32 LineCount = FMath::CeilToInt(GridSize / GridSpacing) + 1;
	float HalfGridSize = GridSize * 0.5f;
	float StartOffset = bCenterGrid ? -HalfGridSize : 0.0f;
	float EndOffset = bCenterGrid ? HalfGridSize : GridSize;

	// X축(전방/후방) 라인 추가 (X-Y 평면, Z=0)
	for (int32 i = 0; i < LineCount; ++i)
	{
		float Position = StartOffset + i * GridSpacing;
		// Y 위치를 변경하면서 X축을 따라 그리기
		FVector StartPoint(StartOffset, Position, 0.0f); // X-Y 평면
		FVector EndPoint(EndOffset, Position, 0.0f);     // X-Y 평면

		// Y=0인 라인은 X축이므로 강조
		FVector4 LineColor = (FMath::Abs(Position) < KINDA_SMALL_NUMBER)
			? FVector4::RED // X축 강조
			: GridColor;

		AddLine(StartPoint, EndPoint, LineColor);
	}

	// Y축(좌/우) 라인 추가 (X-Y 평면, Z=0)
	for (int32 i = 0; i < LineCount; ++i)
	{
		float Position = StartOffset + i * GridSpacing;
		// X 위치를 변경하면서 Y축을 따라 그리기
		FVector StartPoint(Position, StartOffset, 0.0f); // X-Y 평면
		FVector EndPoint(Position, EndOffset, 0.0f);     // X-Y 평면

		// X=0인 라인은 Y축이므로 강조
		FVector4 LineColor = (FMath::Abs(Position) < KINDA_SMALL_NUMBER)
			? FVector4::GREEN // Y축 강조
			: GridColor;

		AddLine(StartPoint, EndPoint, LineColor);
	}

	// 원점 강조와 Z축 표시
	if (bCenterGrid)
	{
		// Z축 표시 (위쪽 방향)
		AddLine(FVector(0, 0, 0), FVector(0, 0, GridSpacing * 10.0f), FVector4::BLUE);

		// 원점에 작은 마커 추가 (옵션)
		float MarkerSize = GridSpacing * 0.25f;
		AddLine(FVector(-MarkerSize, 0, 0), FVector(MarkerSize, 0, 0), FVector4::MAGENTA);
		AddLine(FVector(0, -MarkerSize, 0), FVector(0, MarkerSize, 0), FVector4::CYAN);
	}
}

void FLineBatchManager::Render()
{
	//return;
	if (VertexBuffer.Num() == 0)
		return;

	//Prepare
	ID3D11DeviceContext* DeviceContext = UEngine::Get().GetRenderer()->GetDeviceContext();
	


	// 버텍스 버퍼 업데이트
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DeviceContext->Map(LineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, VertexBuffer.GetData(), sizeof(FLineVertexSimple) * VertexBuffer.Num());
	DeviceContext->Unmap(LineVertexBuffer, 0);

	// 인덱스 버퍼 업데이트
	DeviceContext->Map(LineIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, IndexBuffer.GetData(), sizeof(uint32) * IndexBuffer.Num());
	DeviceContext->Unmap(LineIndexBuffer, 0);

	// 파이프라인 상태 설정
	UINT stride = sizeof(FLineVertexSimple);
	UINT offset = 0;

	// 기본 셰이더랑 InputLayout을 설정

	DeviceContext->IASetVertexBuffers(0, 1, &LineVertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(LineIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(LinePrimitiveTopology);



	DeviceContext->VSSetShader(LineVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(LinePixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(LineInputLayout);


	// 버텍스 쉐이더에 상수 버퍼를 설정
	if (LineConstantBuffer)
	{
		DeviceContext->VSSetConstantBuffers(0, 1, &LineConstantBuffer);


		D3D11_MAPPED_SUBRESOURCE ConstantBufferMSR;

		FLineConstantInfo Constants; 

		URenderer* Renderer = UEngine::Get().GetRenderer();

		Constants.ViewProjectionMatrix = FMatrix::Transpose(UEngine::Get().GetWorld()->GetCamera()->GetViewProjectionMatrix());



		// D3D11_MAP_WRITE_DISCARD는 이전 내용을 무시하고 새로운 데이터로 덮어쓰기 위해 사용
		DeviceContext->Map(LineConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ConstantBufferMSR);
		{
			memcpy(ConstantBufferMSR.pData, &Constants, sizeof(FLineConstantInfo));
		}
		DeviceContext->Unmap(LineConstantBuffer, 0);
	}




	DeviceContext->DrawIndexed((UINT)IndexBuffer.Num(), 0, 0);

}

void FLineBatchManager::Create()
{
	ID3D11Device* Device = UEngine::Get().GetRenderer()->GetDevice();
	ID3D11DeviceContext* DeviceContext = UEngine::Get().GetRenderer()->GetDeviceContext();

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(FLineVertexSimple) * MaxVerticesPerBatch;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 버텍스 버퍼 생성
	HRESULT hr = Device->CreateBuffer(&vertexBufferDesc, nullptr, &LineVertexBuffer);

	if (FAILED(hr))
	{
		return ;
	}
	// 인덱스 버퍼 설명
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.ByteWidth = sizeof(UINT) * MaxIndicesPerBatch; // LineVertex가 아닌 UINT로 수정
	IndexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // IMMUTABLE에서 DYNAMIC으로 변경 (Add 함수에서 업데이트하기 위함)
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 쓰기 접근 권한 추가

	// 인덱스 버퍼 생성
	hr = Device->CreateBuffer(&IndexBufferDesc, nullptr, &LineIndexBuffer);
	if (FAILED(hr))
	{
	}

	// 라인 쉐이더 컴파일 및 생성
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// 버텍스 쉐이더 컴파일
	hr = D3DCompileFromFile(L"Shaders\\ShaderLine_VS.hlsl", nullptr, nullptr, "ShaderLine_VS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);
	if (FAILED(hr))
	{
		// 컴파일 오류 처리
		if (errorBlob)
		{
		}
	}

	// 픽셀 쉐이더 컴파일
	hr = D3DCompileFromFile(L"Shaders\\ShaderLine_PS.hlsl", nullptr, nullptr, "ShaderLine_PS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);
	if (FAILED(hr))
	{
		// 컴파일 오류 처리
		if (errorBlob)
		{
		}
	}



	// 쉐이더 생성
	hr = Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &LineVertexShader);
	if (FAILED(hr))
	{
		// 오류 처리
	}

	hr = Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &LinePixelShader);
	if (FAILED(hr))
	{
		// 오류 처리
	}

	// 입력 레이아웃 설정
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// 입력 레이아웃 생성
	hr = Device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &LineInputLayout);
	if (FAILED(hr))
	{
		// 오류 처리
	}

	// 버텍스/픽셀 쉐이더 블롭 해제
	if (vsBlob) vsBlob->Release();
	if (psBlob) psBlob->Release();

	// 라인 렌더링을 위한 래스터라이저 상태 설정
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE; // 라인은 양면을 볼 수 있도록 컬링 없음
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = TRUE; // 안티앨리어싱된 라인 활성화

	// 래스터라이저 상태 생성
	hr = Device->CreateRasterizerState(&rasterizerDesc, &LineRasterizerState);
	if (FAILED(hr))
	{
		// 오류 처리
	}

	// 상수 버퍼 생성
	D3D11_BUFFER_DESC ConstantBufferDesc = {};
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                        // 매 프레임 CPU에서 업데이트 하기 위해
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;             // 상수 버퍼로 설정
	ConstantBufferDesc.ByteWidth = (sizeof(FLineConstantInfo) + 0xf) & 0xfffffff0;  // 16byte의 배수로 올림
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;            // CPU에서 쓰기 접근이 가능하게 설정

	Device->CreateBuffer(&ConstantBufferDesc, nullptr, &LineConstantBuffer);

	// 기본 토폴로지는 이미 D3D11_PRIMITIVE_TOPOLOGY_LINELIST로 초기화됨
	// 따로 설정할 필요 없음

}


