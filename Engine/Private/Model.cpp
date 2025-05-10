#include "Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
    : CComponent(Prototype),
    m_iNumMeshes{ Prototype.m_iNumMeshes },
    m_Meshes{ Prototype.m_Meshes }
{
    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);
}

HRESULT CModel::Initialize_Prototype(const _wstring& strModelFilePath)
{
    if (FAILED(Ready_Meshes(strModelFilePath)))
        return E_FAIL;

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render()
{
    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        m_Meshes[i]->Bind_Buffers();
        m_Meshes[i]->Render();
    }

    return S_OK;
}

HRESULT CModel::Ready_Meshes(const _wstring& strModelFilePath)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    m_iNumMeshes = 0;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        /* 여기다 파일 입출력해서 정보 메쉬정보 담는 구조체 던지면 될듯?? */

        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext);
        if (nullptr == pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
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
}
