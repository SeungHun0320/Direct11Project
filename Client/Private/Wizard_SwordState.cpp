#include "Wizard_CandleabraState.h"
#include "Wizard_Sword.h"
#include "Wizard_SwordState.h"

CWizard_SwordState_Idle::CWizard_SwordState_Idle(CWizard_Sword* pOwner)
	: CWizard_SwordState { pOwner }
{
}

void CWizard_SwordState_Idle::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Idle::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Idle::Exit()
{
}

void CWizard_SwordState_Idle::Free()
{
	__super::Free();
}

CWizard_SwordState_Detected::CWizard_SwordState_Detected(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Detected::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Detected::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Detected::Exit()
{
}

void CWizard_SwordState_Detected::Free()
{
	__super::Free();
}

CWizard_SwordState_Guard::CWizard_SwordState_Guard(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Guard::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Guard::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Guard::Exit()
{
}

void CWizard_SwordState_Guard::Free()
{
	__super::Free();
}

CWizard_SwordState_Attack::CWizard_SwordState_Attack(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Attack::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Attack::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Attack::Exit()
{
}

void CWizard_SwordState_Attack::Free()
{
	__super::Free();
}

CWizard_SwordState_Move::CWizard_SwordState_Move(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Move::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Move::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Move::Exit()
{
}

void CWizard_SwordState_Move::Free()
{
	__super::Free();
}

CWizard_SwordState_Hit::CWizard_SwordState_Hit(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Hit::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Hit::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Hit::Exit()
{
}

void CWizard_SwordState_Hit::Free()
{
	__super::Free();
}

CWizard_SwordState_Dead::CWizard_SwordState_Dead(CWizard_Sword* pOwner)
	: CWizard_SwordState{ pOwner }
{
}

void CWizard_SwordState_Dead::Enter(_float fTimeDelta)
{
}

void CWizard_SwordState_Dead::Execute(_float fTimeDelta)
{
}

void CWizard_SwordState_Dead::Exit()
{
}

void CWizard_SwordState_Dead::Free()
{
	__super::Free();
}
