#include "CheckPoint.h"
#include "GameInstance.h"

#include "Body_CheckPoint.h"

CCheckPoint::CCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{pDevice, pContext}
{
}

CCheckPoint::CCheckPoint(const CCheckPoint& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CCheckPoint::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCheckPoint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCheckPoint::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CCheckPoint::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CCheckPoint::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCheckPoint::Render()
{
	return __super::Render();
}

void CCheckPoint::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	cout << "첵포 개같이 성공\n";
}

HRESULT CCheckPoint::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;
	return S_OK;
}

HRESULT CCheckPoint::Ready_PartObjects()
{
	CBody_CheckPoint::DESC BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_CheckPoint"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CCheckPoint* CCheckPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCheckPoint* pInstance = new CCheckPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCheckPoint::Clone(void* pArg)
{
	CCheckPoint* pInstance = new CCheckPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCheckPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCheckPoint::Free()
{
    __super::Free();
}
