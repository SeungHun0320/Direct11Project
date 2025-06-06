#include "Blob.h"

#include "GameInstance.h"
#include "Body_Blob.h"
#include "BlobState.h"

CBlob::CBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CBlob::CBlob(const CBlob& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CBlob::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlob::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (FAILED(Ready_States()))
		return E_FAIL;

	m_fDetectDistance = 5.f;
	m_fChaseStopDistance = 10.f;

	Change_States(STATES::IDLE);

	return S_OK;
}

void CBlob::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBlob::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	if (KEY_DOWN(DIK_2))
	{
		Change_States(STATES::HIT);
	}
		

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

void CBlob::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBlob::Render()
{
	return S_OK;
}

void CBlob::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	cout << "블롭 개같이 성공\n";
}

void CBlob::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);

	m_eCurState = eStates;
}

_vector CBlob::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

_bool CBlob::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CBlob::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CBlob::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CBlob::Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed, _float fMinDistance)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookAtLerp(vTarget, fTimeDelta, 10.f);
	m_pTransformCom->Go_Target(vTarget, fTimeDelta, fMinDistance);
}

void CBlob::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CBlob::Hit(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CBlob::Turn(_fvector vAxis, _float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

_float3 CBlob::Get_Scaled()
{
	return m_pTransformCom->Get_Scaled();
}

void CBlob::Scaling(_float3 vScale)
{
	m_pTransformCom->Scaling(vScale);
}

void CBlob::Scaling(_float fX, _float fY, _float fZ)
{
	m_pTransformCom->Scaling(fX, fY, fZ);
}

HRESULT CBlob::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlob::Ready_PartObjects()
{
	CBody_Blob::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Blob"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlob::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)]   = new CBlobState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK)] = new CBlobState_Attack(this);
	m_pStates[ENUM_CLASS(STATES::JUMP)]   = new CBlobState_Jump(this);
	m_pStates[ENUM_CLASS(STATES::HIT)]   = new CBlobState_Hit(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

CBlob* CBlob::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlob* pInstance = new CBlob(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlob");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlob::Clone(void* pArg)
{
	CBlob* pInstance = new CBlob(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlob");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlob::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
