#include "PlayerState.h"
#include "Player.h"
CPlayerState_Hit::CPlayerState_Hit(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_Hit::Enter(_float fTimeDelta)
{
}

void CPlayerState_Hit::Execute(_float fTimeDelta)
{
}

void CPlayerState_Hit::Exit()
{
}

void CPlayerState_Hit::Free()
{
	__super::Free();
}
