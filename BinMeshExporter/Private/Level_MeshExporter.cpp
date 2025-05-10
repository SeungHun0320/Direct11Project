#include "Level_MeshExporter.h"

#include "GameInstance.h"

CLevel_MeshExporter::CLevel_MeshExporter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{pDevice, pContext}
{
}

HRESULT CLevel_MeshExporter::Initialize()
{
	if (FAILED(Ready_ImGui()))
		return E_FAIL;


    return S_OK;
}

void CLevel_MeshExporter::Update(_float fTimeDelta)
{
}

HRESULT CLevel_MeshExporter::Render()
{
    SetWindowText(g_hWnd, TEXT("메쉬 익스포터 레벨입니다."));
	Render_Begin_ImGui();
	Ready_DockSpace();


	FileDialog();

	Render_End_ImGui();
    return S_OK;
}

HRESULT CLevel_MeshExporter::Ready_ImGui()
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

HRESULT CLevel_MeshExporter::Ready_DockSpace()
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
		if (ImGui::BeginMenu(u8"열기"))
		{ 
			if (ImGui::MenuItem(u8"폴더를 선택해서 변환"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../../Client/bin/Resources/Models/";
				config.fileName = "SelectFolderName";

				ImGuiFileDialog::Instance()->OpenDialog(
					"ChooseFolder",            // 다이얼로그 Key
					u8"폴더 선택",              // 타이틀
					"",						  // 폴더 선택 모드래,
					config                    // 시작 경로
				);

			}
			if (ImGui::MenuItem(u8"파일을 선택해서 변환"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../../Client/bin/Resources/Models/";
				config.countSelectionMax = 0;

				ImGuiFileDialog::Instance()->OpenDialog(
					"ChooseFile",            // 다이얼로그 Key
					u8"선택 창",              // 타이틀
					".fbx",        // 필터 (여러 개 가능),
					config                    // 시작 경로
				);
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

HRESULT CLevel_MeshExporter::Render_Begin_ImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	return S_OK;
}

HRESULT CLevel_MeshExporter::Render_End_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return S_OK;
}

HRESULT CLevel_MeshExporter::FileDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ChooseFolder"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			string strSelectedFolderPath = ImGuiFileDialog::Instance()->GetCurrentPath();

			FBXFolder_to_Bin_Exported(strSelectedFolderPath);
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			auto  SelectedFBXFiles = ImGuiFileDialog::Instance()->GetSelection();
			for (const auto& [FileName, FilePath] : SelectedFBXFiles)
			{
				string strName = FileName;
				string strPath = FilePath;
				// 원하는 작업 수행

				FBX_to_Bin_Exproted(strPath);
			}
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}

	return S_OK;
}

HRESULT CLevel_MeshExporter::FBXFolder_to_Bin_Exported(const string& strFilePath)
{
	fs::path inputPath(strFilePath);

	if (!fs::exists(inputPath))
	{
		MSG_BOX("경로가 없는디,,?");
		return E_FAIL;
	}


	for (const auto& entry : fs::recursive_directory_iterator(inputPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".fbx")
		{
			_int a = 0;
		}
		else
		{
			MSG_BOX("FBX가 아닌 다른 확장자가 선택 됐읍니다 ");
		}
	}

	return S_OK;
}

HRESULT CLevel_MeshExporter::FBX_to_Bin_Exproted(const string& strFilePath)
{
	fs::path inputPath(strFilePath);

	if (!fs::exists(inputPath))
	{
		MSG_BOX("경로가 없는디,,?");
		return E_FAIL;
	}

	return S_OK;
}

CLevel_MeshExporter* CLevel_MeshExporter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_MeshExporter* pInstance = new CLevel_MeshExporter(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_MeshExporter::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
