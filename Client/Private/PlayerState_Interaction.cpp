#include "PlayerState.h"
#include "Player.h"

CPlayerState_OpenChest::CPlayerState_OpenChest(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_OpenChest::Enter(_float fTimeDelta)
{
	m_fDuration = 2.2f;
	m_fTimeAcc  = 0.f;

	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::OPEN_CHEST, false, 0.2f);
}

void CPlayerState_OpenChest::Execute(_float fTimeDelta)
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

void CPlayerState_OpenChest::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayerState_OpenChest::Free()
{
	__super::Free();
}

CPlayerState_OnSwitch::CPlayerState_OnSwitch(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_OnSwitch::Enter(_float fTimeDelta)
{
	m_fDuration = 2.7f;
	m_fTimeAcc = 0.f;
	
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::ON_SWITCH, false, 0.2f);
}

void CPlayerState_OnSwitch::Execute(_float fTimeDelta)
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

void CPlayerState_OnSwitch::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayerState_OnSwitch::Free()
{
	__super::Free();
}

CPlayerState_Kneel::CPlayerState_Kneel(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Kneel::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::KNEEL, false, 0.2f);
}

void CPlayerState_Kneel::Execute(_float fTimeDelta)
{
	if (m_pOwner->Play_Animation(fTimeDelta))
	{
		if (m_pOwner->KeyUp(DIK_SPACE))
		{
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
		}
		else if (m_pOwner->IsMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
	}
}

void CPlayerState_Kneel::Exit()
{
}

void CPlayerState_Kneel::Free()
{
	__super::Free();
}
