#include "Camera_TPS.h"
#include "GameInstance.h"


#include "Player.h"

CCamera_TPS::CCamera_TPS(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext)
	: CCamera{ pDeivce, pContext }
{
}

CCamera_TPS::CCamera_TPS(const CCamera_TPS& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_TPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_TPS::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;
	m_fSensor = pDesc->fSensor;
	m_pTarget = pDesc->pTarget;
	m_vOffset = pDesc->vOffset;
	m_fDeadZoneX = pDesc->fDeadZoneX;
	m_fDeadZoneZ = pDesc->fDeadZoneZ;

	if (nullptr != m_pTarget)
	{
		Safe_AddRef(m_pTarget);
		m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pTarget->Get_Component(TEXT("Com_Transform")));

		if (nullptr == m_pTargetTransformCom)
			return E_FAIL;
		else
			Safe_AddRef(m_pTargetTransformCom);
	}
		
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Ready_For_BossCamera(TEXT("SpiderTank"));

	XMStoreFloat3(&m_vCurrentFocusPos, m_pTargetTransformCom->Get_State(STATE::POSITION));
	m_vTargetFocusPos = m_vCurrentFocusPos;

	return S_OK;
}

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
	if (nullptr == m_pTarget || nullptr == m_pTargetTransformCom)
		return;

#pragma region 데드존 카메라(이거 못 쓸듯)
	//if (!dynamic_cast<CPlayer*>(m_pTarget)->IsLockOn())
	//{
	//	_vector vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	//	_vector vCamPos = m_pTransformCom->Get_State(STATE::POSITION);

	//	// 1. 플레이어 -> 카메라 상대 위치
	//	_vector vToTarget = vTargetPos - vCamPos;

	//	_float3 vDelta{};
	//	XMStoreFloat3(&vDelta, vToTarget);

	//	// 3. 벗어났는지 체크
	//	if (fabsf(vDelta.x) > m_fDeadZoneX || fabsf(vDelta.z) > m_fDeadZoneZ)
	//	{
	//		// 4. 목표 위치 재계산 (오프셋 포함)
	//		_vector vTargetCamPos = vTargetPos + XMLoadFloat3(&m_vOffset);

	//		// 5. 부드럽게 이동 (Lerp)
	//		_vector vNewCamPos = XMVectorLerp(vCamPos, vTargetCamPos, m_fSensor * fTimeDelta);
	//		m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);
	//	}
	//}
#pragma endregion

	switch (m_eMode)
	{
	case CAM_MODE::TPS:
		Update_LockOnCamera(fTimeDelta);
		break;

	case CAM_MODE::BOSS:
		Update_BossCamera(fTimeDelta);
		break;

	default:
		break;
	}

	if (m_isShake)
	{
		Update_Camera_Shake(fTimeDelta);
	}

	m_pTransformCom->Move(m_vCurrentShakePos);
	m_pTransformCom->Turn(XMConvertToRadians(m_vCurrentShakeRot.x), XMConvertToRadians(m_vCurrentShakeRot.y), XMConvertToRadians(m_vCurrentShakeRot.z));

	__super::Bind_Matrices();

	m_pTransformCom->Turn(XMConvertToRadians(-m_vCurrentShakeRot.x), XMConvertToRadians(-m_vCurrentShakeRot.y), XMConvertToRadians(-m_vCurrentShakeRot.z));

	_vector vOffset = XMLoadFloat3(&m_vCurrentShakePos) * -1.f;
	_float3 vInversOffset{};
	XMStoreFloat3(&vInversOffset, vOffset);
	m_pTransformCom->Move(vInversOffset);
}

LIFE CCamera_TPS::Update(_float fTimeDelta)
{
	return LIFE::NONE;
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_TPS::Render()
{
	return S_OK;
}

void CCamera_TPS::Update_LockOnCamera(_float fTimeDelta)
{
	if (!m_pTarget)
		return;

	_float fXZRadius = sqrtf(m_vOffset.x * m_vOffset.x + m_vOffset.z * m_vOffset.z);
	_float fFixedAngle = atan2f(m_vOffset.x, m_vOffset.z);

	if (dynamic_cast<CPlayer*>(m_pTarget)->IsLockOn())
	{
		if (60.f >= m_fAngle)
			m_fAngle = min(60.f, m_fAngle + fTimeDelta * 50.f);
	}
	else
	{
		if (50.f <= m_fAngle)
			m_fAngle = max(50.f, m_fAngle - fTimeDelta * 50.f);
	}

	_float fPosXZ = cosf(XMConvertToRadians(m_fAngle)) * fXZRadius;
	_float fPosY = sinf(XMConvertToRadians(m_fAngle)) * fXZRadius;

	_float fPosX = sinf(fFixedAngle) * fPosXZ;
	_float fPosZ = cosf(fFixedAngle) * fPosXZ;

	_vector vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);

	if (CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTarget))
	{
		if (pPlayer->Get_IsTarget()) // 적 타깃 있으면
		{
			_vector vEnemy = pPlayer->Get_TargetState(STATE::POSITION);

			_float fDist = XMVectorGetX(XMVector3Length(vEnemy - vTargetPos));

			if(fDist <= pPlayer->Get_FindDistance())
				vTargetPos = (vTargetPos + vEnemy) * 0.5f;
		}
	}

	XMStoreFloat3(&m_vTargetFocusPos, vTargetPos);
	XMStoreFloat3(&m_vCurrentFocusPos, XMVectorLerp(XMLoadFloat3(&m_vCurrentFocusPos), XMLoadFloat3(&m_vTargetFocusPos), 5.f * fTimeDelta));
	_vector vFocus = XMVectorSetW(XMLoadFloat3(&m_vCurrentFocusPos), 1.f);

	m_pTransformCom->Set_State(STATE::POSITION, vFocus + XMVectorSet(fPosX, fPosY, fPosZ, 0.f));
	m_pTransformCom->LookAt(vFocus);
}

void CCamera_TPS::Update_BossCamera(_float fTimeDelta)
{
	if (!m_pTargetTransformCom || !m_pBossTransformCom)
		return;

	_vector vPlayerPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	_vector vBossPos = m_pBossTransformCom->Get_State(STATE::POSITION);

	_float  fWeightPlayer = 0.7f;
	_vector vFocus = vPlayerPos * fWeightPlayer + vBossPos * (1.f - fWeightPlayer);

	_vector vRawDir = vPlayerPos - vBossPos;
	_float fDistSq = XMVectorGetX(XMVector3LengthSq(vRawDir));

	// 거의 겹쳤다면 디폴트 방향으로
	if (fDistSq < 0.0001f)
	{
		if (!m_bAdjustableCamDir)
		{
			vRawDir = XMVectorSet(0.f, 0.f, 1.f, 0.f); // 기본 전방 방향 (z+)
			m_bAdjustableCamDir = true;
		}
	}
	else
	{
		m_bAdjustableCamDir = false;
		vRawDir = XMVector3Normalize(vRawDir);
	}
	

	_float3 vDir{};
	XMStoreFloat3(&vDir, vRawDir);
	_float fYawAngle = atan2f(vDir.x, vDir.z);

	_float fXZRadius = sqrtf(m_vOffset.x * m_vOffset.x + m_vOffset.z * m_vOffset.z);

	if (dynamic_cast<CPlayer*>(m_pTarget)->IsLockOn())
	{
		if (60.f >= m_fAngle)
			m_fAngle = min(60.f, m_fAngle + fTimeDelta * 50.f);
	}
	else
	{
		if (50.f <= m_fAngle)
			m_fAngle = max(50.f, m_fAngle - fTimeDelta * 50.f);
	}

	_float fPosXZ = cosf(XMConvertToRadians(m_fAngle)) * fXZRadius;
	_float fPosY = sinf(XMConvertToRadians(m_fAngle)) * fXZRadius;

	_float fPosX = sinf(fYawAngle) * fPosXZ;
	_float fPosZ = cosf(fYawAngle) * fPosXZ;

	//XMStoreFloat3(&m_vTargetFocusPos, vTargetPos);
	XMStoreFloat3(&m_vTargetFocusPos, vFocus);
	XMStoreFloat3(&m_vCurrentFocusPos, XMVectorLerp(XMLoadFloat3(&m_vCurrentFocusPos), XMLoadFloat3(&m_vTargetFocusPos), 5.f * fTimeDelta));
	//_vector vFocus = XMVectorSetW(XMLoadFloat3(&m_vCurrentFocusPos), 1.f);

	_vector vCamPos = vFocus + XMVectorSet(fPosX, fPosY, fPosZ, 0.f);

	// focus와 너무 가까우면 보정
	if (XMVectorGetX(XMVector3LengthSq(vCamPos - vFocus)) < 0.0001f)
		vCamPos += XMVectorSet(0.f, 0.f, -1.f, 0.f); // 뒤로 살짝 밀기

	m_pTransformCom->Set_State(STATE::POSITION, vCamPos);
	m_pTransformCom->LookAt(XMVectorSetW(vFocus, 1.f));
}

HRESULT CCamera_TPS::Ready_For_BossCamera(const _wstring& strBossName)
{
	m_pBoss = m_pGameInstance->Find_ObjectByName(ENUM_CLASS(m_eLevelID), TEXT("Layer_Monster"), strBossName);

	if (nullptr != m_pBoss)
	{
		Safe_AddRef(m_pBoss);
		m_pBossTransformCom = dynamic_cast<CTransform*>(m_pBoss->Get_Component(TEXT("Com_Transform")));

		if (nullptr == m_pBossTransformCom)
			return E_FAIL;
		else
			Safe_AddRef(m_pBossTransformCom);
	}

	return S_OK;
}

CCamera_TPS* CCamera_TPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_TPS* pInstance = new CCamera_TPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_TPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_TPS::Clone(void* pArg)
{
	CCamera_TPS* pInstance = new CCamera_TPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_TPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_TPS::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
	Safe_Release(m_pTargetTransformCom);

	Safe_Release(m_pBoss);
	Safe_Release(m_pBossTransformCom);
}	
