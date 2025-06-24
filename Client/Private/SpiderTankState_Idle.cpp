#include "SpiderTankState.h"
#include "SpiderTank.h"

#include "GameInstance.h"

/* --------------------------
		    수면
------------------------- */
CSpiderTankState_Sleep::CSpiderTankState_Sleep(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Sleep::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::SLEEP, true, 0.2f);
}

void CSpiderTankState_Sleep::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	if (m_fDuration <= m_fTimeAcc)
	{
		if (m_pOwner->Find_Player())
		{
			m_pOwner->Change_States(CSpiderTank::STATES::WAKE_UP);
		}

		m_fTimeAcc = 0.f;
	}
}

void CSpiderTankState_Sleep::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_Sleep::Free()
{
	__super::Free();
}

/* --------------------------
		    기상
------------------------- */
CSpiderTankState_WakeUp::CSpiderTankState_WakeUp(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_WakeUp::Enter(_float fTimeDelta)
{
	m_fDuration = 4.5f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::WAKE_UP, false, 0.5f);
}

void CSpiderTankState_WakeUp::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_Camera(CAM_MODE::BOSS);
		m_pOwner->Set_isInBattle(true);
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}

}

void CSpiderTankState_WakeUp::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_WakeUp::Free()
{
	__super::Free();
}

/* --------------------------
		   아이들
------------------------- */
CSpiderTankState_Idle::CSpiderTankState_Idle(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Idle::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fAttackDelay = 1.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::IDLE, true, 0.2f);
}

void CSpiderTankState_Idle::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	const _float fAngle = m_pOwner->Compute_AngleToPlayer();
	const _float fDistance = m_pOwner->Get_DistanceToPlayer();
	const _float fPreferredDist = m_pOwner->Get_PreferredDistance();
	const _float fBackOffset{ 3.5f }, fForOffset{ 1.5f };

	m_pOwner->AttackCoolDownAcc(fTimeDelta);

	_float fRandom = CGameInstance::Get_Instance()->Compute_Random(0.f, 1.f);

	if (fAngle >= XMConvertToRadians(55.f))
	{
		Decide_Rotation();
		return;
	}

	if (fDistance < 5.f && fRandom < 0.3f)
	{
		m_pOwner->Change_States(CSpiderTank::STATES::REVERSE);
		return;
	}

	if (fDistance < fPreferredDist - fBackOffset)
	{
		m_fTooCloseAcc += fTimeDelta;

		if (m_fTooCloseAcc >= 2.f && fAngle < XMConvertToRadians(30.f))
		{
			m_pOwner->Change_States(CSpiderTank::STATES::BACKWARD);
			m_fTooCloseAcc = 0.f;
			return;
		}
	}
	else
		m_fTooCloseAcc = 0.f;
	

	if (fDistance > fPreferredDist + fForOffset)
	{
		m_pOwner->Change_States(CSpiderTank::STATES::FORWARD);
		return;
	}

	if (fDistance > fPreferredDist + 1.5f) {
		if (fAngle < XMConvertToRadians(15.f))
			m_pOwner->Change_States(CSpiderTank::STATES::FORWARD);
		else
			m_pOwner->Change_States(m_pOwner->Is_TargetOnRight() ? CSpiderTank::STATES::RTURN : CSpiderTank::STATES::LTURN);
		return;
	}

	if (fDistance >= fPreferredDist - fBackOffset &&
		fDistance <= fPreferredDist + fForOffset &&
		m_pOwner->Is_AttackCoolDownReady(m_fAttackDelay))
	{
		m_pOwner->Reset_AttackCoolDown();

		switch (m_pOwner->Get_Sequence())
		{
		case 0:
			m_pOwner->Change_States(CSpiderTank::STATES::FULLSWING);
			break;
		case 1:
			m_pOwner->Change_States(CSpiderTank::STATES::FAST_ATTACK);
			break;
		case 2:
			m_pOwner->Change_States(CSpiderTank::STATES::SWING);
			break;
		case 3:
			m_pOwner->Change_States(CSpiderTank::STATES::READY_BOMB);
			break;
		case 4:
			m_pOwner->Change_States(CSpiderTank::STATES::READY_SHOT);
			break;
		case 5:
			m_pOwner->Change_States(CSpiderTank::STATES::LAGER);
			break;
		default:
			m_pOwner->Reset_Sequence();
			return;
		}

		m_pOwner->Add_Sequence();
		return;
	}
}

void CSpiderTankState_Idle::Exit()
{
	m_fTimeAcc = 0.f;
	m_fAttackDelay = 0.f;
}

void CSpiderTankState_Idle::Decide_Rotation()
{
	if (m_pOwner->Is_TargetOnRight())
		m_pOwner->Change_States(CSpiderTank::STATES::RTURN);
	else
		m_pOwner->Change_States(CSpiderTank::STATES::LTURN);
}

void CSpiderTankState_Idle::Free()
{
	__super::Free();
}
