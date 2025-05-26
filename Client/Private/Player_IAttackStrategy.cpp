#include "Player_IAttackStrategy.h"

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
