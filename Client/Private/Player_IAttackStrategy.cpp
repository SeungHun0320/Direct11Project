#include "Player_IAttackStrategy.h"
#include "Player.h"

_int CPlayer_IAttackStrategy::Get_MaxComboCount() const
{
    return m_iMaxCombo;
}

WEAPON_TYPE CPlayer_IAttackStrategy::Get_WeaponType() const
{
    return m_eWeaponType;
}

void CPlayer_IAttackStrategy::Free()
{
    __super::Free();
}


/*----------------------------
              스틱
-------------------------------*/
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

_float CPlayer_StickAttack::Get_Attack() const
{
    return 10.f;
}

_float CPlayer_StickAttack::Get_StaggerValue() const
{
    return 10.f;
}

void CPlayer_StickAttack::Free()
{
    __super::Free();
}

/*----------------------------
              소드
-------------------------------*/
_int CPlayer_SwordAttack::Get_AnimationState(_int iComboIndex) const
{
    switch (iComboIndex)
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

_float CPlayer_SwordAttack::Get_Attack() const
{
    return 20.f;
}

_float CPlayer_SwordAttack::Get_StaggerValue() const
{
    return 40.f;
}

void CPlayer_SwordAttack::Free()
{
    __super::Free();
}
/*----------------------------
              대거
-------------------------------*/

_int CPlayer_DaggerAttack::Get_AnimationState(_int iComboIndex) const
{
    return CPlayer::ANIM_STATES::DAGGER;
}

_float CPlayer_DaggerAttack::Get_Duration(_int iComboIndex) const
{
    return 1.f;
}

_float CPlayer_DaggerAttack::Get_Attack() const
{
    return 5.f;
}

_float CPlayer_DaggerAttack::Get_StaggerValue() const
{
    return 50.f;
}

void CPlayer_DaggerAttack::Free()
{
    __super::Free();
}

