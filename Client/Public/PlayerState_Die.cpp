#include "PlayerState.h"
#include "Player.h"

CPlayerState_Die::CPlayerState_Die(CPlayer* pOwner)
	: CPlayerState { pOwner }
{
}

void CPlayerState_Die::Enter(_float fTimeDelta)
{
}

void CPlayerState_Die::Execute(_float fTimeDelta)
{
}

void CPlayerState_Die::Exit()
{
}

void CPlayerState_Die::Free()
{
	__super::Free();
}
