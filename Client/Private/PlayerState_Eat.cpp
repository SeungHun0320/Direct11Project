#include "PlayerState.h"
#include "Player.h"

#define SPEED 2.5f

CPlayerState_Eat::CPlayerState_Eat(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Eat::Enter(_float fTimeDelta)
{
	m_fDuration = 0.8f;
	m_fTimeAcc  = 0.f;

	for (_uint i = 0; i < CPlayer::MESHES_END; i++)
	{
		if (i == CPlayer::MESH_SHILED && m_pOwner->Has_Shield())
			continue;

		m_pOwner->Set_MeshVisible(CPlayer::PART_BODY, i, true);
	}

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::EAT, false, 0.2f);
}

void CPlayerState_Eat::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

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

void CPlayerState_Eat::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CPlayerState_Eat::Free()
{
	__super::Free();
}
