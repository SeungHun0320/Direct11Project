#include "UI2D_PlayerHPBar.h"

#include "UI.h"
#include "UI_Animation.h"

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

	m_fLerpSpeed = 5.f;

	return S_OK;
}

void CUI2D_PlayerHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerHPBar::Update(_float fTimeDelta)
{
	_float fTargetRatio = *m_pParentHP / max(*m_pParentMaxHP, 0.001f);

	if (*m_pParentMaxHP <= 0.f)
		fTargetRatio = 0.f;

	if (fTargetRatio > m_fHpRatio)
		m_fHpRatio = Lerp(m_fHpRatio, fTargetRatio, fTimeDelta * m_fLerpSpeed);
	else
		m_fHpRatio = fTargetRatio;


	CUI* pCap = dynamic_cast<CUI*>(m_PartObjects[PART_HPBARCAP]);
	if (nullptr != pCap)
	{
		// 체력바 기준값
		_float fHpRatio = clamp(m_fHpRatio, 0.05f, 0.95f);


		_float fBarCenterY = g_iWinSizeY * 0.825f;
		_float fBarHeight = 150.6f;
		_float fCapRadius = 20.6f * 0.5f;

		// 체력바 위치 기준값
		_float fBarTopY = fBarCenterY - (fBarHeight * 0.5f);
		_float fBarBottomY = fBarCenterY + (fBarHeight * 0.5f);

		// 현재 체력 위치 계산 (아래 -> 위)
		_float fHpY = fBarBottomY - (fBarHeight * fHpRatio);

		// 보정
		if (fHpRatio >= 1.f)
			fHpY -= fCapRadius;  // 캡 반지름만큼 위로
		else if (fHpRatio <= 0.f)
			fHpY += fCapRadius;  // 캡 반지름만큼 아래로

		pCap->Set_PositionY(fHpY, 0.f);  // fOffset은 보정 없음
	}

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
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
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

	CUI_Animation::DESC HpBarDesc{};

	HpBarDesc.eLevelID = m_eLevelID;
	HpBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HpBarDesc.fSizeX = 62.f;
	HpBarDesc.fSizeY = 150.6f;
	HpBarDesc.fX = 60.f;
	HpBarDesc.fY = g_iWinSizeY * 0.825f;
	HpBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerHP");
	HpBarDesc.pRatio = &m_fHpRatio;

	if (FAILED(__super::Add_PartObject(PART_HPBAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Animation"), &HpBarDesc)))
		return E_FAIL;

	CUI::DESC HpBarCapDesc{};

	HpBarCapDesc.eLevelID = m_eLevelID;
	HpBarCapDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	HpBarCapDesc.fSizeX = 32.f;
	HpBarCapDesc.fSizeY = 18.3f;
	HpBarCapDesc.fX = 59.5f;
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
