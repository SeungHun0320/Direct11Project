#include "Effect_BossSteam.h"
#include "GameInstance.h"

CEffect_BossSteam::CEffect_BossSteam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :  CEffect_Part{pDevice, pContext}
{
}

CEffect_BossSteam::CEffect_BossSteam(const CEffect_BossSteam& Prototype)
    :  CEffect_Part(Prototype)
{
}

HRESULT CEffect_BossSteam::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_BossSteam::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentisInBattle = pDesc->pParentisInBattle;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEffect_BossSteam::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentisInBattle))
        return;

	__super::Priority_Update(fTimeDelta);
}

LIFE CEffect_BossSteam::Update(_float fTimeDelta)
{
    if (!(*m_pParentisInBattle))
        return LIFE::NONE;

    switch (m_eMoveType)
    {
    case EFFECT_MOVE::DROP:
        m_pVIBufferCom->Drop(fTimeDelta);
        break;
    case EFFECT_MOVE::SPREAD:
        m_pVIBufferCom->Spread(fTimeDelta);
        break;
    case EFFECT_MOVE::CHASE:
        _matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);
        m_pVIBufferCom->MoveTrail(XMVectorSetW(ParentMatrix.r[3], 0.f), fTimeDelta);
        break;
    default:
        break;

    }

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * XMLoadFloat4x4(m_pParentMatrix));

    return LIFE::NONE;
}

void CEffect_BossSteam::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentisInBattle))
        return;

    __super::Late_Update(fTimeDelta);
}

HRESULT CEffect_BossSteam::Render()
{
    if (FAILED(m_pEffectTextureCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pEffectTextureCom[NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
        return E_FAIL;

    return __super::Render();
}

HRESULT CEffect_BossSteam::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;  

    /* For.Com_TextureMask */
    if (FAILED(__super::Add_Component(ENUM_CLASS(*m_pParentLevelID), TEXT("Prototype_Component_Texture_SteamMask"),
        TEXT("Com_TextureMask"), reinterpret_cast<CComponent**>(&m_pEffectTextureCom[MASK]))))
        return E_FAIL;

    /* For.Com_TextureNoise */
    if (FAILED(__super::Add_Component(ENUM_CLASS(*m_pParentLevelID), TEXT("Prototype_Component_Texture_SteamNoise"),
        TEXT("Com_TextureNoise"), reinterpret_cast<CComponent**>(&m_pEffectTextureCom[NOISE]))))
        return E_FAIL;

    return S_OK;
}

CEffect_BossSteam* CEffect_BossSteam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffect_BossSteam* pInstance = new CEffect_BossSteam(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEffect_BossSteam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_BossSteam::Clone(void* pArg)
{
    CEffect_BossSteam* pInstance = new CEffect_BossSteam(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEffect_BossSteam");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_BossSteam::Free()
{
    __super::Free();

	Safe_Release(m_pEffectTextureCom[MASK]); 
	Safe_Release(m_pEffectTextureCom[NOISE]); 
}
