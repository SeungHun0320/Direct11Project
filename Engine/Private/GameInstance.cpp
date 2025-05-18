#include "GameInstance.h"

#include "Picking.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Sound_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDeviceOut, ppContextOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInputDevice = CInput_Device::Create(EngineDesc.hInst, EngineDesc.hWnd);
	if (nullptr == m_pInputDevice)
		return E_FAIL;

	m_pSound_Device = CSound_Device::Create();
	if (nullptr == m_pSound_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDeviceOut, *ppContextOut, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pPicking)
		return E_FAIL;




	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInputDevice->Update();
	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();
	m_pPicking->Update();

	m_pObject_Manager->Update(fTimeDelta);	

	m_pObject_Manager->Late_Update(fTimeDelta);


	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Begin_Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pRenderer->Draw();
	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::End_Draw()
{
	m_pGraphic_Device->Present();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 특정 레벨의 자원을 삭제한다. */
	
	/* 특정 레벨의 객체을 삭제한다. */
	m_pObject_Manager->Clear(iLevelIndex);

	/* 특정 레벨의 원형객체를 삭제한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);
}

void CGameInstance::Object_Clear(_uint iLevelIndex)
{
	m_pObject_Manager->Clear(iLevelIndex);
}


_float CGameInstance::Compute_Random_Normal()
{
	return rand() / static_cast<_float>(RAND_MAX);	
}

_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Compute_Random_Normal();	
}

_string CGameInstance::WStringToString(const _wstring& wstr)
{
	if (wstr.empty())
		return {};

	_int iSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	_string result(iSize - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result.data(), iSize, nullptr, nullptr);
	return result;
}

_wstring CGameInstance::StringToWString(const _string& str)
{
	if (str.empty())
		return {};

	_int iSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	_wstring result(iSize - 1, 0); // -1: null terminator 제거
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, result.data(), iSize);
	return result;
}

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
const map<const _wstring, class CBase*>* CGameInstance::Get_Prototypes(_uint iLevelIndex) const
{
	return m_pPrototype_Manager->Get_Prototypes(iLevelIndex);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
	
}

CGameObject* CGameInstance::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	return m_pObject_Manager->Find_Object(iLevelIndex, strLayerTag, iIndex);
}

list<class CGameObject*>* CGameInstance::Find_ObjectList(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_ObjectList(iLevelIndex, strLayerTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}

#pragma endregion

#pragma region PICKING
void CGameInstance::Transform_Picking_ToLocalSpace(_fmatrix WorldMatrixInverse)
{
	m_pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
}
_bool CGameInstance::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pPicking->Picking_InWorld(vPickedPos, vPointA, vPointB, vPointC);
}
_bool CGameInstance::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pPicking->Picking_InLocal(vPickedPos, vPointA, vPointB, vPointC);
}

const _float3& CGameInstance::Get_MousePos()
{
	return m_pPicking->Get_MousePos();
}

const _float3& CGameInstance::Get_LocalMousePos()
{
	return m_pPicking->Get_LocalMousePos();
}

#pragma endregion

#pragma region INPUT_DEVICE


_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	return m_pInputDevice->Get_DIKeyState(eKeyID);
}


_byte CGameInstance::Get_DIMouseState(DIMK eMouseKeyID)
{
	return m_pInputDevice->Get_DIMouseState(eMouseKeyID);
}

_long CGameInstance::Get_DIMouseMove(DIMM eMouseMoveID)
{
	return m_pInputDevice->Get_DIMouseMove(eMouseMoveID);
}

_bool CGameInstance::Mouse_Down(DIMK eMouseKeyID)
{
	return m_pInputDevice->Mouse_Down(eMouseKeyID);
}

_bool CGameInstance::Mouse_Pressing(DIMK eMouseKeyID)
{
	return m_pInputDevice->Mouse_Pressing(eMouseKeyID);
}

_bool CGameInstance::Mouse_Up(DIMK eMouseKeyID)
{
	return m_pInputDevice->Mouse_Up(eMouseKeyID);
}

_bool CGameInstance::Key_Pressing(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Pressing(eKeyID);
}

_bool CGameInstance::Key_Up(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Up(eKeyID);
}

_bool CGameInstance::Key_Down(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Down(eKeyID);
}

#pragma endregion

#pragma region SOUND_DEVICE

FORCEINLINE
HRESULT CGameInstance::LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut)
{
	return m_pSound_Device->LoadSound(Path, is3D, loop, stream, pOut);
}

FORCEINLINE
CSound_Core* CGameInstance::Get_Single_Sound(const string& strKey)
{
	return m_pSound_Device->Get_Single_Sound(strKey);
}

FORCEINLINE
void CGameInstance::Set_Listener_Position(_fmatrix Matrix, _fvector vel)
{
	m_pSound_Device->Set_Listener_Position(Matrix, vel);
}

FORCEINLINE
void CGameInstance::Set_Master_Volume(_float volume)
{
	m_pSound_Device->Set_Master_Volume(volume);
}
#pragma endregion

#pragma region PIPELINE
void CGameInstance::Set_Transform(D3DTS eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}
const _float4x4* CGameInstance::Get_Transform_Float4x4(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}
_matrix CGameInstance::Get_Transform_Matrix(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}
const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pPicking);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pInputDevice);

	Safe_Release(m_pSound_Device);

	Safe_Release(m_pPipeLine);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();



	
}
