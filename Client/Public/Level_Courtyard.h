#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

#include "Monster.h"

BEGIN(Client)

class CLevel_Courtyard final : public CLevel
{
private:
	CLevel_Courtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Courtyard() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CGlobalShadow* m_pGlobalShadow = { nullptr };
	CSound_Core* m_pBGM = { nullptr };

private:
	vector<CMonster::DESC> m_MonsterDescs;

private:
	HRESULT Ready_Layer_Pawn(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);

private:
	virtual HRESULT Load_Map(const _wstring& strMapFileTag) override;
	virtual HRESULT Ready_Lights() override;
	virtual HRESULT Respawn_Objects() override;
	virtual void    Check_Collision() override;

public:
	static CLevel_Courtyard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END