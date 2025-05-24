#include "Main.h"

CMain::CMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMap{ pDevice, pContext }
{
}

CMain::CMain(const CMain& Prototype)
	: CMap(Prototype)
{
}

HRESULT CMain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMain::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CMain::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CMain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMain::Render()
{
	return  __super::Render();
}

HRESULT CMain::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Main"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CMain* CMain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMain* pInstance = new CMain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMain::Clone(void* pArg)
{
	CMain* pInstance = new CMain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain::Free()
{
	__super::Free();
}
