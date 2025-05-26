#include "Player_IAttackStrategy.h"
#include "Player.h"

_int CPlayer_DaggerAttack::Get_AnimationState(_int iComboIndex) const
{
    return CPlayer::ANIM_STATES::DAGGER;
}

_float CPlayer_DaggerAttack::Get_Duration(_int iComboIndex) const
{
    return 1.f;
}

void CPlayer_DaggerAttack::Free()
{
    __super::Free();
}
