#include "UI2D_PlayerSPBar.h"

#include "UI.h"
#include "UI_Animation.h"

CUI2D_PlayerSPBar::CUI2D_PlayerSPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI2DContainerPart{ pDevice, pContext }
{
}

CUI2D_PlayerSPBar::CUI2D_PlayerSPBar(const CUI2D_PlayerSPBar& Prototype)
	: CUI2DContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerSPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerSPBar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentStamina = pDesc->pParentStamina;
	m_pParentMaxStamina = pDesc->pParentMaxStamina;
	m_pParentStaminaRecorveryStat = pDesc->pParentStaminaRecorveryStat;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fLerpSpeed = 5.f;
	
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(70.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CUI2D_PlayerSPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerSPBar::Update(_float fTimeDelta)
{
	_float fTargetRatio = *m_pParentStamina / max(*m_pParentMaxStamina, 0.001f);

	if (*m_pParentMaxStamina <= 0.f)
		fTargetRatio = 0.f;

	if (fTargetRatio > m_fStaminaRatio)
		m_fStaminaRatio = Lerp(m_fStaminaRatio, fTargetRatio, fTimeDelta * m_fLerpSpeed);
	else
		m_fStaminaRatio = fTargetRatio;


	CUI* pCap = dynamic_cast<CUI*>(m_PartObjects[PART_STAMINABARCAP]);
	if (nullptr != pCap)
	{
		_float fHpRatio = clamp(m_fStaminaRatio, 0.05f, 0.95f);

		_float fBarCenterY = g_iWinSizeY * 0.825f;
		_float fBarHeight = 150.6f;
		_float fCapRadius = 20.6f * 0.5f;

		_float fBarTopY = fBarCenterY - (fBarHeight * 0.5f);
		_float fBarBottomY = fBarCenterY + (fBarHeight * 0.5f);

		_float fHpY = fBarBottomY - (fBarHeight * fHpRatio);

		// 보정
		if (fHpRatio >= 1.f)
			fHpY -= fCapRadius;  // 캡 반지름만큼 위로
		else if (fHpRatio <= 0.f)
			fHpY += fCapRadius;  // 캡 반지름만큼 아래로

		pCap->Set_PositionY(fHpY, 0.f); 
	}

	return __super::Update(fTimeDelta);
}

void CUI2D_PlayerSPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerSPBar::Render()
{
	return S_OK;
}

void CUI2D_PlayerSPBar::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

HRESULT CUI2D_PlayerSPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerSPBar::Ready_PartObjects()
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

	CUI_Animation::DESC StaminaBarDesc{};

	StaminaBarDesc.eLevelID = m_eLevelID;
	StaminaBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	StaminaBarDesc.fSizeX = 62.f;
	StaminaBarDesc.fSizeY = 150.6f;
	StaminaBarDesc.fX = 60.f;
	StaminaBarDesc.fY = g_iWinSizeY * 0.825f;
	StaminaBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerStamina");
	StaminaBarDesc.pRatio = &m_fStaminaRatio;

	if (FAILED(__super::Add_PartObject(PART_STAMINABAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Animation"), &StaminaBarDesc)))
		return E_FAIL;

	CUI::DESC StaminaBarCapDesc{};

	StaminaBarCapDesc.eLevelID = m_eLevelID;
	StaminaBarCapDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	StaminaBarCapDesc.fSizeX = 32.f;
	StaminaBarCapDesc.fSizeY = 18.3f;
	StaminaBarCapDesc.fX = 59.5f;
	StaminaBarCapDesc.fY = g_iWinSizeY * 0.825f;
	StaminaBarCapDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerStaminaCap");

	if (FAILED(__super::Add_PartObject(PART_STAMINABARCAP, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI"), &StaminaBarCapDesc)))
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

CUI2D_PlayerSPBar* CUI2D_PlayerSPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerSPBar* pInstance = new CUI2D_PlayerSPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerSPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerSPBar::Clone(void* pArg)
{
	CUI2D_PlayerSPBar* pInstance = new CUI2D_PlayerSPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerSPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerSPBar::Free()
{
	__super::Free();
}
