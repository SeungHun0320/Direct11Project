#include "MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "Player.h"
#include "Body_Player.h"
#include "Weapon_Player.h"

#include "UI2D_PlayerHPBar.h"
#include "UI2D_PlayerSPBar.h"
#include "UI2D_PlayerMPBar.h"
#include "UI2D_PlayerPotion.h"
#include "UI2D_PlayerItemSlots.h"


#include "UI.h"
#include "UI_Animation.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	//	D3D11_SAMPLER_DESC
}

HRESULT CMainApp::Initialize()
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
	EngineDesc.iNumColliderGroups = static_cast<_uint>(COLLIDER_GROUP::CG_END);

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_For_Fonts()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Object()))
		return E_FAIL;

	if (FAILED(Ready_For_BGM()))
		return E_FAIL;

	if (FAILED(Start_Level(LEVEL::LOGO)))
		return E_FAIL;

    return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;


	++m_iFPS;

	if (1.f < m_fTimeAcc)
	{
		wsprintf(m_szFPS, TEXT("FPS: %d"), m_iFPS);

		m_fTimeAcc = 0.f;
		m_iFPS = 0;
	}
#endif

}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Begin_Draw();

	m_pGameInstance->Draw();

#ifdef _DEBUG
	m_pGameInstance->Draw_Font(TEXT("Font_Money"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
#endif

	m_pGameInstance->End_Draw();
    return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Texture()
{
	/* For.Prototype_Component_Texture_Potion*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Potion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Potion/Potion_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PKeyBoard*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PKeyBoard"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/GameUI/PKeyBoard.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HexBar_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_HexBar_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/HexBar_Back.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HexBar_Notch*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_HexBar_Notch"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/HexBar_Notch.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerHP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerHP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/HP/HPBar.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_PlayerHPCap*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerHPCap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/HP/HPCap.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_PlayerStamina */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerStamina"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Stamina/StaminaBar.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_PlayerStaminaCap*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerStaminaCap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Stamina/StaminaCap.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerMana */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerMana"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Mana/ManaBar.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_PlayerManaCap*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PlayerManaCap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Mana/ManaCap.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemSlot*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_ItemSlot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerStat/Inventory/ItemSlot.png")))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, COLLIDER::SPHERE))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Object()
{
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/*-----------------------------------------플레이어-----------------------------------------------------*/

	/*For.Prototype_Component_Model_Fox*/    /* 야우 확인용 */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Fox"),
	CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("../Bin/Resources/Models/Anim/Fox/_Fox.Model"), PreTransformMatrix))))
	return E_FAIL;

	/*For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Weapon_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Weapon_Player"),
		CWeapon_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*--------------------------------------------플레이어UI(컨테이너파트)--------------------------------------------------*/

	/*For.Prototype_GameObject_UI2D_PlayerHPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerHPBar"),
		CUI2D_PlayerHPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_PlayerStaminaBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerStaminaBar"),
		CUI2D_PlayerSPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_PlayerManaBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerManaBar"),
		CUI2D_PlayerMPBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_PlayerHPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerPotion"),
		CUI2D_PlayerPotion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_PlayerItemSlots */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerItemSlots"),
		CUI2D_PlayerItemSlots::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*--------------------------------------------UI(파트오브젝트)--------------------------------------------------*/

	/*For.Prototype_GameObject_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"),
		CUI::Create(m_pDevice, m_pContext, UI_TYPE::UI2D))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D"),
		CUI::Create(m_pDevice, m_pContext, UI_TYPE::UI3D))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI2D_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_Animation"),
		CUI_Animation::Create(m_pDevice, m_pContext, UI_TYPE::UI2D))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI3D_Animation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D_Animation"),
		CUI_Animation::Create(m_pDevice, m_pContext, UI_TYPE::UI3D))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_For_BGM()
{
	return m_pGameInstance->LoadSound("../Bin/Resources/Sounds/BGM", false, true);
}

HRESULT CMainApp::Ready_For_Fonts()
{
	/*MakeSpriteFont "배찌체" /FontSize:20 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 151ex.spritefont */
	/*MakeSpriteFont "국민연금체 Bold" /FontSize:20 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 OdinRounded.spritefont */
	/* MakeSpriteFont "예스체" /FontSize:20 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 OdinRounded.spritefont*/
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_151"), TEXT("../Bin/Resources/Fonts/151ex.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Money"), TEXT("../Bin/Resources/Fonts/CityzenMoney.spritefont"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eStartLevel)
{
	if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevel))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
    __super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);

	
}
