#include "Obj_Particle.h"

#include "GameInstance.h"

CObj_Particle::CObj_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject {pDevice, pContext}
{
}

CObj_Particle::CObj_Particle(const CObj_Particle& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CObj_Particle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CObj_Particle::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CObj_Particle::Priority_Update(_float fTimeDelta)
{
}

LIFE CObj_Particle::Update(_float fTimeDelta)
{
    m_pVIBufferCom->Drop(fTimeDelta);

    return LIFE::NONE;
}

void CObj_Particle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CObj_Particle::Render()
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

HRESULT CObj_Particle::Ready_Components(void* pArg)
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

HRESULT CObj_Particle::Bind_ShaderResources()
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

CObj_Particle* CObj_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CObj_Particle* pInstance = new CObj_Particle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CObj_Particle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CObj_Particle::Clone(void* pArg)
{
    CObj_Particle* pInstance = new CObj_Particle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CObj_Particle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CObj_Particle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);
}
