#include "PlayerState.h"
#include "Player.h"

#define SPEED 6.f

/*---------------------사다리 탔음------------------------*/
CPlayerState_Ladder::CPlayerState_Ladder(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Ladder::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::LADDER, true, 0.1f);
}

void CPlayerState_Ladder::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(fTimeDelta);

	if (m_pOwner->KeyPressing(DIK_W))
	{
		m_pOwner->Go_Up(fTimeDelta, SPEED);
	}
	else if (m_pOwner->KeyPressing(DIK_S))
	{
		m_pOwner->Go_Down(fTimeDelta, SPEED);
	}

	/* 뭐 사다리에 끝에 닿았다면 */
	if (m_pOwner->KeyDown(DIK_UP))
		m_pOwner->Change_States(CPlayer::STATES::ON_LADDER);
	else if (m_pOwner->KeyDown(DIK_DOWN))
		m_pOwner->Change_States(CPlayer::STATES::OFF_LADDER);

}

void CPlayerState_Ladder::Exit()
{
}

void CPlayerState_Ladder::Free()
{
	__super::Free();
}

/* -------------위에서 아래로 사다리 탔음-----------------*/
CPlayerState_OnLadder::CPlayerState_OnLadder(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_OnLadder::Enter(_float fTimeDelta)
{
	m_fTimeAcc  = 0.f;
	m_fDuration = 0.7f;

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::ON_LADDER, false, 0.2f);
}

void CPlayerState_OnLadder::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta))
	{
		m_pOwner->Change_States(CPlayer::STATES::LADDER);
	}

}

void CPlayerState_OnLadder::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CPlayerState_OnLadder::Free()
{
	__super::Free();
}


/* -------------아래에서 위로 사다리 내렸음 -----------------*/
CPlayerState_OffLadder::CPlayerState_OffLadder(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_OffLadder::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.6f;

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::OFF_LADDER, false, 0.1f);
}

void CPlayerState_OffLadder::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta))
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
	}
	else if (0.4f < m_fTimeAcc)
	{
		m_pOwner->Go_Up(fTimeDelta, 5.f);
		m_pOwner->Move(m_pOwner->Get_State(STATE::LOOK), fTimeDelta, 7.5f);
	}
		
}

void CPlayerState_OffLadder::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CPlayerState_OffLadder::Free()
{
	__super::Free();
}
