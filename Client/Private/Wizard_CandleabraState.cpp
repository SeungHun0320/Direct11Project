#include "Wizard_CandleabraState.h"
#include "Wizard_Candleabra.h"

/* --------------------------
		   ¾ÆÀÌµé
------------------------- */
CWizard_CandleabraState_Idle::CWizard_CandleabraState_Idle(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState {pOwner}
{
}

void CWizard_CandleabraState_Idle::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fCheckTime = 0.5f;

	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::IDLE, true, 0.2f);
}

void CWizard_CandleabraState_Idle::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta);

	if (m_fTimeAcc >= m_fCheckTime)
	{
		if (m_pOwner->Find_Player())
		{
			m_pOwner->Change_States(CWizard_Candleabra::STATES::DETECTED);
		}

		m_fTimeAcc = 0.f;
	}
}

void CWizard_CandleabraState_Idle::Exit()
{
	m_fCheckTime = 0.f;
	m_fTimeAcc = 0.f;
}

void CWizard_CandleabraState_Idle::Free()
{
	__super::Free();
}


/* --------------------------
		   Àû¹ß°ß
------------------------- */

CWizard_CandleabraState_Detected::CWizard_CandleabraState_Detected(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Detected::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.8f;
	m_fChaseDistance = 10.f;
	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::DETECTED, false, 0.2f);

	_string strRandomNum = to_string(rand() % 5);
	m_pOwner->Play_Sound("Aggro_Vo_" + strRandomNum);
}

void CWizard_CandleabraState_Detected::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta))
	{
		if (m_fChaseDistance <= m_pOwner->Get_DistanceToPlayer() && m_pOwner->Get_ChaseStopDistance() >= m_pOwner->Get_DistanceToPlayer())
		{
			m_pOwner->Change_States(CWizard_Candleabra::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CWizard_Candleabra::STATES::GUARD);
	}

}

void CWizard_CandleabraState_Detected::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
	m_fChaseDistance = 0.f;
}

void CWizard_CandleabraState_Detected::Free()
{
	__super::Free();
}

/* --------------------------
		    °¡µå
------------------------- */

CWizard_CandleabraState_Guard::CWizard_CandleabraState_Guard(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Guard::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.5f;
	m_fChaseDistance = 10.f;
	m_fAttackDistnace = 6.f;
	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::GUARD, true, 0.2f);
}

void CWizard_CandleabraState_Guard::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta);

	m_pOwner->LookAt(m_pOwner->Get_TargetPosition(), fTimeDelta, 3.f);

	if (m_fDuration <= m_fTimeAcc)
	{
		if (m_fChaseDistance <= m_pOwner->Get_DistanceToPlayer() && m_pOwner->Get_ChaseStopDistance() >= m_pOwner->Get_DistanceToPlayer())
		{
			m_pOwner->Change_States(CWizard_Candleabra::STATES::MOVE);
		}
		else if (m_fAttackDistnace >= m_pOwner->Get_DistanceToPlayer())
		{
			m_pOwner->Change_States(CWizard_Candleabra::STATES::ATTACK);
		}
		else
			m_fTimeAcc = 0.f;
	}
}

void CWizard_CandleabraState_Guard::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
	m_fChaseDistance = 0.f;
	m_fAttackDistnace = 0.f;
	XMStoreFloat3(&m_vTargetPos, XMVectorZero());
}

void CWizard_CandleabraState_Guard::Free()
{
	__super::Free();
}

/* --------------------------
			Ç«Âï
------------------------- */

CWizard_CandleabraState_Attack::CWizard_CandleabraState_Attack(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Attack::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.1f;
	m_fChaseDistance = 10.f;
	m_isAttacked = false;
	XMStoreFloat3(&m_vTargetPos, m_pOwner->Get_TargetPosition());
	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::ATTACK, false, 0.2f);

	m_fAttackStartTime = 0.5f;

	_string strRandomNum = to_string(rand() % 5);
	m_pOwner->Play_Sound("Attack_Main_Vo_" + strRandomNum);
}

void CWizard_CandleabraState_Attack::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fAttackStartTime <= m_fTimeAcc && !m_isAttacked)
	{
		m_pOwner->Set_Active(CWizard_Candleabra::PART_CANDLEABRA);
		_string strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("Attack_Main_" + strRandomNum);
		m_pOwner->Play_Sound("Attack_Fire_" + strRandomNum);

		m_isAttacked = true;
	}
		

	m_pOwner->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetPos), 1.f), fTimeDelta, 1.f);

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta))
	{
		m_pOwner->Set_Active(CWizard_Candleabra::PART_CANDLEABRA, false);

		if (m_fChaseDistance <= m_pOwner->Get_DistanceToPlayer())
		{
			m_pOwner->Change_States(CWizard_Candleabra::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CWizard_Candleabra::STATES::GUARD);
	}

}

void CWizard_CandleabraState_Attack::Exit()
{
	m_fTimeAcc = 0.f;
	m_fChaseDistance = 0.f;
	m_fDuration = 0.f;
	m_fAttackStartTime = 0.f;
	m_isAttacked = false;
	XMStoreFloat3(&m_vTargetPos, XMVectorZero());
	m_pOwner->Set_Active(CWizard_Candleabra::PART_CANDLEABRA, false);
}

void CWizard_CandleabraState_Attack::Free()
{
	__super::Free();
}


/* --------------------------
			ÀÌµ¿
------------------------- */

CWizard_CandleabraState_Move::CWizard_CandleabraState_Move(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Move::Enter(_float fTimeDelta)
{
	m_fMaxDistance = 6.f;
	XMStoreFloat3(&m_vTargetPos, m_pOwner->Get_TargetPosition());
	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::FORWARD, true, 0.2f);
}

void CWizard_CandleabraState_Move::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta);

	m_pOwner->Go_Target(XMVectorSetW(XMLoadFloat3(&m_vTargetPos), 1.f), fTimeDelta, 6.f);

	if (m_fMaxDistance >= m_pOwner->Get_DistanceToPlayer())
		m_pOwner->Change_States(CWizard_Candleabra::STATES::GUARD);
}

void CWizard_CandleabraState_Move::Exit()
{
	m_fMaxDistance = 0.f;
	XMStoreFloat3(&m_vTargetPos, XMVectorZero());
}

void CWizard_CandleabraState_Move::Free()
{
	__super::Free();
}

/* --------------------------
			ÇÇ°Ý
------------------------- */
CWizard_CandleabraState_Hit::CWizard_CandleabraState_Hit(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Hit::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;

	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::PINCH, false, 0.2f);
}

void CWizard_CandleabraState_Hit::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->Get_IsHit())
	{
		m_fTimeAcc = 0.f;
		m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::PINCH, false, 0.2f);
		m_pOwner->Reset_IsHit();
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CWizard_Candleabra::STATES::GUARD);
}

void CWizard_CandleabraState_Hit::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CWizard_CandleabraState_Hit::Free()
{
	__super::Free();
}

/* --------------------------
			»ç¸Á
------------------------- */

CWizard_CandleabraState_Dead::CWizard_CandleabraState_Dead(CWizard_Candleabra* pOwner)
	: CWizard_CandleabraState{ pOwner }
{
}

void CWizard_CandleabraState_Dead::Enter(_float fTimeDelta)
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CWizard_Candleabra::PART_BODY, CWizard_Candleabra::DEAD, false, 0.2f);
	_string strRandomNum = to_string(rand() % 3);
	m_pOwner->Play_Sound("Death_" + strRandomNum);
}

void CWizard_CandleabraState_Dead::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if(m_pOwner->Play_Animation(CWizard_Candleabra::PART_BODY, fTimeDelta))
		m_pOwner->Set_Dead(true);
}

void CWizard_CandleabraState_Dead::Exit()
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;
}

void CWizard_CandleabraState_Dead::Free()
{
	__super::Free();
}
