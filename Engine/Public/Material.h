#pragma once

#include "Base.h"

/* �޽ð� �̿��ϴ� ���� �������ֱ����� ������ */
/* �ȼ������� ������ �ִ� ���� �����ϱ⤧������ �ؽ��ķ� ��ü. */
/* ���� ������ ����������. */

BEGIN(Engine)

class CMaterial final : public CBase
{
public:
	typedef struct tagTextureInfo
	{
		_wstring strTexturePath;
		TEX_TYPE eTextureType{};

	}TEX_INFO;

	typedef struct tagMaterialDesc
	{	
		vector<TEX_INFO> vecTextures;

	}MATERIAL;

private:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public:
	HRESULT Initialize(const MATERIAL* pAIMaterial);
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, TEX_TYPE eType, _uint iTextureIndex);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*> m_SRVs[ENUM_CLASS(TEX_TYPE::MAX)];

public:
	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const MATERIAL* pMaterial);
	virtual void Free() override;
};

END