#include "Wizard_SwordState.h"
#include "Wizard_Sword.h"

/* --------------------------
			아이들
------------------------- */

CWizard_SwordState_Idle::CWizard_SwordState_Idle(CWizard_Sword* pOwner)
	: CWizard_SwordState {pOwner}
{
}

void CWizard_SwordState_Idle::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::IDLE, true, 0.2f);
}

void CWizard_SwordState_Idle::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta);

	if (m_fTimeAcc >= m_fCheckTime)
	{
		if (m_pOwner->Find_Player())
		{
			m_pOwner->Change_States(CWizard_Sword::STATES::MOVE);
		}

		m_fTimeAcc = 0.f;
	}

}

void CWizard_SwordState_Idle::Exit()
{
	m_fCheckTime = 0.f;
	m_fTimeAcc = 0.f;
}

void CWizard_SwordState_Idle::Free()
{
	__super::Free();
}


/* --------------------------
			기도
------------------------- */
CWizard_SwordState_Pray::CWizard_SwordState_Pray(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Pray::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::PRAY, true, 0.2f);
}

void CWizard_SwordState_Pray::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta);

	if (m_fTimeAcc >= m_fCheckTime)
	{
		if (m_pOwner->Find_Player())
		{
			m_pOwner->Change_States(CWizard_Sword::STATES::DETECTED);
		}

		m_fTimeAcc = 0.f;
	}
}

void CWizard_SwordState_Pray::Exit()
{
	m_fTimeAcc = 0.f;
}

void CWizard_SwordState_Pray::Free()
{
	__super::Free();
}


/* --------------------------
			적발견
------------------------- */
CWizard_SwordState_Detected::CWizard_SwordState_Detected(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Detected::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.8f;
	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::DETECTED, false, 0.2f);
	_string strRandomNum = to_string(rand() % 5);
	m_pOwner->Play_Sound("Aggro_Vo_" + strRandomNum);

}

void CWizard_SwordState_Detected::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta))
	{
		if (10.f <= m_pOwner->Get_DistanceToPlayer() && m_pOwner->Get_ChaseStopDistance() >= m_pOwner->Get_DistanceToPlayer())
		{
			m_pOwner->Change_States(CWizard_Sword::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CWizard_Sword::STATES::ATTACK);
	}

}

void CWizard_SwordState_Detected::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CWizard_SwordState_Detected::Free()
{
	__super::Free();
}


/* --------------------------
			공격
------------------------- */
CWizard_SwordState_Attack::CWizard_SwordState_Attack(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Attack::Enter(_float fTimeDelta)
{
	m_byRandom = rand() % 2;
	m_isFirstAttacked = false;
	m_isSecondAttacked = false;
	XMStoreFloat3(&m_vTargetPos, m_pOwner->Get_TargetPosition());

	if (1 == m_byRandom)
	{
		m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::ATTACK1, false, 0.3f);
		m_fDuration = 2.f;
	}
	else
	{
		m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::ATTACK2, false, 0.2f);
		m_fDuration = 3.f;
	}

	_string strRandomNum = to_string(rand() % 5);
	m_pOwner->Play_Sound("AttackBackHand_Vo_" + strRandomNum);

	m_fAttackStartTime = 1.f;
}

void CWizard_SwordState_Attack::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fAttackStartTime <= m_fTimeAcc && !m_isFirstAttacked)
	{
		m_pOwner->Set_Active();

		_string strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("AttackBackHand_Fire_" + strRandomNum);
		m_pOwner->Play_Sound("AttackBackHand_Main_" + strRandomNum);
		m_isFirstAttacked = true;
	}


	if (0 == m_byRandom && m_fAttackStartTime + 1.f <= m_fTimeAcc && !m_isSecondAttacked)
	{
		_string strRandomNum = to_string(rand() % 5);
		m_pOwner->Play_Sound("AttackForeHand_Vo_" + strRandomNum);
		strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("AttackForeHand_Fire_" + strRandomNum);
		m_pOwner->Play_Sound("AttackForeHand_Main_" + strRandomNum);
		m_isSecondAttacked = true;
	}
		

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta))
	{
		m_pOwner->Set_Active(false);

		if (5.f <= m_pOwner->Get_DistanceToPlayer() && m_pOwner->Get_ChaseStopDistance() >= m_pOwner->Get_DistanceToPlayer())
			m_pOwner->Change_States(CWizard_Sword::STATES::MOVE);
		else
			m_pOwner->Change_States(CWizard_Sword::STATES::IDLE);
	}
	else
		m_pOwner->Go_Target(XMVectorSetW(XMLoadFloat3(&m_vTargetPos), 1.f), fTimeDelta, 3.f);
}

void CWizard_SwordState_Attack::Exit()
{
	XMStoreFloat3(&m_vTargetPos, XMVectorZero());
	m_fAttackStartTime = 0.f;
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_pOwner->Set_Active(false);
	m_isFirstAttacked = false;
	m_isSecondAttacked = false;
}

void CWizard_SwordState_Attack::Free()
{
	__super::Free();
}


/* --------------------------
			이동
------------------------- */
CWizard_SwordState_Move::CWizard_SwordState_Move(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Move::Enter(_float fTimeDelta)
{
	XMStoreFloat3(&m_vTargetPos, m_pOwner->Get_TargetPosition());
	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::FORWARD, true, 0.2f);
}

void CWizard_SwordState_Move::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta);

	m_pOwner->Go_Target(XMVectorSetW(XMLoadFloat3(&m_vTargetPos), 1.f), fTimeDelta, 4.f, 5.f);

	// 만약 포지션에 도착을 했을 때, 플레이어 거리가 멀다면?

	if (10.f > m_pOwner->Get_DistanceToPlayer())
	{
		if (5.f >= m_pOwner->Get_DistanceToPlayer())
			m_pOwner->Change_States(CWizard_Sword::STATES::ATTACK);
		else
			XMStoreFloat3(&m_vTargetPos, m_pOwner->Get_TargetPosition());
	}
	else
		m_pOwner->Change_States(CWizard_Sword::STATES::IDLE);

}

void CWizard_SwordState_Move::Exit()
{
	XMStoreFloat3(&m_vTargetPos, XMVectorZero());
}

void CWizard_SwordState_Move::Free()
{
	__super::Free();
}


/* --------------------------
			피격
------------------------- */
CWizard_SwordState_Hit::CWizard_SwordState_Hit(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Hit::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;

	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::PINCH, false, 0.2f);
}

void CWizard_SwordState_Hit::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->Get_IsHit())
	{
		m_fTimeAcc = 0.f;
		m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::PINCH, false, 0.2f);
		m_pOwner->Reset_IsHit();
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CWizard_Sword::STATES::ATTACK);
}

void CWizard_SwordState_Hit::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CWizard_SwordState_Hit::Free()
{
	__super::Free();
}


/* --------------------------
			사망
------------------------- */
CWizard_SwordState_Dead::CWizard_SwordState_Dead(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Dead::Enter(_float fTimeDelta)
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CWizard_Sword::PART_BODY, CWizard_Sword::DEAD, false, 0.2f);
}

void CWizard_SwordState_Dead::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.4f <= m_fTimeAcc && !m_isDeath)
	{
		_string strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("Death_" + strRandomNum);
		m_isDeath = true;
	}

	if (m_pOwner->Play_Animation(CWizard_Sword::PART_BODY, fTimeDelta))
		m_pOwner->Set_Dead(true);
}

void CWizard_SwordState_Dead::Exit()
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;
	m_isDeath = false;
}

void CWizard_SwordState_Dead::Free()
{
	__super::Free();
}
