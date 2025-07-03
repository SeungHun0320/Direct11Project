#include "UI2D_PlayerPotion.h"
#include "GameInstance.h"

#include "UI.h"
#include "Inventory.h"

CUI2D_PlayerPotion::CUI2D_PlayerPotion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_PlayerPotion::CUI2D_PlayerPotion(const CUI2D_PlayerPotion& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerPotion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerPotion::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pInventory = pDesc->pInventory;

	if (nullptr == m_pInventory)
		return E_FAIL;

	Safe_AddRef(m_pInventory);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	for (_int i = PART_POTION; i >= m_pInventory->Get_NumPotion(); i--)
		Set_UIVisible(i, false);
	
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(445, 40.f, 0.f, 1.f));

	return S_OK;
}

void CUI2D_PlayerPotion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerPotion::Update(_float fTimeDelta)
{
	return 	__super::Update(fTimeDelta);
}

void CUI2D_PlayerPotion::Late_Update(_float fTimeDelta)
{
	_int iCurNumPotion = m_pInventory->Get_CurPotion();
	_int iNumPotion = m_pInventory->Get_NumPotion();

	Set_UIVisible(iNumPotion - 1, true);

	for (_int i = 0; i < iNumPotion; i++)
		Set_TextureIndex(i, i < iCurNumPotion ? 0 : 1);
		

	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerPotion::Render()
{
	return __super::Render();
}

HRESULT CUI2D_PlayerPotion::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerPotion::Ready_PartObjects()
{
	for (_uint i = 0; i <= PART_POTION; i++)
	{
		CUI::DESC PotionDesc{};

		PotionDesc.pParentLevelID = m_pLevelID;
		PotionDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		PotionDesc.fSizeX = 28.5f;
		PotionDesc.fSizeY = 40.f;
		PotionDesc.fX = (g_iWinSizeX * 0.55f) - (25 * i);
		PotionDesc.fY = g_iWinSizeY * 0.25f;
		PotionDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Potion");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &PotionDesc)))
			return E_FAIL;
	}

	CUI::DESC KeyDesc{};

	KeyDesc.pParentLevelID = m_pLevelID;
	KeyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	KeyDesc.fSizeX = 35.f;
	KeyDesc.fSizeY = 35.f;
	KeyDesc.fX = (g_iWinSizeX * 0.59f);
	KeyDesc.fY = g_iWinSizeY * 0.25f;
	KeyDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_PKeyBoard");

	if (FAILED(__super::Add_PartObject(PART_PKEY, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &KeyDesc)))
		return E_FAIL;

	return S_OK;
}

CUI2D_PlayerPotion* CUI2D_PlayerPotion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerPotion* pInstance = new CUI2D_PlayerPotion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerPotion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerPotion::Clone(void* pArg)
{
	CUI2D_PlayerPotion* pInstance = new CUI2D_PlayerPotion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerPotion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerPotion::Free()
{
	__super::Free();

	Safe_Release(m_pInventory);
}
