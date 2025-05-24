#include "Bush.h"
#include "GameInstance.h"

CBush::CBush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{pDevice, pContext}
{
}

CBush::CBush(const CBush& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CBush::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBush::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBush::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBush::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CBush::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBush::Render()
{
	return __super::Render();
}

HRESULT CBush::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Bush"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CBush* CBush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBush* pInstance = new CBush(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBush::Clone(void* pArg)
{
	CBush* pInstance = new CBush(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBush::Free()
{
	__super::Free();
}
