#include "UI_Sprite.h"
#include "GameInstance.h"

CUI_Sprite::CUI_Sprite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI{ pDevice, pContext }
{
}

CUI_Sprite::CUI_Sprite(const CUI_Sprite& Prototype)
    : CUI(Prototype)
{
}

HRESULT CUI_Sprite::Initialize_Prototype(UI_TYPE eType)
{
    m_eType = eType;

    return S_OK;
}

HRESULT CUI_Sprite::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_fMaxFrame = pDesc->fMaxFrame;
    m_iColumns = pDesc->iColumns;
    m_fSpeed = pDesc->fSpeed;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUI_Sprite::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CUI_Sprite::Update(_float fTimeDelta)
{
    Move_Frame(fTimeDelta);

    return  __super::Update(fTimeDelta);
}

void CUI_Sprite::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUI_Sprite::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _int iCurFrame{ static_cast<_int>(m_fCurFrame) }, iMaxFrame{static_cast<_int>(m_fMaxFrame)};

    if (FAILED(m_pShaderCom->Bind_RawValue("g_iFrameIndex", &iCurFrame, sizeof(_int))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_iMaxFrame", &iMaxFrame, sizeof(_int))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_iColumns", &m_iColumns, sizeof(_int))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(m_eUIPass)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CUI_Sprite::Move_Frame(_float fTimeDelta)
{
    m_fCurFrame += m_fMaxFrame * m_fSpeed * fTimeDelta;
    if (m_fCurFrame >= m_fMaxFrame)
        m_fCurFrame = 0.f;
}

HRESULT CUI_Sprite::Ready_Components(void* pArg)
{
    if (__super::Ready_Components(pArg))
        return E_FAIL;

    return S_OK;
}

CUI_Sprite* CUI_Sprite::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType)
{
    CUI_Sprite* pInstance = new CUI_Sprite(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType)))
    {
        MSG_BOX("Failed to Created : CUI_Sprite");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Sprite::Clone(void* pArg)
{
    CUI_Sprite* pInstance = new CUI_Sprite(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Sprite");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Sprite::Free()
{
    __super::Free();
}
