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

private:/* ������Ÿ�� �̸� �޾ƿ� ���� */
	vector<_string> m_ProtoEnvironmentNames;
	vector<_string> m_ProtoItemNames;
	vector<_string> m_ProtoMonsterNames;

	_wstring m_strLayerTag;

private: /* �ִ� �޽� ����Ʈ �ڽ��� ���� */
	vector<_string> m_EnvironmentNames;
	vector<_string> m_ChestNames;
	vector<_string> m_MonsterNames;

private: /* ������Ʈ ����� ������ */
	_bool   m_bAllScale = { false };
	_float  m_fSpeedPerSec = {};
	_float  m_fRotationPerSec = {};
	_float3 m_vScale = {};
	_float4 m_vAngle = {};
	_float3 m_vInitPos = {};

private: /* ���� ���õ� �� ���� */
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

private: /* �ٵ� �ƽ���? */
	void Key_Input();
	HRESULT MapTool();

private: /* ��� �� �ڵ� */
	void    Map_Menu();
	HRESULT Craete_Map(MAP iMapIdx, const _wstring& strLayerTag);

private: /* ������ ������Ʈ */
	HRESULT On_Modify_Object();
	void Adj_Scale_Angle();
	void Seleted_List(LIST eList);

private: /* ����Ʈ �ڽ��� */
	void Add_ListBoxName();
	HRESULT	Environment_ListBox();
	HRESULT Item_ListBox();
	HRESULT Chest_ListBox();
	HRESULT Monster_ListBox();

private: /* �ִԸ޽� ����� �Լ��� */
	void AnimMesh_Menu();
	void Add_Modify_ListBox(vector<_string>& vecNames, const _wstring& strName);
	HRESULT AnimMesh_Chest_ListBox();
	HRESULT AnimMesh_Monster_ListBox();
	HRESULT AnimMesh_Environment_ListBox();

private: /* ��� �ٲٴ� ���ӱ��� ��ư */
	void Change_Mode();

private: /* ���� �ҷ�����, ������Ʈ ��ġ ���� �� */
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