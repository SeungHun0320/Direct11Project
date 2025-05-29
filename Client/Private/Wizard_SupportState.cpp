#include "Wizard_SupportState.h"
#include "Wizard_Support.h"

CWizard_SupportState_Idle::CWizard_SupportState_Idle(CWizard_Support* pOwner)
	: CWizard_SupportState {pOwner}
{
}

void CWizard_SupportState_Idle::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CWizard_Support::PART_BODY, CWizard_Support::IDLE, true, 0.2f);
}

void CWizard_SupportState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->Play_Animation(CWizard_Support::PART_BODY, fTimeDelta);
}

void CWizard_SupportState_Idle::Exit()
{
}

void CWizard_SupportState_Idle::Free()
{
	__super::Free();
}

CWizard_SupportState_Detected::CWizard_SupportState_Detected(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Detected::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Detected::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Detected::Exit()
{
}

void CWizard_SupportState_Detected::Free()
{
	__super::Free();
}

CWizard_SupportState_Guard::CWizard_SupportState_Guard(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Guard::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Guard::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Guard::Exit()
{
}

void CWizard_SupportState_Guard::Free()
{
	__super::Free();
}

CWizard_SupportState_Attack::CWizard_SupportState_Attack(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Attack::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Attack::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Attack::Exit()
{
}

void CWizard_SupportState_Attack::Free()
{
	__super::Free();
}

CWizard_SupportState_Move::CWizard_SupportState_Move(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Move::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Move::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Move::Exit()
{
}

void CWizard_SupportState_Move::Free()
{
	__super::Free();
}

CWizard_SupportState_Hit::CWizard_SupportState_Hit(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Hit::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Hit::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Hit::Exit()
{
}

void CWizard_SupportState_Hit::Free()
{
	__super::Free();
}

CWizard_SupportState_Dead::CWizard_SupportState_Dead(CWizard_Support* pOwner)
	: CWizard_SupportState{ pOwner }
{
}

void CWizard_SupportState_Dead::Enter(_float fTimeDelta)
{
}

void CWizard_SupportState_Dead::Execute(_float fTimeDelta)
{
}

void CWizard_SupportState_Dead::Exit()
{
}

void CWizard_SupportState_Dead::Free()
{
	__super::Free();
}
