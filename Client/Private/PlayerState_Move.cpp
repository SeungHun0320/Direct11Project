#include "PlayerState.h"
#include "Player.h"

#define SPEED 5.f

CPlayerState_Move::CPlayerState_Move(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Move::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::GO_STRAIGHT, true, 0.3f);
}

void CPlayerState_Move::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta);

	if (m_pOwner->IsLockOn())
	{
		m_pOwner->LockOn();
		m_pOwner->LockOnMove(m_pOwner->Get_TargetState(STATE::POSITION), fTimeDelta, 5.f);
	}
	else
	{
		m_pOwner->LockOff();
		m_pOwner->CheckChange_Anim(CPlayer::PART_BODY, CPlayer::GO_STRAIGHT, true, 0.2f);
		m_pOwner->Move(m_pOwner->Get_InputDirectionEx(), fTimeDelta, SPEED);
	}


	if (m_pOwner->KeyDown(DIK_SPACE))
		m_pOwner->Change_States(CPlayer::STATES::DODGE);

	if (m_pOwner->KeyDown(DIK_J)) 
		m_pOwner->Change_States(CPlayer::STATES::ATTACK1);

	if (m_pOwner->KeyDown(DIK_K))
		m_pOwner->Change_States(CPlayer::STATES::WIND_UP);

	if (m_pOwner->KeyDown(DIK_L))
		m_pOwner->Change_States(CPlayer::STATES::EAT);
		
	if (m_pOwner->KeyDown(DIK_P))
		m_pOwner->Change_States(CPlayer::STATES::USE_POTION);

	if (m_pOwner->KeyDown(DIK_SEMICOLON))
		m_pOwner->Change_States(CPlayer::STATES::PARRY);

	if (!m_pOwner->IsAnyMoveKeyPressed())
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
}

void CPlayerState_Move::Exit()
{
}

void CPlayerState_Move::Free()
{
	__super::Free();
}
