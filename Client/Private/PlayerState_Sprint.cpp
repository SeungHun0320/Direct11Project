#include "PlayerState.h"
#include "Player.h"

#define SPEED 9.f

CPlayerState_Sprint::CPlayerState_Sprint(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Sprint::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::SPRINT, true, 0.3f);
}

void CPlayerState_Sprint::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(fTimeDelta);

	_vector vInputDir{};

	if (m_pOwner->IsKeyPressing(DIK_W))
		vInputDir += DIR_FORWARD;

	if (m_pOwner->IsKeyPressing(DIK_A))
		vInputDir += DIR_LEFT;

	if (m_pOwner->IsKeyPressing(DIK_S))
		vInputDir += DIR_BACKWARD;

	if (m_pOwner->IsKeyPressing(DIK_D))
		vInputDir += DIR_RIGHT;

	vInputDir = XMVector3Normalize(vInputDir);
	m_pOwner->Move(vInputDir, fTimeDelta, SPEED);

	if (m_pOwner->IsKeyDown(DIK_SPACE))
	{
		m_pOwner->Change_States(CPlayer::STATES::DODGE);
	}

	if (!m_pOwner->IsAnyMoveKeyPressed())
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
	
	if (m_pOwner->IsKeyUp(DIK_SPACE))
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}
}

void CPlayerState_Sprint::Exit()
{
}

void CPlayerState_Sprint::Free()
{
	__super::Free();
}
