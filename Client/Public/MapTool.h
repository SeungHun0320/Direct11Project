#pragma once

#include "Tool.h"

BEGIN(Client)

class CMapTool final : public CTool
{
public:
	enum MAP { COURTYARD, MAIN, ARENA, MAP_END };
private:
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CMapTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_wstring m_strMapFileTag;

private:
	HRESULT MapTool();

	void    Map_Menu();
	HRESULT Craete_Map(MAP iMapIdx, const _wstring& strLayerTag);

	HRESULT	Environment_ListBox();
	HRESULT Chest_ListBox();
	HRESULT Monster_ListBox();


	void Save();
	void Load();

	HRESULT Save_Map(const _wstring& strMapTag);
	HRESULT Load_Map(const _wstring& strMapTag);


public:
	static CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END