#include "UI_Animation.h"
#include "GameInstance.h"

CUI_Animation::CUI_Animation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI{pDevice, pContext}
{
}

CUI_Animation::CUI_Animation(const CUI_Animation& Prototype)
    : CUI(Prototype)
{
}

HRESULT CUI_Animation::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Animation::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pRatio = pDesc->pRatio;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUI_Animation::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CUI_Animation::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CUI_Animation::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUI_Animation::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", m_pRatio, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(2)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Animation::Ready_Components(void* pArg)
{
    if (__super::Ready_Components(pArg))
        return E_FAIL;

    return S_OK;
}

CUI_Animation* CUI_Animation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Animation* pInstance = new CUI_Animation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Animation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Animation::Clone(void* pArg)
{
	CUI_Animation* pInstance = new CUI_Animation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Animation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Animation::Free()
{
    __super::Free();
}
