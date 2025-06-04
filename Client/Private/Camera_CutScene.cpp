#include "Camera_CutScene.h"
#include "GameInstance.h"

CCamera_CutScene::CCamera_CutScene(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext)
	: CCamera{ pDeivce, pContext }
{
}

CCamera_CutScene::CCamera_CutScene(const CCamera_CutScene& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_CutScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_CutScene::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCamera_CutScene::Priority_Update(_float fTimeDelta)
{

	__super::Bind_Matrices();
}

LIFE CCamera_CutScene::Update(_float fTimeDelta)
{
	return LIFE::NONE;
}

void CCamera_CutScene::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_CutScene::Render()
{
	return S_OK;
}

CCamera_CutScene* CCamera_CutScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_CutScene* pInstance = new CCamera_CutScene(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_CutScene::Clone(void* pArg)
{
	CCamera_CutScene* pInstance = new CCamera_CutScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_CutScene::Free()
{
	__super::Free();
}
