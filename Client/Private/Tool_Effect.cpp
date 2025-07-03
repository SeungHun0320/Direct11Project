#include "Tool_Effect.h"
#include "GameInstance.h"

#include "Particle_Tool.h"

CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool {pDevice, pContext}
{
}

HRESULT CTool_Effect::Initialize()
{
	if (FAILED(Ready_Tool_Particle(TEXT("Layer_Particle_Tool"))))
		return E_FAIL;

	Add_ListBoxName();

	return S_OK;
}

void CTool_Effect::Update(_float fTimeDelta)
{
}

HRESULT CTool_Effect::Render()
{
	ImGui::Begin(u8"이펙트 툴");
	Check_SelectedTool();

	Adjust_ParticleDesc();
	Save_Menu();
	Load_Menu();

	ImGui::End();

	return S_OK;
}

HRESULT CTool_Effect::Render_ExtraUI()
{
	return S_OK;
}

void CTool_Effect::Add_ListBoxName()
{
	vector<_wstring> EffectFilters = {
		L"Particle", L"Effect",
	};

	for (const auto& Pair : *m_pGameInstance->Get_Prototypes(ENUM_CLASS(LEVEL::TOOLS)))
	{
		if (Pair.first.find(TEXT("Prototype_Component_Texture_")) == std::wstring::npos)
			continue;

		for (const auto& KeyWord : EffectFilters)
		{
			if (Pair.first.find(KeyWord) != _wstring::npos)
			{
				m_ProtoTextureNames.push_back(m_pGameInstance->WStringToString(Pair.first));
			}
		}
	}
}

void CTool_Effect::Adjust_ParticleDesc()
{
	_uint iMinNumInstance{ 1 }, iMaxNumInstance{ 10000 };

	ImGui::SliderScalar(u8"인스턴스 갯수", ImGuiDataType_U32, &m_ParticleDesc.iNumInstance, &iMinNumInstance, &iMaxNumInstance);
	ImGui::InputScalar("##NumInstance", ImGuiDataType_U32, &m_ParticleDesc.iNumInstance, &iMinNumInstance, &iMaxNumInstance);

	ImGui::Separator();
	ImGui::DragFloat2(u8"사이즈", reinterpret_cast<_float*>(&m_ParticleDesc.vSize), m_fMinSize);
	ImGui::InputFloat2("##Size", reinterpret_cast<_float*>(&m_ParticleDesc.vSize));

	ImGui::Separator();
	ImGui::DragFloat3(u8"거리", reinterpret_cast<_float*>(&m_ParticleDesc.vRange), m_fMinRange);
	ImGui::InputFloat3("##Range", reinterpret_cast<_float*>(&m_ParticleDesc.vRange));

	ImGui::Separator();
	ImGui::DragFloat3(u8"센터", reinterpret_cast<_float*>(&m_ParticleDesc.vCenter), m_fMinCenter);
	ImGui::InputFloat3("##Center", reinterpret_cast<_float*>(&m_ParticleDesc.vCenter));

	ImGui::Separator();
	ImGui::DragFloat3(u8"피벗", reinterpret_cast<_float*>(&m_ParticleDesc.vPivot), m_fMinPivot);
	ImGui::InputFloat3("##Pivot", reinterpret_cast<_float*>(&m_ParticleDesc.vPivot));

	ImGui::Separator();
	ImGui::DragFloat2(u8"라이프타임", reinterpret_cast<_float*>(&m_ParticleDesc.vLifeTime), m_fMinLifeTime);
	ImGui::InputFloat2("##LifeTime", reinterpret_cast<_float*>(&m_ParticleDesc.vLifeTime));
	
	ImGui::Separator();
	ImGui::DragFloat2(u8"회전값", reinterpret_cast<_float*>(&m_ParticleDesc.vRotationZ), m_fMinRange);
	ImGui::InputFloat2("##Rotation", reinterpret_cast<_float*>(&m_ParticleDesc.vRotationZ));

	ImGui::Separator();
	ImGui::DragFloat2(u8"속도", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed), m_fMinSpeed);
	ImGui::InputFloat2("##Speed", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed));

	ImGui::Checkbox(u8"루프", reinterpret_cast<_bool*>(&m_ParticleDesc.isLoop));

	_int iType = ENUM_CLASS(m_eMove);
	if (ImGui::RadioButton(u8"드롭", iType == ENUM_CLASS(EFFECT_MOVE::DROP)))
		m_eMove = EFFECT_MOVE::DROP;
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"스프레드", iType == ENUM_CLASS(EFFECT_MOVE::SPREAD)))
		m_eMove = EFFECT_MOVE::SPREAD;

	m_pParticleTool->Change_Move(m_eMove);
	
	if (ImGui::RadioButton(u8"스프라이트", m_isSprite))
		m_isSprite = !m_isSprite;

	if (ImGui::RadioButton(u8"소프트 블렌드", m_isSoft))
		m_isSoft = !m_isSoft;

	if (m_isSprite && !m_isSoft)
		m_ePass = EFFECT_PASS::SPRITE;
	else if (m_isSprite && m_isSoft)
		m_ePass = EFFECT_PASS::SPRITE_BLEND;
	else if (!m_isSprite && !m_isSoft)
		m_ePass = EFFECT_PASS::ROTATION;
	else if (!m_isSprite && m_isSoft)
		m_ePass = EFFECT_PASS::ROTATION_BLEND;

	m_pParticleTool->Change_Pass(m_ePass);

	Adjust_ParticeSpriteDesc();

	if (ImGui::Button(u8"적용"))
	{
		m_pVIBufferTool->Set_NumInstnace(m_ParticleDesc.iNumInstance);
		m_pVIBufferTool->Set_Size(m_ParticleDesc.vSize);
		m_pVIBufferTool->Set_Pivot(m_ParticleDesc.vPivot);
		m_pVIBufferTool->Set_isLoop(m_ParticleDesc.isLoop);
		m_pVIBufferTool->Set_Speed(m_ParticleDesc.vSpeed);
		m_pVIBufferTool->Set_Translation(m_ParticleDesc.vCenter, m_ParticleDesc.vRange);
		m_pVIBufferTool->Set_LifeTime(m_ParticleDesc.vLifeTime);
		m_pVIBufferTool->Set_FrameXY(m_ParticleDesc.vFrameXY);
		m_pVIBufferTool->Set_SpriteSpeed(m_ParticleDesc.vFrameSpeed);
		m_pVIBufferTool->Set_RotationZ(m_ParticleDesc.vRotationZ);
	}


	ImGui::Text(u8"색깔");

	ImGui::ColorEdit4("RGBA Color", reinterpret_cast<_float*>(&m_vColor));
	m_pParticleTool->Change_Color(m_vColor);

	Change_TextureListBox();
}

void CTool_Effect::Adjust_ParticeSpriteDesc()
{
	if (!m_isSprite)
		return;

	ImGui::Separator();
	ImGui::InputFloat2(u8"텍스쳐 행, 열", reinterpret_cast<_float*>(&m_ParticleDesc.vFrameXY));

	ImGui::Separator();
	ImGui::DragFloat2(u8"스프라이트 속도", reinterpret_cast<_float*>(&m_ParticleDesc.vFrameSpeed), m_fMinSpeed);
	ImGui::InputFloat2("##SpriteSpeed", reinterpret_cast<_float*>(&m_ParticleDesc.vFrameSpeed));

}

void CTool_Effect::Change_TextureListBox()
{
	static _int iCurrentTextureIndex = { -1 }, iOldTextureIndex = { -1 };

	// 리스트박스 출력
	ImGui::Text(u8"택스쳐 프로토타입");
	if (ImGui::BeginListBox("##ProtoTexture", ImVec2(300, 100)))
	{
		for (_uint i = 0; i < m_ProtoTextureNames.size(); ++i)
		{
			_bool bSelected = (iCurrentTextureIndex == i);
			if (ImGui::Selectable(m_ProtoTextureNames[i].c_str(), bSelected))
				iCurrentTextureIndex = i;
		}
		ImGui::EndListBox();
	}

	if (iCurrentTextureIndex != iOldTextureIndex &&
		0 <= iCurrentTextureIndex &&
		m_ProtoTextureNames.size() >= iCurrentTextureIndex)
	{
		m_pParticleTool->Change_TextureCom(m_pGameInstance->StringToWString(m_ProtoTextureNames[iCurrentTextureIndex]));
		iOldTextureIndex = iCurrentTextureIndex;
	}
}

void CTool_Effect::Save_Load_Menu()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"이펙트"))
		{
			if (ImGui::MenuItem(u8"이펙트 저장"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../bin/DataFiles/Effect/";
				config.fileName = "ChooseFile";

				ImGuiFileDialog::Instance()->OpenDialog(
					"SaveEffect",            // 다이얼로그 Key
					u8"파일 선택",              // 타이틀
					".Effect",        // 필터 (여러 개 가능),
					config                    // 시작 경로
				);
			}
			if (ImGui::MenuItem(u8"이펙트 불러오기"))
			{
				IGFD::FileDialogConfig config{};

				config.path = "../bin/DataFiles/Effect/";
				config.fileName = "ChooseFile";

				ImGuiFileDialog::Instance()->OpenDialog(
					"LoadEffect",            // 다이얼로그 Key
					u8"파일 선택",              // 타이틀
					".Effect",        // 필터 (여러 개 가능),
					config                    // 시작 경로
				);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void CTool_Effect::Save_Menu()
{
	if (ImGuiFileDialog::Instance()->Display("SaveEffect"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			_string FilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			_string FileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			Save_ParticleDesc(FilePath + "/" + FileName);
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}
}

void CTool_Effect::Load_Menu()
{
	if (ImGuiFileDialog::Instance()->Display("LoadEffect"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())  // OK 눌렀다면
		{
			_string FilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			_string FileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			Load_ParticleDesc(FilePath + "/" + FileName);
		}

		ImGuiFileDialog::Instance()->Close(); // 꼭 닫아줘야 다시 열림
	}
}

HRESULT CTool_Effect::Save_ParticleDesc(const _string& strParticleFilePath)
{
	ofstream OutFile(strParticleFilePath, ios::binary);

	if (!OutFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.iNumInstance), sizeof(_uint));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vPivot), sizeof(_float3));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.isLoop), sizeof(_bool));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vSize), sizeof(_float2));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vRange), sizeof(_float3));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vCenter), sizeof(_float3));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vLifeTime), sizeof(_float2));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vSpeed), sizeof(_float2));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vFrameXY), sizeof(_float2));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vFrameSpeed), sizeof(_float2));
	OutFile.write(reinterpret_cast<const _char*>(&m_ParticleDesc.vRotationZ), sizeof(_float2));

	OutFile.close();

	ofstream OutFileEx(strParticleFilePath + "_Ex", ios::binary);

	if (!OutFileEx.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	OutFileEx.write(reinterpret_cast<const _char*>(&m_vColor), sizeof(_float4));
	OutFileEx.write(reinterpret_cast<const _char*>(&m_ePass), sizeof(EFFECT_PASS));
	OutFileEx.write(reinterpret_cast<const _char*>(&m_eMove), sizeof(EFFECT_MOVE));

	OutFileEx.close();

	MSG_BOX("파일 저장 성공");
	return S_OK;
}

HRESULT CTool_Effect::Load_ParticleDesc(const _string& strParticleFilePath)
{
	ifstream LoadFile(strParticleFilePath, ios::binary);

	if (!LoadFile.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.iNumInstance), sizeof(_uint));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vPivot), sizeof(_float3));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.isLoop), sizeof(_bool));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vSize), sizeof(_float2));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vRange), sizeof(_float3));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vCenter), sizeof(_float3));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vLifeTime), sizeof(_float2));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vSpeed), sizeof(_float2));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vFrameXY), sizeof(_float2));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vFrameSpeed), sizeof(_float2));
	LoadFile.read(reinterpret_cast<_char*>(&m_ParticleDesc.vRotationZ), sizeof(_float2));

	m_pVIBufferTool->Set_NumInstnace(m_ParticleDesc.iNumInstance);
	m_pVIBufferTool->Set_Size(m_ParticleDesc.vSize);
	m_pVIBufferTool->Set_Pivot(m_ParticleDesc.vPivot);
	m_pVIBufferTool->Set_isLoop(m_ParticleDesc.isLoop);
	m_pVIBufferTool->Set_Speed(m_ParticleDesc.vSpeed);
	m_pVIBufferTool->Set_Translation(m_ParticleDesc.vCenter, m_ParticleDesc.vRange);
	m_pVIBufferTool->Set_LifeTime(m_ParticleDesc.vLifeTime);
	m_pVIBufferTool->Set_FrameXY(m_ParticleDesc.vFrameXY);
	m_pVIBufferTool->Set_SpriteSpeed(m_ParticleDesc.vFrameSpeed);
	m_pVIBufferTool->Set_RotationZ(m_ParticleDesc.vRotationZ);

	LoadFile.close();

	ifstream LoadFileEx(strParticleFilePath + "_Ex", ios::binary);

	if (!LoadFileEx.is_open())
	{
		MSG_BOX("파일 개방 실패,,");
		return E_FAIL;
	}

	LoadFileEx.read(reinterpret_cast<_char*>(&m_vColor), sizeof(_float4));
	LoadFileEx.read(reinterpret_cast<_char*>(&m_ePass), sizeof(EFFECT_PASS));
	LoadFileEx.read(reinterpret_cast<_char*>(&m_eMove), sizeof(EFFECT_MOVE));

	LoadFileEx.close();

	MSG_BOX("파일 로드 성공");

	return S_OK;
}

HRESULT CTool_Effect::Ready_Tool_Particle(const _wstring& strLayerTag)
{
	CParticle_Tool::DESC tDesc{};
	tDesc.strName = TEXT("Particle_Tool");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Particle_Tool"),
		ENUM_CLASS(LEVEL::TOOLS), strLayerTag, &tDesc)))
		return E_FAIL;

	m_pParticleTool = dynamic_cast<CParticle_Tool*>(m_pGameInstance->Find_ObjectByName(ENUM_CLASS(LEVEL::TOOLS), strLayerTag, tDesc.strName));
	if (nullptr == m_pParticleTool)
		return E_FAIL;

	Safe_AddRef(m_pParticleTool);

	m_pVIBufferTool = dynamic_cast<CVIBuffer_Point_Instance_Tool*>(m_pParticleTool->Get_Component(TEXT("Com_VIBuffer")));
	if (nullptr == m_pVIBufferTool)
		return E_FAIL;

	Safe_AddRef(m_pVIBufferTool);
	

	return S_OK;
}

CTool_Effect* CTool_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect* pInstance = new CTool_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CTool_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Effect::Free()
{
	__super::Free();
	
	Safe_Release(m_pParticleTool);
	Safe_Release(m_pVIBufferTool);

}
