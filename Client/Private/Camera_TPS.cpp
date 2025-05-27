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

	return S_OK;
}

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
	//// 타겟 위치
	//_vector vTargetPos = static_cast<CTransform*>(m_pTarget->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION);

	//// 등 뒤 오프셋 적용 (타겟 월드 기준 변환 없음)
	//_vector vCamPos = vTargetPos + XMLoadFloat3(&m_vOffset);

	//m_pTransformCom->Set_State(STATE::POSITION, vCamPos);

	if (nullptr == m_pTarget && nullptr == m_pTargetTransformCom)
		return;

	_vector vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
	_vector vCamPos = m_pTransformCom->Get_State(STATE::POSITION);

	// 1. 플레이어 -> 카메라 상대 위치
	_vector vToTarget = vTargetPos - vCamPos;

	_float3 vDelta{};
	XMStoreFloat3(&vDelta, vToTarget);

	// 3. 벗어났는지 체크
	if (fabsf(vDelta.x) > m_fDeadZoneX || fabsf(vDelta.z) > m_fDeadZoneZ)
	{
		// 4. 목표 위치 재계산 (오프셋 포함)
		_vector vTargetCamPos = vTargetPos + XMLoadFloat3(&m_vOffset);

		// 5. 부드럽게 이동 (Lerp)
		_vector vNewCamPos = XMVectorLerp(vCamPos, vTargetCamPos, m_fSensor * fTimeDelta);
		m_pTransformCom->Set_State(STATE::POSITION, vNewCamPos);
	}

	__super::Bind_Matrices();
}

LIFE CCamera_TPS::Update(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return LIFE::DEAD;


	return LIFE::NONE;
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_TPS::Render()
{
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
}	
