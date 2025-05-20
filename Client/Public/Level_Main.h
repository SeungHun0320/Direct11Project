#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Main final : public CLevel
{
private:
	CLevel_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Main() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Load_Map(const _wstring& strMapFileTag) override;

public:
	static CLevel_Main* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END