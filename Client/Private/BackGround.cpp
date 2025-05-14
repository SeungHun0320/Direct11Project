#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext }
{

}

CBackGround::CBackGround(const CBackGround& Prototype)
	: CUI( Prototype )
{

}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBackGround::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBackGround::Render()
{
	return __super::Render();
}

HRESULT CBackGround::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackGround* pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}
