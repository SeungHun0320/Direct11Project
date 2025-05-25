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
}

void CPlayerState_Move::Exit()
{
}

void CPlayerState_Move::Free()
{
	__super::Free();
}
