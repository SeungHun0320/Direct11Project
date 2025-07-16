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
	if (0.f < m_pOwner->Get_Stamina())
	{
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::DODGE, false, 0.1f);
		m_fDuration = 0.7f;
		m_pOwner->Set_Collider_Active(CPlayer::PART_BODY, false);
		switch (rand() % 3)
		{
		case 0:
			m_pOwner->Play_Sound("DodgeRoll0");
			break;
		case 1:
			m_pOwner->Play_Sound("DodgeRoll1");
			break;
		case 2:
			m_pOwner->Play_Sound("DodgeRoll2");
			break;
		case 3:
			m_pOwner->Play_Sound("DodgeRoll3");
			break;
		}

	}
	else
	{
		m_pOwner->Set_isNoStamina(true);
		m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::FAIL_DODGE, false, 0.1f);
		m_fDuration = 0.75f;
		m_pOwner->Play_Sound("Dodge_NoStamina");
	}

	m_fTimeAcc = 0.f;

	m_pOwner->Use_Stamina(25.f);
	m_pOwner->Set_isRoll(true);
	XMStoreFloat3(&m_vInputDir, m_pOwner->Get_InputDirectionEx());
}

void CPlayerState_Dodge::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta)) /* 재생 시간 */
	{
		m_pOwner->Set_Collider_Active(CPlayer::PART_BODY);

		m_pOwner->LookTarget(fTimeDelta);


		if (m_pOwner->CheckDodgeComboWeapon())
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
			if (!m_pOwner->IsLockOn())
				m_pOwner->Change_States(CPlayer::STATES::SPRINT);
			else
				m_pOwner->Change_States(CPlayer::STATES::IDLE);
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
	m_pOwner->Set_isRoll(false);
}

void CPlayerState_Dodge::Free()
{
	__super::Free();
}
