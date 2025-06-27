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

void CTool_Effect::Adjust_ParticleDesc()
{
	_uint iMinNumInstance{1}, iMaxNumInstance{10000};

	ImGui::SliderScalar(u8"인스턴스 갯수", ImGuiDataType_U32, &m_ParticleDesc.iNumInstance, &iMinNumInstance, &iMaxNumInstance);
	ImGui::InputScalar("##NumInstance", ImGuiDataType_U32, &m_ParticleDesc.iNumInstance, &iMinNumInstance, &iMaxNumInstance);

	ImGui::Separator();
	ImGui::SliderFloat2(u8"사이즈", reinterpret_cast<_float*>(&m_ParticleDesc.vSize), m_fMinSize, m_fMaxSize);
	ImGui::InputFloat2("##Size", reinterpret_cast<_float*>(&m_ParticleDesc.vSize));
	
	ImGui::Separator();
	ImGui::SliderFloat3(u8"거리", reinterpret_cast<_float*>(&m_ParticleDesc.vRange), m_fMinRange, m_fMaxRange);
	ImGui::InputFloat3("##Range", reinterpret_cast<_float*>(&m_ParticleDesc.vRange));
	
	ImGui::Separator();
	ImGui::SliderFloat3(u8"센터", reinterpret_cast<_float*>(&m_ParticleDesc.vCenter), m_fMinCenter, m_fMaxCenter);
	ImGui::InputFloat3("##Center", reinterpret_cast<_float*>(&m_ParticleDesc.vCenter));
	
	ImGui::Separator();
	ImGui::SliderFloat3(u8"피벗", reinterpret_cast<_float*>(&m_ParticleDesc.vPivot), m_fMinPivot, m_fMaxPivot);
	ImGui::InputFloat3("##Pivot", reinterpret_cast<_float*>(&m_ParticleDesc.vPivot));
	
	ImGui::Separator();
	ImGui::SliderFloat2(u8"라이프타임", reinterpret_cast<_float*>(&m_ParticleDesc.vLifeTime), m_fMinLifeTime, m_fMaxLifeTime);
	ImGui::InputFloat2("##LifeTime", reinterpret_cast<_float*>(&m_ParticleDesc.vLifeTime));
	
	ImGui::Separator();
	ImGui::SliderFloat2(u8"속도", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed), m_fMinSpeed, m_fMaxSpeed);
	ImGui::InputFloat2("##Speed", reinterpret_cast<_float*>(&m_ParticleDesc.vSpeed));
	
	ImGui::Checkbox(u8"루프", reinterpret_cast<_bool*>(&m_ParticleDesc.isLoop));
	

	if (ImGui::Button(u8"적용"))
	{
		m_pVIBufferTool->Set_NumInstnace(m_ParticleDesc.iNumInstance);
		m_pVIBufferTool->Set_Size(m_ParticleDesc.vSize);
		m_pVIBufferTool->Set_Pivot(m_ParticleDesc.vPivot);
		m_pVIBufferTool->Set_isLoop(m_ParticleDesc.isLoop);
		m_pVIBufferTool->Set_Speed(m_ParticleDesc.vSpeed);
		m_pVIBufferTool->Set_Translation(m_ParticleDesc.vCenter, m_ParticleDesc.vRange);
		m_pVIBufferTool->Set_LifeTime(m_ParticleDesc.vLifeTime);

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
