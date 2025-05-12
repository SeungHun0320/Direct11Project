#include "Material.h"

#include "Shader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize(const MATERIAL* pMaterial)
{
    for (auto& TexInfo : pMaterial->vecTextures)
    {
        HRESULT         hr = { };
        ID3D11ShaderResourceView* pSRV = { nullptr };

        string strExt = fs::path(TexInfo.strTexturePath).extension().string();

        if (false == strcmp(strExt.c_str(), ".dds"))
        {
            hr = CreateDDSTextureFromFile(m_pDevice, TexInfo.strTexturePath.c_str(), nullptr, &pSRV);
        }
        else if (false == strcmp(strExt.c_str(), ".tga"))
        {
            MSG_BOX("Failed to TGA");
            hr = E_FAIL;
        }
        else
        {
            hr = CreateWICTextureFromFile(m_pDevice, TexInfo.strTexturePath.c_str(), nullptr, &pSRV);
        }

        if (FAILED(hr))
            return E_FAIL;

        m_SRVs[ENUM_CLASS(TexInfo.eTextureType)].push_back(pSRV);
    }

    return S_OK;
}

HRESULT CMaterial::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, TEX_TYPE eType, _uint iTextureIndex)
{
    if (iTextureIndex >= m_SRVs[ENUM_CLASS(eType)].size())
        return E_FAIL;

    return pShader->Bind_SRV(pConstantName, m_SRVs[ENUM_CLASS(eType)][iTextureIndex]);
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const MATERIAL* pMaterial)
{
    CMaterial* pInstance = new CMaterial(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pMaterial)))
    {
        MSG_BOX("Failed to Created : CMaterial");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMaterial::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    for (size_t i = 0; i < ENUM_CLASS(TEX_TYPE::MAX); i++)
    {
        for (auto& pSRV : m_SRVs[i])
            Safe_Release(pSRV);
        m_SRVs[i].clear();
    }
}