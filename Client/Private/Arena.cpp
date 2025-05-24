#include "Arena.h"

CArena::CArena(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMap{pDevice, pContext }
{
}

CArena::CArena(const CArena& Prototype)
    : CMap(Prototype)
{
}

HRESULT CArena::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CArena::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CArena::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CArena::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

    return __super::Update(fTimeDelta);
}

void CArena::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CArena::Render()
{
    return  __super::Render();
}

HRESULT CArena::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Arena"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CArena* CArena::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CArena* pInstance = new CArena(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArena");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArena::Clone(void* pArg)
{
	CArena* pInstance = new CArena(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CArena");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArena::Free()
{
	__super::Free();
}
