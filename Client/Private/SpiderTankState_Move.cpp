#include "SpiderTankState.h"
#include "SpiderTank.h"

/* --------------------------
		    전진
------------------------- */
CSpiderTankState_Forward::CSpiderTankState_Forward(CSpiderTank* pOwner)
	: CSpiderTankState { pOwner }
{
}

void CSpiderTankState_Forward::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::FORWARD, true, 0.2f);
}

void CSpiderTankState_Forward::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	if (m_pOwner->Go_Target(m_pOwner->Get_TargetPosition(), fTimeDelta, 5.f, m_pOwner->Get_PreferredDistance()))
	{
	}
	else
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);

}

void CSpiderTankState_Forward::Exit()
{
}

void CSpiderTankState_Forward::Free()
{
	__super::Free();
}

/* --------------------------
			후진
------------------------- */
CSpiderTankState_Backward::CSpiderTankState_Backward(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Backward::Enter(_float fTimeDelta)
{
	_vector vPos = m_pOwner->Get_State(STATE::POSITION);
	_vector vTarget = m_pOwner->Get_TargetPosition();

	_vector vDir = XMVector3Normalize(vPos - vTarget);
	XMStoreFloat3(&m_vMoveDir, vDir);

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::BACKWARD, true, 0.3f);
}

void CSpiderTankState_Backward::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	if (m_pOwner->Get_DistanceToPlayer() > m_pOwner->Get_PreferredDistance() ||
		m_pOwner->Compute_AngleToPlayer() > XMConvertToRadians(100.f))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
	else
		m_pOwner->Go_Dir(XMVectorSetW(XMLoadFloat3(&m_vMoveDir), 0.f), fTimeDelta, 4.f);

}

void CSpiderTankState_Backward::Exit()
{
	XMStoreFloat3(&m_vMoveDir, XMVectorZero());
}

void CSpiderTankState_Backward::Free()
{
	__super::Free();
}


/* --------------------------
		 턴(오른쪽)
------------------------- */
CSpiderTankState_TurnRight::CSpiderTankState_TurnRight(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_TurnRight::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::RIGHT, true, 0.3f);
	m_pOwner->Set_TickPerSecond(CSpiderTank::PART_BODY, 90.f);
}

void CSpiderTankState_TurnRight::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	_vector vLook = m_pOwner->Get_State(STATE::LOOK);
	_vector vDir = XMVector3Normalize(m_pOwner->Get_TargetPosition() - m_pOwner->Get_State(STATE::POSITION));

	_float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vLook, vDir));

	m_pOwner->LookAt(XMVectorSetW(m_pOwner->Get_TargetPosition(), 1.f), fTimeDelta, 1.f);

	if (fAngle < XMConvertToRadians(10.f))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_TurnRight::Exit()
{
}

void CSpiderTankState_TurnRight::Free()
{
	__super::Free();
}

/* --------------------------
		 턴(왼쪽)
------------------------- */
CSpiderTankState_TurnLeft::CSpiderTankState_TurnLeft(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_TurnLeft::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::LEFT, true, 0.3f);
	m_pOwner->Set_TickPerSecond(CSpiderTank::PART_BODY, 90.f);
}

void CSpiderTankState_TurnLeft::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta);

	_vector vLook = m_pOwner->Get_State(STATE::LOOK);
	_vector vDir = XMVector3Normalize(m_pOwner->Get_TargetPosition() - m_pOwner->Get_State(STATE::POSITION));

	_float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vLook, vDir));

	m_pOwner->LookAt(XMVectorSetW(m_pOwner->Get_TargetPosition(), 1.f), fTimeDelta, 1.f);

	if (fAngle < XMConvertToRadians(10.f))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
}

void CSpiderTankState_TurnLeft::Exit()
{
}

void CSpiderTankState_TurnLeft::Free()
{
	__super::Free();
}



/* --------------------------
			급후진
------------------------- */
CSpiderTankState_Reverse::CSpiderTankState_Reverse(CSpiderTank* pOwner)
	: CSpiderTankState{ pOwner }
{
}

void CSpiderTankState_Reverse::Enter(_float fTimeDelta)
{
	m_fDuration = 1.2f;
	m_fTimeAcc = 0.f;

	_vector vPos = m_pOwner->Get_State(STATE::POSITION);
	_vector vTarget = m_pOwner->Get_TargetPosition();

	_vector vDir = XMVector3Normalize(vPos - vTarget);
	XMStoreFloat3(&m_vMoveDir, vDir);

	m_pOwner->Change_Animation(CSpiderTank::PART_BODY, CSpiderTank::REVERSE, false, 0.1f);
}

void CSpiderTankState_Reverse::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CSpiderTank::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CSpiderTank::STATES::IDLE);
	}
	else
		m_pOwner->Go_Dir(XMVectorSetW(XMLoadFloat3(&m_vMoveDir), 0.f), fTimeDelta, 10.f);
}

void CSpiderTankState_Reverse::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;

	XMStoreFloat3(&m_vMoveDir, XMVectorZero());
}

void CSpiderTankState_Reverse::Free()
{
	__super::Free();
}
