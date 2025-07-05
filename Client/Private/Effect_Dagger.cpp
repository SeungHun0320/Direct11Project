#include "Effect_Dagger.h"
#include "GameInstance.h"

#include "Effect_Part.h"
#include "Effect_AnimMesh_Ice.h"

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

	m_pParentWeaponType = pDesc->pParentWeaponType;
	m_pParentisDaggerAttack = pDesc->pParentisDaggerAttack;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Dagger::Priority_Update(_float fTimeDelta)
{
	if (WEAPON_TYPE::DAGGER != (*m_pParentWeaponType))
		return;

	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_Dagger::Update(_float fTimeDelta)
{
	if (WEAPON_TYPE::DAGGER != (*m_pParentWeaponType))
		return LIFE::NONE;

	if ((*m_pParentisDaggerAttack) && !m_isUpdate)
	{
		Reset_Animation(PART_ICE);
		Reset_Effect(PART_SWIRL);
		Reset_Effect(PART_STARDUST);
		Set_MeshVisible(PART_ICE, false);

		m_isUpdate = true;
	}
	else if (!(*m_pParentisDaggerAttack))
	{
		Set_MeshVisible(PART_ICE);
		m_isUpdate = false;
	}
		

	return __super::Update(fTimeDelta);
}

void CEffect_Dagger::Late_Update(_float fTimeDelta)
{
	if (WEAPON_TYPE::DAGGER != (*m_pParentWeaponType))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect_Dagger::Render()
{
	return S_OK;
}

void CEffect_Dagger::Reset_Effect(_uint iPart)
{
	if (CEffect_Part* pPart = dynamic_cast<CEffect_Part*>(m_PartObjects[iPart]))
		pPart->Reset_Effect();
}

void CEffect_Dagger::Reset_Animation(_uint iPart)
{
	if(CEffect_AnimMesh* pPart = dynamic_cast<CEffect_AnimMesh*>(m_PartObjects[iPart]))
		pPart->Reset_Animation();	
}

void CEffect_Dagger::Set_MeshVisible(_uint iPart, _bool isVisible)
{
	if (CEffect_AnimMesh* pPart = dynamic_cast<CEffect_AnimMesh*>(m_PartObjects[iPart]))
		pPart->Set_MeshVisible(isVisible);
}

HRESULT CEffect_Dagger::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CEffect_Dagger::Ready_PartObjects()
{
	CEffect_AnimMesh_Ice::DESC IceDesc{};

	IceDesc.pParentLevelID = m_pLevelID;
	IceDesc.pParentMatrix = m_pParentMatrix;
	IceDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_IceBlast");

	if (FAILED(__super::Add_PartObject(PART_ICE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_AnimMesh_IceBlast"), &IceDesc)))
		return E_FAIL;

	CEffect_Part::DESC SwrilDesc{};
	
	SwrilDesc.pParentLevelID = m_pLevelID;
	SwrilDesc.pParentMatrix = m_pParentMatrix;
	SwrilDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Dagger/IceSwirl.Effect_Ex");
	SwrilDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_Swirl");
	SwrilDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_IceSwirl");
	SwrilDesc.eOrientation = CEffect_Part::LOCAL;
	
	if (FAILED(__super::Add_PartObject(PART_SWIRL, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &SwrilDesc)))
		return E_FAIL;
	
	CEffect_Part::DESC StardustDesc{};
	
	StardustDesc.pParentLevelID = m_pLevelID;
	StardustDesc.pParentMatrix = m_pParentMatrix;
	StardustDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Dagger/IceStardust.Effect_Ex");
	StardustDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_Starburst");
	StardustDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_IceStardust");
	StardustDesc.eOrientation = CEffect_Part::LOCAL;
	
	if (FAILED(__super::Add_PartObject(PART_STARDUST, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Part"), &StardustDesc)))
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
