#include "UI.h"

#include "FDevice.h"
#include "FViewMode.h"
#include "Core/Engine.h"
#include "Core/Input/PlayerInput.h"
#include "Debug/DebugConsole.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "Object/Actor/Camera.h"
#include "Object/Actor/Cone.h"
#include "Object/Actor/Cube.h"
#include "Object/Actor/Cylinder.h"
#include "Object/Actor/Sphere.h"
#include "Object/Actor/SpotLight.h"
#include "Object/Actor/StaticMeshObj.h"
#include "Object/Light/SpotLightComponent.h"
#include "Object/World/World.h"
#include "Resource/Util/TObjectIterator.h"
#include "Static/FEditorManager.h"
#include "Static/FUUIDBillBoard.h"


void UI::Initialize(HWND hWnd, const FDevice& Device, UINT ScreenWidth, UINT ScreenHeight)
{
    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // 기본 폰트 크기 설정
    io.FontGlobalScale = 1.0f;
    io.DisplaySize = ScreenSize;
    //io.WantSetMousePos = true;
    // ImGui Backend 초기화
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(Device.GetDevice(), Device.GetDeviceContext());

	ScreenSize = ImVec2(static_cast<float>(ScreenWidth), static_cast<float>(ScreenHeight));
    InitialScreenSize = ScreenSize;
    bIsInitialized = true;
    
    io.DisplaySize = ScreenSize;

    PreRatio = GetRatio();
    CurRatio = GetRatio();
}

void UI::Update()
{
    POINT mousePos;
    if (GetCursorPos(&mousePos)) {
        HWND hwnd = GetActiveWindow();
        ScreenToClient(hwnd, &mousePos);

        ImVec2 CalculatedMousePos = ResizeToScreenByCurrentRatio(ImVec2(mousePos.x, mousePos.y));
        ImGui::GetIO().MousePos = CalculatedMousePos;
        //UE_LOG("MousePos: (%.1f, %.1f), DisplaySize: (%.1f, %.1f)\n",CalculatedMousePos.x, CalculatedMousePos.y, GetRatio().x, GetRatio().y);
    }

    
    // ImGui Frame 생성
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (bWasWindowSizeUpdated)
    {
        PreRatio = CurRatio;
        CurRatio = GetRatio();
        UE_LOG("Current Ratio: %f, %f", CurRatio.x, CurRatio.y);
    }

	RenderSceneManager();
    RenderControlPanel();
    RenderPropertyWindow();
	//RenderShowFlagsPanel();
	//RenderViewModePanel()
    Debug::ShowConsole(bWasWindowSizeUpdated, PreRatio, CurRatio);

	FViewportManager* ViewportManager = UEngine::Get().GetWorld()->GetViewportManager();
	TArray<FViewport*> Viewports = ViewportManager->GetViewports();
	FViewport* FullScreenViewport = ViewportManager->GetFullScreenViewport();
	int32 index = 0;
	for (FViewport* vp : Viewports)
	{
		if (FullScreenViewport && FullScreenViewport != vp)
			continue;
		RenderViewportSettings(vp, index++);
	}

    // ImGui 렌더링
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    bWasWindowSizeUpdated = false;
}


void UI::Shutdown() const
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UI::OnUpdateWindowSize(UINT InScreenWidth, UINT InScreenHeight)
{
    // ImGUI 리소스 다시 생성
    ImGui_ImplDX11_InvalidateDeviceObjects();
    ImGui_ImplDX11_CreateDeviceObjects();
   // ImGui 창 크기 업데이트
	//ScreenSize = ImVec2(static_cast<float>(InScreenWidth), static_cast<float>(InScreenHeight));

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(InScreenWidth), static_cast<float>(InScreenHeight));

    bWasWindowSizeUpdated = true;
}

void UI::RenderControlPanel()
{
    ImGui::Begin("Jungle Control Panel");

    if (bWasWindowSizeUpdated)
    {
        auto* Window = ImGui::GetCurrentWindow();

        ImGui::SetWindowPos(ResizeToScreen(Window->Pos));
        ImGui::SetWindowSize(ResizeToScreen(Window->Size));
    }
    
    ImGui::Text("Hello, Jungle World!");
    ImGui::Text("FPS: %.3f (%.2f ms)", ImGui::GetIO().Framerate , 1000.0f / ImGui::GetIO().Framerate);

    RenderMemoryUsage();
    RenderPrimitiveSelection();
    RenderCameraSettings();
	RenderGridSettings();

    ImGui::End();
}

void UI::RenderMemoryUsage() const
{
    const uint64 ContainerAllocByte = FPlatformMemory::GetAllocationBytes<EAT_Container>();
    const uint64 ContainerAllocCount = FPlatformMemory::GetAllocationCount<EAT_Container>();
    const uint64 ObjectAllocByte = FPlatformMemory::GetAllocationBytes<EAT_Object>();
    const uint64 ObjectAllocCount = FPlatformMemory::GetAllocationCount<EAT_Object>();
    ImGui::Text(
        "Container Memory Uses: %llubyte, Count: %llu",
        ContainerAllocByte,
        ContainerAllocCount
    );
    ImGui::Text(
        "Object Memory Uses: %llubyte, Count: %llu Objects",
        ObjectAllocByte,
        ObjectAllocCount
    );
    ImGui::Text(
        "Total Memory Uses: %llubyte, Count: %llu",
        ContainerAllocByte + ObjectAllocByte,
        ContainerAllocCount + ObjectAllocCount
    );

    ImGui::Separator();
}

void UI::RenderPrimitiveSelection()
{
    const char* items[] = { "StaticMesh", "Sphere", "Cube", "Cylinder", "Cone", "SpotLight"};

    ImGui::Combo("Primitive", &currentItem, items, IM_ARRAYSIZE(items));

    if (ImGui::Button("Spawn"))
    {
        UWorld* World = UEngine::Get().GetWorld();
        for (int i = 0 ;  i < NumOfSpawn; i++)
        {
            if (strcmp(items[currentItem], "Sphere") == 0)
            {
                World->SpawnActor<ASphere>();
            }
            else if (strcmp(items[currentItem], "Cube") == 0)
            {
                World->SpawnActor<ACube>();
            }
            else if (strcmp(items[currentItem], "Cylinder") == 0)
            {
                World->SpawnActor<ACylinder>();
            }
			else if (strcmp(items[currentItem], "Cone") == 0)
			{
				World->SpawnActor<ACone>();
			}
			else if (strcmp(items[currentItem], "SpotLight") == 0)
			{
				World->SpawnActor<ASpotLight>();
			}
			else if (strcmp(items[currentItem], "StaticMesh") == 0)
			{
				World->SpawnActor<AStaticMeshObj>();
			}
            //else if (strcmp(items[currentItem], "Triangle") == 0)
            //{
            //    Actor->AddComponent<UTriangleComp>();   
            //}
        }
    }
    ImGui::SameLine();
    ImGui::InputInt("Number of spawn", &NumOfSpawn, 0);

    ImGui::Separator();

    UWorld* World = UEngine::Get().GetWorld();
    uint32 bufferSize = 100;
    char* SceneNameInput = new char[bufferSize];
    strcpy_s(SceneNameInput, bufferSize, World->SceneName.c_char());
    
	if (ImGui::InputText("Scene Name", SceneNameInput, bufferSize))
	{
		World->SceneName = SceneNameInput;
	}
    
    if (ImGui::Button("New Scene"))
    {
        World->ClearWorld();
    }
    if (ImGui::Button("Save Scene"))
    {
        World->SaveWorld();   
    }
    if (ImGui::Button("Load Scene"))
    {
        World->LoadWorld(SceneNameInput);
    }

	delete[] SceneNameInput;
    ImGui::Separator();
}

void UI::RenderCameraSettings() const
{
    ImGui::Text("Camera");

    ACamera* Camera = FEditorManager::Get().GetCamera();

    bool IsOrthogonal;
    if (Camera->ProjectionMode == ECameraProjectionMode::Orthographic)
    {
        IsOrthogonal = true;
    }
    else if (Camera->ProjectionMode == ECameraProjectionMode::Perspective)
    {
        IsOrthogonal = false;
    }

    //if (ImGui::Checkbox("Orthogonal", &IsOrthogonal))
    //{
    //    if (IsOrthogonal)
    //    {
    //        Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
    //    }
    //    else
    //    {
    //        Camera->ProjectionMode = ECameraProjectionMode::Perspective;
    //    }
    //}

    float FOV = Camera->GetFieldOfView();
    if (ImGui::DragFloat("FOV", &FOV, 0.1f))
    {
        FOV = std::clamp(FOV, 0.01f, 179.99f);
        Camera->SetFieldOfVew(FOV);
    }

    float NearFar[2] = { Camera->GetNear(), Camera->GetFar() };
    if (ImGui::DragFloat2("Near, Far", NearFar, 0.1f))
    {
        NearFar[0] = FMath::Max(0.01f, NearFar[0]);
        NearFar[1] = FMath::Max(0.01f, NearFar[1]);

        if (NearFar[0] < NearFar[1])
        {
            Camera->SetNear(NearFar[0]);
            Camera->SetFar(NearFar[1]);
        }
        else
        {
            if (abs(NearFar[0] - Camera->GetNear()) < 0.00001f)
            {
                Camera->SetFar(NearFar[0] + 0.01f);
            }
            else if (abs(NearFar[1] - Camera->GetFar()) < 0.00001f)
            {
                Camera->SetNear(NearFar[1] - 0.01f);
            }
        }
    }
    
    FVector CameraPosition = Camera->GetActorTransform().GetPosition();
    if (ImGui::DragFloat3("Camera Location", reinterpret_cast<float*>(&CameraPosition), 0.1f))
    {
        FTransform Trans = Camera->GetActorTransform();
        Trans.SetPosition(CameraPosition);
        Camera->SetActorTransform(Trans);
    }

    FVector PrevEulerAngle = Camera->GetActorTransform().GetRotation().GetEuler();
    FVector UIEulerAngle = { PrevEulerAngle.X, PrevEulerAngle.Y, PrevEulerAngle.Z };
    if (ImGui::DragFloat3("Camera Rotation", reinterpret_cast<float*>(&UIEulerAngle), 0.1f))
    {
        FTransform Transform = Camera->GetActorTransform();

        //FVector DeltaEulerAngle = UIEulerAngle - PrevEulerAngle;
        //Transform.Rotate(DeltaEulerAngle);
        
        UIEulerAngle.Y = FMath::Clamp(UIEulerAngle.Y, -Camera->MaxYDegree, Camera->MaxYDegree);
        Transform.SetRotation(UIEulerAngle);
        Camera->SetActorTransform(Transform);
    }
    ImGui::DragFloat("Camera Speed", &Camera->CameraSpeed, 0.1f);
	ImGui::DragFloat("Camera Sensitivity", &Camera->Sensitivity, 0.1f);

    FVector Forward = Camera->GetActorTransform().GetForward();
    FVector Up = Camera->GetActorTransform().GetUp();
    FVector Right = Camera->GetActorTransform().GetRight();

    ImGui::Text("Camera GetForward(): (%.2f %.2f %.2f)", Forward.X, Forward.Y, Forward.Z);
    ImGui::Text("Camera GetUp(): (%.2f %.2f %.2f)", Up.X, Up.Y, Up.Z);
    ImGui::Text("Camera GetRight(): (%.2f %.2f %.2f)", Right.X, Right.Y, Right.Z);
	ImGui::Text("MouseLeftDown: %s", APlayerInput::Get().GetKeyDown(EKeyCode::LButton) ? "True" : "False");
	ImGui::Text("MousePress : %s", APlayerInput::Get().GetKeyPress(EKeyCode::LButton) ? "True" : "False");
	ImGui::Text("MosueLeftUp: %s", APlayerInput::Get().GetKeyUp(EKeyCode::LButton) ? "True" : "False");
	ImGui::Separator();
}

void UI::RenderPropertyWindow() const
{

    ImGui::Begin("Properties");

    if (bWasWindowSizeUpdated)
    {
        auto* Window = ImGui::GetCurrentWindow();

        ImGui::SetWindowPos(ResizeToScreen(Window->Pos));
        ImGui::SetWindowSize(ResizeToScreen(Window->Size));
    }
    
    AActor* selectedActor = FEditorManager::Get().GetSelectedActor();
    if (selectedActor != nullptr)
    {
        FTransform selectedTransform = selectedActor->GetActorTransform();
        float position[] = { selectedTransform.GetPosition().X, selectedTransform.GetPosition().Y, selectedTransform.GetPosition().Z };
        float scale[] = { selectedTransform.GetScale().X, selectedTransform.GetScale().Y, selectedTransform.GetScale().Z };

        if (ImGui::DragFloat3("Translation", position, 0.1f))
        {
            selectedTransform.SetPosition(position[0], position[1], position[2]);
            selectedActor->SetActorTransform(selectedTransform);
        }

        FVector PrevEulerAngle = selectedTransform.GetRotation().GetEuler();
        FVector UIEulerAngle = PrevEulerAngle;
        if (ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&UIEulerAngle), 0.1f))
        {
            FVector DeltaEulerAngle = UIEulerAngle - PrevEulerAngle;

            selectedTransform.Rotate(DeltaEulerAngle);
			UE_LOG("Rotation: %.2f, %.2f, %.2f", DeltaEulerAngle.X, DeltaEulerAngle.Y, DeltaEulerAngle.Z);
            selectedActor->SetActorTransform(selectedTransform);
        }
        if (ImGui::DragFloat3("Scale", scale, 0.1f))
        {
            selectedTransform.SetScale(scale[0], scale[1], scale[2]);
            selectedActor->SetActorTransform(selectedTransform);
        }
		/*if (FEditorManager::Get().GetGizmoHandle() != nullptr)
		{
			AGizmoHandle* Gizmo = FEditorManager::Get().GetGizmoHandle();
            if(Gizmo->GetGizmoType() == EGizmoType::Translate)
			{
				ImGui::Text("GizmoType: Translate");
			}
			else if (Gizmo->GetGizmoType() == EGizmoType::Rotate)
			{
				ImGui::Text("GizmoType: Rotate");
			}
			else if (Gizmo->GetGizmoType() == EGizmoType::Scale)
			{
				ImGui::Text("GizmoType: Scale");
			}
		}*/
    	if (selectedActor->IsA<AStaticMeshObj>())
    	{
    		UStaticMeshComponent* StaticMeshComponent = selectedActor->GetComponentByClass<UStaticMeshComponent>();
    		TArray<char*> MeshItemList;
    		TArray<UStaticMesh*> MeshItems;
    		int i = 0;
    		int CurrentMeshItem;
    		for (TObjectIterator<UStaticMesh> It; It; ++It)
    		{
    			FString MeshFString = It->FResource::GetName();
    			// 스코프를 돌면서 동일한 메모리 주소로 char*이 만들어지는 문제 발생
    			// 메모리 공간을 만들어 문제 해결 
    			auto MeshString = FString::TCHAR_TO_ANSI(MeshFString.c_wchar());
    			
    			if (StaticMeshComponent->GetStaticMesh()->FResource::GetName() == MeshFString)
    			{
    				CurrentMeshItem = i;
    			}
    			MeshItems.Add(*It);
    			MeshItemList.Add(MeshString);
    			i++;
    		}

    		if (ImGui::Combo("StaticMesh Asset", &CurrentMeshItem, MeshItemList.GetData(), MeshItemList.Num()))
    		{
    			FString MeshName = MeshItems[CurrentMeshItem]->FResource::GetName();
    			StaticMeshComponent->SetStaticMesh(MeshName);
    			//StaticMeshComponent->GetRenderResourceCollection().SetTextureBinding(MeshName, 0, true, true);
    			//StaticMeshComponent->GetRenderResourceCollection().SetTextureBinding("DefaultTexture", 1, true, true);
    		}

    		for (auto MeshString : MeshItemList)
    		{
    			free(MeshString);
    		}
    	}
    	if (selectedActor->IsA<ASpotLight>())
    	{
    		// SpotLight 속성 표시
    		ASpotLight* spotLight = static_cast<ASpotLight*>(selectedActor);
    		if (spotLight != nullptr)
    		{
    			ImGui::Separator();
    			ImGui::Text("SpotLight Properties");

    			// SpotLightComponent 가져오기
    			USpotLightComponent* spotLightComp = dynamic_cast<USpotLightComponent*>(spotLight->GetRootComponent());
    			if (spotLightComp != nullptr)
    			{
    				// 조명 색상
    				/*FVector4 lightColor = spotLightComp->GetLightColor();
					float color[4] = { lightColor.X, lightColor.Y, lightColor.Z, lightColor.W };
					if (ImGui::ColorEdit4("Light Color", color))
					{
						spotLightComp->SetLightColor(FVector4(color[0], color[1], color[2], color[3]));
					}*/

    				// 조명 강도
    				/*float intensity = spotLightComp->GetIntensity();
					if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f))
					{
						spotLightComp->SetIntensity(intensity);
					}*/

    				// 감쇠 반경
    				float attenRadius = spotLightComp->GetAttenuationRadius();
    				if (ImGui::DragFloat("Attenuation Radius", &attenRadius, 1.0f, 1.0f, 100.0f))
    				{
    					spotLightComp->SetAttenuationRadius(attenRadius);
    				}

    				// 내부 원뿔 각도 (라디안에서 도로 변환)
    				/*float innerAngleDegrees = spotLightComp->GetInnerConeAngle() * (180.0f / PI);
					if (ImGui::SliderFloat("Inner Cone Angle", &innerAngleDegrees, 0.0f, 90.0f))
					{
						spotLightComp->SetInnerConeAngle(innerAngleDegrees * (PI / 180.0f));
					}*/

    				// 외부 원뿔 각도 (라디안에서 도로 변환)
    				float outerAngleDegrees = spotLightComp->GetOuterConeAngle() * (180.0f / PI);
    				if (ImGui::SliderFloat("Outer Cone Angle", &outerAngleDegrees, 0.0f, 120.0f))
    				{
    					spotLightComp->SetOuterConeAngle(outerAngleDegrees * (PI / 180.0f));
    				}

    				// 그림자 캐스팅 여부
    				/*bool castShadows = spotLightComp->GetCastShadows();
					if (ImGui::Checkbox("Cast Shadows", &castShadows))
					{
						spotLightComp->SetCastShadows(castShadows);
					}*/

    				// 조명 활성화 여부
    				/*bool lightEnabled = spotLightComp->IsLightEnabled();
					if (ImGui::Checkbox("Light Enabled", &lightEnabled))
					{
						spotLightComp->SetLightEnabled(lightEnabled);
					}*/
    			}
    		}
    	}
    }
    ImGui::End();
}

void UI::RenderSceneManager()
{
	ImGui::Begin("SceneManager");
	TArray<AActor*>& Actors = UEngine::Get().GetWorld()->GetActors();

	if (Actors.Num() == 0)
		return;

	if (PrevSize != Actors.Num())
	{
		if (CurActor != nullptr)
		{
			CurActor = nullptr;
		}

		// 사용 전에 항상 비우기
		UUIDNames.Empty();
		UUIDs.Empty();
		UUIDNames.Reserve(Actors.Num());
		UUIDs.Reserve(Actors.Num());

		for (AActor* Actor : Actors)
		{
			if (dynamic_cast<IGizmoInterface*>(Actor))
				continue;
			FString UUIDName = Actor->GetClass()->GetName();
			UUIDName += FString::FromInt(Actor->GetUUID());
			UUIDNames.Add(UUIDName);
			UUIDs.Add(Actor->GetUUID());
		}
	}

	PrevSize = Actors.Num();

	static int SelectUUIDIndex = -1; // Here we store our selected data as an index.

	//int item_highlighted_idx = -1; // Here we store our highlighted data as an index.
		
	if (ImGui::BeginListBox("ActorList"))
	{
		for (const FString& Name : UUIDNames)
		{
			const bool bIsSelected = (UUIDNames.Find(Name) == SelectUUIDIndex);

			if (ImGui::Selectable(Name.c_char(), bIsSelected))
				SelectUUIDIndex = UUIDNames.Find(Name);

			//if (ImGui::IsItemHovered())
			//	item_highlighted_idx = UUIDNames.Find(Name);

			// ImGui가 활성화 된 경우, 특히 Item을 더블클릭하여 활성화 한 경우에만 UI를 통한 SelectActor 호출이 이루어짐
			if (bIsSelected)
			{
				if (ImGui::IsItemHovered())
				{
					//ImGui::SetItemDefaultFocus();
					uint32 UUID = UUIDs[SelectUUIDIndex];

					for (auto Actor : Actors)
					{
						if (dynamic_cast<IGizmoInterface*>(Actor))
							continue;
						if (Actor->GetUUID() == UUID)
						{
							CurActor = Actor;
							FEditorManager::Get().SelectActor(CurActor);
							FUUIDBillBoard::Get().SetTarget(CurActor);
							break;
						}
					}
				}
				if (ImGui::IsMouseDoubleClicked(0))
				{
					//TODO: Focus on the selected Actor (카메라 중앙으로 패닝)
					UE_LOG("DoubleClicked Item: %d", SelectUUIDIndex);
				}
			}
		}
		ImGui::EndListBox();
	}
	ImGui::End();
}

//void UI::RenderShowFlagsPanel() const
//{
//	if (ImGui::Begin("Show Flags"))
//	{
//		bool bPrimitives = FEngineShowFlags::Get().GetSingleFlag(EEngineShowFlags::SF_Primitives);
//		if (ImGui::Checkbox("Primitives", &bPrimitives))
//		{
//			FEngineShowFlags::Get().SetSingleFlag(EEngineShowFlags::SF_Primitives, bPrimitives);
//		}
//
//		bool bBillboardText = FEngineShowFlags::Get().GetSingleFlag(EEngineShowFlags::SF_BillboardText);
//		if (ImGui::Checkbox("Billboard Text", &bBillboardText))
//		{
//			FEngineShowFlags::Get().SetSingleFlag(EEngineShowFlags::SF_BillboardText, bBillboardText);
//		}
//	}
//	ImGui::End();
//}

//void UI::RenderViewModePanel() const
//{
//	if (ImGui::Begin("View Mode"))
//	{													
//		static const char* viewModeNames[] = { "Default", "Solid", "Wireframe" };
//		int currentViewMode = static_cast<int>(FViewMode::Get().GetViewMode());
//
//		if (ImGui::Combo("View Mode", &currentViewMode, viewModeNames, IM_ARRAYSIZE(viewModeNames)))
//		{
//			FViewMode::Get().SetViewMode((static_cast<EViewModeIndex>(currentViewMode)));
//		}
//	}
//	ImGui::End();
//}

void UI::RenderGridSettings() const
{
	UEngine* Engine = &UEngine::Get();
	UWorld* World = Engine->GetWorld();

	if (World == nullptr)
	{
		return;
	}

	if(ImGui::SliderFloat("Grid Size", &World->GetGridSizePtr(), 100.f, 1000.f, "%.2f"))
	{
		World->OnChangedGridSize();
	}
}

void UI::RenderViewportSettings(FViewport* InViewport, int32 index)
{
	// 뷰포트의 Rect를 이용해 창 위치 설정
	FRect rect = InViewport->GetRect();
	FViewportManager* ViewportManager = UEngine::Get().GetWorld()->GetViewportManager();
	FViewport* FullScreenViewport = ViewportManager->GetFullScreenViewport();
	if (FullScreenViewport)
	{
		rect.Top = 0;
		rect.Left = 0;
	}
	ImGui::SetNextWindowPos(ImVec2(rect.Left + 10, rect.Top + 10), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

	FString WindowName = FString("Viewport Settings #") + FString::FromInt(index);
	ImGui::Begin(WindowName.c_char(), nullptr, windowFlags);

	// Render Mode Combo (레이블 변경)
	static const char* ViewModeIndex[] = { "Default", "Solid", "Wireframe" };
	int CurrentViewMode = static_cast<int>(InViewport->GetClient()->GetViewMode());
	ImGui::PushItemWidth(100.0f);	//콤보박스 길이 수정
	if (ImGui::Combo("##Render Mode", &CurrentViewMode, ViewModeIndex, IM_ARRAYSIZE(ViewModeIndex)))
		InViewport->GetClient()->SetViewMode(static_cast<EViewModeIndex>(CurrentViewMode));
	ImGui::PopItemWidth();

	ImGui::SameLine();

	// View Type Combo (레이블 변경 및 enum 타입 확인)
	static const char* LevelViewportIndex[] = { "Perspective", "Top", "Bottom", "Left", "Right", "Front", "Back" };
	int CurrentLevelViewportMode = static_cast<int>(InViewport->GetClient()->GetLevelViewportType());
	ImGui::PushItemWidth(100.0f);	//콤보박스 길이 수정
	if (ImGui::Combo("##View Type", &CurrentLevelViewportMode, LevelViewportIndex, IM_ARRAYSIZE(LevelViewportIndex)))
		InViewport->GetClient()->SetLevelViewportType(static_cast<ELevelViewportType>(CurrentLevelViewportMode));
	ImGui::PopItemWidth();

	ImGui::SameLine();

	// 메뉴 버튼을 클릭하면 커스텀 팝업 열기
	if (ImGui::Button("Display"))
	{
		ImGui::OpenPopup("DisplayPopup");
	}

	// 커스텀 팝업을 고정 크기로 생성
	ImGui::SetNextWindowSize(ImVec2(150, 100), ImGuiCond_Always);
	if (ImGui::BeginPopup("DisplayPopup"))
	{
		static bool bPrimitives = FEngineShowFlags::Get().GetSingleFlag(EEngineShowFlags::SF_Primitives);
		static bool bBillboardText = FEngineShowFlags::Get().GetSingleFlag(EEngineShowFlags::SF_BillboardText);

		if (ImGui::MenuItem("Primitives", nullptr, &bPrimitives))
		{
			FEngineShowFlags::Get().SetSingleFlag(EEngineShowFlags::SF_Primitives, bPrimitives);
		}
		if (ImGui::MenuItem("Billboard Text", nullptr, &bBillboardText))
		{
			FEngineShowFlags::Get().SetSingleFlag(EEngineShowFlags::SF_BillboardText, bBillboardText);
		}
		InViewport->GetClient()->SetEngineShowFlags(FEngineShowFlags::Get().GetAllFlag());

		ImGui::EndPopup();
	}

	ImGui::SameLine();

	if (ImGui::Button("FullScreen"))
	{
		if (FullScreenViewport)
		{
			ViewportManager->SetFullScreenViewport(nullptr);
		}
		else
		{
			ViewportManager->SetFullScreenViewport(InViewport);
			ViewportManager->SetActiveViewport(InViewport);
			ViewportManager->SetActiveIndex(index);
			InViewport->SetRect(FRect(0, 0, UEngine::Get().GetScreenWidth(), UEngine::Get().GetScreenHeight()));
		}
	}

	ImGui::End();
}
