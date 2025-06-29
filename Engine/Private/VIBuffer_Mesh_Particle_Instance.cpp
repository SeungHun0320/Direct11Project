#include "VIBuffer_Mesh_Particle_Instance.h"

#include "GameInstance.h"

CVIBuffer_Mesh_Particle_Instance::CVIBuffer_Mesh_Particle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Mesh_Particle_Instance::CVIBuffer_Mesh_Particle_Instance(const CVIBuffer_Mesh_Particle_Instance& Prototype)
	: CVIBuffer_Instance(Prototype)
	, m_pVertexInstances{ Prototype.m_pVertexInstances }
	, m_iMaterialIndex{ Prototype.m_iMaterialIndex }
{

}

HRESULT CVIBuffer_Mesh_Particle_Instance::Initialize_Prototype(const MESH_DESC* pMeshDesc, const DESC* pArg, _fmatrix PreTransformMatrix)
{
	const DESC* pDesc = static_cast<const DESC*>(pArg);

	m_iMaterialIndex = pMeshDesc->iMaterialIndex;

	m_vPivot = pDesc->vPivot;
	m_isLoop = pDesc->isLoop;

	m_iNumIndexPerInstance = pMeshDesc->iNumIndices;
	m_iVertexInstanceStride = sizeof(VTXMESH_PARTICLE_INSTANCE);
	m_iNumInstance = pDesc->iNumInstance;

	m_iNumVertexBuffers = 2;
	m_iNumVertices = pMeshDesc->iNumVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = m_iNumIndexPerInstance;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEXBUFFER
	m_iVertexStride = sizeof(VTXMESH);
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pMeshDesc->Vertices[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pMeshDesc->Vertices[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTangent, &pMeshDesc->Vertices[i].vTangent, sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pMeshDesc->Vertices[i].vTexcoord, sizeof(_float2));
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion 

#pragma region INDEXBUFFER

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);
	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	memcpy(pIndices, pMeshDesc->Indicies.data(), sizeof(_uint) * m_iNumIndices);

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	memcpy(m_pIndices, pIndices, m_iIndexStride * m_iNumIndices);

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion 

#pragma region INSTANCEBUFFER
	m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iVertexInstanceStride;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.StructureByteStride = m_iVertexInstanceStride;
	m_VBInstanceDesc.MiscFlags = 0;

	m_pVertexInstances = new VTXMESH_PARTICLE_INSTANCE[m_iNumInstance];
	ZeroMemory(m_pVertexInstances, sizeof(VTXMESH_PARTICLE_INSTANCE) * m_iNumInstance);

	m_pSpeeds = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeeds, sizeof(_float) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y);
		_float	fSize = m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y);

		m_pVertexInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pVertexInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		m_pVertexInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

		m_pVertexInstances[i].vTranslation = _float4(
			m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
			m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
			m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
			1.f
		);

		m_pVertexInstances[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y),
			0.f
		);
	}

	m_VBInstanceSubResourceData.pSysMem = m_pVertexInstances;

#pragma endregion 

	return S_OK;
}

HRESULT CVIBuffer_Mesh_Particle_Instance::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Mesh_Particle_Instance::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResorce{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResorce);

	VTXMESH_PARTICLE_INSTANCE* pVertices = static_cast<VTXMESH_PARTICLE_INSTANCE*>(SubResorce.pData);

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;
		pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;

		if (true == m_isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			pVertices[i].vTranslation.y = m_pVertexInstances[i].vTranslation.y;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Particle_Instance::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResorce{};

	/* 이 옵션을 줘야 덮어쓰기를 안함 */
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResorce);

	VTXMESH_PARTICLE_INSTANCE* pVertices = static_cast<VTXMESH_PARTICLE_INSTANCE*>(SubResorce.pData);

	_vector vDir = {};

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		vDir = XMVectorSetW(XMVector3Normalize(XMLoadFloat4(&m_pVertexInstances[i].vTranslation)) - XMLoadFloat3(&m_vPivot), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation,
			XMLoadFloat4(&pVertices[i].vTranslation) + (vDir * m_pSpeeds[i] * fTimeDelta));

		if (true == m_isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			/* 원래 위치로 이동 */
			pVertices[i].vTranslation = m_pVertexInstances[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Mesh_Particle_Instance::Shrink(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE SubResorce{};

	/* 이 옵션을 줘야 덮어쓰기를 안함 */
	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResorce);

	VTXMESH_PARTICLE_INSTANCE* pVertices = static_cast<VTXMESH_PARTICLE_INSTANCE*>(SubResorce.pData);

	_vector vDir = {};

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		//vDir = XMVectorSetW(XMVector3Normalize(XMLoadFloat4(&m_pVertexInstances[i].vTranslation)) - XMLoadFloat3(&m_vPivot), 0.f);

		//XMStoreFloat4(&pVertices[i].vTranslation,
		//	XMLoadFloat4(&pVertices[i].vTranslation) + (vDir * m_pSpeeds[i] * fTimeDelta));

		if (true == m_isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			/* 원래 위치로 이동 */
			//pVertices[i].vTranslation = m_pVertexInstances[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Mesh_Particle_Instance* CVIBuffer_Mesh_Particle_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const MESH_DESC* pMeshArg, const DESC* pArg, _fmatrix PreTransformMatrix)
{
	CVIBuffer_Mesh_Particle_Instance* pInstance = new CVIBuffer_Mesh_Particle_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pMeshArg, pArg, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Mesh_Particle_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Mesh_Particle_Instance::Clone(void* pArg)
{
	CVIBuffer_Mesh_Particle_Instance* pInstance = new CVIBuffer_Mesh_Particle_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Mesh_Particle_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Mesh_Particle_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertexInstances);
		Safe_Delete_Array(m_pSpeeds);
	}

}
