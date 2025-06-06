#include "Wizard_Candleabra.h"

#include "GameInstance.h"
#include "Body_WizardCandleabra.h"
#include "Part_WizardCandleabra.h"
#include "Part_WizardSword.h"

#include "Wizard_CandleabraState.h"

CWizard_Candleabra::CWizard_Candleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWizard{ pDevice, pContext }
{
}

CWizard_Candleabra::CWizard_Candleabra(const CWizard_Candleabra& Prototype)
	: CWizard(Prototype)
{
}

HRESULT CWizard_Candleabra::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWizard_Candleabra::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_fDetectDistance = 12.5f;
	m_fChaseStopDistance = 30.f;

	if(LEVEL::TOOLS != m_eLevelID)
		Change_States(STATES::IDLE);

	m_PartObjects[PART_SWORD]->Set_Active(false);

	return S_OK;
}

void CWizard_Candleabra::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard_Candleabra::Update(_float fTimeDelta)
{

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

void CWizard_Candleabra::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Candleabra::Render()
{
	return S_OK;
}

void CWizard_Candleabra::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	cout << "촛불 개같이 성공\n";
}

void CWizard_Candleabra::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);

	m_eCurState = eStates;
}

_bool CWizard_Candleabra::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CWizard_Candleabra::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CWizard_Candleabra::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CWizard_Candleabra::Set_Active(PART ePart, _bool isActive)
{
	m_PartObjects[ePart]->Set_Active(isActive);
}

HRESULT CWizard_Candleabra::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Candleabra::Ready_PartObjects()
{
	CBody_WizardCandleabra::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Candleabra"), &BodyDesc)))
		return E_FAIL;

	CPart_WizardSword::DESC SwordDesc{};

	SwordDesc.eLevelID = m_eLevelID;
	SwordDesc.pSocketMatrix = dynamic_cast<CBody_WizardCandleabra*>(m_PartObjects[PART_BODY])->Get_SocketMatrix("held_L");
	SwordDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	SwordDesc.vScale = _float3(1.25f, 1.25f, 1.25f);
	SwordDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_SWORD, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Part_WizardSword"), &SwordDesc)))
		return E_FAIL;

	CPart_WizardCandleabra::DESC CandleabraDesc{};

	CandleabraDesc.eLevelID = m_eLevelID;
	CandleabraDesc.pSocketMatrix = dynamic_cast<CBody_WizardCandleabra*>(m_PartObjects[PART_BODY])->Get_SocketMatrix("held_R");
	CandleabraDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	CandleabraDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_CANDLEABRA, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Part_WizardCandleabra"), &CandleabraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWizard_Candleabra::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)]     = new CWizard_CandleabraState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::DETECTED)] = new CWizard_CandleabraState_Detected(this);
	m_pStates[ENUM_CLASS(STATES::GUARD)]    = new CWizard_CandleabraState_Guard(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK)]   = new CWizard_CandleabraState_Attack(this);
	m_pStates[ENUM_CLASS(STATES::MOVE)]     = new CWizard_CandleabraState_Move(this);
	m_pStates[ENUM_CLASS(STATES::HIT)]      = new CWizard_CandleabraState_Hit(this);
	m_pStates[ENUM_CLASS(STATES::DEAD)]     = new CWizard_CandleabraState_Dead(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

CWizard_Candleabra* CWizard_Candleabra::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWizard_Candleabra* pInstance = new CWizard_Candleabra(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWizard_Candleabra");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWizard_Candleabra::Clone(void* pArg)
{
	CWizard_Candleabra* pInstance = new CWizard_Candleabra(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWizard_Candleabra");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWizard_Candleabra::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
