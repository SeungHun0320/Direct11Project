#include "Model.h"

#include "Mesh.h"
#include "Material.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
    : CComponent(Prototype),
    m_iNumMeshes{ Prototype.m_iNumMeshes },
    m_Meshes{ Prototype.m_Meshes },
    m_iNumMaterials{ Prototype.m_iNumMaterials },
    m_Materials{ Prototype.m_Materials }
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex)
{
    if (iMeshIndex >= m_iNumMeshes)
        return E_FAIL;

    _uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

    if (iMaterialIndex >= m_iNumMaterials)
        return E_FAIL;

    return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eType, iTextureIndex);
}

HRESULT CModel::Initialize_Prototype(const _wstring& strModelFilePath)
{
    ifstream InFile(strModelFilePath, ios::binary);
    if (!InFile.is_open())
        return E_FAIL;

    if (FAILED(Ready_Meshes(InFile)))
        return E_FAIL;

    if (FAILED(Ready_Material(InFile)))
        return E_FAIL;

    InFile.close();

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
    m_Meshes[iMeshIndex]->Bind_Buffers();
    m_Meshes[iMeshIndex]->Render();

    return S_OK;
}

HRESULT CModel::Ready_Meshes(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    m_Meshes.reserve(m_iNumMeshes);

    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        /* 여기다 파일 입출력해서 정보 메쉬정보 담는 구조체 던지면 될듯?? */
        /* 나중에 여기서 애님 / 논애님을 분기해야 할 거 같아*/
        CMesh::MESH tDesc{};
        _InFile.read(reinterpret_cast<char*>(&tDesc.iNumVertices), sizeof(_uint));
        _InFile.read(reinterpret_cast<char*>(&tDesc.iNumIndices), sizeof(_uint));

        tDesc.Vertices.resize(tDesc.iNumVertices);
        tDesc.Indicies.resize(tDesc.iNumIndices);

        _InFile.read(reinterpret_cast<char*>(&tDesc.iMaterialIndex), sizeof(_uint));
        _InFile.read(reinterpret_cast<char*>(tDesc.Vertices.data()), sizeof(VTXMESH) * tDesc.iNumVertices);
        _InFile.read(reinterpret_cast<char*>(tDesc.Indicies.data()), sizeof(_uint) * tDesc.iNumIndices);

        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, &tDesc);
        if (nullptr == pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Material(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<_char*>(&m_iNumMaterials), sizeof(_uint));
    m_Materials.reserve(m_iNumMaterials);

    for (_uint i = 0; i < m_iNumMaterials; i++)
    {
        _uint iNumSRVs{};
        _InFile.read(reinterpret_cast<_char*>(&iNumSRVs), sizeof(_uint));

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

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strModelFilePath)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(strModelFilePath)))
    {
        MSG_BOX("Failed to Created : CModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
    CModel* pInstance = new CModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CModel::Free()
{
    __super::Free();

    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);
    m_Meshes.clear();

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();
}
