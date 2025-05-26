#include "Player_IAttackStrategy.h"
#include "Player.h"

_int CPlayer_SwordAttack::Get_AnimationState(_int iComboIndex) const
{
    switch(iComboIndex)
    {
    case 0:
        return CPlayer::ANIM_STATES::SWORD_ATTACK1;
    case 1:
        return CPlayer::ANIM_STATES::SWORD_ATTACK2;
    case 2:
        return CPlayer::ANIM_STATES::SWORD_ATTACK3;
    default:
        return CPlayer::ANIM_STATES::IDLE;
    }

}

_float CPlayer_SwordAttack::Get_Duration(_int iComboIndex) const
{
    switch (iComboIndex)
    {
    case 0:
        return 0.6f;
    case 1:
        return 0.7f;
    case 2:
        return 0.9f;
    default:
        return 0.f;
    }
}

void CPlayer_SwordAttack::Free()
{
    __super::Free();
}


