#include "SpiderTank.h"

#include "GameInstance.h"
#include "Body_SpiderTank.h"

CSpiderTank::CSpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss{pDevice, pContext}
{
}

CSpiderTank::CSpiderTank(const CSpiderTank& Prototype)
	: CBoss(Prototype)
{
}

HRESULT CSpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderTank::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSpiderTank::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CSpiderTank::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CSpiderTank::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank::Render()
{
	return S_OK;
}

HRESULT CSpiderTank::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderTank::Ready_PartObjects()
{
	CBody_SpiderTank::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_SpiderTank"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CSpiderTank* CSpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank* pInstance = new CSpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank::Clone(void* pArg)
{
	CSpiderTank* pInstance = new CSpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank::Free()
{
	__super::Free();
}
