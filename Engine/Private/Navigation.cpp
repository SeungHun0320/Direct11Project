#include "Navigation.h"

#include "Cell.h"
#include "GameInstance.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
    : CComponent(Prototype)
    , m_Cells { Prototype.m_Cells }
#ifdef _DEBUG
    , m_pShader{ Prototype.m_pShader }
#endif
{
    for (auto& pCell : m_Cells)
        Safe_AddRef(pCell);

#ifdef _DEBUG
    Safe_AddRef(m_pShader);
#endif

}

HRESULT CNavigation::Initialize_Prototype(const _wstring& strNavigationFilePath)
{
#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	// 파일 입출력해서 읽어오기

    return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* 어디 셀에서 시작할지 정함 */
	m_iIndex = pDesc->iIndex;

	return S_OK;
}

HRESULT CNavigation::Render()
{
	_float4x4		WorldMatrix;
	/* 내일 객체별로 다른 월드행렬 갖다 쓰게 할 예정 */
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ));

	m_pShader->Begin(0);

	for (auto& pCell : m_Cells)
		pCell->Render();

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strNavigationFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strNavigationFilePath)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
    __super::Free();

    for (auto& pCell : m_Cells)
        Safe_Release(pCell);

#ifdef _DEBUG
    Safe_Release(m_pShader);
#endif
}
