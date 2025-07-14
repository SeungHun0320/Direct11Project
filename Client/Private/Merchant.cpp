#include "Merchant.h"
#include "GameInstance.h"

#include "Body_Merchant.h"
#include "MerchantState.h"

CMerchant::CMerchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster{pDevice, pContext}
{
}

CMerchant::CMerchant(const CMerchant& Prototype)
    : CMonster(Prototype)
{
}

HRESULT CMerchant::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMerchant::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_fDetectDistance = 36.f;
    m_fChaseStopDistance = 10.f;

    /* 공격력 */
    m_fAttack = 999.f;
    m_fStaggerValue = 999.f;

    /* 체력 */
    m_fHp = 999.f;
    m_fMaxHp = m_fHp;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_States()))
        return E_FAIL;

    Change_States(STATES::HIDDEN);

    m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(180.f), XMConvertToRadians(0.f));

    return S_OK;
}

void CMerchant::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CMerchant::Update(_float fTimeDelta)
{
    if (m_pCurState)
    {
        if (m_eCurState != m_ePreState)
        {
            m_pCurState->Enter(fTimeDelta);
            m_ePreState = m_eCurState;
        }

        m_pCurState->Execute(fTimeDelta);
    }

    return __super::Update(fTimeDelta);
}

void CMerchant::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CMerchant::Render()
{
    return S_OK;
}

void CMerchant::Change_States(STATES eStates)
{
    if (m_pCurState)
        m_pCurState->Exit();

    if (nullptr != m_pCurState)
        Safe_Release(m_pCurState);

    m_pCurState = m_pStates[ENUM_CLASS(eStates)];
    Safe_AddRef(m_pCurState);

    m_eCurState = eStates;
}

_vector CMerchant::Get_State(STATE eState)
{
    return m_pTransformCom->Get_State(eState);
}

_bool CMerchant::Play_Animation(PART ePart, _float fTimeDelta)
{
    return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CMerchant::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
    m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CMerchant::Go_Left(_float fTimeDelta, _float fSpeed)
{
    m_pTransformCom->Set_SpeedPerSec(fSpeed);
    m_pTransformCom->Go_Left(fTimeDelta);
}

void CMerchant::Go_Right(_float fTimeDelta, _float fSpeed)
{
    m_pTransformCom->Set_SpeedPerSec(fSpeed);
    m_pTransformCom->Go_Right(fTimeDelta);
}

void CMerchant::Go_Up(_float fTimeDelta, _float fSpeed)
{
    m_pTransformCom->Set_SpeedPerSec(fSpeed);
    m_pTransformCom->Go_Up(fTimeDelta);
}

_bool CMerchant::is_TargetOnRight()
{
    _vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
    _vector vTargetPos = m_pTargetTransform->Get_State(STATE::POSITION);

    _vector vDir = XMVector3Normalize(vTargetPos - vPos);
    _vector vRight = m_pTransformCom->Get_State(STATE::RIGHT);

    return XMVectorGetX(XMVector3Dot(vDir, vRight)) > 0.f;
}

HRESULT CMerchant::Ready_Components(void* pArg)
{
    return S_OK;
}

HRESULT CMerchant::Ready_PartObjects()
{
    CBody_Merchant::DESC	BodyDesc{};

    BodyDesc.pParentLevelID = &m_eLevelID;
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BodyDesc.pOwner = this;
    BodyDesc.pParentisFind = &m_IsFind;

    if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Merchant"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMerchant::Ready_States()
{
    m_pStates[ENUM_CLASS(STATES::HIDDEN)]    = new CMerchantState_Hidden(this);
    m_pStates[ENUM_CLASS(STATES::WAKE_UP)]   = new CMerchantState_WakeUp(this);
    m_pStates[ENUM_CLASS(STATES::IDLE)]      = new CMerchantState_Idle(this);
    m_pStates[ENUM_CLASS(STATES::THANK_YOU)] = new CMerchantState_ThankYou(this);

    for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
    {
        if (nullptr == m_pStates[i])
            return E_FAIL;
    }

    return S_OK;
}

CMerchant* CMerchant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMerchant* pInstance = new CMerchant(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMerchant");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMerchant::Clone(void* pArg)
{
    CMerchant* pInstance = new CMerchant(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CMerchant");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMerchant::Free()
{
    __super::Free();

    Safe_Release(m_pCurState);

    for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
        Safe_Release(m_pStates[i]);
}
