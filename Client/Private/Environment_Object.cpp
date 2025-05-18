#include "Environment_Object.h"

#include "GameInstance.h"

CEnvironment_Object::CEnvironment_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CEnvironment_Object::CEnvironment_Object(const CEnvironment_Object& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CEnvironment_Object::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnvironment_Object::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEnvironment_Object::Priority_Update(_float fTimeDelta)
{
}

LIFE CEnvironment_Object::Update(_float fTimeDelta)
{
	if (MOUSE_DOWN(DIMK::LBUTTON))
	{
		_float3		vTmp = m_pModelCom->Compute_PickedPosition_World(m_pTransformCom->Get_WorldMatrix_Float4x4());
		_float3     vDst = m_pModelCom->Compute_PickedPosition_World_Snap(m_pTransformCom->Get_WorldMatrix_Float4x4());
		_float3		vSrc = m_pModelCom->Compute_PickedPosition_Local(m_pTransformCom->Get_WorldMatrix_Inverse());
		_int a = 10;
	}


	return LIFE::NONE;
}

void CEnvironment_Object::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CEnvironment_Object::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEX_TYPE::DIFFUSE, 0)))
			return E_FAIL;
		 
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL; 

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEnvironment_Object::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CEnvironment_Object::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}