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
		if (ImGui::BeginMenu(u8"폴더 열기"))
		{ 
			if (ImGui::MenuItem(u8"선택해서 변환"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../../Client/bin/Resources/Models/";
				config.countSelectionMax = 0;
				config.fileName = "SelectFolderName";

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
	ImGui::Begin(u8"모델 타입 선택");
	ImGui::RadioButton("Anim", &m_iType, 0);
	ImGui::SameLine();
	ImGui::RadioButton("NonAnim", &m_iType, 1);
	ImGui::End();

	if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			auto  SelectedFBXFiles = ImGuiFileDialog::Instance()->GetSelection();
			if (0 == SelectedFBXFiles.size())
			{
				string strSelectedFolderPath = ImGuiFileDialog::Instance()->GetCurrentPath();
				FBX_to_Bin_Exproted(strSelectedFolderPath);
			}
			else
			{
				for (const auto& [FileName, FilePath] : SelectedFBXFiles)
				{
					string strName = FileName;
					string strPath = FilePath;
					// 원하는 작업 수행

					FBX_to_Bin_Exproted(strPath);
				}
			}
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}

	return S_OK;
}

HRESULT CLevel_MeshExporter::FBX_to_Bin_Exproted(const string& strFilePath)
{
	fs::path InputPath(strFilePath);

	if (!fs::exists(InputPath))
	{
		MSG_BOX("경로가 없는디,,?");
		return E_FAIL;
	}

	if (fs::is_directory(InputPath))
	{
		for (const auto& entry : fs::recursive_directory_iterator(InputPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".fbx")
			{
				fs::path fbxPath = entry.path();
				fs::path OutputPath = fbxPath.parent_path() / (fbxPath.stem().string() + ".Model");

				if (0 == m_iType)
				{
					/* 나중에 리드 애님 모델 추가 */
				}
				else if (1 == m_iType)
				{
					if (FAILED(Read_Non_Anim_Model(fbxPath.string(), OutputPath.string())))
					{
						MSG_BOX("변환 실패,,,");
						return E_FAIL;
					}
				}
			}
		}
	}
	else if (fs::is_regular_file(InputPath) && InputPath.extension() == ".fbx")
	{
		fs::path OutputPath = InputPath.parent_path() / (InputPath.stem().string() + ".Model");

		if (0 == m_iType)
		{
			/* 나중에 리드 애님 모델 추가 */
		}
		else if (1 == m_iType)
		{
			if (FAILED(Read_Non_Anim_Model(InputPath.string(), OutputPath.string())))
			{
				MSG_BOX("변환 실패,,,");
				return E_FAIL;
			}
		}
	}
	else
	{
		MSG_BOX("FBX가 아닌 다른 확장자가 선택 됐읍니다 ");
		return E_FAIL;
	}

	MSG_BOX("개같이 성공");
	return S_OK;
}

HRESULT CLevel_MeshExporter::Read_Anim_Model()
{
	return S_OK;
}

HRESULT CLevel_MeshExporter::Exported_Anim_Model()
{
	return S_OK;
}

HRESULT CLevel_MeshExporter::Read_Non_Anim_Model(const string& strModelPath, const string& strOutPath)
{
	Assimp::Importer Importer;
	const aiScene* pAIScene = { nullptr };

	_uint iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	pAIScene = Importer.ReadFile(strModelPath, iFlag);

	if (nullptr == pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Non_Anim_Meshes(pAIScene->mNumMeshes, pAIScene->mMeshes)))
		return E_FAIL;

	if (FAILED(Ready_Material(strModelPath.c_str(), pAIScene->mNumMaterials, pAIScene->mMaterials)))
		return E_FAIL;


	return Exported_Non_Anim_Model(strOutPath);
}

HRESULT CLevel_MeshExporter::Exported_Non_Anim_Model(const string& strOutPath)
{
	ofstream OutFile(strOutPath, ios::binary);
	if (!OutFile.is_open())
		return E_FAIL;

	/* --------------------------메쉬----------------------- */
	_uint iNumMeshes = static_cast<_uint>(m_vecNonAnimMeshes.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumMeshes), sizeof(_uint));
	for (auto& Mesh : m_vecNonAnimMeshes)
	{
		_uint iNumVertices = static_cast<_uint>(Mesh->Vertices.size());
		_uint iNumIndices = static_cast<_uint>(Mesh->Indices.size());

		OutFile.write(reinterpret_cast<const _char*>(&iNumVertices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&iNumIndices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&Mesh->iMaterialIndex), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(Mesh->Vertices.data()), sizeof(VTXMESH) * iNumVertices);
		OutFile.write(reinterpret_cast<const _char*>(Mesh->Indices.data()), sizeof(_uint) * iNumIndices);
	}

	/* ------------------------마테리얼---------------------------- */

	_uint iNumMaterial = static_cast<_uint>(m_vecMaterials.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumMaterial), sizeof(_uint));

	for (auto& pMaterial : m_vecMaterials)
	{
		/* iNumTextures == iNumSRVs임 진짜 절대 잊지마 */
		_uint iNumTextures = static_cast<_uint>(pMaterial->vecTextures.size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumTextures), sizeof(_uint));

		for (auto& TexInfo : pMaterial->vecTextures)
		{
			OutFile.write(reinterpret_cast<const _char*>(&TexInfo.iTextureType), sizeof(_uint));
			_uint istrLen = static_cast<_uint>(TexInfo.strTexturePath.length());
			OutFile.write(reinterpret_cast<const _char*>(&istrLen), sizeof(_uint));
			OutFile.write(reinterpret_cast<const _char*>(TexInfo.strTexturePath.c_str()), sizeof(_tchar) * istrLen);
		}
	}


	/* -------------------다 썼으니 삭제----------------------- */

	for (auto& Mesh : m_vecNonAnimMeshes)
	{
		Mesh->Vertices.clear();
		Mesh->Indices.clear();
		Safe_Delete(Mesh);
	}
	m_vecNonAnimMeshes.clear();

	for (auto& pMaterial : m_vecMaterials)
	{
		pMaterial->vecTextures.clear();
		Safe_Delete(pMaterial);
	}
	m_vecMaterials.clear();

	OutFile.close();

	return S_OK;
}

HRESULT CLevel_MeshExporter::Ready_Non_Anim_Meshes(_uint iNumMeshes, aiMesh** ppMeshes)
{
	m_vecNonAnimMeshes.reserve(iNumMeshes);

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		NONANIMMESH* pMesh = new NONANIMMESH;
		pMesh->iMaterialIndex = ppMeshes[i]->mMaterialIndex;
		_uint iNumVertices = ppMeshes[i]->mNumVertices;
		_uint iNumIndices = ppMeshes[i]->mNumFaces * 3;

		pMesh->Vertices.reserve(iNumVertices);
		pMesh->Indices.reserve(iNumIndices);

		for (_uint j = 0; j < iNumVertices; j++)
		{
			VTXMESH Vertices = {};

			memcpy(&Vertices.vPosition, &ppMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&Vertices.vNormal, &ppMeshes[i]->mNormals[j], sizeof(_float3));
			memcpy(&Vertices.vTangent, &ppMeshes[i]->mTangents[j], sizeof(_float3));
			if (ppMeshes[i]->HasTextureCoords(0))
				memcpy(&Vertices.vTexcoord, &ppMeshes[i]->mTextureCoords[0][j], sizeof(_float2));

			pMesh->Vertices.push_back(Vertices);
		}

		pMesh->Indices.resize(iNumIndices);
		_uint iIndex = {};

		for (_uint j = 0; j < ppMeshes[i]->mNumFaces; j++)
		{
			pMesh->Indices[iIndex++] = ppMeshes[i]->mFaces[j].mIndices[0];
			pMesh->Indices[iIndex++] = ppMeshes[i]->mFaces[j].mIndices[1];
			pMesh->Indices[iIndex++] = ppMeshes[i]->mFaces[j].mIndices[2];
		}
	
		m_vecNonAnimMeshes.push_back(pMesh);

	}
	return S_OK;
}

HRESULT CLevel_MeshExporter::Ready_Material(const _char* pModelFilePath, _uint iNumMaterial, aiMaterial** pAIMaterial)
{
	m_vecMaterials.reserve(iNumMaterial);

	for (_uint i = 0; i < iNumMaterial; i++)
	{
		MATERIAL* pMaterial = new MATERIAL;

		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			_uint iNumSRVs = pAIMaterial[i]->GetTextureCount(static_cast<aiTextureType>(j));

			TEX_INFO TexInfo{};
			TexInfo.iTextureType = j;

			for (_uint k = 0; k < iNumSRVs; k++)
			{
				aiString     strTexturePath;
				if (FAILED(pAIMaterial[i]->GetTexture(static_cast<aiTextureType>(j), k, &strTexturePath)))
					return E_FAIL;

				_char       szFullPath[MAX_PATH] = {};
				_char       szDrive[MAX_PATH] = {};
				_char       szDir[MAX_PATH] = {};
				_char       szFileName[MAX_PATH] = {};
				_char       szExt[MAX_PATH] = {};

				/* 경로 얻어오는 함수, strTexturePath.data를 쓴 이유는 어심프에서 독자적으로 쓰는 클래스이고, */
				/* .data를 통해 앞주소를 가져올 수 있기 때문 */
				_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

				strcpy_s(szFullPath, szDrive);
				strcat_s(szFullPath, szDir);
				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);

				_tchar      szTextureFilePath[MAX_PATH] = {};

				MultiByteToWideChar(CP_ACP, 0, szFullPath, -1, szTextureFilePath, MAX_PATH);
				
				_wstring strTextureFilePath(szTextureFilePath);

				TexInfo.strTexturePath = strTextureFilePath;

				pMaterial->vecTextures.push_back(TexInfo);

			}
		}

		m_vecMaterials.push_back(pMaterial);
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

	for (auto& Mesh : m_vecNonAnimMeshes)
	{
		Mesh->Vertices.clear();
		Mesh->Indices.clear();
		Safe_Delete(Mesh);
	}
	m_vecNonAnimMeshes.clear();

	for (auto& pMaterial : m_vecMaterials)
	{
		pMaterial->vecTextures.clear();
		Safe_Delete(pMaterial);
	}
	m_vecMaterials.clear();
}
