#include "PlayerState.h"
#include "Player.h"

#define SPEED 2.5f

/* --------------------------
		  와인드업~
------------------------- */

CPlayerState_WindUp::CPlayerState_WindUp(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_WindUp::Enter(_float fTimeDelta)
{
	m_fTimeAcc  = 0.f;
	m_fDuration = 0.f;

	for (_uint i = 0; i < CPlayer::MESHES_END; i++)
	{
		if (i == CPlayer::MESH_SHILED && m_pOwner->Has_Shield())
			continue;

		m_pOwner->Set_MeshVisible(CPlayer::PART_BODY, i, true);
	}

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::WINDUP, false);


	switch (rand() % 5)
	{
	case 0:
		m_pOwner->Play_Sound("Firecracker_fizzle_00");
		break;
	case 1:
		m_pOwner->Play_Sound("Firecracker_fizzle_01");
		break;
	case 2:
		m_pOwner->Play_Sound("Firecracker_fizzle_02");
		break;
	case 3:
		m_pOwner->Play_Sound("Firecracker_fizzle_03");
		break;
	case 4:
		m_pOwner->Play_Sound("Firecracker_fizzle_04");
		break;
	}
}

void CPlayerState_WindUp::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.1f <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		m_pOwner->Play_Sound("Firecracker_fuse");
		m_pOwner->Change_States(CPlayer::STATES::TOSS);
	}
	else
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
			m_pOwner->Move(m_pOwner->Get_InputDirectionEx(), fTimeDelta, SPEED);
	}
}

void CPlayerState_WindUp::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
}

void CPlayerState_WindUp::Free()
{
	__super::Free();
}


/* --------------------------
		  던지기
------------------------- */

CPlayerState_Toss::CPlayerState_Toss(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Toss::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
	m_isToss = false;

	m_pOwner->Change_Animation(CPlayer::PART_BODY, CPlayer::ANIM_STATES::TOSS, false, 0.3f);

	switch (rand() % 3)
	{
	case 0:
		m_pOwner->Play_Sound("Firecracker_toss_00");
		break;
	case 1:
		m_pOwner->Play_Sound("Firecracker_toss_01");
		break;
	case 2:
		m_pOwner->Play_Sound("Firecracker_toss_02");
		break;
	}
}

void CPlayerState_Toss::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.4f <= m_fTimeAcc && !m_isToss)
	{
		m_pOwner->Use_FireCracker();
		m_isToss = true;
	}
		

	if (0.8f <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
	{
		if (m_pOwner->IsAnyMoveKeyPressed())
		{
			m_pOwner->Change_States(CPlayer::STATES::MOVE);
		}
		else
			m_pOwner->Change_States(CPlayer::STATES::IDLE);
	}
}

void CPlayerState_Toss::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 0.f;
	m_isToss = false;
	
}

void CPlayerState_Toss::Free()
{
	__super::Free();
}
