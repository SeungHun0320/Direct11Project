#pragma once

#include "Tool.h"

BEGIN(Client)

class CMapTool final : public CTool
{
public:
	enum MAP { COURTYARD, MAIN, ARENA, MAP_END };
	enum MODEL { BUSH, CHECKPOINT, GRASS, MODEL_END };
private:
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CMapTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void File_Menu();

private:
	vector<_string> m_EnvironmentNames;

private:
	_string  m_strMapFileTag;
	_wstring m_strName;

	class CMap* m_pMap = { nullptr };
private:
	HRESULT MapTool();

	void    Map_Menu();
	HRESULT Craete_Map(MAP iMapIdx, const _wstring& strLayerTag);

	HRESULT	Environment_ListBox();
	HRESULT Chest_ListBox();
	HRESULT Monster_ListBox();

	/* 저장 불러오기, 오브젝트 배치 수정 완 */
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