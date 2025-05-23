#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Shop final : public CLevel
{
private:
	CLevel_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Shop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Load_Map(const _wstring& strMapFileTag) override;
	virtual HRESULT Ready_Lights() override;

public:
	static CLevel_Shop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
