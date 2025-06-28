#include "Model_Instance.h"

#include "Material.h"

CModel_Instance::CModel_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{pDevice, pContext}
{
}

CModel_Instance::CModel_Instance(const CModel_Instance& Prototype)
	:CComponent(Prototype)
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_MeshInstances{ Prototype.m_MeshInstances }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
{
    for (auto& pMeshInstance : m_MeshInstances)
        Safe_AddRef(pMeshInstance);

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);
}

HRESULT CModel_Instance::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    _uint		iMaterialIndex = m_MeshInstances[iMeshIndex]->Get_MaterialIndex();

    if (iMaterialIndex >= m_iNumMaterials)
        return E_FAIL;

    return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eType, iTextureIndex);
}

HRESULT CModel_Instance::Initialize_Prototype(const _wstring& strModelFilePath, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix)
{
    ifstream InFile(strModelFilePath, ios::binary);
    if (!InFile.is_open())
        return E_FAIL;

    if (FAILED(Ready_Meshes(InFile, pArg, PreTransformMatrix)))
        return E_FAIL;

    if (FAILED(Ready_Material(InFile)))
        return E_FAIL;

    InFile.close();

    return S_OK;
}

HRESULT CModel_Instance::Initialize(void* pArg)
{
    for (auto& pMeshInstance : m_MeshInstances)
        pMeshInstance->Initialize(pArg);

    return S_OK;
}

HRESULT CModel_Instance::Render(_uint iMeshIndex)
{
    m_MeshInstances[iMeshIndex]->Bind_Buffers();
    m_MeshInstances[iMeshIndex]->Render();

    return S_OK;
}

HRESULT CModel_Instance::Ready_Meshes(ifstream& _InFile, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    m_MeshInstances.reserve(m_iNumMeshes);

    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        /* 여기다 파일 입출력해서 정보 메쉬정보 담는 구조체 던지면 될듯?? */
        CVIBuffer_Mesh_Instance::MESH_DESC tDesc{};
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumVertices), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumIndices), sizeof(_uint));

        tDesc.Vertices.resize(tDesc.iNumVertices);
        tDesc.Indicies.resize(tDesc.iNumIndices);

        _InFile.read(reinterpret_cast<_char*>(&tDesc.iMaterialIndex), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(tDesc.Vertices.data()), sizeof(VTXMESH) * tDesc.iNumVertices);
        _InFile.read(reinterpret_cast<_char*>(tDesc.Indicies.data()), sizeof(_uint) * tDesc.iNumIndices);

        CVIBuffer_Mesh_Instance* pMeshInstance = CVIBuffer_Mesh_Instance::Create(m_pDevice, m_pContext, &tDesc, pArg, PreTransformMatrix);
        if (nullptr == pMeshInstance)
            return E_FAIL;

        m_MeshInstances.push_back(pMeshInstance);
    }

    return S_OK;
}

HRESULT CModel_Instance::Ready_Material(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<_char*>(&m_iNumMaterials), sizeof(_uint));
    m_Materials.reserve(m_iNumMaterials);

    for (_uint i = 0; i < m_iNumMaterials; i++)
    {
        _uint iNumSRVs{};
        _InFile.read(reinterpret_cast<_char*>(&iNumSRVs), sizeof(_uint));

        //if (0 == iNumSRVs)
        //    continue;

        CMaterial::MATERIAL tDesc{};

        for (_uint j = 0; j < iNumSRVs; j++)
        {
            CMaterial::TEX_INFO tTexInfo{};

            _InFile.read(reinterpret_cast<_char*>(&tTexInfo.eTextureType), sizeof(_uint));
            _uint istrLen{};
            _InFile.read(reinterpret_cast<_char*>(&istrLen), sizeof(_uint));
            tTexInfo.strTexturePath.resize(istrLen);
            _InFile.read(reinterpret_cast<_char*>(tTexInfo.strTexturePath.data()), sizeof(_tchar) * istrLen);

            tDesc.vecTextures.push_back(tTexInfo);
        }

        CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, &tDesc);
        if (nullptr == pMaterial)
            return E_FAIL;

        m_Materials.push_back(pMaterial);
    }

    return S_OK;
}

CModel_Instance* CModel_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strModelFilePath, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix)
{
    CModel_Instance* pInstance = new CModel_Instance(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(strModelFilePath, pArg, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : CModel_Instance");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CModel_Instance::Clone(void* pArg)
{
    CModel_Instance* pInstance = new CModel_Instance(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CModel_Instance");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CModel_Instance::Free()
{
	__super::Free();
    
    for (auto& pMeshInstance : m_MeshInstances)
        Safe_Release(pMeshInstance);
    m_MeshInstances.clear();

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();
}
