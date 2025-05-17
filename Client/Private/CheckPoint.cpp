#include "CheckPoint.h"
#include "GameInstance.h"

CCheckPoint::CCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{pDevice, pContext}
{
}

CCheckPoint::CCheckPoint(const CCheckPoint& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CCheckPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCheckPoint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCheckPoint::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CCheckPoint::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CCheckPoint::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCheckPoint::Render()
{
	return __super::Render();
}

HRESULT CCheckPoint::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototpye_Component_Model_CheckPoint"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CCheckPoint* CCheckPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCheckPoint* pInstance = new CCheckPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCheckPoint::Clone(void* pArg)
{
	CCheckPoint* pInstance = new CCheckPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCheckPoint::Free()
{
    __super::Free();
}
