#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_MapTool final : public CLevel
{
private:
	CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MapTool() = default;

#ifdef  _IMGUI

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_ImGui();

public:
	static CLevel_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

#endif //  _IMGUI
};

END