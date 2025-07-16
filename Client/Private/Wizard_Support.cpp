#include "Wizard_Support.h"

#include "GameInstance.h"
#include "Body_WizardSupport.h"
#include "Part_WizardStaff.h"

#include "UI3D_MobHPBar.h"
#include "UI3D_LockOn.h"

#include "Wizard_SupportState.h"

#include "Wizard_Support_AOE.h"


#include "Player.h"

CWizard_Support::CWizard_Support(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWizard{ pDevice, pContext }
{
}

CWizard_Support::CWizard_Support(const CWizard_Support& Prototype)
	: CWizard(Prototype)
{
}

HRESULT CWizard_Support::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWizard_Support::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_fDetectDistance = 12.5f;
	m_fChaseStopDistance = 30.f;
	m_fCastingDistance = 10.f;


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
		Change_States(STATES::IDLE);

	return S_OK;
}

void CWizard_Support::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard_Support::Update(_float fTimeDelta)
{
	if (m_pCurState)
	{
		if (m_eCurState != m_ePreState)
		{
			m_pCurState->Enter(fTimeDelta);
			m_ePreState = m_eCurState;
		}

		m_pCurState->Execute(fTimeDelta);
	}

	return __super::Update(fTimeDelta);
}

void CWizard_Support::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Support::Render()
{
	return S_OK;
}

void CWizard_Support::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);

	m_eCurState = eStates;
}

void CWizard_Support::Set_State(STATE eState, _fvector vState)
{
	m_pTransformCom->Set_State(eState, vState);
}

HRESULT CWizard_Support::Casting()
{
	CWizard_Support_AOE::DESC tDesc{};

	tDesc.eLevelID = m_eLevelID;
	tDesc.fRotationPerSec = XMConvertToRadians(0.f);
	tDesc.fSpeedPerSec = 0.f;
	tDesc.strName = TEXT("Wizard_Support_AOE");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_MonsterBullet"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

_bool CWizard_Support::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CWizard_Support::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CWizard_Support::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CWizard_Support::On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration)
{
	if (m_isInvincible || STATES::DEAD == m_eCurState)
		return;

	Create_HitEffect();

	m_fHp -= fDamage;
	m_fStaggerGage -= fStaggerValue;
	m_isHit = true;

	if (0 >= m_fHp)
	{
		m_fHp = 0.f;
		Change_States(STATES::DEAD);
		_string strRandomVoiceNum = to_string(rand() % 5);
		m_pSoundCom->Play("Death_Vo_" + strRandomVoiceNum);
	}
	else
	{
		m_fInvicibleTime = fInvicibleDuration;
		m_isInvincible = true;
		Change_States(STATES::HIT);
		_string strRandomNum = to_string(rand() % 7);
		m_pSoundCom->Play("Hurt_Vo_" + strRandomNum);
	}
}

void CWizard_Support::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	__super::On_Collision(MyColliderID, OtherColliderID, pOwner);
}

HRESULT CWizard_Support::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Sound_WizardSupport"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Support::Ready_PartObjects()
{
	CBody_WizardSupport::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Support"), &BodyDesc)))
		return E_FAIL;


	CUI3D_MobHPBar::DESC HPBarDesc{};

	HPBarDesc.pParentLevelID = &m_eLevelID;
	HPBarDesc.iNumPartObjects = CUI3D_MobHPBar::PART_END;
	HPBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HPBarDesc.pParentHP = &m_fHp;
	HPBarDesc.pParentMaxHP = &m_fMaxHp;
	HPBarDesc.pParentIsTargeted = &m_IsLockOnTarget;

	if (FAILED(__super::Add_PartObject(PART_HP, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI3D_MobHPBar"), &HPBarDesc)))
		return E_FAIL;

	CUI3D_LockOn::DESC LockOnDesc{};

	LockOnDesc.pParentLevelID = &m_eLevelID;
	LockOnDesc.iNumPartObjects = CUI3D_LockOn::PART_END;
	LockOnDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	LockOnDesc.pParentIsTargeted = &m_IsLockOnTarget;

	if (FAILED(__super::Add_PartObject(PART_LOCKON, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI3D_LockOn"), &LockOnDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Support::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)]     = new CWizard_SupportState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::CASTING)]  = new CWizard_SupportState_Casting(this);
	m_pStates[ENUM_CLASS(STATES::HIT)]      = new CWizard_SupportState_Hit(this);
	m_pStates[ENUM_CLASS(STATES::TELEPORT)] = new CWizard_SupportState_Teleport(this);
	m_pStates[ENUM_CLASS(STATES::MOVE)]     = new CWizard_SupportState_Move(this);
	m_pStates[ENUM_CLASS(STATES::DEAD)]     = new CWizard_SupportState_Dead(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

void CWizard_Support::Ready_SoundVolume()
{
	m_pSoundCom->SetVolume(0.2f);
}

CWizard_Support* CWizard_Support::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWizard_Support* pInstance = new CWizard_Support(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWizard_Support");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWizard_Support::Clone(void* pArg)
{
	CWizard_Support* pInstance = new CWizard_Support(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWizard_Support");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWizard_Support::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
