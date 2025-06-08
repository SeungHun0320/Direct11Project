#include "PlayerState.h"
#include "Player.h"

CPlayerState_Idle::CPlayerState_Idle(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Idle::Enter(_float fTimeDelta)
{
	m_fHoldTime = 0.f;

	if (CPlayer::STATES::KNEEL == m_pOwner->Get_PreState())
	{
		m_pOwner->Set_TrackPosition(CPlayer::PART_BODY, 1.f);
		m_fDuration = 0.4f;
	}
	else
		m_fDuration = 0.2f;

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::IDLE, true, m_fDuration);
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta);


	if (m_pOwner->IsMoveKeyPressed())
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}

	/* 나중에 충돌여부 판단해서 상호작용 분기 ㄱㄱ */
	if (m_pOwner->KeyPressing(DIK_SPACE))
	{
		m_fHoldTime += fTimeDelta;

		if (1.f <= m_fHoldTime)
		{
			m_pOwner->Change_States(CPlayer::STATES::KNEEL);
			//m_pOwner->Change_States(CPlayer::STATES::OPEN_CHEST);
	
			m_fHoldTime = 0.f;
		}
	}
	else
	{
		if(0.f < m_fHoldTime && 1.f >= m_fHoldTime)
			m_pOwner->Change_States(CPlayer::STATES::DODGE);
	}

	/* 나중에 인벤에 어떤 칸에 어떤 아이템이 장착되어 있는지에 따라서 분기 ㄱ */
	if (m_pOwner->KeyDown(DIK_J))
		m_pOwner->Change_States(CPlayer::STATES::ATTACK1);

	if (m_pOwner->KeyDown(DIK_K))
		m_pOwner->Change_States(CPlayer::STATES::WIND_UP);

	if (m_pOwner->KeyDown(DIK_L))
		m_pOwner->Change_States(CPlayer::STATES::COIN_FLIP);

	if (m_pOwner->KeyDown(DIK_P))
		m_pOwner->Change_States(CPlayer::STATES::USE_POTION);

	if (m_pOwner->KeyDown(DIK_SEMICOLON))
		m_pOwner->Change_States(CPlayer::STATES::PARRY);

	if (m_pOwner->KeyDown(DIK_M))
		m_pOwner->Change_States(CPlayer::STATES::DANCE);

}

void CPlayerState_Idle::Exit()
{
	m_fDuration = 0.f;
	m_fHoldTime = 0.f;
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
