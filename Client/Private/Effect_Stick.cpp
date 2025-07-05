#include "Effect_Stick.h"
#include "GameInstance.h"

#include "Effect_Part.h"

CEffect_Stick::CEffect_Stick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart{ pDevice, pContext }
{
}

CEffect_Stick::CEffect_Stick(const CEffect_Stick& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_Stick::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Stick::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Stick::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Stick::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEffect_Stick::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Stick::Render()
{
	return S_OK;
}

void CEffect_Stick::Reset_Effect(_uint iPart)
{
	dynamic_cast<CEffect_Part*>(m_PartObjects[iPart])->Reset_Effect();
}

HRESULT CEffect_Stick::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Stick::Ready_PartObjects()
{
	CEffect_Part::DESC LineDesc{};

	LineDesc.pParentLevelID = m_pLevelID;
	LineDesc.pParentMatrix = m_pParentMatrix;
	LineDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/PotionLine.Effect_Ex");
	LineDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_CrossLine");
	LineDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_PotionLine");

	if (FAILED(__super::Add_PartObject(PART_LINE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &LineDesc)))
		return E_FAIL;

	CEffect_Part::DESC ConfettiDesc{};

	ConfettiDesc.pParentLevelID = m_pLevelID;
	ConfettiDesc.pParentMatrix = m_pParentMatrix;
	ConfettiDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/Confetti.Effect_Ex");
	ConfettiDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SpinningDiamond");
	ConfettiDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_Confetti");

	if (FAILED(__super::Add_PartObject(PART_CONFETTI, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &ConfettiDesc)))
		return E_FAIL;

	CEffect_Part::DESC HealDesc{};

	HealDesc.pParentLevelID = m_pLevelID;
	HealDesc.pParentMatrix = m_pParentMatrix;
	HealDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/HealthSpell.Effect_Ex");
	HealDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SpinningDiamond");
	HealDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HealthSpell");

	if (FAILED(__super::Add_PartObject(PART_HEALTH_SPELL, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &HealDesc)))
		return E_FAIL;


	return S_OK;
}

CEffect_Stick* CEffect_Stick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Stick* pInstance = new CEffect_Stick(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Stick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Stick::Clone(void* pArg)
{
	CEffect_Stick* pInstance = new CEffect_Stick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Stick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Stick::Free()
{
	__super::Free();
}
