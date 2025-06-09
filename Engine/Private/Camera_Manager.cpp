#include "Camera_Manager.h"
#include "Camera.h"

#include "GameInstance.h"

CCamera_Manager::CCamera_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCamera_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pCameras = new /*map<const _wstring, class CBase*>*/CAMERAS[iNumLevels];

	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strCameraTag)
{
	list<CGameObject*>* pCameraList = m_pGameInstance->Find_ObjectList(iLevelIndex, strLayerTag);
	if (nullptr == pCameraList)
		return E_FAIL;

	for (auto& pObject : *pCameraList)
	{
		if (pObject->Get_Name() == strCameraTag)
		{
			CCamera* pCamera = dynamic_cast<CCamera*>(pObject);

			m_pCameras[iLevelIndex].emplace(strCameraTag, pCamera);
			Safe_AddRef(pCamera);
			break;
		}
	}

	return S_OK;
}

void CCamera_Manager::Set_CameraMode(_uint iLevelIndex, const _wstring& strCameraTag, _uint iModeIndex)
{
	CCamera* pCamera = Find_Camera(iLevelIndex, strCameraTag);
	if (nullptr == pCamera)
		return;

	pCamera->Set_Mode(iModeIndex);
}

_vector CCamera_Manager::Get_CameraState(_uint iLevelIndex, const _wstring& strCameraTag, STATE eState)
{
	CCamera* pCamera = Find_Camera(iLevelIndex, strCameraTag);
	if (nullptr == pCamera)
		return XMVectorZero();

	CTransform* pCameraTransform = dynamic_cast<CTransform*>(pCamera->Get_Component(TEXT("Com_Transform")));

	if (nullptr == pCameraTransform)
		return XMVectorZero();

	return pCameraTransform->Get_State(eState);
}

void CCamera_Manager::Shake_Camera(_uint iLevelIndex, const _wstring& strCameraTag, _float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	CCamera* pCamera = Find_Camera(iLevelIndex, strCameraTag);
	if (nullptr == pCamera)
		return;

	pCamera->Shake_Camera(fIntensity, fDuration, fShakeFreqPos, fShakeFreqRot);
}

void CCamera_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pCameras[iLevelIndex])
		Safe_Release(Pair.second);

	m_pCameras[iLevelIndex].clear();
}

CCamera* CCamera_Manager::Find_Camera(_uint iLevelIndex, const _wstring& strCameraTag)
{
	auto	iter = m_pCameras[iLevelIndex].find(strCameraTag);
	if (iter == m_pCameras[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CCamera_Manager* CCamera_Manager::Create(_uint iNumLevels)
{
	CCamera_Manager* pInstance = new CCamera_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CCamera_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pCameras[i])
			Safe_Release(Pair.second);
		m_pCameras[i].clear();
	}

	Safe_Delete_Array(m_pCameras);

	Safe_Release(m_pGameInstance);
}
