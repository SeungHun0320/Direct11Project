#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Tools final : public CLevel
{
public:
	enum TOOLTYPE { TT_MAP, TT_CAMERA, TT_END };
private:
	CLevel_Tools(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tools() = default;

#ifdef  _IMGUI

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CMapTool* m_pMapTool = { nullptr };

private:
	HRESULT Ready_ImGui();
	HRESULT Ready_DockSpace();
	void ImGui_RenderBegin();
	void ImGui_RenderEnd();

	HRESULT FileDialog();

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Tools();

public:
	static CLevel_Tools* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

#endif //  _IMGUI
};

END