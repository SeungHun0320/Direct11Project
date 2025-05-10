#pragma once
#include "MeshExporter_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(MeshExporter)

class CMeshExporterApp final : public CBase
{
private:
	CMeshExporterApp();
	virtual ~CMeshExporterApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance* m_pGameInstance = { };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	HRESULT Start_Level(LEVEL eStartLevel);

public:
	static CMeshExporterApp* Create();
	virtual void Free() override;

};

END