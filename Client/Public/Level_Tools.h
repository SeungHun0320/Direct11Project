#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

BEGIN(Client)

class CLevel_Tools final : public CLevel
{
public:
	enum TOOL_ACTIVE_TYPE { TOOL_MAP, TOOL_NAVIGATION, TOOL_EFFECT, TOOL_END };

private:
	CLevel_Tools(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tools() = default;

#ifdef  _IMGUI

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CTool_Map*		   m_pMapTool = { nullptr };
	class CTool_Navigation*	   m_pNavigationTool = { nullptr };
	class CTool_Effect*		   m_pEffectTool = { nullptr };

private:
	class CMap*			   m_pMap = { nullptr };

private:
	TOOL_ACTIVE_TYPE	   m_eActiveType = { TOOL_END };

private:
	HRESULT Ready_ImGui();
	HRESULT Ready_DockSpace();
	void ImGui_RenderBegin();
	void ImGui_RenderEnd();

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_For_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Tools();

private:
	virtual HRESULT Ready_Lights() override;


public:
	static CLevel_Tools* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

#endif //  _IMGUI
};

END