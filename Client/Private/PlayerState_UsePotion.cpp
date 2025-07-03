#include "PlayerState.h"
#include "Player.h"

#define SPEED 2.5f

CPlayerState_UsePotion::CPlayerState_UsePotion(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_UsePotion::Enter(_float fTimeDelta)
{
	m_fDuration = 1.9f;
	m_fTimeAcc = 0.f;

	for (_uint i = 0; i < CPlayer::MESHES_END; i++)
	{
		if (i == CPlayer::MESH_SHILED && m_pOwner->Has_Shield())
			continue;

		m_pOwner->Set_MeshVisible(CPlayer::PART_BODY, i, true);
	}

	m_pOwner->Set_MeshVisible(CPlayer::PART_BODY, CPlayer::MESH_POTION, false);
	m_pOwner->Set_MeshVisible(CPlayer::PART_BODY, CPlayer::MESH_POTION2, false);

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::USE_POTION, false, 0.2f);
}

void CPlayerState_UsePotion::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (1.f <= m_fTimeAcc && !m_pOwner->Get_isUsePotion())
	{
		m_pOwner->Heal();
		m_pOwner->Set_isUsePotion(true);
		m_pOwner->Set_Effect_Active(CPlayer::PART_EFFECT_POTION);
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
		m_pOwner->Change_States(CPlayer::STATES::IDLE);
	}
	else
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
			m_pOwner->Move(m_pOwner->Get_InputDirectionEx(), fTimeDelta, SPEED);
	}

}

void CPlayerState_UsePotion::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
	m_pOwner->Set_isUsePotion(false);
}

void CPlayerState_UsePotion::Free()
{
	__super::Free();
}
