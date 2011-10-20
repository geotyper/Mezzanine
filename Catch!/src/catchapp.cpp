#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include "callbacks.h"
#include "buttoncallbacks.h"
#include "levelselectcell.h"
#include "throwablegenerator.h"
#include "widgetcallbacks.h"
#include <cassert>
#include <set>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp()
    : CurrScore(0),
      LastActorThrown(NULL),
      EndTimer(NULL),
      CurrentState(CatchApp::Catch_Init),
      PlaneOfPlay(Plane(Vector3(2.0,1.0,0.0), Vector3(1.0,2.0,0.0), Vector3(1.0,1.0,0.0)))
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    try
    {
        PhysicsConstructionInfo Info;
        Info.PhysicsFlags = (PhysicsConstructionInfo::PCF_LimitlessWorld | PhysicsConstructionInfo::PCF_SoftRigidWorld);
        TheWorld = new World( Info, SceneManager::Generic, "Plugins/plugins.cfg" );
    }catch( exception x){
        //could not create world
    }
    Loader = new LevelLoader();
}

CatchApp::~CatchApp()
{
    delete TheWorld;
    delete Loader;
    CatchApp::TheRealCatchApp = NULL;
}

void CatchApp::MakeGUI()
{
    UIManager* GUI = TheWorld->GetUIManager();
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.85);
    ColourValue Gray(0.2,0.2,0.2,1.0);

    //Make the Main Menu screen and associated layers.
    GUI->LoadGorilla("Catch_Menu");
    UI::Screen* MainMenuScreen = GUI->CreateScreen("MainMenuScreen", "Catch_Menu", UIViewport);
    UI::Layer* MainMenuLayer = MainMenuScreen->CreateLayer("MainMenuLayer",0);

    //Build the Main Menu Screen
    UI::Rectangle* Background = MainMenuLayer->CreateRectangle( UI::RenderableRect(Vector2(-0.16667,0), Vector2(1.33334,1), true));
    Background->SetBackgroundSprite("MainMenuBackground");

    UI::Menu* MainMenuMenu = MainMenuLayer->CreateMenu( "MS_Menu", UI::RenderableRect(Vector2(0.0,0.915), Vector2(1.0,0.086), true));
    MainMenuMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("MMBrickBackground");
    MainMenuMenu->GetRootWindow()->SetAutoHide(false);

    Real MMStartLineHeight = 0.05;
    Real MMTextLineHeight = 0.04;
    Real MMSmallTextLineHeight = 0.03;
    //std::pair<Whole,Real> MainMenuText = GUI->SuggestGlyphIndex(0.04 * MainMenuScreen->GetViewportDimensions().Y,MainMenuScreen->GetPrimaryAtlas());
    UI::TextButton* MMLevelSelectAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_LevelSelect", UI::RenderableRect(Vector2(0.05, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Level Select" );
    MMLevelSelectAccess->SetBackgroundSprite("MMButton");
    MMLevelSelectAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMLevelSelectWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_LevelSelect", UI::RenderableRect(Vector2(0.05,0.02), Vector2(0.90,0.84), true), MMLevelSelectAccess);
    MMLevelSelectWin->GetWindowBack()->SetBackgroundSprite("MMLSBackground");
    UI::PagedCellGrid* MMLevelSelectGrid = MMLevelSelectWin->CreatePagedCellGrid("MS_LevelGrid", UI::RenderableRect(Vector2(0.14,0.14), Vector2(0.72,0.66), true), UI::RenderableRect(Vector2(0.60,0.85), Vector2(0.24,0.06), true), UI::Spn_Separate, 0.05);
    MMLevelSelectGrid->GetGridBack()->SetBackgroundColour(Transparent);
    MMLevelSelectGrid->GetPageSpinner()->GetIncrement()->SetBackgroundSprite("MMIncrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetDecrement()->SetBackgroundSprite("MMDecrementPage");
    MMLevelSelectGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("MMPageBox");
    PopulateLevelList(MMLevelSelectGrid);
    MMLevelSelectGrid->GenerateGrid();
    UI::TextButton* MMLevelStart = MMLevelSelectWin->CreateTextButton("MS_LevelStart", UI::RenderableRect(Vector2(0.42,0.85), Vector2(0.16,0.07), true),MMStartLineHeight,"Start");
    MMLevelStart->SetButtonCallback(new MSStart(MMLevelSelectGrid));
    MMLevelStart->SetBackgroundSprite("MMLevelStart");
    MMLevelStart->SetHoveredSprite("MMLevelStartHovered");

    UI::TextButton* MMOptionsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Options", UI::RenderableRect(Vector2(0.28, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Options" );
    MMOptionsAccess->SetBackgroundSprite("MMButton");
    MMOptionsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMOptionsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Options", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), MMOptionsAccess);
    MMOptionsWin->GetWindowBack()->SetBackgroundSprite("MMOptionsBackground");
    UI::TabSet* MMOptionsTabSet = MMOptionsWin->CreateTabSet("MS_OptionsTS", UI::RenderableRect(Vector2(0.20, 0.31), Vector2(0.60, 0.39), true));
    //video options
    UI::RenderableSetData* MMVideoSet = MMOptionsTabSet->CreateRenderableSet("MS_VideoSet",UI::RenderableRect(Vector2(0.25, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Video Options");
    MMVideoSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    MMVideoSet->Accessor->SetBackgroundSprite("MMButton");
    MMVideoSet->Accessor->SetHoveredSprite("MMHoveredButton");
    UI::Caption* MMResolutionLabel = MMVideoSet->Collection->CreateCaption("MS_ResolutionLabel",UI::RenderableRect(Vector2(0.28, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Video Resolution");
    MMResolutionLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMResolutionList = MMVideoSet->Collection->CreateDropDownList("MS_ResolutionList",UI::RenderableRect(Vector2(0.25, 0.36),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMResolutionList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMResolutionList->GetListToggle()->SetBackgroundSprite("MMListScrollDown");
    MMResolutionList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("MMListScroller");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("MMHoveredListScroller");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("MMListScrollDown");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("MMHoveredListScrollDown");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("MMListScrollUp");
    MMResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("MMHoveredListScrollUp");
    MMResolutionList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    MMResolutionList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    MMResolutionList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    MMResolutionList->GetSelectionList()->SetMaxDisplayedSelections(4);
    MMResolutionList->SetWidgetCallback(new OptsVideoRes());
    UI::CheckBox* MMFullscreenBox = MMVideoSet->Collection->CreateCheckBox("MS_FullscreenBox",UI::RenderableRect(Vector2(0.59, 0.36),Vector2(0.16, 0.05),true),MMTextLineHeight,"Fullscreen");
    MMFullscreenBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMFullscreenBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMFullscreenBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMFullscreenBox->SetWidgetCallback(new OptsVideoFullscreen());
    UI::Caption* MMFSAALabel = MMVideoSet->Collection->CreateCaption("MS_FSAALabel",UI::RenderableRect(Vector2(0.28, 0.46),Vector2(0.24, 0.05),true),MMTextLineHeight,"Anti-Aliasing");
    MMFSAALabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMFSAAList = MMVideoSet->Collection->CreateDropDownList("MS_FSAAList",UI::RenderableRect(Vector2(0.25, 0.51),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMFSAAList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMFSAAList->GetListToggle()->SetBackgroundSprite("MMListScrollDown");
    MMFSAAList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("MMListScroller");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("MMHoveredListScroller");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("MMListScrollDown");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("MMHoveredListScrollDown");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("MMListScrollUp");
    MMFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("MMHoveredListScrollUp");
    MMFSAAList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    MMFSAAList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    MMFSAAList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    MMFSAAList->GetSelectionList()->SetMaxDisplayedSelections(4);
    MMFSAAList->SetWidgetCallback(new OptsVideoFSAA());
    UI::CheckBox* MMStatsBox = MMVideoSet->Collection->CreateCheckBox("MS_StatsBox",UI::RenderableRect(Vector2(0.59, 0.51),Vector2(0.16, 0.05),true),MMTextLineHeight,"Show FPS");
    MMStatsBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMStatsBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMStatsBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMStatsBox->SetWidgetCallback(new OptsVideoStats());
    UI::TextButton* MMVideoOptsApply = MMVideoSet->Collection->CreateTextButton("MS_VideoOptsApply",UI::RenderableRect(Vector2(0.68, 0.64), Vector2(0.10, 0.05), true), MMTextLineHeight, "Apply");
    MMVideoOptsApply->SetBackgroundSprite("MMOptionsApplyButton");
    MMVideoOptsApply->SetHoveredSprite("MMOptionsApplyHoveredButton");
    MMVideoOptsApply->SetButtonCallback(new OptsVideoApply(MMResolutionList,MMFullscreenBox,MMStatsBox));
    //sound options
    Real MMScrollerSize = 0.09;
    UI::RenderableSetData* MMAudioSet = MMOptionsTabSet->CreateRenderableSet("MS_AudioSet",UI::RenderableRect(Vector2(0.53, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Sound Options");
    MMAudioSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    MMAudioSet->Accessor->SetBackgroundSprite("MMButton");
    MMAudioSet->Accessor->SetHoveredSprite("MMHoveredButton");
    UI::Caption* MMMusicVolLabel = MMAudioSet->Collection->CreateCaption("MS_MusicVolLabel",UI::RenderableRect(Vector2(0.38, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Music Volume");
    MMMusicVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* MMMusicVol = MMAudioSet->Collection->CreateScrollbar("MS_MusicVolume",UI::RenderableRect(Vector2(0.30, 0.36),Vector2(0.40, 0.04),true),UI::SB_Separate);
    MMMusicVol->SetScrollerSize(MMScrollerSize);
    MMMusicVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMMusicVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    MMMusicVol->GetScroller()->SetBackgroundSprite("MMScroller");
    MMMusicVol->GetScroller()->SetHoveredSprite("MMHoveredScroller");
    MMMusicVol->GetDownRightButton()->SetBackgroundSprite("MMScrollRight");
    MMMusicVol->GetDownRightButton()->SetHoveredSprite("MMHoveredScrollRight");
    MMMusicVol->GetUpLeftButton()->SetBackgroundSprite("MMScrollLeft");
    MMMusicVol->GetUpLeftButton()->SetHoveredSprite("MMHoveredScrollLeft");
    MMMusicVol->SetWidgetCallback(new OptsMusicVol());
    UI::Caption* MMEffectsVolLabel = MMAudioSet->Collection->CreateCaption("MS_EffectsVolLabel",UI::RenderableRect(Vector2(0.38, 0.42),Vector2(0.24, 0.05),true),MMTextLineHeight,"Effects Volume");
    MMEffectsVolLabel->SetBackgroundSprite("MMButton");
    UI::Scrollbar* MMEffectsVol = MMAudioSet->Collection->CreateScrollbar("MS_EffectsVolume",UI::RenderableRect(Vector2(0.30, 0.47),Vector2(0.40, 0.04),true),UI::SB_Separate);
    MMEffectsVol->SetScrollerSize(MMScrollerSize);
    MMEffectsVol->SetIncrementDistance(MMScrollerSize * 0.5);
    MMEffectsVol->GetScrollBack()->SetBackgroundSprite("MMScrollBackground");
    MMEffectsVol->GetScroller()->SetBackgroundSprite("MMScroller");
    MMEffectsVol->GetScroller()->SetHoveredSprite("MMHoveredScroller");
    MMEffectsVol->GetDownRightButton()->SetBackgroundSprite("MMScrollRight");
    MMEffectsVol->GetDownRightButton()->SetHoveredSprite("MMHoveredScrollRight");
    MMEffectsVol->GetUpLeftButton()->SetBackgroundSprite("MMScrollLeft");
    MMEffectsVol->GetUpLeftButton()->SetHoveredSprite("MMHoveredScrollLeft");
    MMEffectsVol->SetWidgetCallback(new OptsEffectVol());
    UI::Caption* MMDeviceLabel = MMAudioSet->Collection->CreateCaption("MS_DeviceLabel",UI::RenderableRect(Vector2(0.30, 0.55),Vector2(0.24, 0.05),true),MMTextLineHeight,"Audio Device");
    MMDeviceLabel->SetBackgroundSprite("MMButton");
    UI::DropDownList* MMDeviceList = MMAudioSet->Collection->CreateDropDownList("MS_AudioDeviceList",UI::RenderableRect(Vector2(0.27, 0.60),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    MMDeviceList->GetSelection()->SetBackgroundSprite("MMListSelection");
    MMDeviceList->GetListToggle()->SetBackgroundSprite("MMListScrollDown");
    MMDeviceList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("MMListBackground");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("MMListScrollBackground");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("MMListScroller");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("MMHoveredListScroller");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("MMListScrollDown");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("MMHoveredListScrollDown");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("MMListScrollUp");
    MMDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("MMHoveredListScrollUp");
    UI::CheckBox* MMMuteBox = MMAudioSet->Collection->CreateCheckBox("MS_MuteBox",UI::RenderableRect(Vector2(0.62, 0.60),Vector2(0.11, 0.05),true),MMTextLineHeight,"Mute");
    MMMuteBox->GetLabel()->SetBackgroundSprite("MMAppExitButton");
    MMMuteBox->SetCheckedSprite("MMCheckboxChecked","MMHoveredCheckboxChecked");
    MMMuteBox->SetUncheckedSprite("MMCheckboxUnchecked","MMHoveredCheckboxUnchecked");
    MMMuteBox->SetWidgetCallback(new OptsAudioMute());

    UI::TextButton* MMOptsBack = MMOptionsWin->CreateBackButton(UI::RenderableRect(Vector2(0.68, 0.70), Vector2(0.10, 0.05), true), MMTextLineHeight, "Back");
    MMOptsBack->SetBackgroundSprite("MMOptionsApplyButton");
    MMOptsBack->SetHoveredSprite("MMOptionsApplyHoveredButton");

    UI::TextButton* MMCreditsAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_Credits", UI::RenderableRect(Vector2(0.51, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Credits" );
    MMCreditsAccess->SetBackgroundSprite("MMButton");
    MMCreditsAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMCreditsWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_Credits", UI::RenderableRect(Vector2(0.01,0.01), Vector2(0.01,0.01), true), MMCreditsAccess);

    UI::TextButton* MMAppExitAccess = MainMenuMenu->GetRootWindow()->CreateAccessorButton( "MS_AppExit", UI::RenderableRect(Vector2(0.74, 0.93), Vector2(0.22, 0.06), true), MMTextLineHeight, "Exit Game" );
    MMAppExitAccess->SetBackgroundSprite("MMButton");
    MMAppExitAccess->SetHoveredSprite("MMHoveredButton");
    UI::MenuWindow* MMAppExitWin = MainMenuMenu->GetRootWindow()->CreateChildMenuWindow("MS_AppExit", UI::RenderableRect(Vector2(0.25, 0.3), Vector2(0.5, 0.3), true), MMAppExitAccess);
    MMAppExitWin->GetWindowBack()->SetBackgroundSprite("MMAppExitBackground");
    UI::Caption* MMAppExitWarn = MMAppExitWin->CreateCaption("MS_AppExitWarn", UI::RenderableRect(Vector2(0.32, 0.35), Vector2(0.36, 0.07), true), Real(0.04), "Are you sure you want to exit?");
    MMAppExitWarn->SetBackgroundSprite("MMAppExitText");
    UI::TextButton* MMAppExitConf = MMAppExitWin->CreateTextButton("MS_AppExitConf", UI::RenderableRect(Vector2(0.30, 0.47), Vector2(0.18, 0.06), true), Real(0.04), "Yes");
    MMAppExitConf->SetButtonCallback(new AllAppExit());
    MMAppExitConf->SetBackgroundSprite("MMAppExitButton");
    MMAppExitConf->SetHoveredSprite("MMAppExitHoveredButton");
    UI::TextButton* MMAppExitDeny = MMAppExitWin->CreateBackButton(/*"MS_AppExitDeny", */UI::RenderableRect(Vector2(0.52, 0.47), Vector2(0.18, 0.06), true), Real(0.04), "No");
    MMAppExitDeny->SetBackgroundSprite("MMAppExitButton");
    MMAppExitDeny->SetHoveredSprite("MMAppExitHoveredButton");
    //End of Main Menu Screen

    //Make the Game screen and associated layers.
    GUI->LoadGorilla("Catch_Game");
    UI::Screen* GameScreen = GUI->CreateScreen("GameScreen", "Catch_Game", UIViewport);
    UI::Layer* Menu = GameScreen->CreateLayer("MenuLayer", 10);
    UI::Layer* Report = GameScreen->CreateLayer("ReportLayer", 8);
    UI::Layer* ItemShop = GameScreen->CreateLayer("ItemShopLayer", 4);
    UI::Layer* Stats = GameScreen->CreateLayer("StatsLayer", 1);
    UI::Layer* HUD = GameScreen->CreateLayer("HUDLayer", 0);

    //Build the Game Screen
    //Build the HUD layer
    Real GSTextLineHeight = 0.04;
    UI::Caption* Timer = HUD->CreateCaption( "GS_Timer", UI::RenderableRect(Vector2(0.8897, 0.006), Vector2(0.1045, 0.065), true), GSTextLineHeight, "0:00");
    Timer->SetBackgroundSprite("GSTimerArea");
    UI::Rectangle* TimerLogo = HUD->CreateRectangle( UI::RenderableRect(Vector2(0.8355, 0.006), Vector2(0.0542, 0.065), true));
    TimerLogo->SetBackgroundSprite("GSTimerLogo");
    UI::TextButton* MenuButton = HUD->CreateTextButton( "GS_Menu", UI::RenderableRect(Vector2(0.008, 0.935), Vector2(0.16, 0.065), true),GSTextLineHeight,"Menu");
    MenuButton->SetButtonCallback(new GSMenu());
    MenuButton->SetBackgroundSprite("GSMenuStoreButton");
    MenuButton->SetHoveredSprite("GSMenuStoreHoveredButton");
    UI::TextButton* StoreButton = HUD->CreateTextButton( "GS_Store", UI::RenderableRect(Vector2(0.822, 0.935), Vector2(0.16, 0.065), true),GSTextLineHeight,"Store");
    StoreButton->SetButtonCallback(new GSStore());
    StoreButton->SetBackgroundSprite("GSMenuStoreButton");
    StoreButton->SetHoveredSprite("GSMenuStoreHoveredButton");
    UI::Caption* ScoreAmount = HUD->CreateCaption( "GS_ScoreArea", UI::RenderableRect(Vector2(0.128, 0.006), Vector2(0.15, 0.06), true), GSTextLineHeight, "0");
    ScoreAmount->SetBackgroundSprite("GSScoreValueArea");
    UI::Caption* ScoreText = HUD->CreateCaption( "GS_ScoreText", UI::RenderableRect(Vector2(0.008, 0.006), Vector2(0.12, 0.06), true), GSTextLineHeight, "Score");
    ScoreText->SetBackgroundSprite("GSScoreTextArea");
    //End of HUD Layer

    //Build the ItemShop Layer
    UI::Window* ItemShopWin = ItemShop->CreateWidgetWindow("GS_ItemShop", UI::RenderableRect(Vector2(0.25, 0.11), Vector2(0.5, 0.78125), true));
    ItemShopWin->GetWindowBack()->SetBackgroundSprite("GSStoreBackground");
    UI::Caption* ShopTitle = ItemShopWin->CreateCaption("GS_StoreTitle",UI::RenderableRect(Vector2(0.40,0.128),Vector2(0.20,0.06),true),Real(0.06),"Store");
    ShopTitle->SetBackgroundColour(ColourValue::Transparent());
    UI::PagedCellGrid* ItemShopGrid = ItemShopWin->CreatePagedCellGrid("GS_ItemShopGrid", UI::RenderableRect(Vector2(0.31,0.205), Vector2(0.38,0.26), true), UI::RenderableRect(Vector2(0.40,0.47), Vector2(0.20,0.05), true), UI::Spn_Separate, 0.05);
    ItemShopGrid->GetGridBack()->SetBackgroundColour(ColourValue::Transparent());
    ItemShopGrid->GetPageSpinner()->GetIncrement()->SetBackgroundSprite("GSIncrementPage");
    ItemShopGrid->GetPageSpinner()->GetDecrement()->SetBackgroundSprite("GSDecrementPage");
    ItemShopGrid->GetPageSpinner()->GetValueDisplay()->SetBackgroundSprite("GSPageBox");
    UI::Rectangle* ItemShopDescriptionBoxBack = ItemShopWin->CreateRectangle(UI::RenderableRect(Vector2(0.31,0.525), Vector2(0.38,0.26), true));
    ItemShopDescriptionBoxBack->SetBackgroundSprite("GSListBackground");
    UI::MarkupText* ItemShopDescriptionBox = ItemShopWin->CreateMarkupText("GS_DescBox",UI::RenderableRect(Vector2(0.315,0.53), Vector2(0.37,0.25), true),Real(0.04),"This is a more exhaustive and thorough test of the markup text's capabilities.");
    ItemShopDescriptionBox->SetBackgroundColour(ColourValue::Transparent());
    UI::Caption* ItemShopMoneyBox = ItemShopWin->CreateCaption("GS_ItemShopMoney",UI::RenderableRect(Vector2(0.31,0.795),Vector2(0.12,0.06),true),Real(0.05),"$");
    ItemShopMoneyBox->SetBackgroundSprite("GSStoreButton");
    UI::TextButton* ItemShopBuyButton = ItemShopWin->CreateTextButton("GS_ItemShopBuy",UI::RenderableRect(Vector2(0.44,0.795),Vector2(0.12,0.06),true),Real(0.05),"Buy");
    ItemShopBuyButton->SetBackgroundSprite("GSStoreButton");
    ItemShopBuyButton->SetHoveredSprite("GSStoreHoveredButton");
    UI::TextButton* ItemShopBackButton = ItemShopWin->CreateTextButton("GS_ItemShopBack",UI::RenderableRect(Vector2(0.57,0.795),Vector2(0.12,0.06),true),Real(0.05),"Back");
    ItemShopBackButton->SetBackgroundSprite("GSStoreButton");
    ItemShopBackButton->SetHoveredSprite("GSStoreHoveredButton");
    ItemShopBackButton->SetButtonCallback(new GSISReturn());

    ItemShop->Hide();
    //End of ItemShop Layer

    //Build the Menu Layer
    UI::Menu* GSGameMenu = Menu->CreateMenu( "GS_GameMenu", UI::RenderableRect(Vector2(0.35, 0.27), Vector2(0.3, 0.45), true));
    GSGameMenu->GetRootWindow()->GetWindowBack()->SetBackgroundSprite("GSOptionsMenuBackground");
    UI::TextButton* GSRestartButton = GSGameMenu->GetRootWindow()->CreateTextButton( "GS_Restart", UI::RenderableRect(Vector2(0.38, 0.48), Vector2(0.24, 0.05), true), GSTextLineHeight, "Restart Level");
    GSRestartButton->SetButtonCallback(new GSRestart());
    GSRestartButton->SetBackgroundSprite("GSOptionsButton");
    GSRestartButton->SetHoveredSprite("GSOptionsHoveredButton");
    UI::TextButton* GSReturnButton = GSGameMenu->GetRootWindow()->CreateTextButton( "GS_Return", UI::RenderableRect(Vector2(0.38, 0.56), Vector2(0.24, 0.05), true), GSTextLineHeight, "Return To Game");
    GSReturnButton->SetButtonCallback(new GSReturn());
    GSReturnButton->SetBackgroundSprite("GSOptionsButton");
    GSReturnButton->SetHoveredSprite("GSOptionsHoveredButton");
    UI::TextButton* GSGameExitButton = GSGameMenu->GetRootWindow()->CreateTextButton( "GS_GameExit", UI::RenderableRect(Vector2(0.38, 0.64), Vector2(0.24, 0.05), true), GSTextLineHeight, "Exit To Menu");
    GSGameExitButton->SetButtonCallback(new GSMMReturn());
    GSGameExitButton->SetBackgroundSprite("GSOptionsButton");
    GSGameExitButton->SetHoveredSprite("GSOptionsHoveredButton");

    UI::TextButton* GSOptionsAccess = GSGameMenu->GetRootWindow()->CreateAccessorButton("GS_OptionsButton", UI::RenderableRect(Vector2(0.38, 0.32), Vector2(0.24, 0.05), true), GSTextLineHeight, "Options");
    GSOptionsAccess->SetBackgroundSprite("GSOptionsButton");
    GSOptionsAccess->SetHoveredSprite("GSOptionsHoveredButton");
    UI::MenuWindow* GSOptionsWin = GSGameMenu->GetRootWindow()->CreateChildMenuWindow("GS_OptionsWin", UI::RenderableRect(Vector2(0.18, 0.22), Vector2(0.64, 0.55), true), GSOptionsAccess);
    GSOptionsWin->GetWindowBack()->SetBackgroundSprite("GSOptionsBackground");
    UI::TabSet* GSOptionsTabSet = GSOptionsWin->CreateTabSet("GS_OptionsTS", UI::RenderableRect(Vector2(0.20, 0.31), Vector2(0.60, 0.39), true));
    //video options
    UI::RenderableSetData* GSVideoSet = GSOptionsTabSet->CreateRenderableSet("GS_VideoSet",UI::RenderableRect(Vector2(0.25, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Video Options");
    GSVideoSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    GSVideoSet->Accessor->SetBackgroundSprite("GSButton");
    GSVideoSet->Accessor->SetHoveredSprite("GSHoveredButton");
    UI::Caption* GSResolutionLabel = GSVideoSet->Collection->CreateCaption("GS_ResolutionLabel",UI::RenderableRect(Vector2(0.28, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Video Resolution");
    GSResolutionLabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSResolutionList = GSVideoSet->Collection->CreateDropDownList("GS_ResolutionList",UI::RenderableRect(Vector2(0.25, 0.36),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSResolutionList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSResolutionList->GetListToggle()->SetBackgroundSprite("GSListScrollDown");
    GSResolutionList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("GSListScroller");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("GSHoveredListScroller");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("GSListScrollDown");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("GSHoveredListScrollDown");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("GSListScrollUp");
    GSResolutionList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("GSHoveredListScrollUp");
    GSResolutionList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    GSResolutionList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    GSResolutionList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    GSResolutionList->GetSelectionList()->SetMaxDisplayedSelections(4);
    GSResolutionList->SetWidgetCallback(new OptsVideoRes());
    UI::CheckBox* GSFullscreenBox = GSVideoSet->Collection->CreateCheckBox("GS_FullscreenBox",UI::RenderableRect(Vector2(0.59, 0.36),Vector2(0.16, 0.05),true),MMTextLineHeight,"Fullscreen");
    GSFullscreenBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSFullscreenBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSFullscreenBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSFullscreenBox->SetWidgetCallback(new OptsVideoFullscreen());
    UI::Caption* GSFSAALabel = GSVideoSet->Collection->CreateCaption("GS_FSAALabel",UI::RenderableRect(Vector2(0.28, 0.46),Vector2(0.24, 0.05),true),MMTextLineHeight,"Anti-Aliasing");
    GSFSAALabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSFSAAList = GSVideoSet->Collection->CreateDropDownList("GS_FSAAList",UI::RenderableRect(Vector2(0.25, 0.51),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSFSAAList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSFSAAList->GetListToggle()->SetBackgroundSprite("GSListScrollDown");
    GSFSAAList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("GSListScroller");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("GSHoveredListScroller");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("GSListScrollDown");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("GSHoveredListScrollDown");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("GSListScrollUp");
    GSFSAAList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("GSHoveredListScrollUp");
    GSFSAAList->GetSelectionList()->GetVertScroll()->SetIncrementDistance(0.05);
    GSFSAAList->GetSelectionList()->SetTemplateBackgroundColour(ColourValue(1.0,1.0,1.0,0.2));
    GSFSAAList->GetSelectionList()->SetTemplateTextColour(ColourValue::White());
    GSFSAAList->GetSelectionList()->SetMaxDisplayedSelections(4);
    GSFSAAList->SetWidgetCallback(new OptsVideoFSAA());
    UI::CheckBox* GSStatsBox = GSVideoSet->Collection->CreateCheckBox("GS_StatsBox",UI::RenderableRect(Vector2(0.59, 0.51),Vector2(0.16, 0.05),true),MMTextLineHeight,"Show FPS");
    GSStatsBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSStatsBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSStatsBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSStatsBox->SetWidgetCallback(new OptsVideoStats());
    UI::TextButton* GSVideoOptsApply = GSVideoSet->Collection->CreateTextButton("GS_VideoOptsApply",UI::RenderableRect(Vector2(0.68, 0.64), Vector2(0.10, 0.05), true), MMTextLineHeight, "Apply");
    GSVideoOptsApply->SetBackgroundSprite("GSOptionsApplyButton");
    GSVideoOptsApply->SetHoveredSprite("GSOptionsApplyHoveredButton");
    GSVideoOptsApply->SetButtonCallback(new OptsVideoApply(GSResolutionList,GSFullscreenBox,GSStatsBox));
    //sound options
    Real GSScrollerSize = 0.09;
    UI::RenderableSetData* GSAudioSet = GSOptionsTabSet->CreateRenderableSet("GS_AudioSet",UI::RenderableRect(Vector2(0.53, 0.24),Vector2(0.22, 0.06),true),MMTextLineHeight,"Sound Options");
    GSAudioSet->Collection->GetWidgetBack()->SetBackgroundColour(ColourValue::Transparent());
    GSAudioSet->Accessor->SetBackgroundSprite("GSButton");
    GSAudioSet->Accessor->SetHoveredSprite("GSHoveredButton");
    UI::Caption* GSMusicVolLabel = GSAudioSet->Collection->CreateCaption("GS_MusicVolLabel",UI::RenderableRect(Vector2(0.38, 0.31),Vector2(0.24, 0.05),true),MMTextLineHeight,"Music Volume");
    GSMusicVolLabel->SetBackgroundSprite("GSButton");
    UI::Scrollbar* GSMusicVol = GSAudioSet->Collection->CreateScrollbar("GS_MusicVolume",UI::RenderableRect(Vector2(0.30, 0.36),Vector2(0.40, 0.04),true),UI::SB_Separate);
    GSMusicVol->SetScrollerSize(GSScrollerSize);
    GSMusicVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSMusicVol->GetScrollBack()->SetBackgroundSprite("GSScrollBackground");
    GSMusicVol->GetScroller()->SetBackgroundSprite("GSScroller");
    GSMusicVol->GetScroller()->SetHoveredSprite("GSHoveredScroller");
    GSMusicVol->GetDownRightButton()->SetBackgroundSprite("GSScrollRight");
    GSMusicVol->GetDownRightButton()->SetHoveredSprite("GSHoveredScrollRight");
    GSMusicVol->GetUpLeftButton()->SetBackgroundSprite("GSScrollLeft");
    GSMusicVol->GetUpLeftButton()->SetHoveredSprite("GSHoveredScrollLeft");
    GSMusicVol->SetWidgetCallback(new OptsMusicVol());
    UI::Caption* GSEffectsVolLabel = GSAudioSet->Collection->CreateCaption("GS_EffectsVolLabel",UI::RenderableRect(Vector2(0.38, 0.42),Vector2(0.24, 0.05),true),MMTextLineHeight,"Effects Volume");
    GSEffectsVolLabel->SetBackgroundSprite("GSButton");
    UI::Scrollbar* GSEffectsVol = GSAudioSet->Collection->CreateScrollbar("GS_EffectsVolume",UI::RenderableRect(Vector2(0.30, 0.47),Vector2(0.40, 0.04),true),UI::SB_Separate);
    GSEffectsVol->SetScrollerSize(GSScrollerSize);
    GSEffectsVol->SetIncrementDistance(GSScrollerSize * 0.5);
    GSEffectsVol->GetScrollBack()->SetBackgroundSprite("GSScrollBackground");
    GSEffectsVol->GetScroller()->SetBackgroundSprite("GSScroller");
    GSEffectsVol->GetScroller()->SetHoveredSprite("GSHoveredScroller");
    GSEffectsVol->GetDownRightButton()->SetBackgroundSprite("GSScrollRight");
    GSEffectsVol->GetDownRightButton()->SetHoveredSprite("GSHoveredScrollRight");
    GSEffectsVol->GetUpLeftButton()->SetBackgroundSprite("GSScrollLeft");
    GSEffectsVol->GetUpLeftButton()->SetHoveredSprite("GSHoveredScrollLeft");
    GSEffectsVol->SetWidgetCallback(new OptsEffectVol());
    UI::Caption* GSDeviceLabel = GSAudioSet->Collection->CreateCaption("GS_DeviceLabel",UI::RenderableRect(Vector2(0.30, 0.55),Vector2(0.24, 0.05),true),MMTextLineHeight,"Audio Device");
    GSDeviceLabel->SetBackgroundSprite("GSButton");
    UI::DropDownList* GSDeviceList = GSAudioSet->Collection->CreateDropDownList("GS_AudioDeviceList",UI::RenderableRect(Vector2(0.27, 0.60),Vector2(0.30, 0.05),true),MMTextLineHeight,UI::SB_Separate);
    GSDeviceList->GetSelection()->SetBackgroundSprite("GSListSelection");
    GSDeviceList->GetListToggle()->SetBackgroundSprite("GSListScrollDown");
    GSDeviceList->GetSelectionList()->GetBoxBack()->SetBackgroundSprite("GSListBackground");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScrollBack()->SetBackgroundSprite("GSListScrollBackground");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetBackgroundSprite("GSListScroller");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetScroller()->SetHoveredSprite("GSHoveredListScroller");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetBackgroundSprite("GSListScrollDown");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetDownRightButton()->SetHoveredSprite("GSHoveredListScrollDown");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetBackgroundSprite("GSListScrollUp");
    GSDeviceList->GetSelectionList()->GetVertScroll()->GetUpLeftButton()->SetHoveredSprite("GSHoveredListScrollUp");
    UI::CheckBox* GSMuteBox = GSAudioSet->Collection->CreateCheckBox("GS_MuteBox",UI::RenderableRect(Vector2(0.62, 0.60),Vector2(0.11, 0.05),true),MMTextLineHeight,"Mute");
    GSMuteBox->GetLabel()->SetBackgroundSprite("GSAppExitButton");
    GSMuteBox->SetCheckedSprite("GSCheckboxChecked","GSHoveredCheckboxChecked");
    GSMuteBox->SetUncheckedSprite("GSCheckboxUnchecked","GSHoveredCheckboxUnchecked");
    GSMuteBox->SetWidgetCallback(new OptsAudioMute());

    UI::TextButton* GSOptsBack = GSOptionsWin->CreateBackButton(UI::RenderableRect(Vector2(0.68, 0.70), Vector2(0.10, 0.05), true), MMTextLineHeight, "Back");
    GSOptsBack->SetBackgroundSprite("GSOptionsApplyButton");
    GSOptsBack->SetHoveredSprite("GSOptionsApplyHoveredButton");

    Menu->Hide();
    //End of Menu Layer

    //Build the Stats Layer
    UI::Caption* CurFPS = Stats->CreateCaption( "CurFPS", UI::RenderableRect(Vector2(0.16, 0.06), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* CurFPSText = Stats->CreateCaption( "CurFPSText", UI::RenderableRect(Vector2(0.008, 0.06), Vector2(0.15, 0.065), true), Whole(14), "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPS = Stats->CreateCaption( "AvFPS", UI::RenderableRect(Vector2(0.16, 0.105), Vector2(0.06, 0.065), true), Whole(14), "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPSText = Stats->CreateCaption( "AvFPSText", UI::RenderableRect(Vector2(0.008, 0.105), Vector2(0.15, 0.065), true), Whole(14), "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);
    //End of Stats Layer

    //Build the Report Layer
    UI::Window* LevelReport = Report->CreateWidgetWindow("LevelReport", UI::RenderableRect(Vector2(0.2, 0.2), Vector2(0.6, 0.6), true));
    LevelReport->GetWindowBack()->SetBackgroundColour(Gray);
    //TempCaption
    UI::Caption* TempCapt = LevelReport->CreateCaption("GS_TempWarning", UI::RenderableRect(Vector2(0.25, 0.3), Vector2(0.5, 0.3), true), Whole(18), "Future spot of level reports.");
    TempCapt->SetBackgroundColour(Transparent);
    UI::TextButton* FinishButton = LevelReport->CreateTextButton("GS_Finish", UI::RenderableRect(Vector2(0.42, 0.66), Vector2(0.16, 0.08), true), Whole(14), "Finish");
    FinishButton->SetButtonCallback(new GSMMReturn());
    FinishButton->SetBackgroundColour(TransBlack);
    Report->Hide();
    //End of Report Layer
    //End of Game Screen
    GameScreen->Hide();
}

void CatchApp::CreateLoadingScreen()
{
    UIManager* GUI = TheWorld->GetUIManager();
    GUI->LoadGorilla("Catch_Loading");
    Viewport* UIViewport = TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetViewport(0);
    UI::Screen* LoadScreen = GUI->CreateScreen("LoadingScreen", "Catch_Loading", UIViewport);
    UI::Layer* LoadLayer = LoadScreen->CreateLayer("LoadingLayer", 0);
    UI::Rectangle* Load = LoadLayer->CreateRectangle( UI::RenderableRect(Vector2(-0.16667,0), Vector2(1.33334,1), true));
    Load->SetBackgroundSprite("BTSBanner");
    //LoadScreen->RenderOnce();
    crossplatform::RenderPhysWorld();
    Load->SetBackgroundSprite("LoadingBackground");
}

void CatchApp::ConfigResources()
{
    ResourceManager* ResourceMan = TheWorld->GetResourceManager();
    String CommonGroup("Common");
    String datadir = "Data/";
    ResourceMan->AddResourceLocation(datadir, "FileSystem", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"Common.zip", "Zip", CommonGroup, false);
    ResourceMan->AddResourceLocation(datadir+"AdvThrowables.zip", "Zip", CommonGroup, false);
    ResourceMan->AddResourceLocation("Previews/", "FileSystem", CommonGroup, false);
    ResourceMan->InitResourceGroup(CommonGroup);
    /// @todo Change this to parse an actual file
    ThrowableGenerator::ParseThrowables("");
}

void CatchApp::PopulateScoreValues()
{
    ItemScoreValues["Gold"] = 100;
    ItemScoreValues["Pyrite"] = 50;
    ItemScoreValues["Clay"] = 10;
    ItemScoreValues["Uranium"] = 100;
    ItemScoreValues["Rubber"] = 30;
    ItemScoreValues["Lead"] = 30;
    ItemScoreValues["Styrofoam"] = 10;
    ItemScoreValues["Wood"] = 20;
}

void CatchApp::PopulateShopValues()
{
    ShopCostValues["Wooden Plank"] = 50;
}

void CatchApp::PopulateLevelList(UI::PagedCellGrid* Grid)
{
    ResourceManager* ResourceMan = World::GetWorldPointer()->GetResourceManager();
    UIManager* UIMan = World::GetWorldPointer()->GetUIManager();
    ResourceMan->AddResourceLocation("/Previews","FileSystem","Previews",false);
    ResourceMan->InitResourceGroup("Previews");
    UI::RenderableRect CellRect(Vector2(0.1,0.1),Vector2(0.33,0.11),true);
    Vector2 CellSpacing(0.06,0.04);
    Grid->SetFixedCellSize(CellRect.Size);
    Grid->SetCellSpacing(CellSpacing);
    UI::Layer* ParentLayer = Grid->GetLayer();

    std::set<String>* Files = crossplatform::GetDirContents("./Levels");
    if(Files->empty())
        return;
    std::set<String>* Previews = crossplatform::GetDirContents("./Previews");
    for( std::set<String>::iterator it = Files->begin() ; it != Files->end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".lvl"))
            continue;

        String LevelName = FileName.substr(0,(*it).size() - 4);
        LevelSelectCell* CurrCell = new LevelSelectCell(LevelName,CellRect,ParentLayer);
        CurrCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
        CurrCell->GetPreviewBorder()->SetBackgroundSprite("MMLevelPreviewBox");
        CurrCell->GetLevelTitle()->SetText(LevelName);
        CurrCell->GetLevelTitle()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        /// @todo Here's where we would get existing scores and put them in the cells.
        /*String Score;
        String MaxScore;
        String EarnedScore;
        Score+=(EarnedScore+"/"+MaxScore);
        CurrCell->GetEarnedMaxScore()->SetText(Score);// */
        CurrCell->GetEarnedMaxScore()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        if(Previews->count(LevelName+".gorilla"))
        {
            UIMan->LoadGorilla(LevelName);
            CurrCell->GetPreviewImage()->SetBackgroundSprite("LevelPreview",LevelName);
        }
        CurrCell->SetCellCallback(new LevelSelectCB());
        Grid->AddCell(CurrCell);
    }
}

void CatchApp::ChangeState(const CatchApp::GameState &StateToSet)
{
    if(CurrentState == StateToSet)
        return;
    UIManager* UIMan = TheWorld->GetUIManager();
    switch (StateToSet)
    {
        case CatchApp::Catch_GameScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("GameScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_Loading:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("LoadingScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
        case CatchApp::Catch_MenuScreen:
        {
            for( Whole Screen = 0 ; Screen < UIMan->GetNumScreens() ; Screen++ )
            {
                UI::Screen* CurrScreen = UIMan->GetScreen(Screen);
                if("MainMenuScreen" == CurrScreen->GetName())
                    CurrScreen->Show();
                else
                    CurrScreen->Hide();
            }
            break;
        }
    }
    CurrentState = StateToSet;
}

bool CatchApp::CheckEndOfLevel()
{
    if(ScoreAreas.empty())
        return false;
    if(!EndTimer)
    {
        EndTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::StopWatch);
        EndTimer->SetInitialTime(5 * 1000000);
        EndTimer->SetCurrentTime(5 * 1000000);
        EndTimer->SetGoalTime(0);
        EndTimer->Start();
    }
    for( Whole SA = 0 ; SA < ScoreAreas.size() ; SA++ )
    {
        std::vector<ActorBase*>& FinalTest = ScoreAreas[SA]->GetAddedActors();
        for( Whole F = 0 ; F < FinalTest.size() ; F++ )
        {
            if(LastActorThrown == FinalTest[F])
                EndTimer->Reset();
        }
    }
    if(EndTimer->IsStopped())
        return true;
    else
        return false;
}

bool CatchApp::AllStartZonesEmpty()
{
    if(StartAreas.empty())
        return false;
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(!StartAreas[X]->IsEmpty())
            return false;
    }
    return true;
}

bool CatchApp::IsInsideAnyStartZone(ActorBase* Actor)
{
    if(StartAreas.empty())
        return false;
    for( Whole X = 0 ; X < StartAreas.size() ; X++ )
    {
        if(StartAreas[X]->IsInside(Actor))
            return true;
    }
    return false;
}

void CatchApp::UnloadLevel()
{
    if("MainMenu"==Loader->GetCurrentLevel())
        return;
    PhysicsManager* PhysMan = TheWorld->GetPhysicsManager();
    ResourceManager* ResMan = TheWorld->GetResourceManager();
    SceneManager* SceneMan = TheWorld->GetSceneManager();
    ActorManager* ActorMan = TheWorld->GetActorManager();
    TimerManager* TimeMan = TheWorld->GetTimerManager();
    EventManager* EventMan = TheWorld->GetEventManager();
    CollisionShapeManager* CShapeMan = TheWorld->GetCollisionShapeManager();
    MeshManager* MeshMan = TheWorld->GetMeshManager();
    UIManager* UIMan = TheWorld->GetUIManager();

    PhysMan->DestroyAllConstraints();
    ActorMan->DestroyAllActors();
    PhysMan->DestroyAllWorldTriggers();
    PhysMan->DestroyAllAreaEffects();
    CShapeMan->DestroyAllShapes();
    MeshMan->DestroyAllGeneratedMeshes();
    SceneMan->DestroyAllLights();
    SceneMan->DestroyAllParticleEffects();
    SceneMan->DestroyAllWorldNodes();
    SceneMan->DisableSky();
    //ActorMan->DestroyAllActors();
    StartAreas.clear();
    ScoreAreas.clear();
    PlayZone = NULL;

    ResMan->DestroyResourceGroup(Loader->GetCurrentLevel());
    PhysMan->ClearPhysicsMetaData();
    /// @todo Probably should make a "RemoveAll" for the events as well.
    for( EventCollision* OneCollision = EventMan->PopNextCollisionEvent() ; NULL != OneCollision ;  )
    {
        delete OneCollision;
        OneCollision = EventMan->PopNextCollisionEvent();
    }
    CurrScore = 0;
    TimeMan->DestroyTimer(EndTimer);
    EndTimer = NULL;
    UIMan->GetLayer("ReportLayer")->Hide();
    UIMan->GetLayer("MenuLayer")->Hide();
    PhysMan->PauseSimulation(false);
}

CatchApp* CatchApp::GetCatchAppPointer()
{
    assert(0!=CatchApp::TheRealCatchApp);
    return TheRealCatchApp;
}

int CatchApp::GetCatchin()
{
    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&CPreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&CPostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&CPrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&CPostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&CPostRender);
    TheWorld->GetUIManager()->SetPreMainLoopItems(&CPreUI);
    TheWorld->GetUIManager()->SetPostMainLoopItems(&CPostUI);

    const GraphicsSettings& DefSet = TheWorld->GetGraphicsManager()->GetDefaultSettings();
    int WindowFlags = (DefSet.Fullscreen?GameWindow::WF_Fullscreen:0) | GameWindow::WF_FSAA_4;
    TheWorld->GetGraphicsManager()->CreateGameWindow("",DefSet.RenderWidth,DefSet.RenderHeight,WindowFlags);

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);

	TheWorld->GetUIManager()->EnableButtonAutoRegister(true);
    TheWorld->GetUIManager()->AddAutoRegisterCode(MetaCode::MOUSEBUTTON_1);

    TheWorld->GetPhysicsManager()->SetSimulationSubstepModifier(2);

    ConfigResources();
	CreateLoadingScreen();
	ChangeState(CatchApp::Catch_Loading);

	PopulateScoreValues();
    PopulateShopValues();

    //Set logging frequency
    TheWorld->CommitLog();
    TheWorld->SetLoggingFrequency(World::LogNever);

	// Set the Title
    TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->SetWindowCaption("Catch!");
    TheWorld->SetTargetFrameRate(60);

    // Create the Timer
    LevelTimer = TheWorld->GetTimerManager()->CreateSimpleTimer(Timer::Normal);

    //Generate the UI
    MakeGUI();

    Loader->SetNextLevel("MainMenu");
    do{
        ChangeState(CatchApp::Catch_Loading);
        crossplatform::RenderPhysWorld();
        //Actually Load the game stuff
        Loader->LoadLevel();

        if("MainMenu"==Loader->GetCurrentLevel())
            ChangeState(CatchApp::Catch_MenuScreen);
        else
            ChangeState(CatchApp::Catch_GameScreen);
        LevelTimer->Reset();
        LevelTimer->Start();

        //if("Ferris"==Loader->GetCurrentLevel())
        //    TheWorld->GetUIManager()->GetScreen("GameScreen")->Hide();
        //TheWorld->GetPhysicsManager()->PauseSimulation(true);
        //Start the Main Loop
        TheWorld->MainLoop();
        UnloadLevel();
        LevelTimer->Stop();
    } while(Loader->HasALevelToLoad());

	return 0;
}

bool CatchApp::PreInput()
{
    // using the Raw Event Manager, and deleting the events
    if( !CheckForStuff() )
        return false;
    return true;
}

bool CatchApp::PostInput()
{
    CameraController* DefaultControl = TheWorld->GetCameraManager()->GetOrCreateCameraController(TheWorld->GetCameraManager()->GetDefaultCamera());
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_LEFT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_A))
        DefaultControl->StrafeLeft(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_D))
        DefaultControl->StrafeRight(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_UP) || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_W))
        DefaultControl->MoveForward(300 * (TheWorld->GetFrameTime() * 0.001));
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_DOWN)  || InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_S))
        DefaultControl->MoveBackward(300 * (TheWorld->GetFrameTime() * 0.001));
    static bool MouseCam=false;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_HOME) )
        MouseCam=true;
    if( InputQueryTool::IsKeyboardButtonPushed(MetaCode::KEY_END))
        MouseCam=false;
    Vector2 Offset = InputQueryTool::GetMousePrevFrameOffset();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( MetaCode::BUTTON_PRESSING == InputQueryTool::GetKeyboardButtonState(MetaCode::KEY_C) )
    {
        if(TheWorld->GetPhysicsManager()->GetDebugPhysicsRendering())
        {
            TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(0);
            TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);
        }else{
            TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
            TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(1);
        }
    }

    return true;
}

bool CatchApp::PreUI()
{
    return true;
}

bool CatchApp::PostUI()
{
    //User Input through a WorldQueryTool
    static RayQueryTool* RayQueryer = new RayQueryTool();
    static InputQueryTool* InputQueryer = new InputQueryTool();

    //Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(InputQueryer->GetMouseX());
    TheWorld->Log(InputQueryer->GetMouseY());

    static Point2PointConstraint* Dragger=NULL;

    if( InputQueryer->IsMouseButtonPushed(1) )
    {
        if(!TheWorld->GetUIManager()->MouseIsInUISystem())
        {
            Ray *MouseRay = RayQueryer->GetMouseRay(5000);
            //*MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = RayQueryer->GetFirstActorOnRayByPolygon(*MouseRay,phys::WOT_ActorRigid);
            #ifdef PHYSDEBUG
            TheWorld->LogStream << "MouseRay: " << *MouseRay << "| Length: " << MouseRay->Length() << endl;
            TheWorld->Log("PlaneOfPlay"); TheWorld->Log(PlaneOfPlay);
            #endif

            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if(0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("No Actor Clicked on");
                #endif
            }else if(!IsInsideAnyStartZone(ClickOnActor->Actor)){
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor is not in any starting zone");
                #endif
            }else{
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor Clicked on");
                TheWorld->Log(*ClickOnActor);
                TheWorld->Log(*ClickOnActor);
                //TheWorld->Log("ClickOnActor"); TheWorld->Log(*ClickOnActor);
                #endif
                if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
                {
                    if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                    {
                        if(ClickOnActor->Actor->GetType()==ActorBase::Actorrigid) //This is Dragging let's do some checks for sanity
                        {
                            Vector3 LocalPivot = ClickOnActor->Vector;
                            ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                            rigid->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
                            //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                            Dragger = new Point2PointConstraint(rigid, LocalPivot);
                            Dragger->SetTAU(0.001);
                            //Dragger->SetLinearLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetLinearUpperLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularUpperLimit(Vector3(0.f,0.f,0.f));
                            TheWorld->GetPhysicsManager()->AddConstraint(Dragger);
                            Dragger->SetParam(Con_Stop_CFM,0.8,0); Dragger->SetParam(Con_Stop_CFM,0.8,1); Dragger->SetParam(Con_Stop_CFM,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                            Dragger->SetParam(Con_Stop_ERP,0.1,0); Dragger->SetParam(Con_Stop_ERP,0.1,1); Dragger->SetParam(Con_Stop_ERP,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                            firstframe=true;
                            LastActorThrown = rigid;
                        }else{  // since we don't
                            #ifdef PHYSDEBUG
                            TheWorld->Log("Actor is not an ActorRigid.  Aborting.");
                            #endif
                        }
                    }
                }else{
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Actor is Static/Kinematic.  Aborting.");
                    #endif
                }
            }

            // This chunk of code calculates the 3d point that the actor needs to be dragged to
            Vector3 *DragTo = RayQueryer->RayPlaneIntersection(*MouseRay, PlaneOfPlay);
            if (0 == DragTo)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("PlaneOfPlay Not Clicked on");
                #endif
            }else{
                if(Dragger && !firstframe)
                {
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Dragged To");
                    TheWorld->Log(*DragTo);
                    #endif
                    Dragger->SetPivotBLocation(*DragTo);
                }
            }

            if(Dragger && !IsInsideAnyStartZone(LastActorThrown))
            {
                ActorRigid* Act = Dragger->GetActorA();
                TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
                delete Dragger;
                Dragger=NULL;
                Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
            }

            // Here we cleanup everything we needed for the clicking/dragging
            if ( DragTo )
                { delete DragTo; }
            if ( MouseRay )
                { delete MouseRay; }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
            Act->GetPhysicsSettings()->SetActivationState(phys::AAS_DisableDeactivation);
        }
    }
    return true;
}

bool CatchApp::PrePhysics()
{
    return true;
}

bool CatchApp::PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->Actors->FindActor("Column1"));
    //ActS->UpdateSoftBody();

    for( Whole S = 0 ; S < ScoreAreas.size() ; S++ )
    {
        std::vector<ActorBase*>& Added = ScoreAreas[S]->GetAddedActors();
        std::vector<ActorBase*>& Removed = ScoreAreas[S]->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            String ItemName = Added[A]->GetName();
            for( std::map<String,Whole>::iterator Ait = ItemScoreValues.begin() ; Ait != ItemScoreValues.end() ; Ait++ )
            {
                if(ItemName.find((*Ait).first) != String::npos)
                    CurrScore += (*Ait).second;
            }
        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            String ItemName = Removed[R]->GetName();
            for( std::map<String,Whole>::iterator Rit = ItemScoreValues.begin() ; Rit != ItemScoreValues.end() ; Rit++ )
            {
                if(ItemName.find((*Rit).first) != String::npos)
                    CurrScore -= (*Rit).second;
            }
        }
    }

    return true;
}

bool CatchApp::PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld->Log(String("---------- Starting PosterRender CallBack -------------"));
    TheWorld->Log(String("Current Game Time "));

	//getting a message from the event manager)
	EventRenderTime* CurrentTime = TheWorld->GetEventManager()->PopNextRenderTimeEvent();
    Whole LastFrame = 0;

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {
        LastFrame = CurrentTime->getMilliSecondsSinceLastFrame();

        TheWorld->Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld->GetEventManager()->GetNextRenderTimeEvent();
    }

    // Update the timer
    UI::Caption* Timer = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_Timer");
    std::stringstream time;
    Whole TotalSeconds = LevelTimer->GetCurrentTimeInMilli() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;
    if(60 > TotalSeconds)
    {
        Seconds = TotalSeconds;
    }else{
        Seconds = TotalSeconds % 60;
    }
    if(10 > Seconds)
    {
        time << Minutes << ":" << 0 << Seconds;
    }else{
        time << Minutes << ":" << Seconds;
    }
    Timer->SetText(time.str());

    // Update the score
    UI::Caption* ScoreAmount = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("HUDLayer")->GetCaption("GS_ScoreArea");
    std::stringstream Score;
    Score << CurrScore;
    String ScoreOut = Score.str();
    ScoreAmount->SetText(ScoreOut);

    // Update Stat information
    UI::Caption* CurFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("CurFPS");
    UI::Caption* AvFPS = TheWorld->GetUIManager()->GetScreen("GameScreen")->GetLayer("StatsLayer")->GetCaption("AvFPS");
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetPrimaryGameWindow()->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);

    //See if the level is over
    if(AllStartZonesEmpty())
    {
        if(CheckEndOfLevel())
        {
            TheWorld->GetPhysicsManager()->PauseSimulation(true);
            TheWorld->GetUIManager()->GetLayer("ReportLayer")->Show();
        }
    }

    return true;
}

bool CatchApp::CheckForStuff()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { TheWorld->LogAndThrow("Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && MetaCode::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }

        delete OneInput;
        OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { TheWorld->LogAndThrow("Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            TheWorld->Log("Invalid EventID on GameWindow Event");
            TheWorld->LogAndThrow(OneWindowEvent->GetEventID());
        }

        TheWorld->Log(*OneWindowEvent);
        TheWorld->Log(EventGameWindow::GameWindowEventIDToString(OneWindowEvent->GetEventID()));
        stringstream eventxml;
        eventxml << *OneWindowEvent;    // Test XML conversion and reconstruction
        EventGameWindow AnotherWindowEvent(EventGameWindow::GAME_WINDOW_NONE,0,0);
        eventxml >> AnotherWindowEvent;
        TheWorld->Log(AnotherWindowEvent);

        if (OneWindowEvent->GetEventID()==EventGameWindow::GAME_WINDOW_MINIMIZED)
        {
            Audio::Sound* Welcome = NULL;
            Welcome = TheWorld->GetAudioManager()->GetSoundByName("Welcome");
            if(Welcome)
            {
                Welcome->Play2d(false);
            }
        }

        delete OneWindowEvent;
        OneWindowEvent = TheWorld->GetEventManager()->PopNextGameWindowEvent();
    }

    EventCollision* OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    EventCollision SecondCollision(0,0,Vector3(0.5,0.5,0.5),Vector3(1.5,1.5,1.5),Vector3(1,1,1),2.5);
    while(0 != OneCollision)
    {
        if(OneCollision->GetType() != EventBase::Collision)
            { TheWorld->LogAndThrow("Trying to process a non-EventCollision as an EventCollision."); }

        stringstream temp;
        temp << *OneCollision;
        temp >> SecondCollision;
        TheWorld->Log(SecondCollision);

        delete OneCollision;
        OneCollision = TheWorld->GetEventManager()->PopNextCollisionEvent();
    }

    return true;
}

void CatchApp::SetPlayArea(AreaOfPlay* PlayArea)
{
    PlayZone = PlayArea;
}

void CatchApp::RegisterScoreArea(ScoreArea* Score)
{
    ScoreAreas.push_back(Score);
}

void CatchApp::RegisterStartArea(StartingArea* Start)
{
    StartAreas.push_back(Start);
}

void CatchApp::AddThrowable(ActorBase* Throwable)
{
    ThrownItems.push_back(Throwable);
}

LevelLoader* CatchApp::GetLevelLoader()
{
    return Loader;
}

SimpleTimer* CatchApp::GetLevelTimer()
{
    return LevelTimer;
}

SimpleTimer* CatchApp::GetEndTimer()
{
    return EndTimer;
}

#endif
