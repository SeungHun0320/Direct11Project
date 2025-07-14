#include "BaseActor.h"
#include "GameInstance.h"

#include "PartObject.h"

CBaseActor::CBaseActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{pDevice, pContext}
{
}

CBaseActor::CBaseActor(const CBaseActor& Prototype)
	: CContainerObject(Prototype)
{
}

HRESULT CBaseActor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseActor::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CBaseActor::Priority_Update(_float fTimeDelta)
{
	Update_InvicibleTime(fTimeDelta);

	__super::Priority_Update(fTimeDelta);
}

LIFE CBaseActor::Update(_float fTimeDelta)
{
	if (LEVEL::TOOLS != m_eLevelID && nullptr != m_pNavigationCom)
	{
		m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(Engine::STATE::POSITION)));
	}

	return __super::Update(fTimeDelta);
}

void CBaseActor::Late_Update(_float fTimeDelta)
{
	m_isBlocked = false;
	__super::Late_Update(fTimeDelta);
}

HRESULT CBaseActor::Render()
{
	return S_OK;
}

void CBaseActor::Update_InvicibleTime(_float fTimeDelta)
{
	if (!m_isInvincible)
		return;

	m_fInvicibleTime -= fTimeDelta;
	if (0 >= m_fInvicibleTime)
	{
		m_fInvicibleTime = 0.f;
		m_isInvincible = false;
	}
}

void CBaseActor::Start_Invicible(_float fDuration)
{
	m_isInvincible = true;
	m_fInvicibleTime = fDuration;
}

HRESULT CBaseActor::Ready_Components(void* pArg)
{
	CNavigation::DESC tDesc{};
	XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (LEVEL::TOOLS != m_eLevelID)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void CBaseActor::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
}
