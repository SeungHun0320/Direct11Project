#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
//#include "player.h"
//#include "Effect.h"
//#include "Sky.h"

/* 몬스터들 */
#include "SpiderTank.h"

/* 지형(통맵) */
#include "Courtyard.h"
#include "Arena.h"
#include "Main.h"
#include "Shop.h"

/* 지형(각종 환경오브젝트 / 상호작용 오브젝트 ) */
#include "Grass.h"
#include "Bush.h"
#include "CheckPoint.h"
#include "Chest.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;	

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	if(FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::COURTYARD:
		hr = Loading_For_Courtyard();
		break;

	case LEVEL::MAIN:
		hr = Loading_For_Main();
		break;

	case LEVEL::ARENA:
		hr = Loading_For_Arena();
		break;

	case LEVEL::SHOP:
		hr = Loading_For_Shop();
		break;

	case LEVEL::TOOLS:
		hr = Loading_For_Tools();
		break;

	default:
		hr = E_FAIL;
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return E_FAIL;


	return S_OK;
}


HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/Logo.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	m_pGameInstance->LoadSound("../Bin/Resources/Sounds/BGM", false, true);

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Courtyard()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resources/Textures/Player/Player0.png"), 1))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Sky */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
	//	return E_FAIL;


	///* For.Prototype_Component_Texture_Explosion */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
	//	CTexture::Create(m_pGraphic_Device, CTexture::TYPE_2D, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;*/

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototpye_Component_Model_SpiderTank*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_SpiderTank"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Spidertank/Spidertank.Model"), PreTransformMatrix))))
		return E_FAIL;

	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_SpiderTank"),
	//	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Study/Fiona/Fiona.Model"), PreTransformMatrix))))
	//	return E_FAIL;

	/*For.Prototpye_Component_Model_Courtyard*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_Courtyard"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Courtyard/Courtyard.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Bush*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_Bush"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Bush/bush.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Grass*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_Grass"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Grass/grass base.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_CheckPoint*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototpye_Component_Model_CheckPoint"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/TUNIC_Checkpoint/TUNIC_Checkpoint.Model"), PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Cube */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderTank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_SpiderTank"),
		CSpiderTank::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Courtyard */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_Courtyard"),
		CCourtyard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototpye_GameObject_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::COURTYARD), TEXT("Prototype_GameObject_CheckPoint"),
		CCheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
	//	CPlayer::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Sky */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Main()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Arena()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Shop()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tools()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototpye_Component_Model_Courtyard*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Courtyard"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Courtyard/Courtyard.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Arena*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Arena"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Arena/Arena.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Main*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Main"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Main/Main.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Shop*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Shop"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Shop/Shop.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Bush*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Bush"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Bush/bush.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Grass*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Grass"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Grass/grass base.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_CheckPoint*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_CheckPoint"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/TUNIC_Checkpoint/TUNIC_Checkpoint.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototpye_Component_Model_Chest*/
 	PreTransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototpye_Component_Model_Chest"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Environment_Objects/Chest/Chest.Model"), PreTransformMatrix))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Courtyard */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Courtyard"),
		CCourtyard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Arena */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Arena"),
		CArena::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Main */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Main"),
		CMain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Shop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Shop"),
		CShop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototpye_GameOjbect_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_CheckPoint"),
		CCheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototpye_GameOjbect_Chest */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_GameObject_Chest"),
		CChest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CoUninitialize();

	DeleteCriticalSection(&m_CriticalSection);
}
