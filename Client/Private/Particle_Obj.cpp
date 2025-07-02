#include "Particle_Obj.h"

#include "GameInstance.h"

CParticle_Obj::CParticle_Obj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject {pDevice, pContext}
{
}

CParticle_Obj::CParticle_Obj(const CParticle_Obj& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CParticle_Obj::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CParticle_Obj::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CParticle_Obj::Priority_Update(_float fTimeDelta)
{
}

LIFE CParticle_Obj::Update(_float fTimeDelta)
{
    m_pVIBufferCom->Drop(fTimeDelta);

    return LIFE::NONE;
}

void CParticle_Obj::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);
}

HRESULT CParticle_Obj::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticle_Obj::Ready_Components(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPointInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), pDesc->strParticleBufferTag,
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), pDesc->strParticleTextureTag,
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticle_Obj::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CParticle_Obj* CParticle_Obj::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CParticle_Obj* pInstance = new CParticle_Obj(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CParticle_Obj");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CParticle_Obj::Clone(void* pArg)
{
    CParticle_Obj* pInstance = new CParticle_Obj(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CParticle_Obj");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CParticle_Obj::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}
