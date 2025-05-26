#include "PlayerState.h"
#include "Player.h"
#include "Player_IAttackStrategy.h"

#define SPEED 10.f

CPlayerState_Dodge::CPlayerState_Dodge(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Dodge::Enter(_float fTimeDelta)
{
	if (0 >= m_pOwner->Get_Stamina())
	{
		m_pOwner->Change_Animation(CPlayer::ANIM_STATES::FAIL_DODGE, false, 0.1f);
		m_fDuration = 0.65f;
	}
	else
	{
		m_pOwner->Change_Animation(CPlayer::ANIM_STATES::DODGE, false, 0.1f);
		m_fDuration = 0.7f;
	}

	m_fTimeAcc = 0.f;

	XMStoreFloat3(&m_vInputDir, m_pOwner->Get_InputDirection());

	m_pOwner->Use_Stamina(50);
}

void CPlayerState_Dodge::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(fTimeDelta)) /* 재생 시간 */
	{
		if (m_pOwner->KeyPressing(DIK_J) || m_pOwner->KeyDown(DIK_K) || m_pOwner->KeyDown(DIK_L))
		{
			WEAPON_TYPE eWeaponType = m_pOwner->Get_AttackStrategy()->Get_WeaponType();
			switch (eWeaponType)
			{
			case WEAPON_TYPE::SWORD:
				m_pOwner->Change_States(CPlayer::STATES::ATTACK2);
				break;
			default:
				m_pOwner->Change_States(CPlayer::STATES::ATTACK1);
				break;
			}

		}
		else if (m_pOwner->KeyPressing(DIK_SPACE))
		{
			m_pOwner->Change_States(CPlayer::STATES::SPRINT);
		}
		else if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CPlayer::STATES::IDLE);


	}
	else
	{
		m_pOwner->Dodge(XMLoadFloat3(&m_vInputDir), fTimeDelta, SPEED);
	}
		
}

void CPlayerState_Dodge::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
	XMStoreFloat3(&m_vInputDir, XMVectorZero());
}

void CPlayerState_Dodge::Free()
{
	__super::Free();
}
