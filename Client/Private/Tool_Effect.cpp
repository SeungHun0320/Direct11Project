#include "Tool_Effect.h"

CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool {pDevice, pContext}
{
}

HRESULT CTool_Effect::Initialize()
{
	return S_OK;
}

void CTool_Effect::Update(_float fTimeDelta)
{
}

HRESULT CTool_Effect::Render()
{
	ImGui::Begin(u8"ÀÌÆåÆ® Åø");
	Check_SelectedTool();




	ImGui::End();

	return S_OK;
}

HRESULT CTool_Effect::Render_ExtraUI()
{
	return S_OK;
}

CTool_Effect* CTool_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect* pInstance = new CTool_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CTool_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Effect::Free()
{
	__super::Free();
}
