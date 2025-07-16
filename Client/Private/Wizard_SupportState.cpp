#include "Wizard_SupportState.h"
#include "Wizard_Support.h"

#include "GameInstance.h"

/* --------------------------
		   아이들
------------------------- */
CWizard_SupportState_Idle::CWizard_SupportState_Idle(CWizard_Support* pOwner)
	: CWizard_SupportState {pOwner}
{
}

void CWizard_SupportState_Idle::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fCheckTime = 1.f;

	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::IDLE, true, 0.2f);
}

void CWizard_SupportState_Idle::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta);

	if (m_fTimeAcc >= m_fCheckTime)
	{
		if (m_pOwner->Find_Player())
		{
			_string RandomNum = to_string(rand() % 5);
			m_pOwner->Play_Sound("Aggro_" + RandomNum);

			// 거리가 가깝다면
			if(6.f >= m_pOwner->Get_DistanceToPlayer())
				m_pOwner->Change_States(CWizard_Support::STATES::TELEPORT);
			else
				m_pOwner->Change_States(CWizard_Support::STATES::CASTING);
		}

		m_fTimeAcc = 0.f;
	}
}

void CWizard_SupportState_Idle::Exit()
{
	m_fTimeAcc = 0.f;
	m_fCheckTime = 0.f;
}

void CWizard_SupportState_Idle::Free()
{
	__super::Free();
}

/* --------------------------
		    무브
------------------------- */
CWizard_SupportState_Move::CWizard_SupportState_Move(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Move::Enter(_float fTimeDelta)
{
	XMStoreFloat3(&m_vTargetDir, m_pOwner->Get_TargetPosition());
	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::FORWARD, true, 0.2f);
}

void CWizard_SupportState_Move::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta);

	m_pOwner->Go_Target(XMVectorSetW(XMLoadFloat3(&m_vTargetDir), 1.f), fTimeDelta, 6.f);

	if (m_pOwner->Get_CastingDistance() >= m_pOwner->Get_DistanceToPlayer())
		m_pOwner->Change_States(CWizard_Support::STATES::CASTING);
}

void CWizard_SupportState_Move::Exit()
{
	XMStoreFloat3(&m_vTargetDir, XMVectorZero());
}

void CWizard_SupportState_Move::Free()
{
	__super::Free();
}

/* --------------------------
		   피격
------------------------- */
CWizard_SupportState_Hit::CWizard_SupportState_Hit(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Hit::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;

	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::PINCH, false, 0.2f);
}

void CWizard_SupportState_Hit::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->Get_IsHit())
	{
		m_fTimeAcc = 0.f;
		m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::PINCH, false, 0.2f);
		m_pOwner->Reset_IsHit();
	}

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta))
		m_pOwner->Change_States(CWizard_Support::STATES::IDLE);
}

void CWizard_SupportState_Hit::Exit()
{
	m_fTimeAcc = 0.f;
	m_fDuration = 1.f;
}

void CWizard_SupportState_Hit::Free()
{
	__super::Free();
}

/* --------------------------
		   사망
------------------------- */
CWizard_SupportState_Dead::CWizard_SupportState_Dead(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Dead::Enter(_float fTimeDelta)
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::DEAD, false, 0.2f);
	_string RandomNum = to_string(rand() % 3);
	m_pOwner->Play_Sound("Death_" + RandomNum);
}

void CWizard_SupportState_Dead::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta))
		m_pOwner->Set_Dead(true);
}

void CWizard_SupportState_Dead::Exit()
{
	m_fDuration = 5.f;
	m_fTimeAcc = 0.f;
}

void CWizard_SupportState_Dead::Free()
{
	__super::Free();
}

/* --------------------------
		   캐스팅
------------------------- */
CWizard_SupportState_Casting::CWizard_SupportState_Casting(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Casting::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fCastingTimeAcc = 0.f;

	m_fDuration = 1.f;
	m_fCastingTime = 5.f;
	m_isCasting = false;
	XMStoreFloat3(&m_vTargetDir, m_pOwner->Get_TargetPosition());
	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::CASTING_START, false, 0.3f);

	_string strRandomNum = to_string(rand() % 5);
	m_pOwner->Play_Sound("Attack_Vo_" + strRandomNum);
}

void CWizard_SupportState_Casting::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fCastingTimeAcc += fTimeDelta;

	m_pOwner->LookAt(XMVectorSetW(XMLoadFloat3(&m_vTargetDir), 1.f), fTimeDelta, 5.f);

	m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta);

	if (m_fDuration <= m_fTimeAcc && !m_isCasting)
	{
		_string strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("Attack_M_" + strRandomNum);
		strRandomNum = to_string(rand() % 6);
		m_pOwner->Play_Sound("Attack_St_" + strRandomNum);
		m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::CASTING, true, 0.1f);
		m_pOwner->Casting();
		m_isCasting = true;
	}

	if (m_isCasting && m_fCastingTime <= m_fCastingTimeAcc)
	{
		_string strRandomNum = to_string(rand() % 3);
		m_pOwner->Play_Sound("Attack_Impact_" + strRandomNum);

		// 캐스팅 거리보다 멀다면.
		if (m_pOwner->Get_CastingDistance() <= m_pOwner->Get_DistanceToPlayer())
			m_pOwner->Change_States(CWizard_Support::STATES::MOVE);
		//아니라면 아이들
		else
			m_pOwner->Change_States(CWizard_Support::STATES::IDLE);
	}
}

void CWizard_SupportState_Casting::Exit()
{
	m_fTimeAcc  = 0.f;
	m_fDuration = 0.f;
	m_fCastingTime = 0.f;
	m_fCastingTimeAcc = 0.f;
	XMStoreFloat3(&m_vTargetDir, m_pOwner->Get_TargetPosition());
	m_isCasting = false;
}

void CWizard_SupportState_Casting::Free()
{
	__super::Free();
}

/* --------------------------
		   텔레포트
------------------------- */
CWizard_SupportState_Teleport::CWizard_SupportState_Teleport(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Teleport::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::TELEPORT, false, 0.2f);
	_string strRandomNum = to_string(rand() % 3);
	m_pOwner->Play_Sound("Teleport_In_" + strRandomNum);
}

void CWizard_SupportState_Teleport::Execute(_float fTimeDelta)
{
	/* 랜덤한 거리로 이동*/
	if (m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta))
	{
		_float fRandomX = CGameInstance::Get_Instance()->Compute_Random(-3.f, 3.f);
		_float fRandomZ = CGameInstance::Get_Instance()->Compute_Random(-3.f, 3.f);

		/* 개 똥 코드 감사합니다.*/
		/* 나중에 디스크립션이던가 해갖고 랜덤위치 넣어줘야할듯 */
		m_pOwner->Set_State(STATE::POSITION, m_pOwner->Get_State(STATE::POSITION)
			+ XMVectorSet(fRandomX, 0.f, fRandomZ, 0.f));
		m_pOwner->Change_States(CWizard_Support::STATES::IDLE);
	}
}

void CWizard_SupportState_Teleport::Exit()
{
	_string strRandomNum = to_string(rand() % 3);
	m_pOwner->Play_Sound("Teleport_Out_" + strRandomNum);
}

void CWizard_SupportState_Teleport::Free()
{
	__super::Free();
}
