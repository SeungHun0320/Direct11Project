#include "Effect_Dagger.h"
#include "GameInstance.h"

#include "Effect_Part.h"

CEffect_Dagger::CEffect_Dagger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart{ pDevice, pContext }
{
}

CEffect_Dagger::CEffect_Dagger(const CEffect_Dagger& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_Dagger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Dagger::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Dagger::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Dagger::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEffect_Dagger::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Dagger::Render()
{
	return S_OK;
}

void CEffect_Dagger::Effect_Reset(_uint iPart)
{
	dynamic_cast<CEffect_Part*>(m_PartObjects[iPart])->Effect_Reset();
}

HRESULT CEffect_Dagger::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Dagger::Ready_PartObjects()
{
	CEffect_Part::DESC LineDesc{};

	LineDesc.pParentLevelID = m_pLevelID;
	LineDesc.pParentMatrix = m_pParentMatrix;
	LineDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/PotionLine.Effect_Ex");
	LineDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_CrossLine");
	LineDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_PotionLine");

	if (FAILED(__super::Add_PartObject(PART_ICE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &LineDesc)))
		return E_FAIL;

	CEffect_Part::DESC ConfettiDesc{};

	ConfettiDesc.pParentLevelID = m_pLevelID;
	ConfettiDesc.pParentMatrix = m_pParentMatrix;
	ConfettiDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/Confetti.Effect_Ex");
	ConfettiDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SpinningDiamond");
	ConfettiDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_Confetti");

	if (FAILED(__super::Add_PartObject(PART_SWIRL, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &ConfettiDesc)))
		return E_FAIL;

	CEffect_Part::DESC HealDesc{};

	HealDesc.pParentLevelID = m_pLevelID;
	HealDesc.pParentMatrix = m_pParentMatrix;
	HealDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/HealthSpell.Effect_Ex");
	HealDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SpinningDiamond");
	HealDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HealthSpell");

	if (FAILED(__super::Add_PartObject(PART_STARBURST, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &HealDesc)))
		return E_FAIL;


	return S_OK;
}

CEffect_Dagger* CEffect_Dagger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Dagger* pInstance = new CEffect_Dagger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Dagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Dagger::Clone(void* pArg)
{
	CEffect_Dagger* pInstance = new CEffect_Dagger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Dagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Dagger::Free()
{
	__super::Free();
}
