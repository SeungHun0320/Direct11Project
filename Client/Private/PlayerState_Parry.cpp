#include "PlayerState.h"
#include "Player.h"

CPlayerState_Parry::CPlayerState_Parry(CPlayer* pOwner)
	:CPlayerState { pOwner }
{
}

void CPlayerState_Parry::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::PARRY, false, 0.2f);
	m_fTimeAcc = 0.f;
	m_fDuration = 1.1f;

	m_pOwner->Set_Collider_Active(WEAPON_TYPE::SHILED);
}

void CPlayerState_Parry::Execute(_float fTimeDelta)
{
	if (!m_pOwner->Has_Shield())
		m_pOwner->Change_States(CPlayer::STATES::MOVE);

	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		m_pOwner->Set_Collider_Active(WEAPON_TYPE::SHILED, false);

		if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
	}

}

void CPlayerState_Parry::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CPlayerState_Parry::Free()
{
	__super::Free();
}
