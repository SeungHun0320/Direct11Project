#include "PlayerState.h"
#include "Player.h"

CPlayerState_Attack::CPlayerState_Attack(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Attack::Enter(_float fTimeDelta)
{
}

void CPlayerState_Attack::Execute(_float fTimeDelta)
{
}

void CPlayerState_Attack::Exit()
{
}

void CPlayerState_Attack::Free()
{
	__super::Free();
}
