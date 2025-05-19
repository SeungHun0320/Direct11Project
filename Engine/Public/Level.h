#pragma once

#include "Base.h"

/* Ŭ�� �����ڰ� ���� ����Ŭ�������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();	
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Change_Level(_uint iLevelIndex) {
		m_iNextLevel = iLevelIndex;
	}

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

protected:
	_uint					m_iNextLevel{};

protected:
	virtual HRESULT Load_Map(const _wstring& strMapFileTag) { return S_OK; };

public:	
	virtual void Free() override;

};

END