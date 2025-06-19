#include "UI2D_PlayerMPBar.h"

#include "UI.h"
#include "UI_Animation.h"

CUI2D_PlayerMPBar::CUI2D_PlayerMPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_PlayerMPBar::CUI2D_PlayerMPBar(const CUI2D_PlayerMPBar& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerMPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerMPBar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentMana = pDesc->pParentMana;
	m_pParentMaxMana = pDesc->pParentMaxMana;
	m_pParentManaRecorveryStat = pDesc->pParentManaRecorveryStat;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fLerpSpeed = 5.f;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(140.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CUI2D_PlayerMPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerMPBar::Update(_float fTimeDelta)
{
	_float fTargetRatio = *m_pParentMana / max(*m_pParentMaxMana, 0.001f);

	if (*m_pParentMaxMana <= 0.f)
		fTargetRatio = 0.f;

	if (fTargetRatio > m_fManaRatio)
		m_fManaRatio = Lerp(m_fManaRatio, fTargetRatio, fTimeDelta * m_fLerpSpeed);
	else
		m_fManaRatio = fTargetRatio;


	CUI* pCap = dynamic_cast<CUI*>(m_PartObjects[PART_MANABARCAP]);
	if (nullptr != pCap)
	{
		// 체력바 기준값
		_float fHpRatio = clamp(m_fManaRatio, 0.05f, 0.95f);

		_float fBarCenterY = g_iWinSizeY * 0.825f;
		_float fBarHeight = 150.6f;
		_float fCapRadius = 20.6f * 0.5f;

		// 체력바 위치 기준값
		_float fBarTopY = fBarCenterY - (fBarHeight * 0.5f);
		_float fBarBottomY = fBarCenterY + (fBarHeight * 0.5f);

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

void CUI2D_PlayerMPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerMPBar::Render()
{
	return S_OK;
}

HRESULT CUI2D_PlayerMPBar::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerMPBar::Ready_PartObjects()
{
	CUI::DESC BackDesc{};

	BackDesc.pParentLevelID = m_pLevelID;
	BackDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BackDesc.fSizeX = 62.f;
	BackDesc.fSizeY = 186.f;
	BackDesc.fX = 60.f;
	BackDesc.fY = g_iWinSizeY * 0.825f;
	BackDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_HexBar_Back");

	if (FAILED(__super::Add_PartObject(PART_BACK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &BackDesc)))
		return E_FAIL;

	CUI_Animation::DESC ManaBarDesc{};

	ManaBarDesc.pParentLevelID = m_pLevelID;
	ManaBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	ManaBarDesc.fSizeX = 62.f;
	ManaBarDesc.fSizeY = 150.6f;
	ManaBarDesc.fX = 60.f;
	ManaBarDesc.fY = g_iWinSizeY * 0.825f;
	ManaBarDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerMana");
	ManaBarDesc.pRatio = &m_fManaRatio;
	ManaBarDesc.eUIPass = CUI_Animation::PASS_VERTICAL;

	if (FAILED(__super::Add_PartObject(PART_MANABAR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_Animation"), &ManaBarDesc)))
		return E_FAIL;

	CUI::DESC ManaBarCapDesc{};

	ManaBarCapDesc.pParentLevelID = m_pLevelID;
	ManaBarCapDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	ManaBarCapDesc.fSizeX = 32.f;
	ManaBarCapDesc.fSizeY = 18.3f;
	ManaBarCapDesc.fX = 59.5f;
	ManaBarCapDesc.fY = g_iWinSizeY * 0.825f;
	ManaBarCapDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PlayerManaCap");

	if (FAILED(__super::Add_PartObject(PART_MANABARCAP, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &ManaBarCapDesc)))
		return E_FAIL;


	for (_uint i = PART_NOTCH_START; i < PART_NOTCH; i++)
	{
		CUI::DESC NotchDesc{};

		NotchDesc.pParentLevelID = m_pLevelID;
		NotchDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		NotchDesc.fSizeX = 28.6f;
		NotchDesc.fSizeY = 11.6f;
		NotchDesc.fX = 60.f;
		NotchDesc.fY = (g_iWinSizeY * 0.525f) + (62.f * i);
		NotchDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_HexBar_Notch");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &NotchDesc)))
			return E_FAIL;
	}


	return S_OK;
}

CUI2D_PlayerMPBar* CUI2D_PlayerMPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerMPBar* pInstance = new CUI2D_PlayerMPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerMPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerMPBar::Clone(void* pArg)
{
	CUI2D_PlayerMPBar* pInstance = new CUI2D_PlayerMPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerMPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerMPBar::Free()
{
	__super::Free();
}
