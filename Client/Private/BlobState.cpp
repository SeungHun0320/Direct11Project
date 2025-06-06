#include "BlobState.h"
#include "Blob.h"

/* --------------------------
           아이들
------------------------- */

CBlobState_Idle::CBlobState_Idle(CBlob* pOwner)
    : CBlobState{ pOwner }
{
}

void CBlobState_Idle::Enter(_float fTimeDelta)
{
    m_fTimeAcc = 0.f;
    m_fCheckTime = 0.5f;

    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::ANIM_TYPE::IDLE, true, 0.2f);
}

void CBlobState_Idle::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta);

    if (m_fTimeAcc >= m_fCheckTime)
    {
        if (m_pOwner->Find_Player())
        {
            if (4.f <= m_pOwner->Get_DistanceToPlayer())
                m_pOwner->Change_States(CBlob::STATES::ATTACK);

            m_pOwner->Change_States(CBlob::STATES::JUMP);
        }
  
        m_fTimeAcc = 0.f;
    }
}

void CBlobState_Idle::Exit()
{
    m_fCheckTime = 0.f;
    m_fTimeAcc = 0.f;
}

void CBlobState_Idle::Free()
{
    __super::Free();
}

/* --------------------------
            어택
------------------------- */

CBlobState_Attack::CBlobState_Attack(CBlob* pOwner)
    : CBlobState{ pOwner }
{
}

void CBlobState_Attack::Enter(_float fTimeDelta)
{
    m_fDuration = 0.8f;
    m_fTimeAcc = 0.f;

    m_iAttackCount = 0;

    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::ANIM_TYPE::ATTACK, false, 0.2f);

}

void CBlobState_Attack::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fDuration <= m_fTimeAcc)
        m_pOwner->Set_Active();

    if (m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta))
    {
        m_pOwner->Set_Active(false);
        if (4.f <= m_pOwner->Get_DistanceToPlayer())
        {
            m_pOwner->Change_States(CBlob::STATES::JUMP);
        }
        else
            m_pOwner->Change_States(CBlob::STATES::IDLE);
    }
}

void CBlobState_Attack::Exit()
{
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
    m_iAttackCount = 0;
}

void CBlobState_Attack::Free()
{
    __super::Free();
}

/* --------------------------
          점프(이동)
------------------------- */

CBlobState_Jump::CBlobState_Jump(CBlob* pOwner)
    : CBlobState{ pOwner }
{
}

void CBlobState_Jump::Enter(_float fTimeDelta)
{
    m_fDuration = 1.f;
    m_fTimeAcc = 0.f;

    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::ANIM_TYPE::JUMP, true, 0.2f);
}

void CBlobState_Jump::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta);

    if (2.f <= m_pOwner->Get_DistanceToPlayer())
    {
        m_pOwner->Go_Target(m_pOwner->Get_TargetPosition(), fTimeDelta, 4.f);
    }
    else if (m_pOwner->Get_DistanceToPlayer() >= m_pOwner->Get_ChaseStopDistance())
    {
        m_pOwner->Change_States(CBlob::STATES::IDLE);
    }
    else if(m_fDuration <= m_fTimeAcc)
    {
        m_pOwner->Change_States(CBlob::STATES::ATTACK);

        m_fTimeAcc = 0.f;
    }
}

void CBlobState_Jump::Exit()
{
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
}

void CBlobState_Jump::Free()
{
    __super::Free();
}

/* --------------------------
           피격
------------------------- */

CBlobState_Hit::CBlobState_Hit(CBlob* pOwner)
    : CBlobState{ pOwner }
{
}

void CBlobState_Hit::Enter(_float fTimeDelta)
{
    m_fTimeAcc = 0.f;
    m_fDuration = 0.6f;

    m_pOwner->Set_TrackPosition(CBlob::PART_BODY, 0.f);
    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::IDLE, false, 1.f);
    XMStoreFloat3(&m_vHitDir, m_pOwner->Get_State(STATE::LOOK) * -1.f);
}

void CBlobState_Hit::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_fDuration <= m_fTimeAcc|| m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta))
    {
        m_pOwner->Change_States(CBlob::STATES::IDLE);
    }
    else if (0.4f >= m_fTimeAcc)
    {
        _float fScaleOffset = sinf(m_fTimeAcc * 30.f) * 0.1f;
        _float3 vScale = m_pOwner->Get_Scaled();

        vScale.x += fScaleOffset;
        vScale.y += fScaleOffset;
        vScale.z += fScaleOffset;

        m_pOwner->Scaling(vScale);
        m_pOwner->Hit(XMLoadFloat3(&m_vHitDir), fTimeDelta, 4.f);
    }
       

}

void CBlobState_Hit::Exit()
{
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
    m_pOwner->Scaling();
    XMStoreFloat3(&m_vHitDir, XMVectorZero());
}

void CBlobState_Hit::Free()
{
    __super::Free();
}
