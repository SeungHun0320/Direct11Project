#include "Effect_Hit.h"
#include "GameInstance.h"

#include "Effect_Part.h"

CEffect_Hit::CEffect_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectContainerPart{ pDevice, pContext }
{
}

CEffect_Hit::CEffect_Hit(const CEffect_Hit& Prototype)
	:CEffectContainerPart(Prototype)
{
}

HRESULT CEffect_Hit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Hit::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_isActive = true;

	return S_OK;
}

void CEffect_Hit::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Hit::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEffect_Hit::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Hit::Render()
{
	return S_OK;
}

void CEffect_Hit::Effect_Reset(_uint iPart)
{
	dynamic_cast<CEffect_Part*>(m_PartObjects[iPart])->Effect_Reset();
}

HRESULT CEffect_Hit::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Hit::Ready_PartObjects()
{
	CEffect_Part::DESC SplashDesc{};

	SplashDesc.pParentLevelID = m_pLevelID;
	SplashDesc.pParentMatrix = m_pParentMatrix;
	SplashDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Hit/HitSplash.Effect_Ex");
	SplashDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SplashEffect");
	SplashDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HitSplashEffect");

	if (FAILED(__super::Add_PartObject(PART_SPLASH, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &SplashDesc)))
		return E_FAIL;

	CEffect_Part::DESC LineDesc{};

	LineDesc.pParentLevelID = m_pLevelID;
	LineDesc.pParentMatrix = m_pParentMatrix;
	LineDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Hit/HitLine.Effect_Ex");
	LineDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_RingSpikes");
	LineDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_HitLineEffect");

	if (FAILED(__super::Add_PartObject(PART_LINE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &LineDesc)))
		return E_FAIL;


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

CEffect_Hit* CEffect_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Hit* pInstance = new CEffect_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Hit::Clone(void* pArg)
{
	CEffect_Hit* pInstance = new CEffect_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Hit::Free()
{
	__super::Free();
}
