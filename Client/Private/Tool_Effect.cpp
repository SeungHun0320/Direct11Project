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
	ImGui::DragFloat2(u8"스케일", reinterpret_cast<_float*>(&m_ParticleDesc.vScale), m_fMinSize);
	ImGui::InputFloat2("##Scale", reinterpret_cast<_float*>(&m_ParticleDesc.vScale));

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
	ImGui::DragFloat2(u8"속도", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed), m_fMinSpeed);
	ImGui::InputFloat2("##Speed", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed));

	ImGui::Checkbox(u8"루프", reinterpret_cast<_bool*>(&m_ParticleDesc.isLoop));

	static CParticle_Tool::MOVEMENT eType{};
	_int iType = static_cast<_int>(eType);
	if (ImGui::RadioButton(u8"드롭", iType == static_cast<_int>(CParticle_Tool::DROP)))
		eType = CParticle_Tool::DROP;
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"스프레드", iType == static_cast<_int>(CParticle_Tool::SPREAD)))
		eType = CParticle_Tool::SPREAD;

	m_pParticleTool->Change_Move(eType);
	
	if (ImGui::RadioButton(u8"스프라이트", m_isSprite))
	{
		m_isSprite = !m_isSprite;
	}
	m_isSprite == true ? m_pParticleTool->Change_Pass(CParticle_Tool::SPRITE) : m_pParticleTool->Change_Pass(CParticle_Tool::TOOL);

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
		m_pVIBufferTool->Set_Scale(m_ParticleDesc.vScale);
		m_pVIBufferTool->Set_SpriteSpeed(m_ParticleDesc.vFrameSpeed);
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
