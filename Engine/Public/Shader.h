#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	/* 쉐이더의 바인드 버퍼 이후, 렌더 이전에 호출 해 줌*/
	HRESULT Begin(_uint iPassIndex);
	/* 쉐이더에게 매트릭스를 전달해주기 위해서 */
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	/* 쉐이더에게 리소스뷰를 전달해주기 위해서 */
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);

private:
	ID3DX11Effect* m_pEffect = { nullptr };
	vector<ID3D11InputLayout*>		m_InputLayouts;
	_uint							m_iNumPasses = { };


public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END