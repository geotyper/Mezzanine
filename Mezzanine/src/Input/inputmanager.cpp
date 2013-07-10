//© Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _inputinputmanager_cpp
#define _inputinputmanager_cpp

#include "inputmanager.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "entresol.h"
#include "Input/mouse.h"
#include "Input/keyboard.h"
#include "Input/controller.h"

#include "SDL.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        // DeviceUpdateWorkUnit Methods

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(const DeviceUpdateWorkUnit& Other)
            {  }

        DeviceUpdateWorkUnit& DeviceUpdateWorkUnit::operator=(const DeviceUpdateWorkUnit& Other)
            { return *this; }

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(InputManager* Target) :
            TargetManager(Target) {  }

        DeviceUpdateWorkUnit::~DeviceUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void DeviceUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            UInt32 NumControllers = this->TargetManager->GetNumControllers();
            std::vector< MetaCode > MouseCodes;
            std::vector< MetaCode > KeyboardCodes;
            std::vector< std::vector< MetaCode > > ControllerCodes;
            ControllerCodes.resize( NumControllers );

            std::list<EventUserInput*>* UserInput = EventManager::GetSingletonPtr()->GetAllUserInputEvents();
            EventManager::GetSingletonPtr()->RemoveAllSpecificEvents(EventBase::UserInput);

            for(std::list<EventUserInput*>::iterator Iter = UserInput->begin(); !UserInput->empty(); Iter = UserInput->begin())
            {
                EventUserInput* CurrEvent = (*Iter);
                for( Whole X = 0 ; X < CurrEvent->size() ; ++X )
                {
                    MetaCode& CurrCode = CurrEvent->at(X);
                    if( Input::MOUSE_FIRST <= CurrCode.GetCode() && Input::MOUSE_LAST >= CurrCode.GetCode() ){
                        MouseCodes.push_back( CurrCode );
                    }else if( Input::KEY_FIRST <= CurrCode.GetCode() && Input::KEY_LAST >= CurrCode.GetCode() ){
                        KeyboardCodes.push_back( CurrCode );
                    }else if( Input::CONTROLLER_FIRST <= CurrCode.GetCode() && Input::CONTROLLER_LAST >= CurrCode.GetCode() ){
                        ControllerCodes[ CurrCode.GetDeviceIndex() ].push_back( CurrCode );
                    }
                }

                delete CurrEvent;
                UserInput->remove(*Iter);
            }

            this->TargetManager->SystemMouse->_Update(MouseCodes);
            this->TargetManager->SystemKeyboard->_Update(KeyboardCodes);
            for( Whole X = 0 ; X < NumControllers ; ++X )
            {
                this->TargetManager->Controllers.at(X)->_Update( ControllerCodes.at(X) );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputManager Methods

        template<> InputManager* Singleton<InputManager>::SingletonPtr = NULL;

        InputManager::InputManager() :
            SystemMouse(NULL),
            SystemKeyboard(NULL),

            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
        {
            this->Priority = 5;
            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_JOYSTICK & InitSDLSystems) == 0 )
            {
                if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Joystick input, SDL Error: ") + SDL_GetError()); }
            }
            if( !(SDL_INIT_GAMECONTROLLER | InitSDLSystems) )
            {
                if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Game Controller input, SDL Error: ") + SDL_GetError()); }
            }

            this->SystemMouse = new Mouse();
            this->SystemKeyboard = new Keyboard();
            this->DetectControllers();

            this->DeviceUpdateWork = new DeviceUpdateWorkUnit(this);
        }

        InputManager::InputManager(XML::Node& XMLNode) :
            SystemMouse(NULL),
            SystemKeyboard(NULL),

            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
        {
            this->Priority = 5;
            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_JOYSTICK & InitSDLSystems) == 0 )
            {
                if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Joystick input, SDL Error: ") + SDL_GetError()); }
            }
            if( !(SDL_INIT_GAMECONTROLLER | InitSDLSystems) )
            {
                if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE) < 0 )
                    { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Game Controller input, SDL Error: ") + SDL_GetError()); }
            }

            this->SystemMouse = new Mouse();
            this->SystemKeyboard = new Keyboard();
            this->DetectControllers();

            this->DeviceUpdateWork = new DeviceUpdateWorkUnit(this);
        }

        InputManager::~InputManager()
        {
            this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->DeviceUpdateWork );
            delete DeviceUpdateWork;

            delete SystemMouse;
            delete SystemKeyboard;
            this->ReleaseAllControllers();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputDevice Management

        Mouse* InputManager::GetSystemMouse() const
        {
            return this->SystemMouse;
        }

        Keyboard* InputManager::GetSystemKeyboard() const
        {
            return this->SystemKeyboard;
        }

        Controller* InputManager::GetController(const UInt16 Index) const
        {
            return this->Controllers.at(Index);
        }

        UInt16 InputManager::GetNumControllers() const
        {
            return this->Controllers.size();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputDevice Detection

        UInt16 InputManager::DetectControllers()
        {
            UInt16 Count;
            for( Count = 0 ; Count < SDL_NumJoysticks() ; ++Count )
            {
                SDL_Joystick* InternalControl = SDL_JoystickOpen(Count);
                Input::Controller* NewController = new Input::Controller( InternalControl, Count );
                this->Controllers.push_back( NewController );
                this->InternalControllers.push_back( InternalControl );
            }
            return Count;
        }

        void InputManager::ReleaseAllControllers()
        {
            if( Controllers.empty() )
                return;

            for( ControllerIterator ContIt = Controllers.begin() ; ContIt != Controllers.end() ; ++ContIt )
            {
                delete (*ContIt);
            }
            for( std::vector<void*>::iterator ContIt = InternalControllers.begin() ; ContIt != InternalControllers.end() ; ++ContIt )
            {
                SDL_JoystickClose( (SDL_Joystick*)(*ContIt) );
            }
            this->Controllers.clear();
            this->InternalControllers.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        DeviceUpdateWorkUnit* InputManager::GetDeviceUpdateWork()
        {
            return this->DeviceUpdateWork;
        }

        ///////////////////////////////////////////////////////////////////////////////
        //Inherited from ManagerBase

        void InputManager::Initialize()
        {
            this->TheEntresol->GetScheduler().AddWorkUnit( this->DeviceUpdateWork );
            Mezzanine::EventManager* EventMan = EventManager::GetSingletonPtr();
            if( EventMan )
                this->DeviceUpdateWork->AddDependency( EventMan->GetEventWorkUnit() );

            this->Initialized = true;
        }

        void InputManager::DoMainLoopItems()
        {

        }

        ManagerBase::ManagerType InputManager::GetInterfaceType() const
            { return ManagerBase::InputManager; }

        String InputManager::GetImplementationTypeName() const
            { return "DefaultInputManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultInputManagerFactory Methods

        DefaultInputManagerFactory::DefaultInputManagerFactory()
        {
        }

        DefaultInputManagerFactory::~DefaultInputManagerFactory()
        {
        }

        String DefaultInputManagerFactory::GetManagerTypeName() const
        {
            return "DefaultInputManager";
        }

        ManagerBase* DefaultInputManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if(InputManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return InputManager::GetSingletonPtr();
            }else return new InputManager();
        }

        ManagerBase* DefaultInputManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if(InputManager::SingletonValid())
            {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return InputManager::GetSingletonPtr();
            }else return new InputManager(XMLNode);
        }

        void DefaultInputManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }
    }//Input
}//Mezzanine

#endif
