#include "Level_Tools.h"
#include "Level_Loading.h"

CLevel_Tools::CLevel_Tools(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

#ifdef _IMGUI

HRESULT CLevel_Tools::Initialize()
{

	if (FAILED(Ready_ImGui()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Tools::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_RETURN))
	{
		if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOGO))))
			return;
	}
}

HRESULT CLevel_Tools::Render()
{
	SetWindowText(g_hWnd, TEXT("툴 레벨입니다."));
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	Ready_DockSpace();

	MapTool();
	CameraTool();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	return S_OK;
}

HRESULT CLevel_Tools::Ready_ImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("C://Windows//Fonts//YES24.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesKorean());

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CLevel_Tools::Ready_DockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_MenuBar;


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"파일"))
		{
			IGFD::FileDialogConfig config{};

			config.path = "../../";

			ImGuiFileDialog::Instance()->OpenDialog(
				"ChooseFile",            // 다이얼로그 Key
				"파일 선택",              // 타이틀
				".png,.fbx,.txt",        // 필터 (여러 개 가능),
				config                    // 시작 경로
			);

			if (ImGuiFileDialog::Instance()->Display("OpenFileDialog"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
				{
					std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
				}

				ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();

	return S_OK;
}

HRESULT CLevel_Tools::MainTool()
{
	ImGui::Begin(u8"툴의 왕이 되고싶다");

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	if (ImGui::BeginTabBar(u8"툴 바"))
	{
		if (ImGui::BeginTabItem(u8"맵툴"))
		{
			MapTool();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"카메라툴"))
		{
			CameraTool();
			ImGui::EndTabItem();
		}
	}
	ImGui::SameLine();
	ImGui::EndTabBar();

	ImGui::End();

	return S_OK;
}

HRESULT CLevel_Tools::CameraTool()
{
	ImGui::Begin(u8"카메라 툴");

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	if (ImGui::Button(u8"응애쓰"))
	{
		int a = 0;
	}

	ImGui::End();

	return S_OK;
}

HRESULT CLevel_Tools::MapTool()
{
	ImGui::Begin(u8"맵 툴");

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	if (ImGui::Button(u8"지형 생성"))
	{
		int a = 0;
	}

	ImGui::End();

	return S_OK;
}

CLevel_Tools* CLevel_Tools::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tools* pInstance = new CLevel_Tools(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tools");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tools::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

#endif // _IMGUI