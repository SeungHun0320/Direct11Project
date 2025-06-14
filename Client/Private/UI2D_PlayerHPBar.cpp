#include "UI2D_PlayerHPBar.h"
#include "UI.h"

CUI2D_PlayerHPBar::CUI2D_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI2DContainerPart{pDevice, pContext}
{
}

CUI2D_PlayerHPBar::CUI2D_PlayerHPBar(const CUI2D_PlayerHPBar& Prototype)
	: CUI2DContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerHPBar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentHP = pDesc->pParentHP;
	m_pParentMaxHP = pDesc->pParentMaxHP;
	m_pParentHPRecorveryStat = pDesc->pParentHPRecorveryStat;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	

	return S_OK;
}

void CUI2D_PlayerHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerHPBar::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CUI2D_PlayerHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerHPBar::Render()
{
	return S_OK;
}

void CUI2D_PlayerHPBar::Set_UIVisible(_uint iPart, _bool isVisible)
{
}

HRESULT CUI2D_PlayerHPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerHPBar::Ready_PartObjects()
{
	CUI::DESC BackDesc{};

	BackDesc.eLevelID = m_eLevelID;
	BackDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BackDesc.fSizeX = 62.f;
	BackDesc.fSizeY = 186.f;
	BackDesc.fX = 60.f;
	BackDesc.fY = g_iWinSizeY * 0.825f;
	BackDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_HexBar_Back");

	if (FAILED(__super::Add_PartObject(PART_BACK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI"), &BackDesc)))
		return E_FAIL;

	CUI::DESC HpBarDesc{};

	HpBarDesc.eLevelID = m_eLevelID;
	HpBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HpBarDesc.fSizeX = 37.f;
	HpBarDesc.fSizeY = 129.f;
	HpBarDesc.fX = 59.f;
	HpBarDesc.fY = g_iWinSizeY * 0.825f;
	HpBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerHP");

	if (FAILED(__super::Add_PartObject(PART_HPBAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI"), &HpBarDesc)))
		return E_FAIL;

	CUI::DESC HpBarCapDesc{};

	HpBarCapDesc.eLevelID = m_eLevelID;
	HpBarCapDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HpBarCapDesc.fSizeX = 33.3f;
	HpBarCapDesc.fSizeY = 20.6f;
	HpBarCapDesc.fX = 59.f;
	HpBarCapDesc.fY = g_iWinSizeY * 0.825f;
	HpBarCapDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerHPCap");

	if (FAILED(__super::Add_PartObject(PART_HPBARCAP, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI"), &HpBarCapDesc)))
		return E_FAIL;


	for (_uint i = PART_NOTCH_START; i < PART_NOTCH; i++)
	{
		CUI::DESC NotchDesc{};

		NotchDesc.eLevelID = m_eLevelID;
		NotchDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		NotchDesc.fSizeX = 28.6f;
		NotchDesc.fSizeY = 11.6f;
		NotchDesc.fX = 60.f;
		NotchDesc.fY = (g_iWinSizeY * 0.525f) + (62.f * i);
		NotchDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_HexBar_Notch");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI"), &NotchDesc)))
			return E_FAIL;
	}


	return S_OK;
}

CUI2D_PlayerHPBar* CUI2D_PlayerHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerHPBar* pInstance = new CUI2D_PlayerHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerHPBar::Clone(void* pArg)
{
	CUI2D_PlayerHPBar* pInstance = new CUI2D_PlayerHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerHPBar::Free()
{
	__super::Free();
}
