#include "PlayerState.h"
#include "Player.h"

#define SPEED 5.f

CPlayerState_Move::CPlayerState_Move(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Move::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::GO_STRAIGHT, true, 0.1f);
}

void CPlayerState_Move::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(fTimeDelta);

	if (m_pOwner->Get_Dead())
		m_pOwner->Change_States(CPlayer::STATES::DIE);

	if (m_pOwner->Get_IsHit())
		m_pOwner->Change_States(CPlayer::STATES::HIT);

	m_pOwner->Move(m_pOwner->Get_InputDirection(), fTimeDelta, SPEED);

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
