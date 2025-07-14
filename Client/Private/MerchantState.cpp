#include "MerchantState.h"
#include "Merchant.h"

/* --------------------------
		    숨기
------------------------- */

CMerchantState_Hidden::CMerchantState_Hidden(CMerchant* pOwner)
	: CMerchantState { pOwner }
{
}

void CMerchantState_Hidden::Enter(_float fTimeDelta)
{
	m_fTimeAcc = 0.f;
	m_fCheckTime = 0.5f;

	m_pOwner->Change_Animation(CMerchant::PART_BODY, CMerchant::HIDDEN, true, 0.2f);
}

void CMerchantState_Hidden::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Play_Animation(CMerchant::PART_BODY, fTimeDelta);

	if (m_fTimeAcc >= m_fCheckTime)
	{
		if (m_pOwner->Find_Player())
		{
			m_pOwner->Set_IsFind(true);
			m_pOwner->Change_States(CMerchant::STATES::WAKE_UP);
		}

		m_fTimeAcc = 0.f;
	}
}

void CMerchantState_Hidden::Exit()
{
	m_fCheckTime = 0.f;
	m_fTimeAcc = 0.f;
}

void CMerchantState_Hidden::Free()
{
	__super::Free();
}

/* --------------------------
			기상
------------------------- */

CMerchantState_WakeUp::CMerchantState_WakeUp(CMerchant* pOwner)
	: CMerchantState{ pOwner }
{
}

void CMerchantState_WakeUp::Enter(_float fTimeDelta)
{
	m_fDuration = 4.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CMerchant::PART_BODY, CMerchant::WAKE_UP, false, 0.3f);
}

void CMerchantState_WakeUp::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pOwner->Go_Up(fTimeDelta, 7.5f);

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CMerchant::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CMerchant::STATES::IDLE);
	}
}

void CMerchantState_WakeUp::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CMerchantState_WakeUp::Free()
{
	__super::Free();
}

/* --------------------------
		   아이들
------------------------- */

CMerchantState_Idle::CMerchantState_Idle(CMerchant* pOwner)
	: CMerchantState{ pOwner }
{
}

void CMerchantState_Idle::Enter(_float fTimeDelta)
{
	m_fDuration = 0.8f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CMerchant::PART_BODY, CMerchant::IDLE, true, 0.5f);
}

void CMerchantState_Idle::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_pOwner->is_TargetOnRight())
		m_pOwner->Go_Right(fTimeDelta, 2.f);
	else
		m_pOwner->Go_Left(fTimeDelta, 2.f);


	m_pOwner->Play_Animation(CMerchant::PART_BODY, fTimeDelta);

	// 아이템을 샀다고 판단되면, 감사합니다~
}

void CMerchantState_Idle::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CMerchantState_Idle::Free()
{
	__super::Free();
}

/* --------------------------
		   땡큐~~
------------------------- */

CMerchantState_ThankYou::CMerchantState_ThankYou(CMerchant* pOwner)
	: CMerchantState{ pOwner }
{
}

void CMerchantState_ThankYou::Enter(_float fTimeDelta)
{
	m_fDuration = 3.f;
	m_fTimeAcc = 0.f;

	m_pOwner->Change_Animation(CMerchant::PART_BODY, CMerchant::THANK_YOU, true, 0.5f);
}

void CMerchantState_ThankYou::Execute(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CMerchant::PART_BODY, fTimeDelta))
	{
		m_pOwner->Change_States(CMerchant::STATES::IDLE);
	}

}

void CMerchantState_ThankYou::Exit()
{
	m_fDuration = 0.f;
	m_fTimeAcc = 0.f;
}

void CMerchantState_ThankYou::Free()
{
	__super::Free();
}

