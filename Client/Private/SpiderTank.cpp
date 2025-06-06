#include "SpiderTank.h"

#include "GameInstance.h"
#include "Body_SpiderTank.h"

#include "SpiderTankState.h"

#include "SpiderTank_Bullet.h"
#include "SpiderTank_Orb.h"

CSpiderTank::CSpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss{pDevice, pContext}
{
}

CSpiderTank::CSpiderTank(const CSpiderTank& Prototype)
	: CBoss(Prototype)
{
}

HRESULT CSpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderTank::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (Ready_States())
		return E_FAIL;

	m_fDetectDistance = 10.f;
	m_fChaseStopDistance = 20.f;
	m_fPreferredDistance = 13.5f;

	m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(180.f), XMConvertToRadians(0.f));

	Change_States(STATES::SLEEP);

	return S_OK;
}

void CSpiderTank::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CSpiderTank::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	__super::Update(fTimeDelta);

	if (KEY_DOWN(DIK_2))
		Change_States(STATES::KNOCKBACK);
	if (KEY_DOWN(DIK_3))
		Change_States(STATES::PINCH);
	if (KEY_DOWN(DIK_4))
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

	return LIFE::NONE;
}

void CSpiderTank::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderTank::Render()
{
	return S_OK;
}

void CSpiderTank::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
}

void CSpiderTank::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);

	m_eCurState = eStates;
}

_vector CSpiderTank::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

_bool CSpiderTank::Is_CurrentAnim(PART ePart, _uint iNextIndex)
{
	return m_PartObjects[ePart]->Is_CurrentAnim(iNextIndex);
}

_bool CSpiderTank::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CSpiderTank::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CSpiderTank::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CSpiderTank::Set_TickPerSecond(PART ePart, _float fTickPerSecond)
{
	m_PartObjects[ePart]->Set_TickPerSecond(fTickPerSecond);
}

_bool CSpiderTank::Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed, _float fMinDistance)
{
	if (m_fDistanceToPlayer <= fMinDistance)
		return false;

	m_pTransformCom->Set_SpeedPerSec(fSpeed);

	_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
	_vector vDir = XMVector3Normalize(vTarget - m_pTransformCom->Get_State(STATE::POSITION));

	_float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vLook, vDir));
	_float fSpeedFactor = max(0.1f, fAngle / XMConvertToRadians(180.f));
	_float fLerpSpeed = 0.5f + (3.0f * fSpeedFactor);

	m_pTransformCom->LookAtLerpEx(vTarget, fTimeDelta, fLerpSpeed);
	m_pTransformCom->Go_Target(vTarget, fTimeDelta, fMinDistance);

	return true;
}

void CSpiderTank::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CSpiderTank::Go_Dir(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CSpiderTank::Hit(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CSpiderTank::Turn(_fvector vAxis, _float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CSpiderTank::LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->LookAtLerpEx(vDir, fTimeDelta, fSpeed);
}

_float CSpiderTank::Compute_AngleToPlayer()
{
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vTargetPos = m_pTargetTransform->Get_State(STATE::POSITION);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);
	_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);

	_float fDot = XMVectorGetX(XMVector3Dot(vDir, vLook));
	fDot = clamp(fDot, -1.f, 1.f);

	return acosf(fDot);
}

_float CSpiderTank::Compute_LookSppedByAngle(_float fAngle)
{
	// 각도에 따른 회전 속도 동적 조절
	_float fLerpSpeed = 1.f;
	if (fAngle > XMConvertToRadians(150.f))
		fLerpSpeed = 2.f;
	else if (fAngle > XMConvertToRadians(90.f))
		fLerpSpeed = 1.75f;
	else if (fAngle > XMConvertToRadians(45.f))
		fLerpSpeed = 1.5f;
	else if (fAngle > XMConvertToRadians(20.f))
		fLerpSpeed = 1.25f;
	else
		fLerpSpeed = 1.f;

	return fLerpSpeed;
}

_bool CSpiderTank::Is_TargetOnRight()
{
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vTargetPos = m_pTargetTransform->Get_State(STATE::POSITION);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);
	_vector vRight = m_pTransformCom->Get_State(STATE::RIGHT);

	return XMVectorGetX(XMVector3Dot(vDir, vRight)) > 0.f;
}

HRESULT CSpiderTank::Shot_Bullet()
{
	CSpiderTank_Bullet::DESC tDesc{};

	tDesc.eLevelID = m_eLevelID;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.fSpeedPerSec = 50.f;
	tDesc.strName = TEXT("SpiderTank_Bullet");
	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	_vector vTarget = Get_TargetPosition();

	XMStoreFloat3(&tDesc.vDir, vTarget);

	tDesc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y + 7.5f, vPos.z);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_MonsterBullet"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderTank::Shot_Bomb()
{
	CSpiderTank_Orb::DESC tDesc{};
	
	tDesc.eLevelID = m_eLevelID;
	tDesc.fRotationPerSec = XMConvertToRadians(180.f);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.strName = TEXT("SpiderTank_Orb");
	XMStoreFloat3(&tDesc.vDir, m_pTransformCom->Get_State(STATE::LOOK));
	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	tDesc.WorldMatrix = XMMatrixTranslation(vPos.x , vPos.y + 13.f , vPos.z);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_MonsterBullet"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CSpiderTank::LookAtYaw(_vector vDir, _float fLerpRatio)
{
	m_pTransformCom->LookAtYaw(vDir, fLerpRatio);
}

void CSpiderTank::Change_Camera(CAM_MODE eMode)
{
	m_pGameInstance->Set_CameraMode(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), ENUM_CLASS(eMode));
}

HRESULT CSpiderTank::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderTank::Ready_PartObjects()
{
	CBody_SpiderTank::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_SpiderTank"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderTank::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::SLEEP)]		= new CSpiderTankState_Sleep(this);
	m_pStates[ENUM_CLASS(STATES::WAKE_UP)]		= new CSpiderTankState_WakeUp(this);
	m_pStates[ENUM_CLASS(STATES::IDLE)]			= new CSpiderTankState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::FORWARD)]		= new CSpiderTankState_Forward(this);
	m_pStates[ENUM_CLASS(STATES::BACKWARD)]		= new CSpiderTankState_Backward(this);
	m_pStates[ENUM_CLASS(STATES::LTURN)]        = new CSpiderTankState_TurnLeft(this);
	m_pStates[ENUM_CLASS(STATES::RTURN)]        = new CSpiderTankState_TurnRight(this);
	m_pStates[ENUM_CLASS(STATES::REVERSE)]      = new CSpiderTankState_Reverse(this);
	m_pStates[ENUM_CLASS(STATES::LAGER)]		= new CSpiderTankState_Lager(this);
	m_pStates[ENUM_CLASS(STATES::SPAWNMOB)]     = new CSpiderTankState_SpawnMob(this);
	m_pStates[ENUM_CLASS(STATES::FAST_ATTACK)]  = new CSpiderTankState_FastAttack(this);
	m_pStates[ENUM_CLASS(STATES::SWING)]		= new CSpiderTankState_Swing(this);
	m_pStates[ENUM_CLASS(STATES::FULLSWING)]	= new CSpiderTankState_FullSwing(this);
	m_pStates[ENUM_CLASS(STATES::READY_SHOT)]	= new CSpiderTankState_ReadyShot(this);
	m_pStates[ENUM_CLASS(STATES::SHOT)]			= new CSpiderTankState_Shot(this);
	m_pStates[ENUM_CLASS(STATES::END_SHOT)]		= new CSpiderTankState_EndShot(this);
	m_pStates[ENUM_CLASS(STATES::READY_BOMB)]	= new CSpiderTankState_ReadyBomb(this);
	m_pStates[ENUM_CLASS(STATES::SHOT_BOMB)]	= new CSpiderTankState_ShotBomb(this);
	m_pStates[ENUM_CLASS(STATES::END_BOMB)]		= new CSpiderTankState_EndBomb(this);
	m_pStates[ENUM_CLASS(STATES::PINCH)]		= new CSpiderTankState_Pinch(this);
	m_pStates[ENUM_CLASS(STATES::KNOCKBACK)]	= new CSpiderTankState_KnockBack(this);
	m_pStates[ENUM_CLASS(STATES::DEAD)]			= new CSpiderTankState_Dead(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

CSpiderTank* CSpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderTank* pInstance = new CSpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderTank::Clone(void* pArg)
{
	CSpiderTank* pInstance = new CSpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderTank::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
