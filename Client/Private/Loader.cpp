#include "Loader.h"

#include "GameInstance.h"

#include "Camera_TPS.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
//#include "Effect.h"
#include "Sky.h"

/* 몬스터들 */
#include "SpiderTank.h"
#include "Body_SpiderTank.h"
// 총알 추가
#include "SpiderTank_Orb.h"
#include "SpiderTank_Bullet.h"
#include "SpiderTank_Lager.h"

#include "UI2D_BossHpBar.h"

#include "Blob.h"
#include "Body_Blob.h"

#include "Wizard_Candleabra.h"
#include "Body_WizardCandleabra.h"
#include "Part_WizardCandleabra.h"

#include "Wizard_Support.h"
#include "Body_WizardSupport.h"
#include "Part_WizardStaff.h"
//총알 추가
#include "Wizard_Support_AOE.h"

#include "Wizard_Sword.h"
#include "Body_WizardSword.h"
#include "Part_WizardSword.h"

#include "UI3D_MobHPBar.h"
#include "UI3D_LockOn.h"

/* 지형(통맵) */
#include "Courtyard.h"
#include "Arena.h"
#include "Main.h"
#include "Shop.h"

/* 지형(각종 환경오브젝트 / 상호작용 오브젝트 ) */
#include "Body_Grass.h"
#include "Grass.h"

#include "Body_Bush.h"
#include "Bush.h"

#include "Body_CheckPoint.h"
#include "CheckPoint.h"

#include "Body_Chest.h"
#include "Chest.h"

/* 아이템들 */
#include "Item_Potion.h"

#include "UI3D_Interaction.h"


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
		hr = Loading_For_Logo(LEVEL::LOGO);
		break;

	case LEVEL::COURTYARD:
		hr = Loading_For_Courtyard(LEVEL::COURTYARD);
		break;

	case LEVEL::ARENA:
		hr = Loading_For_Arena(LEVEL::ARENA);
		break;

	case LEVEL::SHOP:
		hr = Loading_For_Shop(LEVEL::SHOP);
		break;

	case LEVEL::TOOLS:
		hr = Loading_For_Tools(LEVEL::TOOLS);
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


HRESULT CLoader::Loading_For_Logo(LEVEL eLevelID)
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/Logo.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Courtyard(LEVEL eLevelID)
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Enemy_HexBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Enemy_HexBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster/Enemy_HexBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Enemy_HPBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Enemy_HPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster/Enemy_HpBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnCircle*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnCircle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnHex*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnHex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnHex.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_SpaceKeyBoard*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_SpaceKeyBoard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/SpaceKeyBoard.png")))))
		return E_FAIL;


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
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Courtyard*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Courtyard"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Courtyard/Courtyard.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Bush*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Bush"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Bush/bush.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Grass*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Grass"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Grass/grass base.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_CheckPoint*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_CheckPoint"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/TUNIC_Checkpoint/TUNIC_Checkpoint.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Chest*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Chest"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Environment_Objects/Chest/Chest.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Potion*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Potion"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Item/Potion/Potion.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Blob*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Blob"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Blob/Blob.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Wizard_Support*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
 	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Support"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Support/Wizard_Support.Model"), PreTransformMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Wizard_Candleabra*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Candleabra"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Candleabra/Wizard_Candleabra.Model"), PreTransformMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Wizard_Sword*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Sword"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Sword/Wizard_Sword.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardCandleabra*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardCandleabra"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Candleabra.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardSword*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardSword"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Sword.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardStaff*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardStaff"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Staff.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_exclamation_mark*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_exclamation_mark"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Effect/exclamation_mark.Model"), PreTransformMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));
	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationData/Courtyard.Nav")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_TPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Courtyard */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Courtyard"),
		CCourtyard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Bush"),
		CBody_Bush::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Grass"),
		CBody_Grass::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_CheckPoint"),
		CBody_CheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_CheckPoint"),
		CCheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Chest */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Chest"),
		CBody_Chest::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Chest */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Chest"),
		CChest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Potion */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Potion"),
		CItem_Potion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Blob */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Blob"),
		CBody_Blob::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Blob */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Blob"),
		CBlob::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Part_WizardCandleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardCandleabra"),
		CPart_WizardCandleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Part_WizardSword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardSword"),
		CPart_WizardSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Support_AOE*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Support_AOE"),
		CWizard_Support_AOE::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Body_Candleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Candleabra"),
		CBody_WizardCandleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Candleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Candleabra"),
		CWizard_Candleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Part_WizardStaff */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardStaff"),
		CPart_WizardStaff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Body_Support */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Support"),
		CBody_WizardSupport::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Support */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Support"),
		CWizard_Support::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Sword"),
		CBody_WizardSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Sword"),
		CWizard_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_MobHPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_MobHPBar"),
		CUI3D_MobHPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_LockOn */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_LockOn"),
		CUI3D_LockOn::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"),
		CUI3D_Interaction::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Arena(LEVEL eLevelID)
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/*For.Prototype_Component_Texture_Sky*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnCircle*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnCircle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnHex*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnHex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnHex.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_BossHexBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_BossHexBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss/BossHexbar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BossHpBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_BossHpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss/BossHpbar.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_BossHpDivider*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_BossHpDivider"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss/BossHpDivider.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_BossName*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_BossName"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Boss/BossName.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SpaceKeyBoard*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_SpaceKeyBoard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/SpaceKeyBoard.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Arena*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Arena"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Arena/Arena.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SpiderTank*/
	PreTransformMatrix = XMMatrixScaling(0.008f, 0.008f, 0.008f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_SpiderTank"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Spidertank/Spidertank.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SpiderTankOrb*/
	PreTransformMatrix = XMMatrixScaling(0.008f, 0.008f, 0.008f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_SpiderTankOrb"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Spidertank/Orb/Spidertank_Orb.Model"), PreTransformMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));
	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationData/Arena.Nav")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_TPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Arena */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Arena"),
		CArena::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_SpiderTank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_SpiderTank"),
		CBody_SpiderTank::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_SpiderTank */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_SpiderTank"),
		CSpiderTank::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderTank_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_SpiderTank_Bullet"),
		CSpiderTank_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderTank_Orb */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_SpiderTank_Orb"),
		CSpiderTank_Orb::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderTank_Lager */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_SpiderTank_Lager"),
		CSpiderTank_Lager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_BossHPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI2D_BossHPBar"),
		CUI2D_BossHPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_LockOn */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_LockOn"),
		CUI3D_LockOn::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"),
		CUI3D_Interaction::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Shop(LEVEL eLevelID)
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_SpaceKeyBoard*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_SpaceKeyBoard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/SpaceKeyBoard.png")))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Shop*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Shop"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Shop/Shop.Model"), PreTransformMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));
	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationData/Shop.Nav")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_TPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Camera_TPS"),
		CCamera_TPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Shop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Shop"),
		CShop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"),
		CUI3D_Interaction::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tools(LEVEL eLevelID)
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/*For.Prototype_Component_Texture_Sky*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Enemy_HexBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Enemy_HexBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster/Enemy_HexBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HexBar_Notch*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_Enemy_HPBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Monster/Enemy_HpBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnCircle*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnCircle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnCircle.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LockOnHex*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_LockOnHex"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/LockOnHex.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SpaceKeyBoard*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Texture_SpaceKeyBoard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/SpaceKeyBoard.png")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Courtyard*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Courtyard"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Courtyard/Courtyard.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Arena*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Arena"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Fortress/Arena/Arena.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Shop*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Shop"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Map/Shop/Shop.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Bush*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Bush"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Bush/bush.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Grass*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Grass"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/Grass/grass base.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_CheckPoint*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_CheckPoint"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Environment_Objects/TUNIC_Checkpoint/TUNIC_Checkpoint.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Chest*/
 	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Chest"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Environment_Objects/Chest/Chest.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Potion*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Potion"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/NonAnim/Item/Potion/Potion.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Blob*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Blob"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Blob/Blob.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Wizard_Support*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Support"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Support/Wizard_Support.Model"), PreTransformMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Wizard_Candleabra*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Candleabra"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Candleabra/Wizard_Candleabra.Model"), PreTransformMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Wizard_Sword*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Wizard_Sword"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Sword/Wizard_Sword.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardCandleabra*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardCandleabra"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Candleabra.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardSword*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardSword"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Sword.Model"), PreTransformMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Weapon_WizardStaff*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardStaff"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("../Bin/Resources/Models/Anim/Monster/Wizard/Weapons/Staff.Model"), PreTransformMatrix))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Courtyard */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Courtyard"),
		CCourtyard::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Arena */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Arena"),
		CArena::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Main */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Main"),
	//	CMain::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Prototype_GameObject_Shop */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Shop"),
		CShop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Bush"),
		CBody_Bush::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Bush */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Bush"),
		CBush::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Grass"),
		CBody_Grass::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_CheckPoint"),
		CBody_CheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_CheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_CheckPoint"),
		CCheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Chest */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Chest"),
		CBody_Chest::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Chest */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Chest"),
		CChest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Potion */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Potion"),
		CItem_Potion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Blob */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Blob"),
		CBody_Blob::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Blob */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Blob"),
		CBlob::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Part_WizardCandleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardCandleabra"),
		CPart_WizardCandleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Part_WizardSword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardSword"),
		CPart_WizardSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Body_Candleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Candleabra"),
		CBody_WizardCandleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Candleabra */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Candleabra"),
		CWizard_Candleabra::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Part_WizardStaff */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Part_WizardStaff"),
		CPart_WizardStaff::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Body_Support */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Support"),
		CBody_WizardSupport::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Support */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Support"),
		CWizard_Support::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Body_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Body_Sword"),
		CBody_WizardSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Wizard_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Wizard_Sword"),
		CWizard_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_MobHPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_MobHPBar"),
		CUI3D_MobHPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_LockOn */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_LockOn"),
		CUI3D_LockOn::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_Interaction */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"),
		CUI3D_Interaction::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(eLevelID), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
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
