#include "Model.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

#include "Transform.h"


CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
    , m_pGameInstance{ CGameInstance::Get_Instance() }
{
    Safe_AddRef(m_pGameInstance);
}

CModel::CModel(const CModel& Prototype)
    : CComponent(Prototype)
    , m_iNumMeshes{ Prototype.m_iNumMeshes }
    , m_Meshes{ Prototype.m_Meshes }
    , m_iNumMaterials{ Prototype.m_iNumMaterials }
    , m_Materials{ Prototype.m_Materials }
    , m_eType{ Prototype.m_eType }
    , m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
    , m_iNumAnimations{ Prototype.m_iNumAnimations }
    , m_pGameInstance{ CGameInstance::Get_Instance() }
    , m_iRootBoneIndex { Prototype.m_iRootBoneIndex }
    , m_vPreRootPosition {Prototype.m_vPreRootPosition }
{
    Safe_AddRef(m_pGameInstance);

    for (auto& pPrototypeBone : Prototype.m_Bones)
        m_Bones.push_back(pPrototypeBone->Clone());

    for (auto& pMesh : m_Meshes)
        Safe_AddRef(pMesh);

    for (auto& pMaterial : m_Materials)
        Safe_AddRef(pMaterial);

    for (auto& pPrototypeAnim : Prototype.m_Animations)
        m_Animations.push_back(pPrototypeAnim->Clone());
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

HRESULT CModel::Bind_Bone_Matrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
    return m_Meshes[iMeshIndex]->Bind_Bone_Matrices(pShader, pConstantName, m_Bones);
}

HRESULT CModel::Initialize_Prototype(MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix)
{
    ifstream InFile(strModelFilePath, ios::binary);
    if (!InFile.is_open())
        return E_FAIL;

    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
    m_eType = eType;

    if (m_eType == MODEL::ANIM)
    {
        if (FAILED(Ready_Bones(InFile)))
            return E_FAIL;

        if (FAILED(Ready_Anim_Meshes(InFile)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(Ready_NonAnim_Meshes(InFile)))
            return E_FAIL;
    }

    if (FAILED(Ready_Material(InFile)))
        return E_FAIL;

    if (m_eType == MODEL::ANIM)
    {
        if (FAILED(Ready_Animations(InFile)))
            return E_FAIL;
    }

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

_matrix CModel::MatrixLerp(_fmatrix CurrentTransfomaionMatrix, _cmatrix NextTransformMatrix, _float fRatio)
{
    _vector			vScale{}, vRotation{}, vPosition{};

    _vector			vSourScale{}, vDestScale{};
    _vector			vSourRotation{}, vDestRotation{};
    _vector			vSourTranslation{}, vDestTranslation{};

    XMMatrixDecompose(&vSourScale, &vSourRotation, &vSourTranslation, CurrentTransfomaionMatrix);
    XMMatrixDecompose(&vDestScale, &vDestRotation, &vDestTranslation, NextTransformMatrix);

    vScale    = XMVectorLerp(vSourScale, vDestScale, fRatio);
    vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
    vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);

    return  XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
}

_bool CModel::Play_Animation(_float fTimeDelta)
{
    _bool		isFinished = { false };

    if (m_isBlended)
    {
        Animation_Blend(fTimeDelta);
    }
    else
    {
        /* 1. 현재 애니메이션에 맞는 뼈의 상태를 읽어와서 뼈의 TrnasformationMatrix를 갱신해준다. */
        isFinished = m_Animations[m_iCurrentAnimIndex]->Update_Bones(fTimeDelta, m_Bones, m_isLoop);
    }

    /* 2. 전체 뼈를 순회하면서 뼈들의 ColmbinedTransformationMatix를 부모에서부터 자식으로 갱신해준다. */
    for (auto& pBone : m_Bones)
    {
        pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
    }

    return isFinished;
}

void CModel::Animation_Blend(_float fTimeDelta)
{
    /* 뼈 갯수만큼 있어야함 */
    vector<_matrix> CurTransfomationMatrices(m_Bones.size());
    vector<_matrix> NextTransfomationMatrices(m_Bones.size());

    for (_uint i = 0; i < m_Bones.size(); i++)
    {
        CurTransfomationMatrices[i]  = XMLoadFloat4x4(m_Bones[i]->Get_TransformationMatrix());
        NextTransfomationMatrices[i] = XMLoadFloat4x4(m_Bones[i]->Get_TransformationMatrix());
    }

    m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(CurTransfomationMatrices, 0.f);
    m_Animations[m_iNextAnimIndex]->Update_First_TransformationMatrices(NextTransfomationMatrices);

    m_fBlendTickPerSecond += fTimeDelta;

    _float fRatio = m_fBlendTickPerSecond / m_fBlendDuration;

    if (1.f <= fRatio)
    {
        fRatio = 1.f;
        m_Animations[m_iCurrentAnimIndex]->Reset();
        m_iCurrentAnimIndex = m_iNextAnimIndex;
        m_fBlendTickPerSecond = 0.f;
        m_isBlended = false;

    }

    /* 구한 트랜스포메이션 행렬끼리 보간 한 값을 뼈에 세팅 */
    for (_uint i = 0; i < m_Bones.size(); i++)
        m_Bones[i]->Set_TransformationMatrix(MatrixLerp(CurTransfomationMatrices[i], NextTransfomationMatrices[i], fRatio));
}

_vector CModel::Compute_RootPosition()
{
    _vector			vScale{}, vRotation{}, vPosition{};
 
    XMMatrixDecompose(&vScale, &vRotation, &vPosition, XMLoadFloat4x4(m_Bones[m_iRootBoneIndex]->Get_CombinedTransformationMatrix()));

    _vector fDelta = vPosition - XMLoadFloat3(&m_vPreRootPosition);
    XMStoreFloat3(&m_vPreRootPosition, vPosition);

    return fDelta;
}

void CModel::Update_RootPosition()
{
    _vector			vScale{}, vRotation{}, vPosition{};
    XMMatrixDecompose(&vScale, &vRotation, &vPosition, XMLoadFloat4x4(m_Bones[m_iRootBoneIndex]->Get_TransformationMatrix()));

    m_Bones[m_iRootBoneIndex]->Set_TransformationMatrix(XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, XMVectorZero()));

}

_float3 CModel::Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse)
{
    _float3 vResultPos{};
    _float fMinDist = FLT_MAX;

    for (auto& pMesh : m_Meshes)
    {
        _float fDist{};
        _float3 vPickedPos = pMesh->Compute_PickedPosition_LocalEx(WorldMatrixInverse, fDist);

        if (fDist < fMinDist)
        {
            vResultPos = vPickedPos;
            fMinDist = fDist;
        }

    }

    return vResultPos;
}

_bool CModel::Compute_PickedPosition_World(const _float4x4* pWorldMatrix, _float3& fOutPos, _float& fOutDist)
{
    _float3 vResultPos{};
    _float fMinDist = FLT_MAX;
    _bool bHit = false;

    const _float3 vRayOrigin = m_pGameInstance->Get_MousePos();

    for (auto& pMesh : m_Meshes)
    {
        _float fDist = FLT_MAX;
        _float3 vPickedPos = pMesh->Compute_PickedPosition_WorldEx(pWorldMatrix, fDist);

        if (fDist < fMinDist)
        {
            vResultPos = vPickedPos;
            fMinDist = fDist;
            bHit = true;
        }


    }

    if (bHit)
    {
        fOutDist = fMinDist;
        fOutPos = vResultPos;
    }

    return bHit;
}

HRESULT CModel::Ready_Bones(ifstream& _InFile)
{
    _uint iNumBones{};
    _InFile.read(reinterpret_cast<_char*>(&iNumBones), sizeof(_uint));

    for (_uint i = 0; i < iNumBones; i++)
    {
        CBone::BONE tDesc{};
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iParentBoneIndex), sizeof(_int));
        _uint istrLen{};
        _InFile.read(reinterpret_cast<_char*>(&istrLen), sizeof(_uint));
        tDesc.strName.resize(istrLen);
        _InFile.read(reinterpret_cast<_char*>(tDesc.strName.data()), sizeof(_char) * istrLen);
        _InFile.read(reinterpret_cast<_char*>(&tDesc.TransformationMatrix), sizeof(_float4x4));

        CBone* pBone = CBone::Create(&tDesc);
        if (nullptr == pBone)
            return E_FAIL;

        m_Bones.push_back(pBone);
    }

    for (_uint i = 0; i < m_Bones.size(); i++)
    {
        if (m_Bones[i]->Compare_Name("root"))
        {
            m_Bones[i]->Set_ParentBoneIndex(2);
            break;
        }
    }

    return S_OK;
}

HRESULT CModel::Ready_NonAnim_Meshes(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    m_Meshes.reserve(m_iNumMeshes);

    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        /* 여기다 파일 입출력해서 정보 메쉬정보 담는 구조체 던지면 될듯?? */
        CMesh::NONANIMMESH tDesc{};
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumVertices), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumIndices), sizeof(_uint));

        tDesc.Vertices.resize(tDesc.iNumVertices);
        tDesc.Indicies.resize(tDesc.iNumIndices);

        _InFile.read(reinterpret_cast<_char*>(&tDesc.iMaterialIndex), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(tDesc.Vertices.data()), sizeof(VTXMESH) * tDesc.iNumVertices);
        _InFile.read(reinterpret_cast<_char*>(tDesc.Indicies.data()), sizeof(_uint) * tDesc.iNumIndices);

        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, &tDesc, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;

        m_Meshes.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Anim_Meshes(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    m_Meshes.reserve(m_iNumMeshes);

    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        /* 여기다 파일 입출력해서 정보 메쉬정보 담는 구조체 던지면 될듯?? */
        CMesh::ANIMMESH tDesc{};

        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumVertices), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumIndices), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumBoneIndices), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumOffsetMatrices), sizeof(_uint));

        tDesc.Vertices.resize(tDesc.iNumVertices);
        tDesc.Indicies.resize(tDesc.iNumIndices);
        tDesc.BoneIndices.resize(tDesc.iNumBoneIndices);
        tDesc.OffsetMatrices.resize(tDesc.iNumOffsetMatrices);
       
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iMaterialIndex), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(tDesc.Vertices.data()), sizeof(VTXANIMMESH) * tDesc.iNumVertices);
        _InFile.read(reinterpret_cast<_char*>(tDesc.Indicies.data()), sizeof(_uint) * tDesc.iNumIndices);

        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumBones), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(tDesc.BoneIndices.data()), sizeof(_uint) * tDesc.iNumBoneIndices);
        _InFile.read(reinterpret_cast<_char*>(tDesc.OffsetMatrices.data()), sizeof(_float4x4) * tDesc.iNumOffsetMatrices);


        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, &tDesc, XMLoadFloat4x4(&m_PreTransformMatrix));
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

HRESULT CModel::Ready_Animations(ifstream& _InFile)
{
    /* 이쪽에서 파일입출력해서 갯수를 먼저 받아오기  */
    _InFile.read(reinterpret_cast<_char*>(&m_iNumAnimations), sizeof(_uint));
    m_Animations.reserve(m_iNumAnimations);

    for (_uint i = 0; i < m_iNumAnimations; i++)
    {
        CAnimation::ANIMATION tDesc{};
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumChannels), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.fTickPerSecond), sizeof(_float));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.fDuration), sizeof(_float));

        CAnimation* pAnimation = CAnimation::Create(&tDesc, _InFile, m_Bones);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType, strModelFilePath, PreTransformMatrix)))
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

    Safe_Release(m_pGameInstance);

    for (auto& pBone : m_Bones)
        Safe_Release(pBone);
    m_Bones.clear();

    for (auto& pMesh : m_Meshes)
        Safe_Release(pMesh);
    m_Meshes.clear();

    for (auto& pMaterial : m_Materials)
        Safe_Release(pMaterial);
    m_Materials.clear();

    for (auto& pAnimation : m_Animations)
        Safe_Release(pAnimation);
    m_Animations.clear();

    m_Materials.clear();
}
