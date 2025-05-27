#include "PlayerState.h"
#include "Player.h"

CPlayerState_Die::CPlayerState_Die(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_Die::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::DIE, false, 0.1f);

	m_fTimeAcc = 0.f;
	m_fDuration = 1.6f;
}

void CPlayerState_Die::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
}

void CPlayerState_Die::Exit()
{
	m_pOwner->Set_Dead(false);
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CPlayerState_Die::Free()
{
	__super::Free();
}
