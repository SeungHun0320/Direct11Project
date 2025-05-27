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
            m_pOwner->Change_States(CBlob::STATES::JUMP);

        m_fTimeAcc = 0.f;
    }
}

void CBlobState_Idle::Exit()
{
    m_fCheckTime = 0.f;
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
    m_fTimeAcc = 0.f;
    m_fDuration = 0.f;
    m_iAttackCount = 0;

    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::ANIM_TYPE::ATTACK, false, 0.2f);
}

void CBlobState_Attack::Execute(_float fTimeDelta)
{
    m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta);
}

void CBlobState_Attack::Exit()
{
    m_fTimeAcc = 0.f;
    m_fDuration = 0.f;
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
    m_pOwner->Change_Animation(CBlob::PART_BODY, CBlob::ANIM_TYPE::JUMP, true, 0.2f);
}

void CBlobState_Jump::Execute(_float fTimeDelta)
{
    m_pOwner->Play_Animation(CBlob::PART_BODY, fTimeDelta);

        //m_pOwner->Change_States(CBlob::STATES::IDLE);
    //else
    //    m_pOwner
}

void CBlobState_Jump::Exit()
{
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
}

void CBlobState_Hit::Execute(_float fTimeDelta)
{
}

void CBlobState_Hit::Exit()
{
}

void CBlobState_Hit::Free()
{
    __super::Free();
}
