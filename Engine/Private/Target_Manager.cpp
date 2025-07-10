#include "Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Initialize()
{
    return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Craete(m_pDevice, m_pContext, iWidth, iHeight, ePixelFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*> MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _wstring& strMRTTag, _bool isTargetClear, ID3D11DepthStencilView* pDSV, _bool isDepthClear)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	/* 다 그리고 나서 다시 셋해주기 위해 백버퍼와 깊이버퍼를 들고옴 */
	/* 내부적으로 레퍼런스 카운트를 올림 */
	m_pContext->OMGetRenderTargets(1, &m_pBackBuffer, &m_pOriginalDSV);

	ID3D11DepthStencilView* pDepthStencilView = nullptr == pDSV ? m_pOriginalDSV : pDSV;

	if(true == isDepthClear)
		m_pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	_uint iNumTargets = { 0 };

	ID3D11RenderTargetView* pRenderTargets[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		if(true == isTargetClear)
			pRenderTarget->Clear();
		pRenderTargets[iNumTargets++] = pRenderTarget->Get_RTV();
	}

	if (8 < iNumTargets)
		return E_FAIL;

	/* 렌더타겟의 갯수만큼 배열의 주소를 던진다 */
	m_pContext->OMSetRenderTargets(iNumTargets, pRenderTargets, pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	/* 다시 백퍼버를 셋해준다 */
	m_pContext->OMSetRenderTargets(1, &m_pBackBuffer, m_pOriginalDSV);
	/* 겟이 내부적으로 레퍼카운트를 올리기때문에 여기서는 릴리즈 해주고 nullptr로 바꿔줌 */
	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pOriginalDSV);

	m_pBackBuffer = nullptr;
	m_pOriginalDSV  = nullptr;

	return S_OK;
}

HRESULT CTarget_Manager::Bind_ShaderResource(const _wstring& strTargetTag, const _char* pContantName, CShader* pShader)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(pContantName, pShader);
}

HRESULT CTarget_Manager::Copy_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Copy_Resource(pDest);
}

#ifdef  _DEBUG

HRESULT CTarget_Manager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);

	return S_OK;
}

HRESULT CTarget_Manager::Render_Debug(const _wstring strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	/* 렌더타겟 다 돌면서 렌더해야겠지요? */
	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Render(pShader, pVIBuffer);

	return S_OK;
}

#endif

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto iter = m_RenderTargets.find(strTargetTag);
	if (iter == m_RenderTargets.end())
		return nullptr;

    return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _wstring& strMRTTag)
{
	auto iter = m_MRTs.find(strMRTTag);
	if (iter == m_MRTs.end())
		return nullptr;

    return &iter->second;
}

CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTarget_Manager* pInstance = new CTarget_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CTarget_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTarget_Manager::Free()
{
    __super::Free();

	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pOriginalDSV);

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);
	m_RenderTargets.clear();


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
