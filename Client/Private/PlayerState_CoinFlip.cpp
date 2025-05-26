#include "PlayerState.h"
#include "Player.h"

CPlayerState_CoinFlip::CPlayerState_CoinFlip(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_CoinFlip::Enter(_float fTimeDelta)
{
	m_fDuration = 1.9f;
	m_fTimeAcc  = 0.f;

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::COIN_FLIP, false, 0.1f);
}

void CPlayerState_CoinFlip::Execute(_float fTimeDelta)
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

void CPlayerState_CoinFlip::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayerState_CoinFlip::Free()
{
	__super::Free();
}
