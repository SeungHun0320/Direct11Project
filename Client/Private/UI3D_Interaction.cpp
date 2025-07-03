#include "UI3D_Interaction.h"
#include "GameInstance.h"

#include "UI.h"

CUI3D_Interaction::CUI3D_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI3D_Interaction::CUI3D_Interaction(const CUI3D_Interaction& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI3D_Interaction::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI3D_Interaction::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentIsCollisioned = pDesc->pParentIsCollisioned;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI3D_Interaction::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI3D_Interaction::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CUI3D_Interaction::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParentIsCollisioned))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CUI3D_Interaction::Render()
{
	return S_OK;
}

HRESULT CUI3D_Interaction::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI3D_Interaction::Ready_PartObjects()
{
	CUI::DESC SpaceDesc{};

	SpaceDesc.pParentLevelID = m_pLevelID;
	SpaceDesc.pParentMatrix = m_pParentMatrix;
	SpaceDesc.fSizeX = 42.6f;
	SpaceDesc.fSizeY = 23.6f;
	SpaceDesc.fOffset = 2.5f;
	SpaceDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_SpaceKeyBoard");

	if (FAILED(__super::Add_PartObject(PART_SPACE, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D"), &SpaceDesc)))
		return E_FAIL;

	return S_OK;
}

CUI3D_Interaction* CUI3D_Interaction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI3D_Interaction* pInstance = new CUI3D_Interaction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI3D_Interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI3D_Interaction::Clone(void* pArg)
{
	CUI3D_Interaction* pInstance = new CUI3D_Interaction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI3D_Interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI3D_Interaction::Free()
{
	__super::Free();
}
