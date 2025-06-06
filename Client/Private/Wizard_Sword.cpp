#include "Wizard_Sword.h"

#include "GameInstance.h"
#include "Body_WizardSword.h"
#include "Part_WizardSword.h"

#include "Wizard_SwordState.h"

CWizard_Sword::CWizard_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CWizard_Sword::CWizard_Sword(const CWizard_Sword& Prototype)
	: CMonster(Prototype)
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

	if (LEVEL::TOOLS != m_eLevelID)
	{
		if (1.f == m_pTransformCom->Get_SpeedPerSec())
			Change_States(STATES::PRAY);
		else
			Change_States(STATES::IDLE);
	}

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

void CWizard_Sword::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	cout << "위자드소드 개같이 성공\n";
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

_vector CWizard_Sword::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
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

void CWizard_Sword::Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed, _float fMinDistance)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookAtLerpEx(vTarget, fTimeDelta, 10.f);
	m_pTransformCom->Go_Target(vTarget, fTimeDelta, fMinDistance);
}

void CWizard_Sword::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CWizard_Sword::Hit(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CWizard_Sword::Turn(_fvector vAxis, _float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CWizard_Sword::LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->LookAtLerpEx(vDir, fTimeDelta, fSpeed);
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
