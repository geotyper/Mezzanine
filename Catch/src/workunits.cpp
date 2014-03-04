#ifndef _workunits_cpp
#define _workunits_cpp

#include "workunits.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// QueuedSubscriber Methods

QueuedSubscriber::QueuedSubscriber(UI::UIManager* UIMan, ManagerBase* Target) :
    TargetManager(Target),
    UITarget(UIMan),
    Updating(false)
    {  }

QueuedSubscriber::~QueuedSubscriber()
    { this->ClearEvents(); }

///////////////////////////////////////////////////////////////////////////////
// Convenience and Utility

QueuedSubscriber::ConstEventIterator QueuedSubscriber::GetFirstEvent() const
    { return this->Events.begin(); }

QueuedSubscriber::ConstEventIterator QueuedSubscriber::GetEndEvent() const
    { return this->Events.end(); }

void QueuedSubscriber::ClearEvents()
    { this->Events.clear(); }

///////////////////////////////////////////////////////////////////////////////
// Inherited

void QueuedSubscriber::_NotifyEvent(EventArgumentsPtr Args)
{
    if( !this->Updating ) {
        this->Events.push_back(Args);
    }
}

///////////////////////////////////////////////////////////////////////////////
// AudioSettingsWorkUnit Methods

AudioSettingsWorkUnit::AudioSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target)
    { this->SettingsSubscriber = new QueuedSubscriber(UITarget,Target); }

AudioSettingsWorkUnit::~AudioSettingsWorkUnit()
    { delete this->SettingsSubscriber; }

QueuedSubscriber* AudioSettingsWorkUnit::GetSettingsSubscriber() const
    { return this->SettingsSubscriber; }

void AudioSettingsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    this->SettingsSubscriber->Updating = true;
    UI::UIManager* UIMan = this->SettingsSubscriber->UITarget;
    Audio::AudioManager* AudioMan = static_cast<Audio::AudioManager*>( this->SettingsSubscriber->TargetManager );
    QueuedSubscriber::ConstEventIterator EndEvent = this->SettingsSubscriber->GetEndEvent();
    for( QueuedSubscriber::ConstEventIterator EvIt = this->SettingsSubscriber->GetFirstEvent() ; EvIt != EndEvent ; ++EvIt )
    {
        EventArgumentsPtr Args = (*EvIt);
        UI::WidgetEventArgumentsPtr WidArgs = CountedPtrCast<UI::WidgetEventArguments>( Args );
        if( WidArgs->EventName == UI::Scrollbar::EventScrollValueChanged ) {
            UI::ScrollbarValueChangedArgumentsPtr ScrollArgs = CountedPtrCast<UI::ScrollbarValueChangedArguments>( WidArgs );
            // Check for "MS_MusicVolume" or "GS_MusicVolume"
            // Else check for "MS_EffectsVolume" or "GS_EffectsVolume"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MusicVolume", true ) ) {
                AudioMan->SetMusicVolume( ScrollArgs->NewScrollerValue );
            }else if( StringTools::EndsWith( WidArgs->WidgetName, "_EffectsVolume", true ) ) {
                AudioMan->SetEffectVolume( ScrollArgs->NewScrollerValue );
            }
        }else if( WidArgs->EventName == UI::CheckBox::EventSelected ) {
            // Check for "MS_MuteBox" or "GS_MuteBox"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                AudioMan->SetMasterMute(true);
            }
        }else if( WidArgs->EventName == UI::CheckBox::EventDeselected ) {
            // Check for "MS_MuteBox" or "GS_MuteBox"
            if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                AudioMan->SetMasterMute(false);
            }
        }else if( WidArgs->EventName == UI::PagedContainer::EventChildFocusGained ) {
            //
            // Fill in later
            //
        }else if( WidArgs->EventName == UI::Widget::EventVisibilityShown ) {
            // Start by getting the proper screen
            UI::Screen* EventScreen = NULL;
            // Check for "M" from "MS_" prefix on menu screen widgets.
            // Else check for "G" from "GS_" prefix on game screen widgets.
            if( WidArgs->WidgetName.at(0) == 'M' ) {
                EventScreen = UIMan->GetScreen("MainMenuScreen");
            }else if( WidArgs->WidgetName.at(0) == 'G' ) {
                EventScreen = UIMan->GetScreen("GameScreen");
            }
            // Proceed only if we have a valid screen
            if( EventScreen != NULL ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_MusicVolume", true ) ) {
                    // Update the widget to the current system music volume
                    UI::Scrollbar* MusicVol = static_cast<UI::Scrollbar*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    MusicVol->SetScrollerValue( AudioMan->GetMusicVolume() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_EffectsVolume", true ) ) {
                    // Update the widget to the current system effects volume
                    UI::Scrollbar* EffectsVol = static_cast<UI::Scrollbar*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    EffectsVol->SetScrollerValue( AudioMan->GetEffectVolume() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_MuteBox", true ) ) {
                    // Update the widget to the current system mute setting
                    UI::CheckBox* MuteBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    MuteBox->ManualSelect( AudioMan->IsMuted() );
                }/*else if( StringTools::EndsWith( WidArgs->WidgetName, "_AudioDeviceList", true ) ) {
                    UI::DropDownList* AudioDeviceList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* DeviceContainer = AudioDeviceList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( DeviceContainer->GetNumListItems() == 0 ) {
                        Whole DetectedDevices = AudioMan->GetAvailablePlaybackDeviceCount();
                        for( Whole DeviceIndex = 0 ; DeviceIndex < DetectedDevices ; ++DeviceIndex )
                        {
                            String DeviceName = AudioMan->GetAvailablePlaybackDeviceName(DeviceIndex);
                            DeviceContainer->CreateSingleLineListItem(DeviceName,DeviceName);
                        }
                        AudioDeviceList->UpdateChildDimensions();
                    }
                    AudioDeviceList->UpdateCurrentSelection( DeviceContainer->GetListItem( AudioMan->GetCurrentPlaybackDeviceName() ) );
                }//*/
            }
        }
    }
    this->SettingsSubscriber->ClearEvents();
    this->SettingsSubscriber->Updating = false;
}

///////////////////////////////////////////////////////////////////////////////
// VideoSettingsWorkUnit Methods

VideoSettingsWorkUnit::VideoSettingsWorkUnit(UI::UIManager* UITarget, ManagerBase* Target)
    { this->SettingsSubscriber = new QueuedSubscriber(UITarget,Target); }

VideoSettingsWorkUnit::~VideoSettingsWorkUnit()
    { delete this->SettingsSubscriber; }

QueuedSubscriber* VideoSettingsWorkUnit::GetSettingsSubscriber() const
    { return this->SettingsSubscriber; }

void VideoSettingsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    this->SettingsSubscriber->Updating = true;
    UI::UIManager* UIMan = this->SettingsSubscriber->UITarget;
    Graphics::GraphicsManager* GraphicsMan = static_cast<Graphics::GraphicsManager*>( this->SettingsSubscriber->TargetManager );
    Graphics::GameWindow* CatchWindow = GraphicsMan->GetGameWindow(0);
    QueuedSubscriber::ConstEventIterator EndEvent = this->SettingsSubscriber->GetEndEvent();
    for( QueuedSubscriber::ConstEventIterator EvIt = this->SettingsSubscriber->GetFirstEvent() ; EvIt != EndEvent ; ++EvIt )
    {
        EventArgumentsPtr Args = (*EvIt);
        UI::WidgetEventArgumentsPtr WidArgs = CountedPtrCast<UI::WidgetEventArguments>( Args );

        // Start by getting the proper screen
        UI::Screen* EventScreen = NULL;
        // Check for "M" from "MS_" prefix on menu screen widgets.
        // Else check for "G" from "GS_" prefix on game screen widgets.
        if( WidArgs->WidgetName.at(0) == 'M' ) {
            EventScreen = UIMan->GetScreen("MainMenuScreen");
        }else if( WidArgs->WidgetName.at(0) == 'G' ) {
            EventScreen = UIMan->GetScreen("GameScreen");
        }

        // Proceed only if we have a valid screen
        if( EventScreen != NULL ) {
            if( WidArgs->EventName == UI::Button::EventDeactivated ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_VideoOptsApply", true ) ) {
                    String NamePrefix;
                    if( EventScreen->GetName() == "MainMenuScreen" ) {
                        NamePrefix = "MS";
                    }else if( EventScreen->GetName() == "GameScreen" ) {
                        NamePrefix = "GS";
                    }

                    // Get our pointers
                    Graphics::GameWindow* CatchWindow = GraphicsMan->GetGameWindow(0);
                    UI::UIManager* UIMan = EventScreen->GetManager();
                    UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
                    // Create our window related data
                    Graphics::WindowSettings NewSettings;
                    NewSettings.Fullscreen = static_cast<UI::CheckBox*>( EventScreen->GetWidget(NamePrefix + "_FullscreenBox") )->IsSelected();
                    // Get render resolution data
                    String SelectedRes = static_cast<UI::DropDownList*>( EventScreen->GetWidget(NamePrefix + "_ResolutionList") )->GetSelectionText();
                    String StrWidth = SelectedRes.substr(0,SelectedRes.find_first_of("x"));
                    String StrHeight = SelectedRes.substr(SelectedRes.find_last_of("x") + 1);
                    NewSettings.WinRes.Width = StringTools::ConvertToWhole(StrWidth);
                    NewSettings.WinRes.Height = StringTools::ConvertToWhole(StrHeight);
                    // Apply the resolution and fullscreen settings
                    CatchWindow->SetRenderOptions(NewSettings);//*/
                    // Now get the FSAA setting and pass it along
                    String FSAASetting = static_cast<UI::DropDownList*>( EventScreen->GetWidget(NamePrefix + "_FSAAList") )->GetSelectionText();
                    CatchWindow->SetFSAALevel( StringTools::ConvertToWhole( FSAASetting.substr(1,1) ) );

                    // Apply other settings
                    UI::CheckBox* FPSStatsBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget(NamePrefix + "_StatsBox") );
                    StatsScreen->GetWidget("SS_CurrentFPS")->SetVisible( FPSStatsBox->IsSelected() );
                    StatsScreen->GetWidget("SS_AverageFPS")->SetVisible( FPSStatsBox->IsSelected() );//*/
                }
            }else if( WidArgs->EventName == UI::Widget::EventVisibilityShown ) {
                if( StringTools::EndsWith( WidArgs->WidgetName, "_FSAAList", true ) ) {
                    UI::DropDownList* FSAAList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* FSAAContainer = FSAAList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( FSAAContainer->GetNumListItems() == 0 ) {
                        FSAAContainer->CreateSingleLineListItem("No AA","No AA");
                        for( Whole FSAALevel = 1 ; FSAALevel <= 4 ; ++FSAALevel )
                        {
                            Whole Temp = 2;
                            for( Whole X = 1 ; X < FSAALevel ; ++X )
                                { Temp <<= 1; }

                            StringStream FSAALevelName;
                            FSAALevelName << "x" << Temp << " AA";
                            FSAAContainer->CreateSingleLineListItem(FSAALevelName.str(),FSAALevelName.str());
                        }
                        FSAAList->UpdateChildDimensions();
                    }
                    Whole FSAALevel = CatchWindow->GetFSAALevel();
                    if( FSAALevel == 0 ) {
                        FSAAList->UpdateCurrentSelection( FSAAContainer->GetListItem( "No AA" ) );
                    }else{
                        StringStream CurrentFSAAName;
                        CurrentFSAAName << "x" << FSAALevel << " AA";
                        FSAAList->UpdateCurrentSelection( FSAAContainer->GetListItem( CurrentFSAAName.str() ) );
                    }
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_ResolutionList", true ) ) {
                    UI::DropDownList* ResolutionList = static_cast<UI::DropDownList*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::ListBox* ResolutionContainer = ResolutionList->GetSelectionList();
                    // If we have no items, we need to populate them
                    if( ResolutionContainer->GetNumListItems() == 0 ) {
                        const Graphics::GraphicsManager::ResolutionContainer& DetectedResolutions = GraphicsMan->GetSupportedResolutions();
                        for( Graphics::GraphicsManager::ConstResolutionIterator ResIt = DetectedResolutions.begin() ; ResIt != DetectedResolutions.end() ; ++ResIt )
                        {
                            String ResolutionName = (*ResIt).GetAsString();
                            ResolutionContainer->CreateSingleLineListItem(ResolutionName,ResolutionName);
                        }
                        ResolutionList->UpdateChildDimensions();
                    }
                    ResolutionList->UpdateCurrentSelection( ResolutionContainer->GetListItem( CatchWindow->GetResolution().GetAsString() ) );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_FullscreenBox", true ) ) {
                    UI::CheckBox* FullscreenBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    FullscreenBox->ManualSelect( CatchWindow->GetFullscreen() );
                }else if( StringTools::EndsWith( WidArgs->WidgetName, "_StatsBox", true ) ) {
                    UI::CheckBox* StatsBox = static_cast<UI::CheckBox*>( EventScreen->GetWidget( WidArgs->WidgetName ) );
                    UI::Screen* StatsScreen = UIMan->GetScreen("StatsScreen");
                    StatsBox->ManualSelect( StatsScreen->GetWidget("SS_CurrentFPS")->GetVisible() || StatsScreen->GetWidget("SS_AverageFPS")->GetVisible() );
                }// Is a widget we're looking for
            }// Is an event we're looking for
        }// If we have a screen
    }// For each event
    this->SettingsSubscriber->ClearEvents();
    this->SettingsSubscriber->Updating = false;
}

///////////////////////////////////////////////////////////////////////////////
// CatchPreInputWorkUnit Methods

CatchPreInputWorkUnit::CatchPreInputWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPreInputWorkUnit::~CatchPreInputWorkUnit()
    {  }

void CatchPreInputWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    EventManager* EventMan = EventManager::GetSingletonPtr();
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = EventMan->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        if(OneInput->GetType()!=EventBase::UserInput)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventUserInput as an EventUserInput."); }

        //we check each MetaCode in each Event
        /*for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            //Is the key we just pushed ESCAPE
            if(Input::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && Input::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }// */

        delete OneInput;
        OneInput = EventMan->PopNextUserInputEvent();
    }

    EventGameWindow* OneWindowEvent = EventMan->PopNextGameWindowEvent();
    while(0 != OneWindowEvent)
    {
        if(OneWindowEvent->GetType()!=EventBase::GameWindow)
            { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Trying to process a non-EventGameWindow as an EventGameWindow."); }

        if(!OneWindowEvent->IsEventIDValid())
        {
            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid EventID on GameWindow Event: " + OneWindowEvent->GetEventID());
        }

        delete OneWindowEvent;
        OneWindowEvent = EventMan->PopNextGameWindowEvent();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostInputWorkUnit Methods

CatchPostInputWorkUnit::CatchPostInputWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostInputWorkUnit::~CatchPostInputWorkUnit()
    {  }

void CatchPostInputWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    Graphics::CameraManager* CamMan = Entresol::GetSingletonPtr()->GetCameraManager();
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    Input::Keyboard* SysKeyboard = InputMan->GetSystemKeyboard();
    CameraController* DefaultControl = CamMan->GetOrCreateCameraController(CamMan->GetCamera(0));
    if( SysKeyboard->IsButtonPressed(Input::KEY_LEFT) || SysKeyboard->IsButtonPressed(Input::KEY_A) )
        DefaultControl->StrafeLeft(300 * (this->CatchApplication->TheEntresol->GetLastFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_RIGHT) || SysKeyboard->IsButtonPressed(Input::KEY_D) )
        DefaultControl->StrafeRight(300 * (this->CatchApplication->TheEntresol->GetLastFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_UP) || SysKeyboard->IsButtonPressed(Input::KEY_W) )
        DefaultControl->MoveForward(300 * (this->CatchApplication->TheEntresol->GetLastFrameTimeMilliseconds() * 0.001));
    if( SysKeyboard->IsButtonPressed(Input::KEY_DOWN)  || SysKeyboard->IsButtonPressed(Input::KEY_S) )
        DefaultControl->MoveBackward(300 * (this->CatchApplication->TheEntresol->GetLastFrameTimeMilliseconds() * 0.001));
    static bool MouseCam = false;
    if( SysKeyboard->IsButtonPressing(Input::KEY_HOME) )
        MouseCam = true;
    if( SysKeyboard->IsButtonPressing(Input::KEY_END) )
        MouseCam = false;
    Vector2 Offset = SysMouse->GetMouseDelta();
    if( MouseCam && Vector2(0,0) != Offset )
        DefaultControl->Rotate(Offset.X * 0.01,Offset.Y * 0.01,0);

    if( Input::BUTTON_PRESSING == SysKeyboard->GetButtonState(Input::KEY_C) )
    {
        Physics::PhysicsManager* PhysMan = Entresol::GetSingletonPtr()->GetPhysicsManager();
        if(PhysMan->GetDebugRenderingMode())
        {
            PhysMan->SetDebugRenderingMode(Physics::DDM_NoDebug);
        }else{
            PhysMan->SetDebugRenderingMode(Physics::DDM_DrawWireframe);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostUIWorkUnit Methods

CatchPostUIWorkUnit::CatchPostUIWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostUIWorkUnit::~CatchPostUIWorkUnit()
    {  }

void CatchPostUIWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
    Input::Mouse* SysMouse = InputMan->GetSystemMouse();
    static Physics::Point2PointConstraint* Dragger = NULL;

    if( SysMouse->IsButtonPressed(1) )
    {
        if( !UI::UIManager::GetSingletonPtr()->MouseIsInUISystem() )
        {
            Ray MouseRay = RayQueryTool::GetMouseRay(5000);

            bool firstframe=false;
            if( RayCaster.GetFirstObjectOnRayByPolygon(MouseRay,Mezzanine::WO_DebrisRigid | Mezzanine::WO_DebrisSoft) )
            {
                Debris* CastResult = static_cast<Debris*>( RayCaster.LastQueryResultsObjectPtr() );
                Vector3 LocalPivot = RayCaster.LastQueryResultsOffset();
                if( CastResult->GetType() == Mezzanine::WO_DebrisRigid &&
                    this->CatchApplication->IsInsideAnyStartZone( CastResult ) &&
                    !Dragger )
                {
                    if( !( static_cast<RigidDebris*>( CastResult )->GetRigidProxy()->IsStaticOrKinematic() ) )
                    {
                        RigidDebris* rigid = static_cast<RigidDebris*>( RayCaster.LastQueryResultsObjectPtr() );
                        rigid->GetRigidProxy()->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
                        Dragger = new Physics::Point2PointConstraint(rigid->GetRigidProxy(), LocalPivot);
                        Dragger->SetTAU(0.001);
                        Entresol::GetSingletonPtr()->GetPhysicsManager()->AddConstraint(Dragger);
                        Dragger->SetParam(Physics::Con_Stop_CFM,0.8,-1);
                        Dragger->SetParam(Physics::Con_CFM,0.8,-1);
                        //Dragger->SetParam(Physics::Con_Stop_CFM,0.8,0); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,1); Dragger->SetParam(Physics::Con_Stop_CFM,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                        Dragger->SetParam(Physics::Con_Stop_ERP,0.1,-1);
                        Dragger->SetParam(Physics::Con_ERP,0.1,-1);
                        //Dragger->SetParam(Physics::Con_Stop_ERP,0.1,0); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,1); Dragger->SetParam(Physics::Con_Stop_ERP,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                        firstframe = true;
                        this->CatchApplication->LastObjectThrown = rigid;
                    }
                }
            }

            if(Dragger && RayCaster.RayPlaneIntersection(MouseRay, this->CatchApplication->PlaneOfPlay))
            {
                if(!firstframe)
                    { Dragger->SetPivotBLocation(RayCaster.LastQueryResultsOffset()); }
            }

            if(Dragger &&
               !this->CatchApplication->IsInsideAnyStartZone(this->CatchApplication->LastObjectThrown))
            {
                Physics::RigidProxy* Prox = Dragger->GetProxyA();
                Entresol::GetSingletonPtr()->GetPhysicsManager()->RemoveConstraint(Dragger);
                Prox->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
            }
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            Physics::RigidProxy* Prox = Dragger->GetProxyA();
            Entresol::GetSingletonPtr()->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger = NULL;
            Prox->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// CatchPostInputWorkUnit Methods

CatchPostGraphicsWorkUnit::CatchPostGraphicsWorkUnit(CatchApp* Target) :
    CatchApplication(Target) {  }

CatchPostGraphicsWorkUnit::~CatchPostGraphicsWorkUnit()
    {  }

void CatchPostGraphicsWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
{
    // Update the timer
    /// @todo UI Update
    /*UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::OpenRenderableContainerWidget* HUDCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_HUD"));
    UI::Caption* Timer = static_cast<UI::Caption*>(HUDCont->GetAreaRenderable("GS_Timer"));
    std::stringstream time;
    Whole TotalSeconds = this->CatchApplication->GetLevelTimer()->GetCurrentTimeInMilliseconds() / 1000;
    Whole Minutes = TotalSeconds / 60;
    Whole Seconds;

    if(60 > TotalSeconds) Seconds = TotalSeconds;
    else Seconds = TotalSeconds % 60;

    if(10 > Seconds) time << Minutes << ":" << 0 << Seconds;
    else time << Minutes << ":" << Seconds;

    Timer->SetText(time.str());

    // Update the score
    LevelScorer::ScorePair Scores = this->CatchApplication->GetLevelScorer()->CalculateThrowableScore();

    UI::Caption* ScoreAmount = static_cast<UI::Caption*>( HUDCont->GetAreaRenderable("GS_ScoreArea") );
    ScoreAmount->SetText( StringTools::ConvertToString( Scores.first + Scores.second ) );

    // Update Stat information
    Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
    UI::OpenRenderableContainerWidget* StatsCont = static_cast<UI::OpenRenderableContainerWidget*>(GameScreen->GetWidget("GS_Stats"));
    UI::Caption* CurFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("CurFPS"));
    UI::Caption* AvFPS = static_cast<UI::Caption*>(StatsCont->GetAreaRenderable("AvFPS"));
    CurFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetLastFPS()));
    AvFPS->SetText(StringTools::ConvertToString(GraphicsMan->GetGameWindow(0)->GetAverageFPS()));

    StringStream FPSStream;
    FPSStream << "<FPSReport CurrentFPS=\"" << GraphicsMan->GetGameWindow(0)->GetLastFPS() << "\" AverageFPS=\"" << GraphicsMan->GetGameWindow(0)->GetAverageFPS() << "\" />" << std::endl;
    this->CatchApplication->TheEntresol->Log(FPSStream.str());

    //See if the level is over
    if(this->CatchApplication->CurrentState != CatchApp::Catch_ScoreScreen)
    {
        if(this->CatchApplication->CheckEndOfLevel())
        {
            this->CatchApplication->ChangeState(CatchApp::Catch_ScoreScreen);
        }
    }//*/
}

#endif
