#include "GameInstance.h"

#include "Shadow.h"
#include "Frustum.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "RayPicking.h"
#include "PixelPicking.h"
#include "Sound_Device.h"
#include "Input_Device.h"
#include "Font_Manager.h"
#include "Light_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Camera_Manager.h"
#include "Target_Manager.h"
#include "Collider_Manager.h"
#include "Prototype_Manager.h"


IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	srand(unsigned(time(NULL)));

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

	/* 렌더러보다 먼저 만들어져야 함 */
	m_pTarget_Manager = CTarget_Manager::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pRayPicking = CRayPicking::Create(*ppDeviceOut, *ppContextOut, EngineDesc.hWnd, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pRayPicking)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pCamera_Manager = CCamera_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	m_pCollider_Manager = CCollider_Manager::Create(EngineDesc.iNumColliderGroups);
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	/* 통맵을 여러번 렌더타겟을 나눠서 그리니까 프레임이 너무 박살나서 피킹은 우선 생성에서 제외했음,, */
	/* 후처리 쉐이더에서 GPU를 안썼기때문에 프레임이 박살나던거였음 */
	m_pPixelPicking = CPixelPicking::Create(*ppDeviceOut, *ppContextOut, EngineDesc.hWnd);
	if (nullptr == m_pPixelPicking)
		return E_FAIL;

	m_pShadow = CShadow::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pShadow)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	/* 맵언맵하면서 프레임 박살남,, */
	//m_pPixelPicking->Update();

	m_pInputDevice->Update();
	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();
	m_pRayPicking->Update();

	m_pFrustum->Transform_ToWorldSpace();

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
	m_pCamera_Manager->Clear(iLevelIndex);

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
void CGameInstance::Change_Level(_uint iLevelIndex)
{
	m_pLevel_Manager->Change_Level(iLevelIndex);
}
void CGameInstance::Respawn_Objects()
{
	m_pLevel_Manager->Respawn_Objects();
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

CGameObject* CGameInstance::Find_Picked_Object(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Picked_Object(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Find_ObjectByName(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strObjectName)
{
	return m_pObject_Manager->Find_ObjectByName(iLevelIndex, strLayerTag, strObjectName);
}

void CGameInstance::Layer_Clear(_uint iLevelIndex, const _wstring& strLayerTag)
{
	m_pObject_Manager->Layer_Clear(iLevelIndex, strLayerTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#ifdef _DEBUG
HRESULT CGameInstance::Add_DebugComponent(CComponent* pDebugCom)
{
	return m_pRenderer->Add_DebugComponent(pDebugCom);
}
#endif 

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
	m_pRayPicking->Transform_ToLocalSpace(WorldMatrixInverse);
}
_bool CGameInstance::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pRayPicking->Picking_InWorld(vPickedPos, vPointA, vPointB, vPointC);
}
_bool CGameInstance::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pRayPicking->Picking_InLocal(vPickedPos, vPointA, vPointB, vPointC);
}

_bool CGameInstance::Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pRayPicking->Picking_InWorldEx(vPickedPos, fDist, vPointA, vPointB, vPointC);
}
_bool CGameInstance::Picking_InLocalEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pRayPicking->Picking_InLocalEx(vPickedPos, fDist, vPointA, vPointB, vPointC);
}

const _float3& CGameInstance::Get_MousePos()
{
	return m_pRayPicking->Get_MousePos();
}

const _float3& CGameInstance::Get_MouseRay()
{
	return m_pRayPicking->Get_MouseRay();
}

const _float3& CGameInstance::Get_LocalMousePos()
{
	return m_pRayPicking->Get_LocalMousePos();
}

const _float3& CGameInstance::Get_LocalMouseRay()
{
	return m_pRayPicking->Get_LocalMouseRay();
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
const _float4x4* CGameInstance::Get_Transform_Float4x4_Inv(D3DTS eState) const
{
	return  m_pPipeLine->Get_Transform_Float4x4_Inv(eState);
}
_matrix CGameInstance::Get_Transform_Matrix_Inv(D3DTS eState) const
{
	return  m_pPipeLine->Get_Transform_Matrix_Inv(eState);
}
const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}
void CGameInstance::Set_Far(_float fFar)
{
	m_pPipeLine->Set_Far(fFar);
}
void CGameInstance::Set_Near(_float fNear)
{
	m_pPipeLine->Set_Near(fNear);
}
const _float* CGameInstance::Get_Far_Ptr() const
{
	return m_pPipeLine->Get_Far_Ptr();
}
const _float* CGameInstance::Get_Near_Prt() const
{
	return m_pPipeLine->Get_Near_Prt();
}
#pragma endregion


#pragma region LIGHT_MANAGER
const LIGHT_DESC* CGameInstance::Get_Light(_uint iIndex)
{
	return m_pLight_Manager->Get_Light(iIndex);
}
HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}
HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}
void CGameInstance::Clear_Lights()
{
	m_pLight_Manager->Clear_Lights();
}
#pragma endregion


#pragma region FONT_MANAGER
HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
void CGameInstance::Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pFont_Manager->Draw(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}
#pragma endregion


#pragma region CAMERA_MANAGER
HRESULT CGameInstance::Add_Camera(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strCameraTag)
{
	return m_pCamera_Manager->Add_Camera(iLevelIndex, strLayerTag, strCameraTag);
}
void CGameInstance::Set_CameraMode(_uint iLevelIndex, const _wstring& strCameraTag, _uint iModeIndex)
{
	m_pCamera_Manager->Set_CameraMode(iLevelIndex, strCameraTag, iModeIndex);
}
_vector CGameInstance::Get_CameraState(_uint iLevelIndex, const _wstring& strCameraTag, STATE eState)
{
	return m_pCamera_Manager->Get_CameraState(iLevelIndex, strCameraTag, eState);
}
void CGameInstance::Shake_Camera(_uint iLevelIndex, const _wstring& strCameraTag, _float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	m_pCamera_Manager->Shake_Camera(iLevelIndex, strCameraTag, fIntensity, fDuration, fShakeFreqPos, fShakeFreqRot);
}
#pragma endregion


#pragma region COLLIDER_MANAGER
#ifdef _DEBUG
void CGameInstance::Reset_Colliders()
{
	m_pCollider_Manager->Reset_Colliders();
}
#endif

void CGameInstance::Reset_SlidingVectors()
{
	m_pCollider_Manager->Reset_SlidingVectors();
}

void CGameInstance::Delete_Collider(const CGameObject* pOwner)
{
	m_pCollider_Manager->Delete_Collider(pOwner);
}

void CGameInstance::Clear_Colliders()
{
	m_pCollider_Manager->Clear();
}
void CGameInstance::Clear_ColliderGroup(_uint iColliderGroup)
{
	m_pCollider_Manager->Clear_ColliderGroup(iColliderGroup);
}
HRESULT CGameInstance::Add_Collider(CCollider* pCollider, _uint iColliderGroupID)
{
	return m_pCollider_Manager->Add_Collider(pCollider, iColliderGroupID);
}
void CGameInstance::Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2)
{
	m_pCollider_Manager->Intersect(iColliderGroupID1, iColliderGroupID2);
}
#pragma endregion

#pragma region TARGET_MANAGER

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}
HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}
HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strTargetTag, const _char* pConstantName, CShader* pShader)
{
	return m_pTarget_Manager->Bind_ShaderResource(strTargetTag, pConstantName, pShader);
}
HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, _bool isTargetClear, ID3D11DepthStencilView* pDSV,  _bool isDepthClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, isTargetClear, pDSV, isDepthClear);
} 
HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}
HRESULT CGameInstance::Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest)
{
	return m_pTarget_Manager->Copy_Resource(strTargetTag, pDest);
}

#ifdef _DEBUG

HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}
HRESULT CGameInstance::Render_MRT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}

#endif

_bool CGameInstance::Picking(_float4* pOut)
{
	return m_pPixelPicking->Picking(pOut);
}
const _float4x4* CGameInstance::Get_Light_ViewMatrix()
{
	return m_pShadow->Get_Light_ViewMatrix();
}
const _float4x4* CGameInstance::Get_Light_ProjMatrix()
{
	return m_pShadow->Get_Light_ProjMatrix();
}
HRESULT CGameInstance::Ready_Light_For_Shadow(const SHADOW_DESC& Desc)
{
	return m_pShadow->Ready_Light_For_Shadow(Desc);
}
_bool CGameInstance::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}
#pragma endregion


void CGameInstance::Release_Engine()
{
	Safe_Release(m_pFrustum);

	Safe_Release(m_pShadow);

	Safe_Release(m_pPixelPicking);

	Safe_Release(m_pCollider_Manager);

	Safe_Release(m_pFont_Manager);

	Safe_Release(m_pRayPicking);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pCamera_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pInputDevice);

	Safe_Release(m_pSound_Device);

	Safe_Release(m_pPipeLine);

	Safe_Release(m_pLight_Manager);

	Safe_Release(m_pEvent_Manager);

	Safe_Release(m_pGraphic_Device);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();	
}
