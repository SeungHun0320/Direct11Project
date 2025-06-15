#include "PlayerState.h"
#include "Player.h"
#include "Player_IAttackStrategy.h"

/* --------------------------
           어택 1
------------------------- */

CPlayerState_Attack1::CPlayerState_Attack1(CPlayer* pOwner)
	: CPlayerState_Attack{ pOwner }
{
}

void CPlayerState_Attack1::Enter(_float fTimeDelta)
{
	auto pStrategy = m_pOwner->Get_AttackStrategy();
	CPlayer::ANIM_STATES eCombo = static_cast<CPlayer::ANIM_STATES>(pStrategy->Get_AnimationState(0));
	m_fDuration   = pStrategy->Get_Duration(0);
    m_iMaxCombo   = pStrategy->Get_MaxComboCount();
    m_eWeaponType = pStrategy->Get_WeaponType();

    m_pOwner->SetUp_AttackMeshVisible(m_eWeaponType);

    if (WEAPON_TYPE::DAGGER == m_eWeaponType)
        m_pOwner->Use_Mana(30.f);

    m_fTimeAcc = 0.f;
    m_fAttackStartTime = 0.3f;

    XMStoreFloat3(&m_vInputDir, m_pOwner->Get_State(STATE::LOOK));
	m_pOwner->Change_Animation(CPlayer::PART_BODY, eCombo, false, 0.1f);
}

void CPlayerState_Attack1::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    _bool isBlocked = m_pOwner->Get_IsBlocked();

    if (m_pOwner->KeyDown(DIK_J))
        m_isAttackCombo = true;

    if(m_fAttackStartTime <= m_fTimeAcc)
        m_pOwner->Set_Active(m_eWeaponType);

    if (WEAPON_TYPE::STICK == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {
            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_isAttackCombo)
            {
                m_isAttackCombo = false;
                m_pOwner->Change_States(CPlayer::STATES::ATTACK2);
            }
            else if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
            {
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
            }
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if(!isBlocked)
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 1.5f);
        }
    }
    else if (WEAPON_TYPE::SWORD == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {
            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_isAttackCombo)
            {
                m_isAttackCombo = false;
                m_pOwner->Change_States(CPlayer::STATES::ATTACK2);
            }
            else if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
            {
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
            }
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if (!isBlocked)
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 3.f);
        }
    }
    else if (WEAPON_TYPE::DAGGER == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {
            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
            {
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
            }
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if (!isBlocked)
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 3.f);
        }

    }


    if (m_pOwner->Get_Dead())
        m_pOwner->Change_States(CPlayer::STATES::DIE);
}

void CPlayerState_Attack1::Exit()
{
    m_isAttackCombo = false;
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
    m_iMaxCombo = 0;
    XMStoreFloat3(&m_vInputDir, XMVectorZero());
    m_pOwner->Set_Active(m_eWeaponType, false);
}

void CPlayerState_Attack1::Free()
{
	__super::Free();
}

/* --------------------------
           어택 2
------------------------- */

CPlayerState_Attack2::CPlayerState_Attack2(CPlayer* pOwner)
	: CPlayerState_Attack{ pOwner }
{
}

void CPlayerState_Attack2::Enter(_float fTimeDelta)
{
    auto pStrategy = m_pOwner->Get_AttackStrategy();
    CPlayer::ANIM_STATES eCombo = static_cast<CPlayer::ANIM_STATES>(pStrategy->Get_AnimationState(1));
    m_fDuration = pStrategy->Get_Duration(1);
    m_iMaxCombo = pStrategy->Get_MaxComboCount();
    m_eWeaponType = pStrategy->Get_WeaponType();

    m_fTimeAcc = 0.f;

    m_pOwner->SetUp_AttackMeshVisible(m_eWeaponType);
    m_pOwner->Set_Active(m_eWeaponType);

    XMStoreFloat3(&m_vInputDir, m_pOwner->Get_State(STATE::LOOK));
	m_pOwner->Change_Animation(CPlayer::PART_BODY, eCombo, false, 0.1f);

}

void CPlayerState_Attack2::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;
    
    _bool isBlocked = m_pOwner->Get_IsBlocked();

    if (m_pOwner->KeyDown(DIK_J)) // m_fTimeAcc >= m_fDuration && 
        m_isAttackCombo = true;

    if (WEAPON_TYPE::STICK == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {
            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_isAttackCombo)
            {
                m_isAttackCombo = false;
                m_pOwner->Change_States(CPlayer::STATES::ATTACK1);
            }
            else if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
            {
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
            }
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if (!isBlocked)
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 3.f);
        }

    }
    else if (WEAPON_TYPE::SWORD == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {
            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_isAttackCombo)
            {
                m_isAttackCombo = false;

                m_pOwner->Change_States(CPlayer::STATES::ATTACK3);
            }
            else if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if (!isBlocked)
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 2.f);
        }
    }
}

void CPlayerState_Attack2::Exit()
{
    m_isAttackCombo = false;
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
    m_iMaxCombo = 0;
    XMStoreFloat3(&m_vInputDir, XMVectorZero());
    m_pOwner->Set_Active(m_eWeaponType, false);
}

void CPlayerState_Attack2::Free()
{
	__super::Free();
}

/* --------------------------
           어택 3
------------------------- */

CPlayerState_Attack3::CPlayerState_Attack3(CPlayer* pOwner)
	: CPlayerState_Attack{ pOwner }
{
}

void CPlayerState_Attack3::Enter(_float fTimeDelta)
{
    auto pStrategy = m_pOwner->Get_AttackStrategy();
    CPlayer::ANIM_STATES eCombo = static_cast<CPlayer::ANIM_STATES>(pStrategy->Get_AnimationState(2));
    m_fDuration = pStrategy->Get_Duration(2);
    m_iMaxCombo = pStrategy->Get_MaxComboCount();
    m_eWeaponType = pStrategy->Get_WeaponType();

    m_fTimeAcc = 0.f;

    m_pOwner->Set_Active(m_eWeaponType);

    XMStoreFloat3(&m_vInputDir, m_pOwner->Get_State(STATE::LOOK));
	m_pOwner->Change_Animation(CPlayer::PART_BODY, eCombo, false, 0.1f);
}

void CPlayerState_Attack3::Execute(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta;

    if (m_pOwner->Get_Dead())
        m_pOwner->Change_States(CPlayer::STATES::DIE);


    if (m_fTimeAcc >= m_fDuration && m_pOwner->KeyDown(DIK_J))
        m_isAttackCombo = true;

    if (WEAPON_TYPE::SWORD == m_eWeaponType)
    {
        if (m_fDuration <= m_fTimeAcc || m_pOwner->Play_Animation(CPlayer::PART_BODY, fTimeDelta))
        {

            m_pOwner->Set_Active(m_eWeaponType, false);

            if (m_isAttackCombo)
            {
                m_isAttackCombo = false;
                m_pOwner->Change_States(CPlayer::STATES::ATTACK1);
            }
            else if (m_pOwner->IsAnyMoveKeyPressed())
            {
                m_pOwner->Change_States(CPlayer::STATES::MOVE);
            }
            else
            {
                m_pOwner->Change_States(CPlayer::STATES::IDLE);
            }
        }
        else if (0.3f <= m_fTimeAcc)
        {
            if(!m_pOwner->Get_IsBlocked())
                m_pOwner->Go_Dir(XMLoadFloat3(&m_vInputDir), fTimeDelta, 7.5f);
        }

    }
}

void CPlayerState_Attack3::Exit()
{
    m_isAttackCombo = false;
    m_fDuration = 0.f;
    m_fTimeAcc = 0.f;
    XMStoreFloat3(&m_vInputDir, XMVectorZero());
    m_pOwner->Set_Active(m_eWeaponType, false);
}

void CPlayerState_Attack3::Free()
{
	__super::Free();
}