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
    SetWindowText(g_hWnd, TEXT("�޽� �ͽ����� �����Դϴ�."));
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
		if (ImGui::BeginMenu(u8"���� ����"))
		{ 
			if (ImGui::MenuItem(u8"�����ؼ� ��ȯ"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../../Client/bin/Resources/Models/";
				config.countSelectionMax = 0;
				config.fileName = "SelectFolderName";

				ImGuiFileDialog::Instance()->OpenDialog(
					"ChooseFile",            // ���̾�α� Key
					u8"���� â",              // Ÿ��Ʋ
					".fbx",        // ���� (���� �� ����),
					config                    // ���� ���
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
	ImGui::Begin(u8"�� Ÿ�� ����");
	ImGui::RadioButton("Anim", &m_iType, 0);
	ImGui::SameLine();
	ImGui::RadioButton("NonAnim", &m_iType, 1);
	ImGui::End();

	if (ImGuiFileDialog::Instance()->Display("ChooseFile"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK �����ٸ�
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
					// ���ϴ� �۾� ����

					FBX_to_Bin_Exproted(strPath);
				}
			}
		}

		ImGuiFileDialog::Instance()->Close(); // �� �ݾ���� �ٽ� ����
	}

	return S_OK;
}

HRESULT CLevel_MeshExporter::FBX_to_Bin_Exproted(const string& strFilePath)
{
	fs::path InputPath(strFilePath);

	if (!fs::exists(InputPath))
	{
		MSG_BOX("��ΰ� ���µ�,,?");
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
					if (FAILED(Read_Anim_Model(fbxPath.string(), OutputPath.string())))
					{
						MSG_BOX("��ȯ ����,,,");
						return E_FAIL;
					}
				}
				else if (1 == m_iType)
				{
					if (FAILED(Read_Non_Anim_Model(fbxPath.string(), OutputPath.string())))
					{
						MSG_BOX("��ȯ ����,,,");
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
			if (FAILED(Read_Anim_Model(InputPath.string(), OutputPath.string())))
			{
				MSG_BOX("��ȯ ����,,,");
				return E_FAIL;
			}
		}
		else if (1 == m_iType)
		{
			if (FAILED(Read_Non_Anim_Model(InputPath.string(), OutputPath.string())))
			{
				MSG_BOX("��ȯ ����,,,");
				return E_FAIL;
			}
		}
	}
	else
	{
		MSG_BOX("FBX�� �ƴ� �ٸ� Ȯ���ڰ� ���� �����ϴ� ");
		return E_FAIL;
	}

	MSG_BOX("������ ����");
	return S_OK;
}

HRESULT CLevel_MeshExporter::Read_Anim_Model(const string& strModelPath, const string& strOutPath)
{
	Assimp::Importer Importer;
	const aiScene* pAIScene = { nullptr };

	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	pAIScene = Importer.ReadFile(strModelPath, iFlag);

	if (nullptr == pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(-1, pAIScene->mRootNode)))
		return E_FAIL;

	if (FAILED(Ready_Anim_Meshes(pAIScene->mNumMeshes, pAIScene->mMeshes)))
		return E_FAIL;

	if (FAILED(Ready_Material(strModelPath.c_str(), pAIScene->mNumMaterials, pAIScene->mMaterials)))
		return E_FAIL;

	if (FAILED(Ready_Animations(pAIScene->mNumAnimations, pAIScene->mAnimations)))
		return E_FAIL;


	return Exported_Anim_Model(strOutPath);

}

HRESULT CLevel_MeshExporter::Exported_Anim_Model(const string& strOutPath)
{
	ofstream OutFile(strOutPath, ios::binary);
	if (!OutFile.is_open())
		return E_FAIL;

	/* ---------------------------��----------------------------- */
	_uint iNumBones = static_cast<_uint>(m_vecBones.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumBones), sizeof(_uint));
	for (auto& pBone : m_vecBones)
	{
		OutFile.write(reinterpret_cast<const _char*>(&pBone->iParentBoneIndex), sizeof(_int));
		_uint istrLen = static_cast<_uint>(pBone->strName.length());
		OutFile.write(reinterpret_cast<const _char*>(&istrLen), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(pBone->strName.c_str()), sizeof(_char) * istrLen);
		OutFile.write(reinterpret_cast<const _char*>(&pBone->TransformationMatrix), sizeof(XMFLOAT4X4));
	}

	/* ---------------------------�޽�----------------------------- */
	_uint iNumMeshes = static_cast<_uint>(m_vecAnimMeshes.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumMeshes), sizeof(_uint));
	for (auto& Mesh : m_vecAnimMeshes)
	{
		_uint iNumVertices = static_cast<_uint>(Mesh->Vertices.size());
		_uint iNumIndices = static_cast<_uint>(Mesh->Indices.size());
		_uint iNumBoneIndices = static_cast<_uint>(Mesh->BoneIndices.size());
		_uint iNumOffsetMatrices = static_cast<_uint>(Mesh->OffsetMatrices.size());

		OutFile.write(reinterpret_cast<const _char*>(&iNumVertices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&iNumIndices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&iNumBoneIndices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&iNumOffsetMatrices), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(&Mesh->iMaterialIndex), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(Mesh->Vertices.data()), sizeof(VTXANIMMESH) * iNumVertices);
		OutFile.write(reinterpret_cast<const _char*>(Mesh->Indices.data()), sizeof(_uint) * iNumIndices);
		OutFile.write(reinterpret_cast<const _char*>(&Mesh->iNumBones), sizeof(_uint));
		OutFile.write(reinterpret_cast<const _char*>(Mesh->BoneIndices.data()), sizeof(_uint) * iNumBoneIndices);
		OutFile.write(reinterpret_cast<const _char*>(Mesh->OffsetMatrices.data()), sizeof(_float4x4) * iNumOffsetMatrices);
	}

	/* ------------------------���׸���---------------------------- */
	_uint iNumMaterial = static_cast<_uint>(m_vecMaterials.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumMaterial), sizeof(_uint));

	for (auto& pMaterial : m_vecMaterials)
	{
		/* iNumTextures == iNumSRVs�� ��¥ ���� ������ */
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

	/*------------------------ �ִϸ��̼� ----------------------------*/

	/* �ִϸ��̼� ���� */
	_uint iNumAnimation = static_cast<_uint>(m_vecAnimations.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumAnimation), sizeof(_uint));

	for (auto& pAnimation : m_vecAnimations)
	{
		/* ä�� ���� */
		_uint iNumChannels = static_cast<_uint>(pAnimation->Channels.size());
		OutFile.write(reinterpret_cast<const _char*>(&iNumChannels), sizeof(_uint));

		/* ä���� ����ִ� ƽ�ۼ�����, ��ü ��� �Ÿ� */
		OutFile.write(reinterpret_cast<const _char*>(&pAnimation->fTickPerSecond), sizeof(_float));
		OutFile.write(reinterpret_cast<const _char*>(&pAnimation->fDuration), sizeof(_float));

		for (auto& Channel : pAnimation->Channels)
		{
			/* Ű������ ����, CBone�� �� �ε��� */
			OutFile.write(reinterpret_cast<const _char*>(&Channel.iNumKeyFrames), sizeof(_uint));
			OutFile.write(reinterpret_cast<const _char*>(&Channel.iBoneIndex), sizeof(_uint));

			for (auto& KeyFrame : Channel.KeyFrames)
			{
				/* ũ�� ���� �̵�, �� ���¸� ���ؾ� �ϴ� �����ġ, ����� �ƴ������� ������ ���ֱ� ���ؼ� */
				OutFile.write(reinterpret_cast<const _char*>(&KeyFrame.vScale), sizeof(_float3));
				OutFile.write(reinterpret_cast<const _char*>(&KeyFrame.vRotation), sizeof(_float4));
				OutFile.write(reinterpret_cast<const _char*>(&KeyFrame.vTranslation), sizeof(_float3));
				OutFile.write(reinterpret_cast<const _char*>(&KeyFrame.fTrackPosition), sizeof(_float));
			}
		}
	}


	/* -------------------�� ������ ����----------------------- */

	/* �� */
	for (auto& pBone : m_vecBones)
		Safe_Delete(pBone);
	m_vecBones.clear();

	/* �ִ� �޽� */
	for (auto& pAnimMesh : m_vecAnimMeshes)
	{
		pAnimMesh->BoneIndices.clear();
		pAnimMesh->Indices.clear();
		pAnimMesh->Vertices.clear();
		pAnimMesh->OffsetMatrices.clear();
		Safe_Delete(pAnimMesh);
	}
	m_vecAnimMeshes.clear();

	/* ���׸��� */
	for (auto& pMaterial : m_vecMaterials)
	{
		pMaterial->vecTextures.clear();
		Safe_Delete(pMaterial);
	}
	m_vecMaterials.clear();

	/* �ִϸ��̼� */
	for (auto& pAnimation : m_vecAnimations)
	{
		for (auto& Channel : pAnimation->Channels)
		{
			Channel.KeyFrames.clear();
		}
		pAnimation->Channels.clear();

		Safe_Delete(pAnimation);
	}
	m_vecAnimations.clear();

	OutFile.close();

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

	/* --------------------------�޽�----------------------- */
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

	/* ------------------------���׸���---------------------------- */

	_uint iNumMaterial = static_cast<_uint>(m_vecMaterials.size());
	OutFile.write(reinterpret_cast<const _char*>(&iNumMaterial), sizeof(_uint));

	for (auto& pMaterial : m_vecMaterials)
	{
		/* iNumTextures == iNumSRVs�� ��¥ ���� ������ */
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

	/* -------------------�� ������ ����----------------------- */

	for (auto& pNonAnimMesh : m_vecNonAnimMeshes)
	{
		pNonAnimMesh->Vertices.clear();
		pNonAnimMesh->Indices.clear();
		Safe_Delete(pNonAnimMesh);
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

HRESULT CLevel_MeshExporter::Ready_Bones(_int iParentBoneIndex, const aiNode* pAINode)
{
	BONE* pBone = new BONE;

	pBone->strName = pAINode->mName.data;

	memcpy(&pBone->TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	XMStoreFloat4x4(&pBone->TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&pBone->TransformationMatrix)));

	pBone->iParentBoneIndex = iParentBoneIndex;

	m_vecBones.push_back(pBone);

	_int	iParentIndex = static_cast<_int>(m_vecBones.size() - 1);

	for (_uint i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(iParentIndex, pAINode->mChildren[i]);
	}

	return S_OK;
}

HRESULT CLevel_MeshExporter::Ready_Anim_Meshes(_uint iNumMeshes, aiMesh** ppMeshes)
{
	m_vecAnimMeshes.reserve(iNumMeshes);

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		ANIMMESH* pMesh = new ANIMMESH;
		pMesh->iMaterialIndex = ppMeshes[i]->mMaterialIndex;
		_uint iNumVertices = ppMeshes[i]->mNumVertices;
		_uint iNumIndices = ppMeshes[i]->mNumFaces * 3;

		pMesh->Vertices.reserve(iNumVertices);
		pMesh->Indices.reserve(iNumIndices);

		for (_uint j = 0; j < iNumVertices; j++)
		{
			VTXANIMMESH Vertices = {};

			memcpy(&Vertices.vPosition, &ppMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&Vertices.vNormal, &ppMeshes[i]->mNormals[j], sizeof(_float3));
			memcpy(&Vertices.vTangent, &ppMeshes[i]->mTangents[j], sizeof(_float3));
			for (_uint k = 0; k < 8; k++)
			{
				if (ppMeshes[i]->HasTextureCoords(k))
				{
					memcpy(&Vertices.vTexcoord, &ppMeshes[i]->mTextureCoords[k][j], sizeof(_float2));
					break;
				}

			}

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

		pMesh->iNumBones = ppMeshes[i]->mNumBones;

		for (_uint j = 0; j < pMesh->iNumBones; j++)
		{
			/* �θ� �ڽĵ��� ���� ���輺�� ǥ��(x) -> aiNode */
			/* �� �޽��� � �����鿡�� ������ �ݴϴ�. and �󸶳� ������ �ݴϴ�. -> mBones */
			aiBone* pAIBone = ppMeshes[i]->mBones[j];

			_float4x4		OffsetMatrix;
			memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
			pMesh->OffsetMatrices.push_back(OffsetMatrix);

			_uint iBoneIndex = {};

			auto	iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](BONE* pBone)->_bool
				{
					if (pBone->strName == pAIBone->mName.data)
						return true;

					++iBoneIndex;

					return false;
				});

			/* �� �޽ÿ� ������ �ִ� ���� �� CBone ���� ���°�� �� �־�����(iIndex)�� ��Ƴ���. */
			pMesh->BoneIndices.push_back(iBoneIndex);

			/* i��° ���� � �������� ������ �ִµ�?*/
			_uint		iNumWeights = pAIBone->mNumWeights;

			for (_uint k = 0; k < iNumWeights; k++)
			{
				/* i��° ���� ������ �ִ� j��° ������ ���� */
				aiVertexWeight	AIWeight = pAIBone->mWeights[k];

				/* ����ġ�� ���ؼ� 0.f��� ����ִٴ� ��, ä���ֱ� */
				if (0.f == pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.x)
				{
					pMesh->Vertices[AIWeight.mVertexId].vBlendIndices.x = j;
					pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.x = AIWeight.mWeight;
				}
				else if (0.f == pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.y)
				{
					pMesh->Vertices[AIWeight.mVertexId].vBlendIndices.y = j;
					pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.y = AIWeight.mWeight;
				}
				else if (0.f == pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.z)
				{
					pMesh->Vertices[AIWeight.mVertexId].vBlendIndices.z = j;
					pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.z = AIWeight.mWeight;
				}
				else if (0.f == pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.w)
				{
					pMesh->Vertices[AIWeight.mVertexId].vBlendIndices.w = j;
					pMesh->Vertices[AIWeight.mVertexId].vBlendWeights.w = AIWeight.mWeight;
				}
			}
		}

		/* ���� ���ٸ� */
		if (0 == pMesh->iNumBones)
		{
			/* �Ϻη� �Ѱ� ����� �ְ� */
			pMesh->iNumBones = 1;

			/* ���ε��� �����ؼ� */
			_uint	iBoneIndex = {};

			/* ���̸��̶� �޽��̸��̶� ���ؼ� ������ ���ε��� ���� ��Ű�� */
			auto	iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](BONE* pBone)->_bool
				{
					if (pBone->strName == ppMeshes[i]->mName.data)
						return true;

					++iBoneIndex;

					return false;
				});

			/* ���ε��� ���ε��� �־��ֱ� */
			if (iter == m_vecBones.end())
			{
				/* �÷��̾� ����,,, �̰� �³�? */
				if(i == 0) // ����
					pMesh->BoneIndices.push_back(26);
				if(i == 1) // Į
					pMesh->BoneIndices.push_back(43);
				if(i == 2)
					pMesh->BoneIndices.push_back(44);
				if(i == 3)
					pMesh->BoneIndices.push_back(45);
				if(i == 4)
					pMesh->BoneIndices.push_back(46);
				if (i == 5) // ���
				{
					pMesh->BoneIndices.push_back(52);
				}
			
				//pMesh->BoneIndices.push_back(--iBoneIndex);
			}
			else
				pMesh->BoneIndices.push_back(iBoneIndex);

			/* ���� �����ϱ� ��������� �׵��� ��. */
			_float4x4		OffsetMatrix;
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
			pMesh->OffsetMatrices.push_back(OffsetMatrix);

		}
		m_vecAnimMeshes.push_back(pMesh);
	}

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
			for (_uint k = 0; k < 8; k++)
			{
				if (ppMeshes[i]->HasTextureCoords(k))
				{
					memcpy(&Vertices.vTexcoord, &ppMeshes[i]->mTextureCoords[k][j], sizeof(_float2));
					break;
				}
			}

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

				/* ��� ������ �Լ�, strTexturePath.data�� �� ������ ��������� ���������� ���� Ŭ�����̰�, */
				/* .data�� ���� ���ּҸ� ������ �� �ֱ� ���� */
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

HRESULT CLevel_MeshExporter::Ready_Animations(_uint iNumAnimations, aiAnimation** ppAnimations)
{
	m_vecAnimations.reserve(iNumAnimations);

	for (_uint i = 0; i < iNumAnimations; i++)
	{
		ANIMATION* pAnimation = new ANIMATION;

		pAnimation->fTickPerSecond = static_cast<_float>(ppAnimations[i]->mTicksPerSecond);
		pAnimation->fDuration = static_cast<_float>(ppAnimations[i]->mDuration);

		pAnimation->Channels.reserve(ppAnimations[i]->mNumChannels);

		for (_uint j = 0; j < ppAnimations[i]->mNumChannels; j++)
		{
			CHANNEL pChannel{};

			pChannel.iNumKeyFrames = max(ppAnimations[i]->mChannels[j]->mNumScalingKeys, ppAnimations[i]->mChannels[j]->mNumRotationKeys);
			pChannel.iNumKeyFrames = max(ppAnimations[i]->mChannels[j]->mNumPositionKeys, pChannel.iNumKeyFrames);

			_float3 vScale{};
			_float4 vRotation{};
			_float3 vTranslation{};

			for (_uint k = 0; k < pChannel.iNumKeyFrames; k++)
			{
				KEYFRAME KeyFrame{};

				if (k < ppAnimations[i]->mChannels[j]->mNumScalingKeys)
				{
					memcpy(&vScale, &ppAnimations[i]->mChannels[j]->mScalingKeys[k].mValue, sizeof(_float3));
					KeyFrame.fTrackPosition = static_cast<_float>(ppAnimations[i]->mChannels[j]->mScalingKeys[k].mTime);
				}

				if (k < ppAnimations[i]->mChannels[j]->mNumRotationKeys)
				{
					vRotation.x = ppAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
					vRotation.y = ppAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
					vRotation.z = ppAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
					vRotation.w = ppAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
					KeyFrame.fTrackPosition = static_cast<_float>(ppAnimations[i]->mChannels[j]->mRotationKeys[k].mTime);
				}

				if (k < ppAnimations[i]->mChannels[j]->mNumPositionKeys)
				{
					memcpy(&vTranslation, &ppAnimations[i]->mChannels[j]->mPositionKeys[k].mValue, sizeof(_float3));
					KeyFrame.fTrackPosition = static_cast<_float>(ppAnimations[i]->mChannels[j]->mPositionKeys[k].mTime);
				}

				KeyFrame.vScale = vScale;
				KeyFrame.vRotation = vRotation;
				KeyFrame.vTranslation = vTranslation;

				pChannel.KeyFrames.push_back(KeyFrame);
			}

			auto	iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](BONE* pBone)->_bool
				{
					if (pBone->strName == ppAnimations[i]->mChannels[j]->mNodeName.data)
						return true;

					++pChannel.iBoneIndex;

					return false;
				});


			pAnimation->Channels.push_back(pChannel);
		}


		m_vecAnimations.push_back(pAnimation);
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
	
	for (auto& pBone : m_vecBones)
		Safe_Delete(pBone);
	m_vecBones.clear();

	for (auto& pAnimMesh : m_vecAnimMeshes)
	{
		pAnimMesh->BoneIndices.clear();
		pAnimMesh->Indices.clear();
		pAnimMesh->Vertices.clear();
		Safe_Delete(pAnimMesh);
	}
	m_vecAnimMeshes.clear();

	for (auto& pNonAnimMesh : m_vecNonAnimMeshes)
	{
		pNonAnimMesh->Vertices.clear();
		pNonAnimMesh->Indices.clear();
		Safe_Delete(pNonAnimMesh);
	}
	m_vecNonAnimMeshes.clear();

	for (auto& pMaterial : m_vecMaterials)
	{
		pMaterial->vecTextures.clear();
		Safe_Delete(pMaterial);
	}
	m_vecMaterials.clear();
}
