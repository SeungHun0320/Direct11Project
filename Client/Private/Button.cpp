#include "Button.h"
#include "GameInstance.h"

/*�ΰ�ȭ���� ��ư�̰�, ��Ʈ�� ����� ���°Ŷ� ��ŷ���̶��� ���� ������ �� ����*/
CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI{pDevice, pContext}
{
}

CButton::CButton(const CButton& Prototype)
    :CUI(Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CButton::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CButton::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CButton::Render()
{
    return __super::Render();
}

HRESULT CButton::Ready_Components(void* pArg)
{
    __super::Ready_Components(pArg);

    return S_OK;
}

CButton* CButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButton* pInstance = new CButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButton::Clone(void* pArg)
{
	CButton* pInstance = new CButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();
}
