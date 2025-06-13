#include "UI2D_PlayerPotion.h"
#include "GameInstance.h"

#include "UI_Potion.h"

CUI2D_PlayerPotion::CUI2D_PlayerPotion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI2DContainerPart{ pDevice, pContext }
{
}

CUI2D_PlayerPotion::CUI2D_PlayerPotion(const CUI2D_PlayerPotion& Prototype)
	: CUI2DContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerPotion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerPotion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(500, 100.f, 0.f, 1.f));

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
	for (_uint i = 0; i < PART_END; i++)
	{
		CUI_Potion::DESC PotionDesc{};

		PotionDesc.eLevelID = m_eLevelID;
		PotionDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		PotionDesc.fSizeX = 114.f;
		PotionDesc.fSizeY = 160.f;
		PotionDesc.fX = (g_iWinSizeX * 0.5f) + (15 * i);
		PotionDesc.fY = g_iWinSizeY * 0.25f;

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Potion"), &PotionDesc)))
			return E_FAIL;
	}

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
}
