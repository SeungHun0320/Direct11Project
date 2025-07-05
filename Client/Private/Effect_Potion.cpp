#include "Effect_Potion.h"
#include "GameInstance.h"

#include "Effect_Part.h"

CEffect_Potion::CEffect_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart{ pDevice, pContext }
{
}

CEffect_Potion::CEffect_Potion(const CEffect_Potion& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_Potion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Potion::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);	

	m_pParentIsUsePotion = pDesc->pParentIsUsePotion;

	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Potion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Potion::Update(_float fTimeDelta)
{
	if (!(*m_pParentIsUsePotion))
		m_isUse = false;

	if ((*m_pParentIsUsePotion) && !m_isUse)
	{
		for (_uint i = 0; i < PART_END; i++)
			Reset_Effect(i);
		m_isUse = true;
	}

	return __super::Update(fTimeDelta);
}

void CEffect_Potion::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Potion::Render()
{
	return S_OK;
}

void CEffect_Potion::Reset_Effect(_uint iPart)
{
	dynamic_cast<CEffect_Part*>(m_PartObjects[iPart])->Reset_Effect();
}

HRESULT CEffect_Potion::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Potion::Ready_PartObjects()
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

CEffect_Potion* CEffect_Potion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Potion* pInstance = new CEffect_Potion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Potion::Clone(void* pArg)
{
	CEffect_Potion* pInstance = new CEffect_Potion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Potion::Free()
{
	__super::Free();
}
