#include "Part_Player.h"

#include "GameInstance.h"

CPart_Player::CPart_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{pDevice, pContext}
{
}

CPart_Player::CPart_Player(const CPart_Player& Prototype)
    : CPartObject(Prototype)
{
}

const _float4x4* CPart_Player::Get_SocketMatrix(const _string& strBoneName)
{
    return m_pModelCom->Get_BoneMatrix(strBoneName);
}

HRESULT CPart_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPart_Player::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_eLevelID = pDesc->eLevelID;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPart_Player::Priority_Update(_float fTimeDelta)
{
}

LIFE CPart_Player::Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

    return LIFE::NONE;
}

void CPart_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPart_Player::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMesh = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMesh; i++)
    {
        //m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0);

        if (m_pModelCom->Get_MeshVisible(i))
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
            return E_FAIL;

        m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

#ifdef _DEBUG

    m_pColliderCom->Render();

#endif

    return S_OK;
}

_bool CPart_Player::Play_Animation(_float fTimeDelta)
{
    return m_pModelCom->Play_Animation(fTimeDelta);
}

void CPart_Player::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
    m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CPart_Player::Set_MeshVisible(_uint iIndex, _bool IsVisible)
{
    m_pModelCom->Set_MeshVisible(iIndex, IsVisible);
}

void CPart_Player::Set_TrackPosition(_float fTrackPosition)
{
    m_pModelCom->Set_CurrnetTrackPosition(fTrackPosition);
}

_bool CPart_Player::Is_CurrentAnim(_uint iNextIndex)
{
    return m_pModelCom->Is_CurrentAnim(iNextIndex);
}

void CPart_Player::Set_Active(_bool isActive)
{
    m_pColliderCom->Set_Active(isActive);
}

CCollider* CPart_Player::Get_Collider(_uint iColliderIndex) const
{
    return m_pColliderCom;
}

HRESULT CPart_Player::Ready_Components(void* pArg)
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPart_Player::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

void CPart_Player::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);
}
