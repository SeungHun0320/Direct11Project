#pragma once

#include "Client_Defines.h"
#include "Level.h"

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
	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Map(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Environmnet_Object(const _wstring& strLayerTag);

private:
	virtual HRESULT Load_Map(const _wstring& strMapFileTag) override;

public:
	static CLevel_Courtyard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END