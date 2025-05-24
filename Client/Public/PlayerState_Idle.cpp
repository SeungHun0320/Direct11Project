#include "PlayerState.h"
#include "Player.h"

CPlayerState_Idle::CPlayerState_Idle(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Idle::Enter(_float fTimeDelta)
{
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
}

void CPlayerState_Idle::Exit()
{
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
