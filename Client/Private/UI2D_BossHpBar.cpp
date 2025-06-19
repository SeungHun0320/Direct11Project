#include "UI2D_BossHPBar.h"
#include "GameInstance.h"

#include "UI.h"
#include "UI_Animation.h"

CUI2D_BossHPBar::CUI2D_BossHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_BossHPBar::CUI2D_BossHPBar(const CUI2D_BossHPBar& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_BossHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_BossHPBar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentHP = pDesc->pParentHP;
	m_pParentMaxHP = pDesc->pParentMaxHP;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fLerpSpeed = 5.f;

	return S_OK;
}

void CUI2D_BossHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_BossHPBar::Update(_float fTimeDelta)
{
	_float fTargetRatio = *m_pParentHP / max(*m_pParentMaxHP, 0.001f);

	if (*m_pParentMaxHP <= 0.f)
		fTargetRatio = 0.f;

	if (fTargetRatio > m_fHpRatio)
		m_fHpRatio = Lerp(m_fHpRatio, fTargetRatio, fTimeDelta * m_fLerpSpeed);
	else
		m_fHpRatio = fTargetRatio;

	return __super::Update(fTimeDelta);
}

void CUI2D_BossHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI2D_BossHPBar::Render()
{
	m_pGameInstance->Draw_Font(TEXT("Font_Money"), TEXT("공성병기"), _float2(1100.f, 560.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

void CUI2D_BossHPBar::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

HRESULT CUI2D_BossHPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_BossHPBar::Ready_PartObjects()
{
	CUI::DESC BackDesc{};

	BackDesc.pParentLevelID = m_pLevelID;
	BackDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BackDesc.fSizeX = 559.2f;
	BackDesc.fSizeY = 72.4f;
	BackDesc.fX = (g_iWinSizeX * 0.75f);
	BackDesc.fY = g_iWinSizeY * 0.9f;
	BackDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_BossHexBar");

	if (FAILED(__super::Add_PartObject(PART_BACK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &BackDesc)))
		return E_FAIL;

	CUI_Animation::DESC HpBarDesc{};

	HpBarDesc.pParentLevelID = m_pLevelID;
	HpBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HpBarDesc.fSizeX = 559.2f;
	HpBarDesc.fSizeY = 72.4f;
	HpBarDesc.fX = (g_iWinSizeX * 0.75f);
	HpBarDesc.fY = g_iWinSizeY * 0.9f;
	HpBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_BossHpBar");
	HpBarDesc.pRatio = &m_fHpRatio;
	HpBarDesc.eUIPass = CUI_Animation::PASS_HORIZONTAL_L2R;

	if (FAILED(__super::Add_PartObject(PART_HPBAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_Animation"), &HpBarDesc)))
		return E_FAIL;

	for (_uint i = PART_DIVIDER_START; i <= PART_DIVIDER; i++)
	{
		CUI::DESC DividerDesc{};
		DividerDesc.pParentLevelID = m_pLevelID;
		DividerDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		DividerDesc.fSizeX = 1.6f;
		DividerDesc.fSizeY = 52.f;
		DividerDesc.fX = (g_iWinSizeX * 0.4f) + (150 * i);
		DividerDesc.fY = g_iWinSizeY * 0.9f;
		DividerDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_BossHpDivider");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &DividerDesc)))
			return E_FAIL;
	}

	CUI::DESC NameDesc{};
	NameDesc.pParentLevelID = m_pLevelID;
	NameDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	NameDesc.fSizeX = 358.4f;
	NameDesc.fSizeY = 38.4f;
	NameDesc.fX = (g_iWinSizeX * 0.8f);
	NameDesc.fY = g_iWinSizeY * 0.85f;
	NameDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_BossName");

	if (FAILED(__super::Add_PartObject(PART_NAME, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &NameDesc)))
		return E_FAIL;

	return S_OK;
}

CUI2D_BossHPBar* CUI2D_BossHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_BossHPBar* pInstance = new CUI2D_BossHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_BossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_BossHPBar::Clone(void* pArg)
{
	CUI2D_BossHPBar* pInstance = new CUI2D_BossHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_BossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_BossHPBar::Free()
{
	__super::Free();
}
