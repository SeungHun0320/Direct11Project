#pragma once

/* ������ Ŭ���̾�Ʈ ���� ��ũ�� ��Ȱ�� �����Ѵ�. */
/* ���� ���� �����ϴ� ������ �̱��� Ŭ�����̴�. */
/* ���� �����ڰ� Ŭ�󰳓T�ڿ��� �����ְ���� �Լ��� ... */
#include "Prototype_Manager.h"

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

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

	_string WStringToString(const _wstring& wstr);
	_wstring StringToWString(const _string& str);

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	const map<const _wstring, class CBase*>* Get_Prototypes(_uint iLevelIndex) const;
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	/* �ش緹���� ���̾��±׿� �ε����� �ش�Ǵ� ������Ʈ�� ��ȯ�Ѵ�. */
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	/* �ش緹���� ���̾��±׿� �ش�Ǵ� ������Ʈ����Ʈ�� ��ȯ�Ѵ�. */
	list<class CGameObject*>* Find_ObjectList(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
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
	const _float3& Get_MousePos();
	const _float3& Get_LocalMousePos();
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
	// Out�� nullptr ������ �̱ۻ���� ��ϵ�
	HRESULT LoadSound(const string& Path, _bool is3D = FALSE, _bool loop = FALSE, _bool stream = FALSE, unordered_map<string, class CSound_Core*>* _Out_ pOut = nullptr);
	// �� ���巡�� �ø��Ŵ� �и��� ���ߴ� ���� �ø��� ������ ��Ű�ÿ�
	class CSound_Core* Get_Single_Sound(const string& strKey);
	// ������ ������ ����(������ �䱸�ؼ� Ʈ������ �޾���)
	void Set_Listener_Position(_fmatrix Matrix, _fvector vel);
	// ��ü ���� ����
	void Set_Master_Volume(_float volume);
#pragma endregion

#pragma region PIPELINE
	void Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
	const _float4x4* Get_Transform_Float4x4(D3DTS eState) const;
	_matrix Get_Transform_Matrix(D3DTS eState) const;
	const _float4* Get_CamPosition() const;
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
	class CPicking*				m_pPicking = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END