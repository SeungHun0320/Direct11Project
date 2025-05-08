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
	SetWindowText(g_hWnd, TEXT("�� �����Դϴ�."));
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
		if (ImGui::BeginMenu(u8"����"))
		{
			IGFD::FileDialogConfig config{};

			config.path = "../../";

			ImGuiFileDialog::Instance()->OpenDialog(
				"ChooseFile",            // ���̾�α� Key
				"���� ����",              // Ÿ��Ʋ
				".png,.fbx,.txt",        // ���� (���� �� ����),
				config                    // ���� ���
			);

			if (ImGuiFileDialog::Instance()->Display("OpenFileDialog"))
			{
				if (ImGuiFileDialog::Instance()->IsOk())  // OK �����ٸ�
				{
					std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
					std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
				}

				ImGuiFileDialog::Instance()->Close(); // �� �ݾ���� �ٽ� ����
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
	ImGui::Begin(u8"���� ���� �ǰ�ʹ�");

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	if (ImGui::BeginTabBar(u8"�� ��"))
	{
		if (ImGui::BeginTabItem(u8"����"))
		{
			MapTool();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"ī�޶���"))
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
	ImGui::Begin(u8"ī�޶� ��");

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	if (ImGui::Button(u8"���־�"))
	{
		int a = 0;
	}

	ImGui::End();

	return S_OK;
}

HRESULT CLevel_Tools::MapTool()
{
	ImGui::Begin(u8"�� ��");

	ImGui::Dummy(ImVec2(0.0f, 15.0f));
	if (ImGui::Button(u8"���� ����"))
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