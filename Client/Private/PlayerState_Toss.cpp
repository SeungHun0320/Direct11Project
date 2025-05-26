#include "PlayerState.h"
#include "Player.h"

#define SPEED 2.5f

/*---------------------- 와인드업~ --------------------------*/
CPlayerState_WindUp::CPlayerState_WindUp(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_WindUp::Enter(_float fTimeDelta)
{
	m_fTimeAcc  = 0.f;
	m_fDuration = 0.f;

	for (_uint i = 0; i < CPlayer::MESHES::MESHES_END; i++)
	{
		if (i == CPlayer::MESHES::MESH_SHILED && m_pOwner->Get_IsShield())
			continue;

		m_pOwner->Set_MeshVisible(i, true);
	}

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::WINDUP, false, 0.1f);
}

void CPlayerState_WindUp::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.8f <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta))
	{
		m_pOwner->Change_States(CPlayer::STATES::TOSS);
	}
	else
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
			m_pOwner->Move(m_pOwner->Get_InputDirection(), fTimeDelta, SPEED);
	}
}

void CPlayerState_WindUp::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CPlayerState_WindUp::Free()
{
	__super::Free();
}


/*----------------------- 던지기 ------------------------*/
CPlayerState_Toss::CPlayerState_Toss(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Toss::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::TOSS, false, 0.3f);
}

void CPlayerState_Toss::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.8f <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta))
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
	}
}

void CPlayerState_Toss::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;

	
}

void CPlayerState_Toss::Free()
{
	__super::Free();
}
