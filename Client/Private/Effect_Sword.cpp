#include "Effect_Sword.h"
#include "GameInstance.h"

#include "Effect_Part.h"

CEffect_Sword::CEffect_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart{ pDevice, pContext }
{
}

CEffect_Sword::CEffect_Sword(const CEffect_Sword& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Sword::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Sword::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Sword::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEffect_Sword::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Sword::Render()
{
	return S_OK;
}

void CEffect_Sword::Reset_Effect(_uint iPart)
{
	dynamic_cast<CEffect_Part*>(m_PartObjects[iPart])->Reset_Effect();
}

HRESULT CEffect_Sword::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Sword::Ready_PartObjects()
{
	CEffect_Part::DESC SplashDesc{};

	SplashDesc.pParentLevelID = m_pLevelID;
	SplashDesc.pParentMatrix = m_pParentMatrix;
	SplashDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Hit/Hit.Effect_Ex");
	SplashDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_RingSpikes");
	SplashDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HitEffect");

	if (FAILED(__super::Add_PartObject(PART_SPLASH, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &SplashDesc)))
		return E_FAIL;

	//CEffect_Part::DESC LineDesc{};

	//LineDesc.pParentLevelID = m_pLevelID;
	//LineDesc.pParentMatrix = m_pParentMatrix;
	//LineDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/PotionLine.Effect_Ex");
	//LineDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_CrossLine");
	//LineDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_SwordEffect");

	//if (FAILED(__super::Add_PartObject(PART_LINE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &LineDesc)))
	//	return E_FAIL;


	//CEffect_Part::DESC HealDesc{};

	//HealDesc.pParentLevelID = m_pLevelID;
	//HealDesc.pParentMatrix = m_pParentMatrix;
	//HealDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Potion/HealthSpell.Effect_Ex");
	//HealDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SpinningDiamond");
	//HealDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HealthSpell");

	//if (FAILED(__super::Add_PartObject(PART_TRAIL, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &HealDesc)))
	//	return E_FAIL;


	return S_OK;
}

CEffect_Sword* CEffect_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Sword* pInstance = new CEffect_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Sword::Clone(void* pArg)
{
	CEffect_Sword* pInstance = new CEffect_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Sword::Free()
{
	__super::Free();
}
