#include "Wizard_Sword.h"

#include "GameInstance.h"
#include "Body_WizardSword.h"
#include "Part_WizardSword.h"

#include "Wizard_SwordState.h"


#include "Player.h"

CWizard_Sword::CWizard_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWizard{ pDevice, pContext }
{
}

CWizard_Sword::CWizard_Sword(const CWizard_Sword& Prototype)
	: CWizard(Prototype)
{
}

HRESULT CWizard_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWizard_Sword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_fDetectDistance = 12.5f;
	m_fChaseStopDistance = 30.f;


	/* 공격력 */
	m_fAttack = 10.f;
	m_fStaggerValue = 5.f;

	/* 체력 */
	m_fHp = 100.f;
	m_fMaxHp = m_fHp;

	/* 그로기 */
	m_fStaggerGage = 20.f;
	m_fMaxStaggerGage = m_fStaggerGage;

	if (LEVEL::TOOLS != m_eLevelID)
	{
		if (1.f == m_pTransformCom->Get_SpeedPerSec())
			Change_States(STATES::PRAY);
		else
			Change_States(STATES::IDLE);
	}

	m_PartObjects[PART_SWORD]->Set_Active(false);

	return S_OK;
}

void CWizard_Sword::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard_Sword::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	if (KEY_DOWN(DIK_2))
	{
		Change_States(STATES::HIT);
	}
	if (KEY_DOWN(DIK_3))
		Change_States(STATES::DEAD);


	if (m_pCurState)
	{
		if (m_eCurState != m_ePreState)
		{
			m_pCurState->Enter(fTimeDelta);
			m_ePreState = m_eCurState;
		}

		m_pCurState->Execute(fTimeDelta);
	}

	return	__super::Update(fTimeDelta);
}

void CWizard_Sword::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Sword::Render()
{
	return S_OK;
}

void CWizard_Sword::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);

	m_eCurState = eStates;
}

_bool CWizard_Sword::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CWizard_Sword::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CWizard_Sword::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CWizard_Sword::Set_Active(_bool isActive)
{
	m_PartObjects[PART_SWORD]->Set_Active(isActive);
}

void CWizard_Sword::On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration)
{
	if (m_isInvincible || m_bDead)
		return;

	m_fHp -= fDamage;
	m_fStaggerGage -= fStaggerValue;
	m_isHit = true;

	if (0 >= m_fHp)
	{
		m_fHp = 0.f;
		Change_States(STATES::DEAD);
	}
	else
	{
		m_fInvicibleTime = fInvicibleDuration;
		m_isInvincible = true;
		Change_States(STATES::HIT);
	}
}

void CWizard_Sword::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	__super::On_Collision(MyColliderID, OtherColliderID, pOwner);

	if (CI_WEAPON(static_cast<COLLIDER_ID>(OtherColliderID)))
	{
		if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(pOwner))
		{
			On_Hit(pPlayer->Get_AttackValue(), pPlayer->Compute_StaggerValue());
		}
	}
}

HRESULT CWizard_Sword::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Sword::Ready_PartObjects()
{
	CBody_WizardSword::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Sword"), &BodyDesc)))
		return E_FAIL;

	CPart_WizardSword::DESC SwordDesc{};

	SwordDesc.eLevelID = m_eLevelID;
	SwordDesc.pSocketMatrix = dynamic_cast<CBody_WizardSword*>(m_PartObjects[PART_BODY])->Get_SocketMatrix("held_L");
	SwordDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	SwordDesc.vScale = m_pTransformCom->Get_Scaled();
	SwordDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_SWORD, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Part_WizardSword"), &SwordDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Sword::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)]		= new CWizard_SwordState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::PRAY)]		= new CWizard_SwordState_Pray(this);
	m_pStates[ENUM_CLASS(STATES::DETECTED)] = new CWizard_SwordState_Detected(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK)]	= new CWizard_SwordState_Attack(this);
	m_pStates[ENUM_CLASS(STATES::MOVE)]		= new CWizard_SwordState_Move(this);
	m_pStates[ENUM_CLASS(STATES::HIT)]		= new CWizard_SwordState_Hit(this);
	m_pStates[ENUM_CLASS(STATES::DEAD)]		= new CWizard_SwordState_Dead(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

CWizard_Sword* CWizard_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWizard_Sword* pInstance = new CWizard_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWizard_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWizard_Sword::Clone(void* pArg)
{
	CWizard_Sword* pInstance = new CWizard_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWizard_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWizard_Sword::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
