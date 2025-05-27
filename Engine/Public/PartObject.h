#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct tagPartObjectDesc : CGameObject::DESC
	{
		const _float4x4* pParentMatrix {nullptr};
	}DESC;
protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _bool Play_Animation(_float fTimeDelta) { return true; };
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend) {};
	virtual void Set_MeshVisible(_uint iIndex, _bool IsVisible) {};
	virtual void Set_TrackPosition(_float fTrackPosition) {};

protected:
	const _float4x4* m_pParentMatrix = { nullptr };
	_float4x4		 m_CombindWorldMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END