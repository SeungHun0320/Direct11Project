#pragma once

#include "Tool.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CMapTool final : public CTool
{
public:
	enum MAP { COURTYARD, MAIN, ARENA, SHOP, MAP_END };
	enum ENVIRONMENT_MODEL { BUSH, CHECKPOINT, GRASS, MODEL_END };
	enum ITEM_MODEL { POTION, IM_END };
	enum ENEMY_MODEL { BLOB, EM_END };
	enum CHEST_TYPE { CT_MONEY, CT_ITEM, CT_END };
	enum MODE { CREATE, MODIFY, NORMAL, MODE_END };
	enum LIST { ENVIRONMENT, ITEM, CHEST, ENEMY, LIST_END };
private:
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CMapTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void File_Menu();

private:/* 프로토타입 이름 받아올 벡터 */
	vector<_string> m_ProtoEnvironmentNames;
	vector<_string> m_ProtoItemNames;
	vector<_string> m_ProtoMonsterNames;

	_wstring m_strLayerTag;

private: /* 애님 메쉬 리스트 박스용 벡터 */
	vector<_string> m_EnvironmentNames;
	vector<_string> m_ChestNames;
	vector<_string> m_MonsterNames;

private: /* 오브젝트 제어용 변수들 */
	_bool   m_bAllScale = { false };
	_float  m_fSpeedPerSec = {};
	_float  m_fRotationPerSec = {};
	_float3 m_vScale = {};
	_float4 m_vAngle = {};
	_float3 m_vInitPos = {};

private: /* 현재 선택된 맵 정보 */
	MAP m_eCurrentMap = { MAP_END };
	MAP m_ePreMap = { MAP_END };

private:
	ImGuizmo::OPERATION m_eGizmoOp = { ImGuizmo::TRANSLATE };
	_bool m_bMode[MODE_END] = { false };
	_bool m_bLists[LIST_END] = { false };
	_bool m_bFirst = { false };

	_string  m_strMapFileTag;
	_wstring m_strName;

private:
	CGameObject* m_pModifyObject = { nullptr };
	class CMap* m_pMap = { nullptr };

private: /* 다들 아시죠? */
	void Key_Input();
	HRESULT MapTool();

private: /* 통맵 쪽 코드 */
	void    Map_Menu();
	HRESULT Craete_Map(MAP iMapIdx, const _wstring& strLayerTag);

private: /* 수정할 오브젝트 */
	HRESULT On_Modify_Object();
	void Adj_Scale_Angle();
	void Seleted_List(LIST eList);

private: /* 리스트 박스들 */
	void Add_ListBoxName();
	HRESULT	Environment_ListBox();
	HRESULT Item_ListBox();
	HRESULT Chest_ListBox();
	HRESULT Monster_ListBox();

private: /* 애님메쉬 제어용 함수들 */
	void AnimMesh_Menu();
	void Add_Modify_ListBox(vector<_string>& vecNames, const _wstring& strName);
	HRESULT AnimMesh_Chest_ListBox();
	HRESULT AnimMesh_Monster_ListBox();
	HRESULT AnimMesh_Environment_ListBox();

private: /* 모드 바꾸는 아임구이 버튼 */
	void Change_Mode();

private: /* 저장 불러오기, 오브젝트 배치 수정 완 */
	void Load_Map_Menu();
	HRESULT Save_Map(const _string& strMapTag);
	HRESULT Load_Map(const _string& strMapTag);

private:
	HRESULT Craete_Camera(const _wstring& strLayerTag);


public:
	static CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END