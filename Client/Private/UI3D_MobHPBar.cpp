#include "UI3D_MobHPBar.h"

#include "UI.h"
#include "UI_Animation.h"

CUI3D_MobHPBar::CUI3D_MobHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI3D_MobHPBar::CUI3D_MobHPBar(const CUI3D_MobHPBar& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI3D_MobHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI3D_MobHPBar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentHP = pDesc->pParentHP;
	m_pParentMaxHP = pDesc->pParentMaxHP;
	m_pParentIsTargeted = pDesc->pParentIsTargeted;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fLerpSpeed = 5.f;

	return S_OK;
}

void CUI3D_MobHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI3D_MobHPBar::Update(_float fTimeDelta)
{
	if (!(*m_pParentIsTargeted))
		return LIFE::NONE;

	_float fTargetRatio = *m_pParentHP / max(*m_pParentMaxHP, 0.001f);

	if (*m_pParentMaxHP <= 0.f)
		fTargetRatio = 0.f;

	if (fTargetRatio > m_fHpRatio)
		m_fHpRatio = Lerp(m_fHpRatio, fTargetRatio, fTimeDelta * m_fLerpSpeed);
	else
		m_fHpRatio = fTargetRatio;


	return __super::Update(fTimeDelta);
}

void CUI3D_MobHPBar::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParentIsTargeted))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CUI3D_MobHPBar::Render()
{
	return S_OK;
}


HRESULT CUI3D_MobHPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI3D_MobHPBar::Ready_PartObjects()
{
	CUI::DESC BackDesc{};

	BackDesc.pParentLevelID = m_pLevelID;
	BackDesc.pParentMatrix = m_pParentMatrix;
	BackDesc.fSizeX = 124.5f;
	BackDesc.fSizeY = 41.f;
	BackDesc.fOffset = -1.5f;
	BackDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Enemy_HexBar");

	if (FAILED(__super::Add_PartObject(PART_BACK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D"), &BackDesc)))
		return E_FAIL;

	CUI_Animation::DESC HpBarDesc{};

	HpBarDesc.pParentLevelID = m_pLevelID;
	HpBarDesc.pParentMatrix = m_pParentMatrix;
	HpBarDesc.fSizeX = 124.5f;
	HpBarDesc.fSizeY = 41.f;
	HpBarDesc.fOffset = -1.5f;
	HpBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Enemy_HPBar");
	HpBarDesc.pRatio = &m_fHpRatio;
	HpBarDesc.eUIPass = CUI::PASS_HORIZONTAL_R2L;

	if (FAILED(__super::Add_PartObject(PART_HPBAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D_Animation"), &HpBarDesc)))
		return E_FAIL;

	return S_OK;
}

CUI3D_MobHPBar* CUI3D_MobHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI3D_MobHPBar* pInstance = new CUI3D_MobHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI3D_MobHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI3D_MobHPBar::Clone(void* pArg)
{
	CUI3D_MobHPBar* pInstance = new CUI3D_MobHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI3D_MobHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI3D_MobHPBar::Free()
{
	__super::Free();
}
