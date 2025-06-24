#include "SpiderTankState.h"
#include "SpiderTank.h"

/* --------------------------
		    ÇÉÄ¡
------------------------- */
CSpiderTankState_Pinch::CSpiderTankState_Pinch(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Pinch::Enter(_float fTimeDelta)
{
	m_fDuration = 0.6f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::PINCH, false, 0.2f);
}

void CSpiderTankState_Pinch::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
}

void CSpiderTankState_Pinch::Exit()
{
	m_fDuration = 0.5f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_Pinch::Free()
{
	__super::Free();
}


/* --------------------------
		    ³Ë¹é
------------------------- */
CSpiderTankState_KnockBack::CSpiderTankState_KnockBack(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_KnockBack::Enter(_float fTimeDelta)
{
	m_fDuration = 1.95f;
	m_fTimeAcc = 0.f;

	_vector vPos = m_pOwner->Get_State(STATE::POSITION);
	_vector vTarget = m_pOwner->Get_TargetPosition();

	_vector vDir = XMVector3Normalize(vPos - vTarget);
	XMStoreFloat3(&m_vMoveDir, vDir);

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::KNOCKBACK, false, 0.2f);
}

void CSpiderTankState_KnockBack::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	else
		m_pOwner->Go_Dir(XMVectorSetW(XMLoadFloat3(&m_vMoveDir), 0.f), fTimeDelta, 6.f);
}

void CSpiderTankState_KnockBack::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_KnockBack::Free()
{
	__super::Free();
}

/* --------------------------
		    »ç¸Á
------------------------- */
CSpiderTankState_Dead::CSpiderTankState_Dead(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Dead::Enter(_float fTimeDelta)
{
	m_fDuration = 5.25f;
	m_fDuration = 10.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::DEAD_START, false, 0.2f);
	m_pOwner->Set_Dead(true);
}

void CSpiderTankState_Dead::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	if (m_fDuration <= m_fTimeAcc)
	{
		m_pOwner->Change_Camera(CAM_MODE::TPS);
		m_pOwner->Set_isInBattle(false);
	}
		
}

void CSpiderTankState_Dead::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_Dead::Free()
{
	__super::Free();
}
