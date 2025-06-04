#include "PlayerState.h"
#include "Player.h"

#define SPEED 9.f

CPlayerState_Hit::CPlayerState_Hit(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_Hit::Enter(_float fTimeDelta)
{
	m_eHitType = m_pOwner->Get_HitType();

	switch (m_eHitType)
	{
	case HIT_TYPE::NORMAL:
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::HIT, false, 0.1f);
		m_fDuration = 0.5f;
		break;

	case HIT_TYPE::STAGGER:
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::STAGGER, false, 0.1f);
		m_fDuration = 0.7f;
		break;
	}

	m_fTimeAcc  = 0.f;
	m_fDownTime = 0.f;

	XMStoreFloat3(&m_vStaggerDir, -m_pOwner->Get_State(STATE::LOOK));
}

void CPlayerState_Hit::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		if (HIT_TYPE::NORMAL == m_eHitType)
		{
			if (m_pOwner->IsAnyMoveKeyPressed())
			{
				m_pOwner->Change_States(CPlayer::STATES::MOVE);
			}
			else
				m_pOwner->Change_States(CPlayer::STATES::IDLE);
		}
		else if (HIT_TYPE::STAGGER == m_eHitType)
		{
			m_fDownTime += fTimeDelta;

			if (1.f <= m_fDownTime)
				m_pOwner->Change_States(CPlayer::STATES::GET_UP);
		}

	}
	else
	{
		if (HIT_TYPE::STAGGER == m_eHitType)
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
	XMStoreFloat3(&m_vStaggerDir, XMVectorZero());

	m_pOwner->Set_Hit(false);
}

void CPlayerState_Hit::Free()
{
	__super::Free();
}
