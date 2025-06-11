#pragma once
#include "Tool.h"

BEGIN(Engine)
class CGameObject;
class CCell;
class CShader;
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
	CShader*  m_pShader = { nullptr };
	_bool	  m_bMode[MODE_END] = { false };
	_float3	  m_vClickedPos = {};
	_float3	  m_vCurrentPos = {};
	_string   m_strNaviFileTag;

private:
	vector<_float3>      m_ClickedPoints;
	vector<CCell*>		 m_Cells;


	_int				 m_iSelectedCellIndex = {};
	/* 정렬용 */
	_float				 m_fSnapRange = 1.5f;

private: /* 편의성 */
	void	Key_Input();
	void	Change_Mode();
	HRESULT Render_Cells();

private: /* 셀 생성 관련 */
	void Add_ClickedPoint(_float3 vWorldPos);
	HRESULT Create_Cell();
	_float3 Snap_NearCellPoint(const _float3& vPickedPos);
	void    Sort_PointsCW(vector<_float3>& Points);

private: /* 셀 저장 불러오기 */
	void    Save_Load_Menu();
	HRESULT Save_Navigation(const _string& strNaviFileTag);
	HRESULT Load_Navigation(const _string& strNaviFileTag);


public:
	static CNavigationTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END