#pragma once
#include "Tool.h"

BEGIN(Engine)
class CGameObject;
class CCell;
class CShader;
END

BEGIN(Client)

class CTool_Navigation final : public CTool
{
public:
	enum MODE { CREATE, MODIFY, NORMAL, MODE_END };

private:
	CTool_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CTool_Navigation() = default;

public:
	void Set_Map(class CMap* pMap);
	
public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ExtraUI() override;

public: 
	virtual HRESULT Render_UI() override;

private: /* 맵툴 연동 */
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
	/* 정렬용 */
	_float				 m_fSnapRange = 0.5f;

private: /* 편의성 */
	void	Key_Input();
	void	Change_Mode();
	HRESULT Render_Cells();

private: /* 셀 생성 관련 */
	void	Add_ClickedPoint(_float3 vWorldPos);
	_float3 Snap_NearCellPoint(const _float3& vPickedPos);
	void    Sort_PointsCW(vector<_float3>& Points);
	_bool   Check_Cells(vector<_float3>& Points);
	HRESULT Create_Cell();

private: /* 셀 삭제 관련 */
	CCell* Find_Cell(_fvector vPickedPos);
	void   Delete_Cell(_fvector vPickedPos);

private: /* 셀 저장 불러오기 */
	void    Save_Load_Menu();
	HRESULT Save_Navigation(const _string& strNaviFileTag);
	HRESULT Load_Navigation(const _string& strNaviFileTag);


public:
	static CTool_Navigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END