#include "PlayerState.h"
#include "Player.h"

CPlayerState_Dodge::CPlayerState_Dodge(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Dodge::Enter(_float fTimeDelta)
{
	if (0 >= m_pOwner->Get_Stamina())
		m_pOwner->Change_Animation(CPlayer::ANIM_STATES::FAIL_DODGE, false, 0.1f);
	else
		m_pOwner->Change_Animation(CPlayer::ANIM_STATES::DODGE, false, 0.1f);
	m_fTimeAcc = 0.f;
	m_IsDodgeQueue = false;

	XMStoreFloat3(&m_vInputDir, m_pOwner->Get_InputDirection());
	if (XMVector3Equal(XMLoadFloat3(&m_vInputDir), XMVectorZero()))
		XMStoreFloat3(&m_vInputDir, m_pOwner->Get_State(STATE::LOOK)); 

	m_pOwner->Use_Stamina(50);
}

void CPlayerState_Dodge::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.8 <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta)) /* 재생 시간 */
	{
		if (m_pOwner->IsKeyPressing(DIK_SPACE))
		{
			m_pOwner->Change_States(CPlayer::STATES::SPRINT);
		}
		else if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
		{
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
		}

	}
	else
	{
		m_pOwner->Dodge(XMLoadFloat3(&m_vInputDir), fTimeDelta);
	}
		
}

void CPlayerState_Dodge::Exit()
{
	m_fTimeAcc = 0.f;
	m_IsDodgeQueue = false;
	XMStoreFloat3(&m_vInputDir, XMVectorZero());
}

void CPlayerState_Dodge::Free()
{
	__super::Free();
}
