#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg);

private:
	_uint			m_iNumVerticesX = {};
	_uint			m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumVerticesX, _uint iNumVerticesZ);
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END