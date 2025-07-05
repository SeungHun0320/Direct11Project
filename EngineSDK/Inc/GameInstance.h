#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Prototype_Manager.h"
#include "Event_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void Update_Engine(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	void Object_Clear(_uint iLevelIndex);
	HRESULT Begin_Draw();
	HRESULT Draw();
	HRESULT End_Draw();

#pragma region UTILITY

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

	_string WStringToString(const _wstring& wstr);
	_wstring StringToWString(const _string& str);

#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	void Change_Level(_uint iLevelIndex);
	void Respawn_Objects();
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	const map<const _wstring, class CBase*>* Get_Prototypes(_uint iLevelIndex) const;
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	/* 해당레벨의 레이어태그와 인덱스에 해당되는 오브젝트를 반환한다. */
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	/* 해당레벨의 레이어태그에 해당되는 오브젝트리스트를 반환한다. */
	list<class CGameObject*>* Find_ObjectList(_uint iLevelIndex, const _wstring& strLayerTag);
	/* 피킹된 오브젝트를 반환한다. */
	class CGameObject* Find_Picked_Object(_uint iLevelIndex, const _wstring& strLayerTag);
	/* 이거 좀 이상하긴 함 ㅋㅋ*/
	class CGameObject* Find_ObjectByName(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strObjectName);
	void Layer_Clear(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#ifdef _DEBUG
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif

#pragma endregion
	
#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion
//
#pragma region PICKING
	void Transform_Picking_ToLocalSpace(_fmatrix WorldMatrixInverse);
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocalEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	const _float3& Get_MousePos();
	const _float3& Get_MouseRay();
	const _float3& Get_LocalMousePos();
	const _float3& Get_LocalMouseRay();
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKeyState(_ubyte eKeyID);
	_byte Get_DIMouseState(DIMK eMouseKeyID);
	_long Get_DIMouseMove(DIMM eMouseMoveID);
	_bool Mouse_Down(DIMK eMouseKeyID);
	_bool Mouse_Pressing(DIMK eMouseKeyID);
	_bool Mouse_Up(DIMK eMouseKeyID);
	_bool Key_Pressing(_ubyte eKeyID);
	_bool Key_Up(_ubyte eKeyID);
	_bool Key_Down(_ubyte eKeyID);
#pragma endregion

#pragma region SOUND_DEVICE
	// Out에 nullptr 넣으면 싱글사운드로 등록됨
	HRESULT LoadSound(const string& Path, _bool is3D = FALSE, _bool loop = FALSE, _bool stream = FALSE, unordered_map<string, class CSound_Core*>* _Out_ pOut = nullptr);
	// 난 에드래퍼 올릴거다 분명히 말했다 래퍼 올린다 릴리즈 시키시오
	class CSound_Core* Get_Single_Sound(const string& strKey);
	// 리스너 포지션 제어(업룩포 요구해서 트랜스폼 받았음)
	void Set_Listener_Position(_fmatrix Matrix, _fvector vel);
	// 전체 볼륨 제어
	void Set_Master_Volume(_float volume);
#pragma endregion

#pragma region PIPELINE
	void Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
	const _float4x4* Get_Transform_Float4x4(D3DTS eState) const;
	_matrix Get_Transform_Matrix(D3DTS eState) const;

	const _float4x4* Get_Transform_Float4x4_Inv(D3DTS eState) const;
	_matrix Get_Transform_Matrix_Inv(D3DTS eState) const;

	const _float4* Get_CamPosition() const;

	void Set_Far(_float fFar);
	void Set_Near(_float fNear);

	const _float* Get_Far_Ptr() const;
	const _float* Get_Near_Prt() const;
#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_Light(_uint iIndex);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void Clear_Lights();
#pragma endregion

#pragma region FONT_MANAGER
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	void Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
#pragma endregion

#pragma region CAMERA_MANAGER
	HRESULT Add_Camera(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strCameraTag);
	void    Set_CameraMode(_uint iLevelIndex, const _wstring& strCameraTag, _uint iModeIndex);
	_vector Get_CameraState(_uint iLevelIndex, const _wstring& strCameraTag, STATE eState);
	void Shake_Camera(_uint iLevelIndex, const _wstring& strCameraTag, _float fIntensity = 1.f, _float fDuration = 1.f, _float fShakeFreqPos = 100.f, _float fShakeFreqRot = 40.f);
#pragma endregion

#pragma region COLLIDER_MANAGER
#ifdef _DEBUG
	void Reset_Colliders();
#endif // _DEBUG
	void Reset_SlidingVectors();
	void Delete_Collider(const class CGameObject* pOwner);
	void Clear_Colliders();
	void Clear_ColliderGroup(_uint iColliderGroup);
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	void    Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);
#pragma endregion

#pragma region EVENT_MANAGER
	template<typename... Args>
	HRESULT Subscribe_Event(const _wstring& strEventTag, Delegate<Args...> dlg)
	{
		return m_pEvent_Manager->Subscribe(strEventTag, dlg);
	}

	template<typename... Args>
	void Publish_Event(const _wstring& strEventTag, Args... args)
	{
		m_pEvent_Manager->Publish(strEventTag, args...);
	}

	template<typename... Args>
	void Unsubscribe_Event(void* pListener)
	{
		m_pEvent_Manager->Unsubscribe<Args...>(pListener);
	}

#pragma endregion

#pragma region TARGET_MANAGER
	/* 매니저가 직접 렌더타겟을 만들고, 관리해준다. */
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	/* 특정 타겟들을 장치에 동시에(최대 8개) 바인딩 해준다 */
	HRESULT Bind_RT_ShaderResource(const _wstring& strTargetTag, const _char* pConstantName, class CShader* pShader);
	HRESULT Begin_MRT(const _wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest);
#ifdef _DEBUG
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_MRT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

#pragma endregion

#pragma region PIXELPICKING
	_bool Picking(_float4* pOut);
#pragma endregion



private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CInput_Device*		m_pInputDevice = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CSound_Device*		m_pSound_Device = { nullptr };
	class CPipeLine*			m_pPipeLine = { nullptr };
	class CRayPicking*			m_pRayPicking = { nullptr };
	class CLight_Manager*		m_pLight_Manager = { nullptr };
	class CFont_Manager*		m_pFont_Manager = { nullptr };
	class CCamera_Manager*		m_pCamera_Manager = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };
	class CEvent_Manager*		m_pEvent_Manager = { nullptr };
	class CTarget_Manager*      m_pTarget_Manager = { nullptr };
	class CPixelPicking*		m_pPixelPicking = { nullptr };	

public:
	void Release_Engine();
	virtual void Free() override;
};

END