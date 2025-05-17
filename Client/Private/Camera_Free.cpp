#include "Camera_Free.h"
#include "GameInstance.h"
CCamera_Free::CCamera_Free(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext)
    : CCamera {pDeivce, pContext}
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
    : CCamera (Prototype)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_eLevelID = pDesc->eLevelID;
	m_fSensor = pDesc->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
    return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);

	__super::Bind_Matrices();
}

LIFE CCamera_Free::Update(_float fTimeDelta)
{
	return LIFE::NONE;
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
    return S_OK;
}

void CCamera_Free::Key_Input(_float fTimeDelta)
{
	/* 카메라의 움직임은 항상 프라이오리 업데이트에서 해주는게 맞는듯 */
	/* 왜? 카메라 매트릭스 연산을 통해 다른 객체들도 그려질 수 있게 하기 위해서 */
	if (KEY_PRESSING(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (KEY_DOWN(DIK_R))
	{
		m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 20.f, -15.f, 1.f));
	}

	if (LEVEL::TOOLS != m_eLevelID || MOUSE_PRESSING(DIMK::RBUTTON))
	{
		_long			MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fSensor);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), MouseMove * fTimeDelta * m_fSensor);
		}
	}
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
