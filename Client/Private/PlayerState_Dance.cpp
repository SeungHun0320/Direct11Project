#include "PlayerState.h"
#include "Player.h"

CPlayerState_Dance::CPlayerState_Dance(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Dance::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::DANCE, true, 0.1f);
}

void CPlayerState_Dance::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta);

	if (m_pOwner->IsAnyMoveKeyPressed())
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}
}

void CPlayerState_Dance::Exit()
{
}

void CPlayerState_Dance::Free()
{
	__super::Free();
}
