#include "FDevice.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/InputLayout.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/Rasterizer.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/ConstantBuffer.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Primitive/UGeometryGenerator.h"


void FDevice::InitResource()
{
	std::shared_ptr<FVertexShader> VS = FVertexShader::Load(L"Shaders/ShaderW0.hlsl","Simple_VS","mainVS");
	FInputLayout::Create("Simple_VS" , VS);
	FPixelShader::Load(L"Shaders/ShaderW0.hlsl","Simple_PS","mainPS");

	FPixelShader::Load(L"Shaders/ShaderW0.hlsl","Picking_PS","PickingPS");
	{
		
		std::shared_ptr<FVertexShader> VS = FVertexShader::Load(L"Shaders/Font_VS.hlsl","Font_VS","Font_VS");
		//FInputLayout::Create("Font_VS" , VS);
	}

	FConstantBuffer::Create("DefaultConstantBuffer", sizeof(FConstantsComponentData));

	
	//FPixelShader::Load(L"Shaders/Font_PS.hlsl","Font_PS","Font_PS");


	
	
	D3D11_RASTERIZER_DESC RasterizerDesc = {};
	RasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	RasterizerDesc.CullMode = D3D11_CULL_BACK;  // 백 페이스 컬링
	RasterizerDesc.FrontCounterClockwise = FALSE;
	
	FRasterizer::Create("DefaultRasterizer", RasterizerDesc);

	{
		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
		DepthStencilDesc.DepthEnable = true;
		// 깊이 테스트만 하고 안쓸수도 있다.
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		DepthStencilDesc.StencilEnable = false;

		FDepthStencilState::Create("DefaultDepthStencilState", DepthStencilDesc);
	}
	

	{
		// Blend
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		// src srcColor * src의 알파
		// 1, 0, 0(, 1) * 1.0f
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		FBlendState::Create("DefaultBlendState", blendDesc);
	}
	
	{
		// Blend
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		// src srcColor * src의 알파
		// 1, 0, 0(, 1) * 1.0f
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		//안개처럼 알파값이 계속 누적됨
		FBlendState::Create("AddBlendState", blendDesc);
	}

	////Material
	{
		std::shared_ptr<FMaterial> Mat = FMaterial::Create("DefaultMaterial");
		Mat->SetRasterizer("DefaultRasterizer");
		Mat->SetBlendState("DefaultBlendState");
		Mat->SetDepthState("DefaultDepthStencilState");
		Mat->SetPixelShader("Simple_PS");
		Mat->SetVertexShader("Simple_VS");
	}
	
	/// Mesh
	{
		TArray<FVertexSimple> vertices;
		TArray<uint32> indices;
		float size = 1.f;

		UGeometryGenerator::CreateCube(size, &vertices, &indices);
		
		FVertexBuffer::Create(FString("Cube"), vertices);
		FIndexBuffer::Create(FString("Cube"), indices);
		FMesh::Create("Cube");
	}

	{
		TArray<FVertexSimple> vertices;
		TArray<uint32> indices;
		int slices = 16;
		int stacks = 16;
		int32 radius = 1.f;
		float height = 1.f;

		UGeometryGenerator::CreateSphere(radius, slices, stacks, &vertices, &indices);
		
		FVertexBuffer::Create(FString("Sphere"), vertices);
		FIndexBuffer::Create(FString("Sphere"), indices);
		FMesh::Create("Sphere");

		
	}

	{
		FVertexSimple tempArray[] =
		{
			{  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f } 
		};
		TArray<FVertexSimple> vertices;

		vertices.Add(tempArray[0]);
		vertices.Add(tempArray[1]);
		vertices.Add(tempArray[2]);
		
		uint32 TriangleIndices[3] =
		{
			0, 1, 2
		};
		
		TArray<uint32> indices;
		indices.Add(TriangleIndices[0]);
		indices.Add(TriangleIndices[1]);
		indices.Add(TriangleIndices[2]);
		
		FVertexBuffer::Create(FString("Triangle"), vertices);
		FIndexBuffer::Create(FString("Triangle"), indices);
		FMesh::Create("Triangle");
		
	}
	
	{
		FVertexSimple tempArray[] =
		{
			{  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
			{  0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
		};

		TArray<FVertexSimple> vertices;

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

		FVertexBuffer::Create(FString("Quad"), vertices);
		FIndexBuffer::Create(FString("Quad"), indices);

		FMesh::Create("Quad");
	}

	{
		FVertexSimple tempArray[2] =
		{
			{ -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
		};
		TArray<FVertexSimple> vertices;

		vertices.Add(tempArray[0]);
		vertices.Add(tempArray[1]);
		
		uint32 tempIndices[2] =
		{
			0, 1
		};
		
		TArray<uint32> indices;
		indices.Add(tempIndices[0]);
		indices.Add(tempIndices[1]);
		
		FVertexBuffer::Create(FString("Line"), vertices);
		FIndexBuffer::Create(FString("Line"), indices);
		
		FMesh::Create(FString("Line"), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	{
		TArray<FVertexSimple> vertices;
		TArray<uint32> indices;
		int slices = 36;
		int stacks = 36;
		float bRadius = .2f;
		float tRdius = .2f;
		float height = 1.f;

		UGeometryGenerator::CreateCylinder(bRadius, tRdius, height, slices, stacks, &vertices , &indices);
		
		FVertexBuffer::Create(FString("Cylinder"), vertices);
		FIndexBuffer::Create(FString("Cylinder"), indices);
		 
		FMesh::Create(FString("Cylinder"));
	}

	{
		TArray<FVertexSimple> vertices;
		TArray<uint32> indices;
		int slices = 36;
		int stacks = 6;
		float radius = 1.f;
		float height = 1.f;

		UGeometryGenerator::CreateCone(radius, height, slices, stacks, &vertices, &indices);
		FVertexBuffer::Create(FString("Cone"), vertices);
		FIndexBuffer::Create(FString("Cone"), indices);
	}
	
	// {
	// 	FMesh::Create("Mesh_VS", "Mesh_PS", "Mesh_PS");
	// 	
	// }
	// VertexShader = FVertexShader::Find("Simple_VS");
	// PixelShader = FPixelShader::Find("Simple_PS");
	//
	// // TODO: 이거는 나중에 매쉬같은데서  만들어야함
	// InputLayout = FInputLayout::Find("Simple_VS");
	//
	// BlendState = FBlendState::Find("DefaultBlendState");
	// DepthStencilStat = FDepthStencilState::Find("DefaultDepthStencilState");
	// Rasterizer = FRasterizer::Find("DefaultRasterizer");
	// ConstantBuffer = FConstantBuffer::Find("DefaultConstantBuffer");
	//
	// ConstantBufferBinding = std::make_shared<FConstantBufferBinding>();
	//
	// //std::shared_ptr<FVertexShader> vertexShaderPtr;/* 초기화 */
	// //FShader* shaderPtr = static_cast<FShader*>(vertexShaderPtr.get()); // 내부 포인터 추출
	// ConstantBufferBinding->Res = ConstantBuffer;
	// ConstantBufferBinding->CPUDataPtr = &ConstantsComponentData;
	// ConstantBufferBinding->DataSize = sizeof(ConstantsComponentData);
	// ConstantBufferBinding->ParentShader = VertexShader.get();
	// ConstantBufferBinding->BindPoint = 0;
}
