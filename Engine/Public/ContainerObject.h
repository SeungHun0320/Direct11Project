#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CContainerObject abstract : public CGameObject
{ 
public:
	typedef struct tagContainerObjectDesc : CGameObject::DESC
	{
		_uint		iNumPartObjects{};
	}DESC;
protected:
	CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainerObject(const CContainerObject& Prototype);
	virtual ~CContainerObject() = default;

public: /* ¸ÊÅø ¹Ó´Ù. */
	const _uint Get_NumPartObjects() const {
		return m_iNumPartObjects;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_uint							m_iNumPartObjects = {};
	vector<class CPartObject*>		m_PartObjects;

protected:
	HRESULT Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END