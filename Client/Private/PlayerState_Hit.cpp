#include "PlayerState.h"
#include "Player.h"

#define SPEED 9.f

CPlayerState_Hit::CPlayerState_Hit(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_Hit::Enter(_float fTimeDelta)
{
	m_isStagger = m_pOwner->Get_IsStagger();

	if (m_isStagger)
	{
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::STAGGER, false, 0.1f);
		m_fDuration = 0.7f;
	}
	else
	{
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::HIT, false, 0.1f);
		m_fDuration = 0.5f;
	}

	m_fTimeAcc  = 0.f;
	m_fDownTime = 0.f;

	XMStoreFloat3(&m_vStaggerDir, -m_pOwner->Get_State(STATE::LOOK));
}

void CPlayerState_Hit::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->Get_IsHit() && !m_isStagger)
	{
		m_fTimeAcc = 0.f;
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::HIT, false, 0.1f);
		m_pOwner->Reset_IsHit();
		m_fDuration = 0.5f;
	}

	if (!m_isStagger)
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		if (!m_isStagger)
		{
			if (m_pOwner->IsAnyMoveKeyPressed())
			{
				m_pOwner->Change_States(CPlayer::STATES::MOVE);
			}
			else
				m_pOwner->Change_States(CPlayer::STATES::IDLE);
		}
		if (m_isStagger)
		{
			m_fDownTime += fTimeDelta;

			if (1.f <= m_fDownTime)
				m_pOwner->Change_States(CPlayer::STATES::GET_UP);
		}

	}
	else
	{
		if (m_isStagger)
		{
			m_pOwner->Go_Dir(XMLoadFloat3(&m_vStaggerDir), fTimeDelta, SPEED);
		}
	}
}

void CPlayerState_Hit::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDownTime = 0.f;
	m_fDuration = 0.f;
	m_pOwner->Set_IsStagger();
	XMStoreFloat3(&m_vStaggerDir, XMVectorZero());
}

void CPlayerState_Hit::Free()
{
	__super::Free();
}
