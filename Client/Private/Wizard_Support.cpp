#include "Wizard_Support.h"

#include "GameInstance.h"
#include "Body_WizardSupport.h"
#include "Part_WizardStaff.h"

#include "Wizard_SupportState.h"

#include "Wizard_Support_AOE.h"

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

void CWizard_Support::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Support::Render()
{
	return S_OK;
}

void CWizard_Support::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	cout << "위자드서포트 개같이 성공\n";
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

HRESULT CWizard_Support::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
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
