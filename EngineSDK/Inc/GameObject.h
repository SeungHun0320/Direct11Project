#pragma once

#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::DESC
	{
		_wstring strName;
	}DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	CComponent* Get_Component(const _wstring& strComponentTag);

	const _wstring& Get_Name() {
		return m_strName;
	}

	_bool Get_Dead() {
		return m_bDead;
	}

	void Set_Dead(_bool bDead) {
		m_bDead = bDead;
	}
		

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance* m_pGameInstance = { nullptr };

protected:
	map<const _wstring, class CComponent*>	m_Components;
	class CTransform*						m_pTransformCom = { nullptr };

protected:
	_bool m_bDead = { false };
	_wstring m_strName;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END