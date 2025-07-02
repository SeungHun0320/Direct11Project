#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	/* 디버그용 렌더를 가상함수로 빼주기 위해 만듬. */
	/* 공통된 부모인 컴포넌트를 렌더러에서 그려주기 위해 */
	virtual HRESULT Render() { return S_OK; }

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	_bool						m_isCloned = { false };

public:
	virtual CComponent* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END