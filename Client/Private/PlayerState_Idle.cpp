#include "PlayerState.h"
#include "Player.h"

CPlayerState_Idle::CPlayerState_Idle(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Idle::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::IDLE, true, 0.15f);
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(fTimeDelta);

	if (m_pOwner->Get_IsHit())
		m_pOwner->Change_States(CPlayer::STATES::HIT);

	if (m_pOwner->Get_Dead())
		m_pOwner->Change_States(CPlayer::STATES::DIE);

	if (m_pOwner->KeyDown(DIK_W) || m_pOwner->KeyDown(DIK_A) || m_pOwner->KeyDown(DIK_S) || m_pOwner->KeyDown(DIK_D))
	{
		m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}

	if (m_pOwner->KeyDown(DIK_SPACE))
		m_pOwner->Change_States(CPlayer::STATES::DODGE);

	/* 나중에 인벤에 어떤 칸에 어떤 아이템이 장착되어 있는지에 따라서 분기 ㄱ */
	if (m_pOwner->KeyDown(DIK_J))
		m_pOwner->Change_States(CPlayer::STATES::ATTACK1);

	if (m_pOwner->KeyDown(DIK_K))
		m_pOwner->Change_States(CPlayer::STATES::WIND_UP);

	if(m_pOwner->KeyDown(DIK_L))
		int a = 0;// 먹는 행동, 코인토스 실험 ㄱ

	if (m_pOwner->KeyDown(DIK_P))
		m_pOwner->Change_States(CPlayer::STATES::USE_POTION);

	if (m_pOwner->KeyDown(DIK_SEMICOLON))
		m_pOwner->Change_States(CPlayer::STATES::PARRY);

	if (m_pOwner->KeyDown(DIK_M))
		m_pOwner->Change_States(CPlayer::STATES::DANCE);

}

void CPlayerState_Idle::Exit()
{
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
