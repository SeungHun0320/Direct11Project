#include "PlayerState.h"
#include "Player.h"

CPlayerState_Idle::CPlayerState_Idle(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Idle::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::IDLE, true, 0.2f, true);
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(fTimeDelta);

	if (m_pOwner->IsKeyDown(DIK_W) || m_pOwner->IsKeyDown(DIK_A) || m_pOwner->IsKeyDown(DIK_S) || m_pOwner->IsKeyDown(DIK_D))
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}

	if (m_pOwner->IsKeyDown(DIK_SPACE)) 
		m_pOwner->Change_States(CPlayer::STATES::DODGE);

	if (m_pOwner->IsKeyDown(DIK_J) || m_pOwner->IsKeyDown(DIK_K) || m_pOwner->IsKeyDown(DIK_L))
		m_pOwner->Change_States(CPlayer::STATES::ATTACK1);

}

void CPlayerState_Idle::Exit()
{
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
