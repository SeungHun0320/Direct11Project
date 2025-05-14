#include "MeshExporterApp.h"

#include "GameInstance.h"
#include "Level_MeshExporter.h"

CMeshExporterApp::CMeshExporterApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMeshExporterApp::Initialize()
{
#ifdef _CONSOL

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif

	ENGINE_DESC			EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInst = g_hInst;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = static_cast<_uint>(LEVEL::LEVEL_END);


	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::MESH_EXPORTER)))
		return E_FAIL;

	return S_OK;
}

void CMeshExporterApp::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_ESCAPE))
		PostQuitMessage(0);

	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMeshExporterApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

	m_pGameInstance->End_Draw();

	return S_OK;
}

HRESULT CMeshExporterApp::Start_Level(LEVEL eStartLevel)
{
	if (FAILED(m_pGameInstance->Change_Level(ENUM_CLASS(eStartLevel), CLevel_MeshExporter::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMeshExporterApp* CMeshExporterApp::Create()
{
	CMeshExporterApp* pInstance = new CMeshExporterApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMeshExporterApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshExporterApp::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);
}
