#include "UI3D_LockOn.h"
#include "GameInstance.h"

#include "UI.h"

CUI3D_LockOn::CUI3D_LockOn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI3D_LockOn::CUI3D_LockOn(const CUI3D_LockOn& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI3D_LockOn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI3D_LockOn::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentIsTargeted = pDesc->pParentIsTargeted;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CUI3D_LockOn::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	// 1. 중심 위치 구하기 (ParentMatrix 기준)
	_matrix matParent = XMLoadFloat4x4(m_pParentMatrix);
	_vector vCenter = XMVector3TransformCoord(XMVectorZero(), matParent);

	_matrix ViewProjMat =
		m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW) *
		m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

	// 3. 회전 각도 누적
	m_fHexAngleAcc += fTimeDelta * XMConvertToRadians(180.f); // 회전 속도

	if (m_fHexAngleAcc > XM_2PI)
		m_fHexAngleAcc -= XM_2PI;

	const _float fRadius = 85.f;
	const _uint iHexCount = PART_HEX - PART_HEX_START + 1;


	CUI* pRing = static_cast<CUI*>(m_PartObjects[PART_RING]);
	_vector vRingScreenPos = pRing->Get_State(STATE::POSITION);

	_float fRingX = XMVectorGetX(vRingScreenPos);
	_float fRingY = XMVectorGetY(vRingScreenPos);

	for (_uint i = 0; i < iHexCount; ++i)
	{
		_float fAngleOffset = XM_2PI / iHexCount * i;
		_float fFinalAngle = m_fHexAngleAcc + fAngleOffset;

		// 순수 2D 회전 (스크린 상)
		_float fOffsetX = cosf(fFinalAngle) * fRadius;
		_float fOffsetY = sinf(fFinalAngle) * fRadius;

		_float fFinalX = fRingX + fOffsetX;
		_float fFinalY = fRingY + fOffsetY;

		_float4 vHexPos = { fFinalX, fFinalY, 0.f, 1.f };
		static_cast<CUI*>(m_PartObjects[PART_HEX_START + i])->Set_State(STATE::POSITION, XMLoadFloat4(&vHexPos));
	}

}

LIFE CUI3D_LockOn::Update(_float fTimeDelta)
{
	if (!(*m_pParentIsTargeted))
		return LIFE::NONE;

	return __super::Update(fTimeDelta);
}

void CUI3D_LockOn::Late_Update(_float fTimeDelta)
{
	if (!(*m_pParentIsTargeted))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CUI3D_LockOn::Render()
{
	return S_OK;
}

void CUI3D_LockOn::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

void CUI3D_LockOn::Set_TextureIndex(_uint iPart, _uint iTextureIdx)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_TextureIndex(iTextureIdx);
}

HRESULT CUI3D_LockOn::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI3D_LockOn::Ready_PartObjects()
{
	CUI::DESC RingDesc{};

	RingDesc.pParentLevelID = m_pLevelID;
	RingDesc.pParentMatrix = m_pParentMatrix;
	RingDesc.fSizeX = 180.f;
	RingDesc.fSizeY = 180.f;
	RingDesc.fOffset = 1.f;
	RingDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_LockOnCircle");

	if (FAILED(__super::Add_PartObject(PART_RING, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D"), &RingDesc)))
		return E_FAIL;

	for (_uint i = PART_HEX_START; i <= PART_HEX; i++)
	{
		CUI::DESC HexDesc{};

		HexDesc.pParentLevelID = m_pLevelID;
		HexDesc.pParentMatrix = m_pParentMatrix;
		HexDesc.fSizeX = 50.f;
		HexDesc.fSizeY = 50.f;
		HexDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_LockOnHex");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D"), &HexDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CUI3D_LockOn* CUI3D_LockOn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI3D_LockOn* pInstance = new CUI3D_LockOn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI3D_LockOn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI3D_LockOn::Clone(void* pArg)
{
	CUI3D_LockOn* pInstance = new CUI3D_LockOn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI3D_LockOn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI3D_LockOn::Free()
{
	__super::Free();
}
