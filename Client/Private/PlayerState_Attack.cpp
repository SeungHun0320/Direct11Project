#include "PlayerState.h"
#include "Player.h"

/*-------------------------어택 1---------------------------------*/

CPlayerState_Attack1::CPlayerState_Attack1(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Attack1::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::STICK_ATTACK1, false, 0.2f);
}

void CPlayerState_Attack1::Execute(_float fTimeDelta)
{

}

void CPlayerState_Attack1::Exit()
{
}

void CPlayerState_Attack1::Free()
{
	__super::Free();
}

/*--------------------------어택 2--------------------------------*/

CPlayerState_Attack2::CPlayerState_Attack2(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Attack2::Enter(_float fTimeDelta)
{
	m_pOwner->Change_Animation(CPlayer::ANIM_STATES::STICK_ATTACK2, false, 0.2f);
}

void CPlayerState_Attack2::Execute(_float fTimeDelta)
{
}

void CPlayerState_Attack2::Exit()
{
}

void CPlayerState_Attack2::Free()
{
	__super::Free();
}

/*--------------------------어택 3--------------------------------*/

CPlayerState_Attack3::CPlayerState_Attack3(CPlayer* pOwner)
	: CPlayerState{ pOwner }
{
}

void CPlayerState_Attack3::Enter(_float fTimeDelta)
{
}

void CPlayerState_Attack3::Execute(_float fTimeDelta)
{
}

void CPlayerState_Attack3::Exit()
{
}

void CPlayerState_Attack3::Free()
{
	__super::Free();
}