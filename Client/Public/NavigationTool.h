#pragma once
#include "Tool.h"

BEGIN(Engine)
class CGameObject;
class CCell;
END

BEGIN(Client)

class CNavigationTool : public CTool
{
public:
	enum MODE { CREATE, NORMAL, MODE_END };

protected:
	CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CNavigationTool() = default;

public:
	_bool IsFocused() const {
		return m_IsFocused;
	}

public:
	void Set_Map(class CMap* pMap);
	
public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ExtraUI() override;
public:
	virtual HRESULT Render_UI() override;

private:
	class CMap* m_pMap = { nullptr };

private:  /* 편의성 */
	_bool		m_bMode[MODE_END] = { false };
	_float3		m_vClickedPos = {};

private:
	vector<_float3>      m_ClickedPoints;
	vector<CCell*>		 m_Cells;


	_int				 m_iSelectedCellIndex = {};
	/* 정렬용 */
	_float				 m_fSnapRange = 1.f;

private: /* 편의성 */
	void Key_Input();
	void Change_Mode();

private:
	void Add_ClickedPoint(_float3 vWorldPos);
	HRESULT Create_Cell();


public:
	static CNavigationTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END