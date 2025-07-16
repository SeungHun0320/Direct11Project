#include "PlayerState.h"
#include "Player.h"

CPlayerState_WakeUp::CPlayerState_WakeUp(CPlayer* pOwner)
	:CPlayerState { pOwner }
{
}

void CPlayerState_WakeUp::Enter(_float fTimeDelta)
{
	m_fDuration = 0.f;
	m_fTimeAcc  = 0.f;

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::WAKE_UP, false);
	m_pOwner->Play_Sound("WakeUp");
}

void CPlayerState_WakeUp::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (3.3f <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
}

void CPlayerState_WakeUp::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayerState_WakeUp::Free()
{
	__super::Free();
}
