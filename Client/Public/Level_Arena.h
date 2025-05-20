#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Arena final : public CLevel
{
private:
	CLevel_Arena(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Arena() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Load_Map(const _wstring& strMapFileTag) override;

public:
	static CLevel_Arena* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END