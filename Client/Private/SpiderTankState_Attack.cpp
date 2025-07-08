#include "SpiderTankState.h"
#include "SpiderTank.h"

/* --------------------------
		   ·¹ÀÌÀú
------------------------- */
CSpiderTankState_Lager::CSpiderTankState_Lager(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Lager::Enter(_float fTimeDelta)
{
	m_fDuration = 4.1f;
	m_fTimeAcc  = 0.f;
	m_fLagerTime = 1.8f;
	m_isShot = false;
	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::LAGER, false, 0.3f);

}

void CSpiderTankState_Lager::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fLagerTime <= m_fTimeAcc && !m_isShot)
	{
		m_pOwner->Shot_Lager();
		m_isShot = true;
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_Lager::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_fLagerTime = 0.f;
	m_isShot = false;
}

void CSpiderTankState_Lager::Free()
{
	__super::Free();
}


/* --------------------------
		  Àâ¸÷¼ÒÈ¯
------------------------- */
CSpiderTankState_SpawnMob::CSpiderTankState_SpawnMob(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_SpawnMob::Enter(_float fTimeDelta)
{
	m_fDuration = 2.f;
	m_fTimeAcc = 0.f;
	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::SPAWNMOB, false, 0.3f);
}

void CSpiderTankState_SpawnMob::Execute(_float fTimeDelta)
{
	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_SpawnMob::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_SpawnMob::Free()
{
	__super::Free();
}

/* --------------------------
		  ºü¸¥°ø°Ý
------------------------- */
CSpiderTankState_FastAttack::CSpiderTankState_FastAttack(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_FastAttack::Enter(_float fTimeDelta)
{
	m_fDuration = 2.2f;
	m_fTimeAcc = 0.f;
	m_fAttackStartTime = 1.f;

	_vector vLook  = m_pOwner->Get_State(STATE::LOOK);
	_vector vRight = m_pOwner->Get_State(STATE::RIGHT);

	_vector vDir = XMVector3Normalize(m_pOwner->Get_TargetPosition() - m_pOwner->Get_State(STATE::POSITION));

	_float fDot = XMVectorGetX(XMVector3Dot(vRight, vDir));

	if (0.3f < fDot)
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::R_ATTACK, false, 0.2f);
	else if (-0.3f > fDot)
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::L_ATTACK, false, 0.2f);
	else
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::MID_ATTACK, false, 0.2f);


}

void CSpiderTankState_FastAttack::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fAttackStartTime <= m_fTimeAcc)
	{
		m_pOwner->Set_Active(CSpiderTank::LEFT_ARM);
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Set_Active(CSpiderTank::LEFT_ARM, false);

		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_FastAttack::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_fAttackStartTime = 1.f;
	m_pOwner->Set_Active(CSpiderTank::LEFT_ARM, false);
}

void CSpiderTankState_FastAttack::Free()
{
	__super::Free();
}

/* --------------------------
		    ½ºÀ®
------------------------- */
CSpiderTankState_Swing::CSpiderTankState_Swing(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Swing::Enter(_float fTimeDelta)
{
	m_fDuration = 2.2f;
	m_fTimeAcc = 0.f;

	_vector vLook = m_pOwner->Get_State(STATE::LOOK);
	_vector vRight = m_pOwner->Get_State(STATE::RIGHT);

	_vector vDir = XMVector3Normalize(m_pOwner->Get_TargetPosition() - m_pOwner->Get_State(STATE::POSITION));

	_float fDot = XMVectorGetX(XMVector3Dot(vRight, vDir));

	if (-0.2f <= fDot)
	{
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::R_SWING, false, 0.2f);
		m_eArm = RIGHT;
	}
	else
	{
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::L_SWING, false, 0.2f);
		m_eArm = LEFT;
	}

	m_fAttackStartTime = 0.75f;

}

void CSpiderTankState_Swing::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fAttackStartTime <= m_fTimeAcc)
	{
		if (LEFT == m_eArm)
			m_pOwner->Set_Active(CSpiderTank::LEFT_ARM);
		else if (RIGHT == m_eArm)
			m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM);
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		if (LEFT == m_eArm)
			m_pOwner->Set_Active(CSpiderTank::LEFT_ARM, false);
		else if (RIGHT == m_eArm)
			m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM, false);

		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_Swing::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_fAttackStartTime = 0.75f;
	if (LEFT == m_eArm)
		m_pOwner->Set_Active(CSpiderTank::LEFT_ARM, false);
	else if (RIGHT == m_eArm)
		m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM, false);
}

void CSpiderTankState_Swing::Free()
{
	__super::Free();
}


/* --------------------------
			Ç®½ºÀ®
------------------------- */
CSpiderTankState_FullSwing::CSpiderTankState_FullSwing(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_FullSwing::Enter(_float fTimeDelta)
{
	m_fDuration = 4.f;
	m_fTimeAcc = 0.f;
	m_bBlendStarted = false;
	m_fAttackStartTime = 2.35f;

	_vector vPos = m_pOwner->Get_State(STATE::POSITION);
	_vector vTarget = m_pOwner->Get_TargetPosition();

	_vector vDir = XMVector3Normalize(vTarget - vPos);
	XMStoreFloat3(&m_vMoveDir, vDir);
	m_pOwner->LookAtYaw(vDir, 0.5f);

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::FULLSWING, false, 0.4f);
}

void CSpiderTankState_FullSwing::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fAttackStartTime <= m_fTimeAcc)
		m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM);

	if (3.f <= m_fTimeAcc && !m_bBlendStarted)
	{
		m_bBlendStarted = true;
		m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::RIGHT, false, 0.5f);
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{

		m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM, false);
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);

	}
	else if (2.15f <= m_fTimeAcc && 3.1f >= m_fTimeAcc)
		m_pOwner->Go_Dir(XMVectorSetW(XMLoadFloat3(&m_vMoveDir), 0.f), fTimeDelta, 15.f);
	
}

void CSpiderTankState_FullSwing::Exit()
{
	m_fDuration = 4.f;
	m_fTimeAcc = 0.f;
	m_bBlendStarted = false;
	m_fAttackStartTime = 2.35f;
	XMStoreFloat3(&m_vMoveDir, XMVectorZero());
	m_pOwner->Set_Active(CSpiderTank::RIGHT_ARM, false);
}

void CSpiderTankState_FullSwing::Free()
{
	__super::Free();
}


/* --------------------------
	    ÃÑ¾Ë»ç°ÝÁØºñ
------------------------- */
CSpiderTankState_ReadyShot::CSpiderTankState_ReadyShot(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_ReadyShot::Enter(_float fTimeDelta)
{
	m_fDuration = 0.9f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::READY_SHOT, false, 0.2f);
}

void CSpiderTankState_ReadyShot::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	
	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CSpiderTank::STATES::SHOT);

}

void CSpiderTankState_ReadyShot::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc  = 0.f;
}

void CSpiderTankState_ReadyShot::Free()
{
	__super::Free();
}

/* --------------------------
		  ÃÑ¾Ë»ç°Ý
------------------------- */
CSpiderTankState_Shot::CSpiderTankState_Shot(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Shot::Enter(_float fTimeDelta)
{
	// 6ÃÊÀÓ
	m_fDuration = 6.f;
	m_fTimeAcc = 0.f;

	m_fReloadTime = 0.2f;
	m_fShotTime = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::SHOT, true, 0.2f);
}

void CSpiderTankState_Shot::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fShotTime += fTimeDelta;

	_vector vPos = m_pOwner->Get_State(STATE::POSITION);
	_vector vTarget = m_pOwner->Get_TargetPosition();

	_vector vDir = XMVector3Normalize(vTarget - vPos);

	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	if (m_fReloadTime <= m_fShotTime)
	{
		if(m_pOwner->Compute_AngleToPlayer() <= XMConvertToRadians(60.f))
			m_pOwner->Shot_Bullet();

		m_fShotTime = 0.f;
	}

	if (m_fDuration <= m_fTimeAcc)
	{
		m_pOwner->Change_States(CSpiderTank::STATES::END_SHOT);
	}
	else
	{
		if (m_pOwner->Compute_AngleToPlayer() <= XMConvertToRadians(60.f))
		{
			m_pOwner->LookAt(XMVectorSetW(m_pOwner->Get_TargetPosition(), 1.f), fTimeDelta, 1.f);
			m_pOwner->Go_Dir(XMVectorSetW(vDir, 0.f), fTimeDelta, 2.f);
		}
	}
		
}

void CSpiderTankState_Shot::Exit()
{
	m_fDuration = 6.f;
	m_fTimeAcc = 0.f;

	m_fReloadTime = 0.1f;
	m_fShotTime = 0.f;
}

void CSpiderTankState_Shot::Free()
{
	__super::Free();
}


/* --------------------------
		ÃÑ¾Ë»ç°ÝÁßÁö
------------------------- */
CSpiderTankState_EndShot::CSpiderTankState_EndShot(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_EndShot::Enter(_float fTimeDelta)
{
	m_fDuration = 0.8f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::END_SHOT, true, 0.2f);
}

void CSpiderTankState_EndShot::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_EndShot::Exit()
{
	m_fDuration = 0.0f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_EndShot::Free()
{
	__super::Free();
}


/* --------------------------
		 ÆøÅº¹ß»çÁØºñ
------------------------- */
CSpiderTankState_ReadyBomb::CSpiderTankState_ReadyBomb(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_ReadyBomb::Enter(_float fTimeDelta)
{
	m_fDuration = 1.2f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::READY_BOMB, false, 0.3f);
}

void CSpiderTankState_ReadyBomb::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::SHOT_BOMB);
	}

}

void CSpiderTankState_ReadyBomb::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_ReadyBomb::Free()
{
	__super::Free();
}


/* --------------------------
		   ÆøÅº¹ß»ç
------------------------- */
CSpiderTankState_ShotBomb::CSpiderTankState_ShotBomb(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_ShotBomb::Enter(_float fTimeDelta)
{
	m_fDuration = 1.5f;
	m_fTimeAcc = 0.f;

	m_iShotCount = 0;
	m_fReloadTime = 0.5f;
	m_fShotTime = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::SHOT_BOMB, true, 0.3f);
}

void CSpiderTankState_ShotBomb::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fShotTime += fTimeDelta;

	if (m_fReloadTime <= m_fShotTime && 3 != m_iShotCount)
	{
		m_pOwner->Shot_Bomb();
		m_fShotTime = 0.f;
		++m_iShotCount;
	}
	
	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::END_BOMB);
	}
}

void CSpiderTankState_ShotBomb::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;

	m_iShotCount = 0;
	m_fReloadTime = 0.f;
	m_fShotTime = 0.f;
}

void CSpiderTankState_ShotBomb::Free()
{
	__super::Free();
}


/* --------------------------
		 ÆøÅº¹ß»çÁßÁö
------------------------- */
CSpiderTankState_EndBomb::CSpiderTankState_EndBomb(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_EndBomb::Enter(_float fTimeDelta)
{
	m_fDuration = 1.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::END_BOMB, false, 0.3f);
}

void CSpiderTankState_EndBomb::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_EndBomb::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CSpiderTankState_EndBomb::Free()
{
	__super::Free();
}
