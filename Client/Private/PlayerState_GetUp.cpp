#include "PlayerState.h"
#include "Player.h"

CPlayerState_GetUp::CPlayerState_GetUp(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_GetUp::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::GET_UP, false, 0.2f);

	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CPlayerState_GetUp::Execute(_float fTimeDelta)
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
		
}

void CPlayerState_GetUp::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CPlayerState_GetUp::Free()
{
	__super::Free();
}
