#include "Player_IAttackStrategy.h"
#include "Player.h"

_int CPlayer_StickAttack::Get_AnimationState(_int iComboIndex) const
{
    switch (iComboIndex)
    {
    case 0:
        return CPlayer::ANIM_STATES::STICK_ATTACK1;
    case 1:
        return CPlayer::ANIM_STATES::STICK_ATTACK2;
    default:
        return CPlayer::ANIM_STATES::IDLE;
    }
}

_float CPlayer_StickAttack::Get_Duration(_int iComboIndex) const
{
    switch (iComboIndex)
    {
    case 0:
        return 0.5f;
    case 1:
        return 0.5f;
    default:
        return 0.f;
    }
}

void CPlayer_StickAttack::Free()
{
    __super::Free();
}
