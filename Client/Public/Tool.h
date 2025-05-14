#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CTool abstract : public CBase
{
protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CTool() = default;
public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	class CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END