#include "Level_Tools.h"
#include "Level_Loading.h"

#include "MapTool.h"
#include "Camera_Free.h"

CLevel_Tools::CLevel_Tools(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

#ifdef _IMGUI

HRESULT CLevel_Tools::Initialize()
{
	if(FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_ImGui()))
		return E_FAIL;

	if (FAILED(Ready_Tools()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Tools::Update(_float fTimeDelta)
{
	//if (KEY_DOWN(DIK_RETURN))
	//{
	//	if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::LOADING),
	//		CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::LOGO))))
	//		return;
	//}
}

HRESULT CLevel_Tools::Render()
{
	SetWindowText(g_hWnd, TEXT("�� �����Դϴ�."));
	ImGui_RenderBegin();
	Ready_DockSpace();

	FileDialog();
	m_pMapTool->Render();

	ImGui_RenderEnd();
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

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu(u8"����"))
	//	{
	//		if (ImGui::BeginMenu(u8"����"))
	//		{
	//			IGFD::FileDialogConfig config{};

	//			config.path = "../bin/Resources/";

	//			ImGuiFileDialog::Instance()->OpenDialog(
	//				"ChooseFile",            // ���̾�α� Key
	//				u8"���� ����",              // Ÿ��Ʋ
	//				".png,.fbx,.txt",        // ���� (���� �� ����),
	//				config                    // ���� ���
	//			);

	//			ImGui::EndMenu();
	//		}
	//		ImGui::EndMenu();
	//	}

	//	ImGui::EndMenuBar();
	//}

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();

	return S_OK;
}

void CLevel_Tools::ImGui_RenderBegin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void CLevel_Tools::ImGui_RenderEnd()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HRESULT CLevel_Tools::FileDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK �����ٸ�
		{
			std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
		}

		ImGuiFileDialog::Instance()->Close(); // �� �ݾ���� �ٽ� ����
	}

	return S_OK;
}

HRESULT CLevel_Tools::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::DESC tDesc = {};

	tDesc.eLevelID = LEVEL::TOOLS;
	tDesc.fSensor = 0.1f;

	tDesc.vEye = _float3(0.f, 20.f, -15.f);
	tDesc.vAt = _float3(0.f, 0.f, 0.f);
	tDesc.fFov = XMConvertToRadians(60.f);
	tDesc.fNear = 0.1f;
	tDesc.fFar = 3000.f;
	tDesc.fSpeedPerSec = 10.f;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(tDesc.eLevelID), strLayerTag, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tools::Ready_Tools()
{
	m_pMapTool = CMapTool::Create(m_pDevice, m_pContext);
	if (nullptr == m_pMapTool)
		return E_FAIL;

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

	Safe_Release(m_pMapTool);
}

#endif // _IMGUI