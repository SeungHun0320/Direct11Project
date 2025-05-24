#include "PlayerState.h"
#include "Player.h"

CPlayerState_Move::CPlayerState_Move(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Move::Enter(_float fTimeDelta)
{
}

void CPlayerState_Move::Execute(_float fTimeDelta)
{
}

void CPlayerState_Move::Exit()
{
}

void CPlayerState_Move::Free()
{
	__super::Free();
}
