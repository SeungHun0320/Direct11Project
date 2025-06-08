#include "PlayerState.h"
#include "Player.h"

#define SPEED 9.f

CPlayerState_Sprint::CPlayerState_Sprint(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Sprint::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::SPRINT, true, 0.1f);
}

void CPlayerState_Sprint::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta);

	m_pOwner->Move(m_pOwner->Get_InputDirectionEx(), fTimeDelta, SPEED);

	if (m_pOwner->KeyDown(DIK_SPACE))
	{
		m_pOwner->Change_States(CPlayer::STATES::DODGE);
	}

	if (!m_pOwner->IsAnyMoveKeyPressed())
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
	
	if (m_pOwner->KeyUp(DIK_SPACE))
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}

	/* 나중에 인벤에 어떤 칸에 어떤 아이템이 장착되어 있는지에 따라서 분기 ㄱ */
	if (m_pOwner->KeyDown(DIK_J) || m_pOwner->KeyDown(DIK_K) || m_pOwner->KeyDown(DIK_L))
		m_pOwner->Change_States(CPlayer::STATES::ATTACK1);
}

void CPlayerState_Sprint::Exit()
{
}

void CPlayerState_Sprint::Free()
{
	__super::Free();
}
