#include "UI.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI::CUI(const CUI& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI::Priority_Update(_float fTimeDelta)
{
}

void CUI::Update(_float fTimeDelta)
{
}

void CUI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CUI::Render()
{
	/* 셰이더에게 본인의 월드행렬을 넘겨줌 */
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* 직교투영용 행렬들을 따로 셰이더에게 넘겨줌 (뷰, 투영)*/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 인자로 들어간 상수 0은 텍스쳐인덱스 */
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	/* 인자로 들어간 상수 0 은 패스의 번호 */
	/* 이 패스로 그리기를 하겠다는 뜻 */
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CUI* CUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI* pInstance = new CUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI::Clone(void* pArg)
{
	CUI* pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
