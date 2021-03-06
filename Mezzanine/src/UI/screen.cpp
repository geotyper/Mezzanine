// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/textureatlas.h"
#include "UI/mousehoverstrategy.h"
#include "UI/brutestrategy.h"
#include "UI/layoutstrategy.h"

#include "UI/button.h"
#include "UI/checkbox.h"
#include "UI/dropdownlist.h"
#include "UI/editbox.h"
#include "UI/gridcontainer.h"
#include "UI/horizontalcontainer.h"
#include "UI/horizontalscrollbar.h"
#include "UI/linelist.h"
#include "UI/listbox.h"
#include "UI/menuentry.h"
#include "UI/radiobutton.h"
#include "UI/scrollbar.h"
#include "UI/spinner.h"
#include "UI/stackbutton.h"
#include "UI/tabset.h"
#include "UI/verticalcontainer.h"
#include "UI/verticalscrollbar.h"
#include "UI/widget.h"
#include "UI/window.h"

#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/graphicsmanager.h"
#include "Graphics/scenemanager.h"

#include "MathTools/mathtools.h"
#include "exception.h"

#include <OgreRoot.h>
#include <OgreMatrix4.h>
#include <OgreRenderSystem.h>
#include <OgreRenderOperation.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Child Processing Functors

        // Keeps this file form being documented by doxygen
        /// @cond DontDocumentInternal

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Simple functor for appending all vertices in the renderable tree to a vector.
        /// @details
        ///////////////////////////////////////
        class VertexCollectFunctor
        {
            public:
                /// @brief A pointer to the buffer storing Vertex data to be rendered.
                ScreenRenderData* Data;

                /// @brief Class constructor.
                /// @param pData A pointer to the buffer that Vertex data will be appended to.
                VertexCollectFunctor(ScreenRenderData* pData) : Data(pData) {}
                /// @brief Class constructor.
                ~VertexCollectFunctor() {}

                /// @brief Function Operator.
                /// @param Quad The QuadRenderable to have it's Vertex data appended to the buffer.
                Boole operator()(QuadRenderable* Quad)
                {
                    Quad->_AppendRenderData(*Data);
                    return false;
                }
        };//VertexCollectFunctor

        ///////////////////////////////////////////////////////////////////////////////
        // OgreVertex

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Simple class that facilitates conversions when inserting vertex's into the video buffer.
        /// @details
        ///////////////////////////////////////
        struct OgreVertex
        {
            /// @brief Vertex Position.  Z component should almost always be 0.
            Ogre::Vector3 Position;
            /// @brief Vertex Colour.  Can tint Quads using images or be used as a flat colour.
            Ogre::ColourValue Colour;
            /// @brief Texture Coordinates.  How a texture should be mapped onto the quad.
            Ogre::Vector2 UV;
        };

        ///////////////////////////////////////////////////////////////////////////////
        // ScreenInternalData Methods

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Basic struct holding some of the internal bits of this class that could not be placed on the class directly.
        /// @details
        ///////////////////////////////////////
        struct ScreenInternalData : public Ogre::RenderQueueListener
        {
            /// @brief A pointer to the Screen this is listening for.
            Screen* ParentScreen;
            /// @brief The render operation to be passed into the render system to draw the Screen.
            Ogre::RenderOperation RenderOp;
            /// @brief A pointer to the actual render system that will be doing the rendering.
            Ogre::RenderSystem* RenderSys;
            /// @brief A pointer to the hardware buffer containing the UI vertex data.
            Ogre::HardwareVertexBufferSharedPtr VertexBuffer;

            /// @brief Callback for when a specific group in the render queue has started render operations.
            void renderQueueStarted(Ogre::uint8, const Ogre::String&, bool&) {  }
            /// @brief Callback for when a specific group in the render queue has ended render operations.
            /// @param queueGroupId The render queue group that has ended rendering.
            void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
            {
                if( queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && this->RenderSys->_getViewport() == this->ParentScreen->GetViewport()->_GetOgreViewport() ) {
                    if( this->ParentScreen->IsVisible() ) {
                        this->ParentScreen->_RenderScreen();
                    }
                }
            }
        };//ScreenInternalData

        /// @endcond

        ///////////////////////////////////////////////////////////////////////////////
        // ScreenRenderData Methods

        void ScreenRenderData::Clear()
            { this->Vertices.clear(); }

        Whole ScreenRenderData::Size()
            { return this->Vertices.size(); }

        void ScreenRenderData::Append(ScreenRenderData& OtherData)
            { this->Vertices.insert(this->Vertices.end(),OtherData.Vertices.begin(),OtherData.Vertices.end()); }

        VertexData& ScreenRenderData::operator[](const Whole& Index)
            { return this->Vertices.at(Index); }

        ///////////////////////////////////////////////////////////////////////////////
        // Screen Methods

        Screen::Screen(const String& RendName, const String& Atlas, Graphics::Viewport* WindowViewport, UIManager* Manager) :
            QuadRenderable(RendName,this),

            PrimaryAtlas(Atlas),
            Scale(1,1,1),
            MouseHitPosition(-1,-1),

            UIMan(Manager),
            GameViewport(WindowViewport),
            MouseStrat(NULL),

            Orientation(Mezzanine::OM_Degree_0)
        {
            this->AddAllDefaultWidgetFactories();
            /*this->ActDims.Size.X = (Real)this->GameViewport->GetActualWidth();
            this->ActDims.Size.Y = (Real)this->GameViewport->GetActualHeight();
            this->InverseSize.X = 1 / this->ActDims.Size.X;
            this->InverseSize.Y = 1 / this->ActDims.Size.Y;// */

            this->SID = new ScreenInternalData();
            this->SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            this->SID->ParentScreen = this;

            Graphics::SceneManager* SceneMan = this->GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            this->VertexTransform.SetTransform(Vector3(0,0,0),this->Scale,Quaternion(0,0,0,1));
            this->CreateVertexBuffer(32 * 6);

            this->SetMousePassthrough(true);
            this->SetMouseHoverStrategy(new BruteStrategy());
            this->_SetLayoutStrat(new LayoutStrategy());
        }

        Screen::Screen(const XML::Node& XMLNode, UIManager* Manager) :
            QuadRenderable(this),
            Scale(1,1,1),
            MouseHitPosition(-1,-1),
            UIMan(Manager),
            GameViewport(NULL),
            MouseStrat(NULL),
            Orientation(Mezzanine::OM_Degree_0)
        {
            this->AddAllDefaultWidgetFactories();

            this->SID = new ScreenInternalData();
            this->SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            this->SID->ParentScreen = this;

            Graphics::SceneManager* SceneMan = this->GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            this->CreateVertexBuffer(32 * 6);
            this->SetMouseHoverStrategy(new BruteStrategy());
            this->_SetLayoutStrat(new LayoutStrategy());

            this->ProtoDeSerialize(XMLNode);
        }

        Screen::~Screen()
        {
            this->DestroyAllWidgets();
        }

        Graphics::SceneManager* Screen::GetSceneManager() const
        {
            /// @todo This function exists (as opposed to storing a pointer that doesn't change) so that if changes in the
            /// viewport configuration occur this will pick up on that.  However the render queue listener that is added in
            /// this class' constructor never gets re-assigned.  This needs to be fixed.  Until then if a change does occur
            /// the UI will be rendered at a different time then it needs to be, potentially overwritten by the scene render.
            if( GameViewport ) {
                Graphics::CameraProxy* Cam = this->GameViewport->GetCamera();
                if( Cam ) {
                    Graphics::SceneManager* SceneMan = static_cast<Graphics::SceneManager*>( Cam->GetCreator() );
                    if( SceneMan ) return SceneMan;
                    else return NULL;
                }else return NULL;
            }else return NULL;
        }

        WidgetFactory* Screen::GetWidgetFactoryExcept(const String& WidgetTypeName)
        {
            WidgetFactoryIterator WidFactIt = this->WidgetFactories.find( WidgetTypeName );
            if( WidFactIt  != this->WidgetFactories.end() ) {
                return (*WidFactIt).second;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to create a " + WidgetTypeName + " Widget without it's factory registered.");
            }
        }

        Widget* Screen::CheckAndInsertExcept(Widget* ToInsert)
        {
            String WidgetName = ToInsert->GetName();
            std::pair<WidgetIterator,Boole> InsertReturn = this->Widgets.insert( std::pair<String,Widget*>(WidgetName,ToInsert) );
            if( !InsertReturn.second )
                { MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"Widget with name \"" + WidgetName + "\" already exists."); }
            return ToInsert;
        }

        void Screen::PrepareRenderSystem()
        {
            this->SID->RenderSys->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setViewMatrix( Ogre::Matrix4::IDENTITY );
            Graphics::SceneManager* SceneMan = GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->_setPass( UIManager::GetSingletonPtr()->GetAtlas(PrimaryAtlas)->_Get2DPass() );
        }

        void Screen::CreateVertexBuffer(const Whole& InitialSize)
        {
            this->SID->RenderOp.vertexData = OGRE_NEW Ogre::VertexData;
            this->SID->RenderOp.vertexData->vertexStart = 0;

            Ogre::VertexDeclaration* VertexDecl = this->SID->RenderOp.vertexData->vertexDeclaration;
            size_t Offset = 0;

            // Position.
            VertexDecl->addElement(0,0,Ogre::VET_FLOAT3,Ogre::VES_POSITION);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            // Colour
            VertexDecl->addElement(0,Offset,Ogre::VET_FLOAT4,Ogre::VES_DIFFUSE);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT4);

            // Texture Coordinates
            VertexDecl->addElement(0,Offset,Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

            this->SID->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                VertexDecl->getVertexSize(0),
                InitialSize,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                false);

            this->SID->RenderOp.vertexData->vertexBufferBinding->setBinding(0,this->SID->VertexBuffer);
            this->SID->RenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            this->SID->RenderOp.useIndexes = false;
        }

        void Screen::DestroyVertexBuffer()
        {
            OGRE_DELETE this->SID->RenderOp.vertexData;
            this->SID->RenderOp.vertexData = 0;
            this->SID->VertexBuffer.setNull();
        }

        void Screen::ResizeVertexBuffer(const Whole& RequestedSize)
        {
            if( this->SID->VertexBuffer.isNull() )
                this->CreateVertexBuffer(RequestedSize);

            if(RequestedSize > this->SID->VertexBuffer->getNumVertices() ) {
                // Update only by powers of 2
                Whole NewVertexBufferSize = 1;
                while(NewVertexBufferSize < RequestedSize)
                    NewVertexBufferSize <<= 1;

                this->SID->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                    this->SID->RenderOp.vertexData->vertexDeclaration->getVertexSize(0),
                    NewVertexBufferSize,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                    false);

                this->SID->RenderOp.vertexData->vertexStart = 0;
                this->SID->RenderOp.vertexData->vertexBufferBinding->setBinding(0,this->SID->VertexBuffer);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Visibility Methods

        void Screen::SetVisible(Boole CanSee)
            { this->Visible = CanSee; }

        Boole Screen::GetVisible() const
            { return this->Visible; }

        Boole Screen::IsVisible() const
            { return this->Visible; }

        void Screen::Show()
            { this->SetVisible(true); }

        void Screen::Hide()
            { this->SetVisible(false); }

        Renderable::RenderableType Screen::GetRenderableType() const
            { return Renderable::RT_Screen; }

        const Vector2& Screen::GetViewportDimensions() const
            { return this->ActDims.Size; }

        void Screen::CheckViewportSize()
        {
            Vector2 CurrentSize((Real)this->GameViewport->GetActualWidth(),(Real)this->GameViewport->GetActualHeight());
            if( this->ActDims.Size != CurrentSize || ( this->InverseSize.X == 0.0 || this->InverseSize.Y == 0.0 ) ) {
                Rect OldRect(this->ActDims);
                Rect NewRect(Vector2(0,0),CurrentSize);
                this->UpdateDimensions(OldRect,NewRect);

                this->InverseSize.X = 1.0 / this->ActDims.Size.X;
                this->InverseSize.Y = 1.0 / this->ActDims.Size.Y;
            }
        }

        Graphics::Viewport* Screen::GetViewport() const
            { return this->GameViewport; }

        UIManager* Screen::GetManager() const
            { return this->UIMan; }

        ///////////////////////////////////////////////////////////////////////////////
        // Mouse Hover Methods

        void Screen::SetMouseHoverStrategy(MouseHoverStrategy* Strategy)
        {
            if(this->MouseStrat) {
                delete this->MouseStrat;
                this->MouseStrat = NULL;
            }
            this->MouseStrat = Strategy;
            this->MouseStrat->_SetScreen(this);
        }

        MouseHoverStrategy* Screen::GetMouseHoverStrategy() const
        {
            return this->MouseStrat;
        }

        Widget* Screen::FindHoveredWidget(const Vector2& MousePos)
        {
            if( this->GetVisible() ) {
                Widget* Ret = this->MouseStrat->FindHoveredWidget(MousePos);
                if( Ret != NULL ) this->MouseHitPosition = MousePos;
                else this->MouseHitPosition.SetValues(-1,-1);

                return Ret;
            }
            return NULL;
        }

        const Vector2& Screen::GetMouseHitPosition() const
        {
            return this->MouseHitPosition;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // WidgetFactory Management

        void Screen::AddWidgetFactory(WidgetFactory* ToBeAdded)
        {
            this->WidgetFactories.insert( std::pair<String,WidgetFactory*>(ToBeAdded->GetWidgetTypeName(),ToBeAdded) );
        }

        void Screen::RemoveWidgetFactory(WidgetFactory* ToBeRemoved)
        {
            this->RemoveWidgetFactory( ToBeRemoved->GetWidgetTypeName() );
        }

        void Screen::RemoveWidgetFactory(const String& ImplName)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ImplName );
            if( FactIt != this->WidgetFactories.end() ) {
                this->WidgetFactories.erase(FactIt);
            }
        }

        void Screen::DestroyWidgetFactory(WidgetFactory* ToBeDestroyed)
        {
            this->DestroyWidgetFactory( ToBeDestroyed->GetWidgetTypeName() );
        }

        void Screen::DestroyWidgetFactory(const String& ImplName)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ImplName );
            if( FactIt != this->WidgetFactories.end() ) {
                delete (*FactIt).second;
                this->WidgetFactories.erase(FactIt);
            }
        }

        void Screen::DestroyAllWidgetFactories()
        {
            for( WidgetFactoryIterator FactIt = this->WidgetFactories.begin() ; FactIt != this->WidgetFactories.end() ; ++FactIt )
            {
                delete (*FactIt).second;
            }
            this->WidgetFactories.clear();
        }

        void Screen::AddAllDefaultWidgetFactories()
        {
            WidgetFactoryIterator FactIt;
            // Generic Widget
            FactIt = this->WidgetFactories.find( Widget::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new GenericWidgetFactory() );
            // Button
            FactIt = this->WidgetFactories.find( Button::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new ButtonFactory() );
            // StackButton
            FactIt = this->WidgetFactories.find( StackButton::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new StackButtonFactory() );
            // RadioButton
            FactIt = this->WidgetFactories.find( RadioButton::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new RadioButtonFactory() );
            // CheckBox
            FactIt = this->WidgetFactories.find( CheckBox::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new CheckBoxFactory() );
            // EditBox
            FactIt = this->WidgetFactories.find( EditBox::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new EditBoxFactory() );

            // HorizontalScrollbar
            FactIt = this->WidgetFactories.find( HorizontalScrollbar::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new HorizontalScrollbarFactory() );
            // VerticalScrollbar
            FactIt = this->WidgetFactories.find( VerticalScrollbar::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new VerticalScrollbarFactory() );
            // Spinner
            FactIt = this->WidgetFactories.find( Spinner::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new SpinnerFactory() );

            // MenuEntry
            FactIt = this->WidgetFactories.find( MenuEntry::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new MenuEntryFactory() );
            // TabSet
            FactIt = this->WidgetFactories.find( TabSet::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new TabSetFactory() );
            // ListBox
            FactIt = this->WidgetFactories.find( ListBox::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new ListBoxFactory() );
            // DropDownList
            FactIt = this->WidgetFactories.find( DropDownList::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new DropDownListFactory() );

            // HorizontalContainer
            FactIt = this->WidgetFactories.find( HorizontalContainer::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new HorizontalContainerFactory() );
            // VerticalContainer
            FactIt = this->WidgetFactories.find( VerticalContainer::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new VerticalContainerFactory() );
            // GridContainer
            FactIt = this->WidgetFactories.find( GridContainer::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new GridContainerFactory() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Widget Management

        Widget* Screen::CreateWidget(const XML::Node& WidgetNode)
        {
            String TypeName = WidgetNode.Name();
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( TypeName );
            if( FactIt != this->WidgetFactories.end() ) {
                return this->CheckAndInsertExcept( (*FactIt).second->CreateWidget(WidgetNode,this) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to create widget of type \"" + TypeName + "\", which has no factory registered.");
            }
        }

        Widget* Screen::CreateWidget(const String& TypeName, const String& RendName, const NameValuePairMap& Params)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( TypeName );
            if( FactIt != this->WidgetFactories.end() ) {
                return this->CheckAndInsertExcept( (*FactIt).second->CreateWidget(RendName,Params,this) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to create widget of type \"" + TypeName + "\", which has no factory registered.");
            }
        }

        Widget* Screen::CreateWidget(const String& TypeName, const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( TypeName );
            if( FactIt != this->WidgetFactories.end() ) {
                return this->CheckAndInsertExcept( (*FactIt).second->CreateWidget(RendName,RendRect,Params,this) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_INVALID_EXCEPTION,"Attempting to create widget of type \"" + TypeName + "\", which has no factory registered.");
            }
        }

        Widget* Screen::GetWidget(const String& Name)
        {
            WidgetIterator WidIt = this->Widgets.find(Name);
            if( WidIt != this->Widgets.end() ) return (*WidIt).second;
            else return NULL;
        }

        Whole Screen::GetNumWidgets()
        {
            return this->Widgets.size();
        }

        void Screen::DestroyWidget(Widget* ToBeDestroyed)
        {
            // Remove the widget from the Widget container.
            WidgetIterator WidIt = this->Widgets.find( ToBeDestroyed->GetName() );
            if( WidIt != this->Widgets.end() )
                this->Widgets.erase(WidIt);

            // Delete the Widget
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ToBeDestroyed->GetTypeName() );
            if( FactIt != this->WidgetFactories.end() ) {
                (*FactIt).second->DestroyWidget( ToBeDestroyed );
            }
        }

        void Screen::DestroyAllWidgets()
        {
            for( ChildIterator ChildIt = this->ChildWidgets.begin() ; ChildIt != this->ChildWidgets.end() ; ++ChildIt )
            {
                (*ChildIt)->_NotifyParenthood(NULL);
                WidgetFactoryIterator FactIt = this->WidgetFactories.find( (*ChildIt)->GetTypeName() );
                if( FactIt != this->WidgetFactories.end() ) {
                    (*FactIt).second->DestroyWidget( (*ChildIt) );
                }
            }
            this->ChildWidgets.clear();
            this->Widgets.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Widget Creation Methods

        Widget* Screen::CreateWidget(const String& RendName)
        {
            Widget* NewWidget = static_cast<GenericWidgetFactory*>( this->GetWidgetFactoryExcept( Widget::TypeName ) )->CreateWidget( RendName, this );
            this->CheckAndInsertExcept( NewWidget );
            return NewWidget;
        }

        Widget* Screen::CreateWidget(const String& RendName, const UnifiedRect& RendRect)
        {
            Widget* NewWidget = static_cast<GenericWidgetFactory*>( this->GetWidgetFactoryExcept( Widget::TypeName ) )->CreateWidget( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewWidget );
            return NewWidget;
        }

        Button* Screen::CreateButton(const String& RendName)
        {
            Button* NewButton = static_cast<ButtonFactory*>( this->GetWidgetFactoryExcept( Button::TypeName ) )->CreateButton( RendName, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        Button* Screen::CreateButton(const String& RendName, const UnifiedRect& RendRect)
        {
            Button* NewButton = static_cast<ButtonFactory*>( this->GetWidgetFactoryExcept( Button::TypeName ) )->CreateButton( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        StackButton* Screen::CreateStackButton(const String& RendName)
        {
            StackButton* NewButton = static_cast<StackButtonFactory*>( this->GetWidgetFactoryExcept( StackButton::TypeName ) )->CreateStackButton( RendName, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        StackButton* Screen::CreateStackButton(const String& RendName, const UnifiedRect& RendRect)
        {
            StackButton* NewButton = static_cast<StackButtonFactory*>( this->GetWidgetFactoryExcept( StackButton::TypeName ) )->CreateStackButton( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        RadioButton* Screen::CreateRadioButton(const String& RendName)
        {
            RadioButton* NewButton = static_cast<RadioButtonFactory*>( this->GetWidgetFactoryExcept( RadioButton::TypeName ) )->CreateRadioButton( RendName, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        RadioButton* Screen::CreateRadioButton(const String& RendName, const UnifiedRect& RendRect)
        {
            RadioButton* NewButton = static_cast<RadioButtonFactory*>( this->GetWidgetFactoryExcept( RadioButton::TypeName ) )->CreateRadioButton( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        CheckBox* Screen::CreateCheckBox(const String& RendName)
        {
            CheckBox* NewCheckBox = static_cast<CheckBoxFactory*>( this->GetWidgetFactoryExcept( CheckBox::TypeName ) )->CreateCheckBox( RendName, this );
            this->CheckAndInsertExcept( NewCheckBox );
            return NewCheckBox;
        }

        CheckBox* Screen::CreateCheckBox(const String& RendName, const UnifiedRect& RendRect)
        {
            CheckBox* NewCheckBox = static_cast<CheckBoxFactory*>( this->GetWidgetFactoryExcept( CheckBox::TypeName ) )->CreateCheckBox( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewCheckBox );
            return NewCheckBox;
        }

        EditBox* Screen::CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, FontData* EditFont)
        {
            EditBox* NewEdit = static_cast<EditBoxFactory*>( this->GetWidgetFactoryExcept( EditBox::TypeName ) )->CreateEditBox( RendName, EditLayerType, EditFont, this );
            this->CheckAndInsertExcept( NewEdit );
            return NewEdit;
        }

        EditBox* Screen::CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, const String& EditFontName)
        {
            EditBox* NewEdit = static_cast<EditBoxFactory*>( this->GetWidgetFactoryExcept( EditBox::TypeName ) )->CreateEditBox( RendName, EditLayerType, EditFontName, this );
            this->CheckAndInsertExcept( NewEdit );
            return NewEdit;
        }

        EditBox* Screen::CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, FontData* EditFont)
        {
            EditBox* NewEdit = static_cast<EditBoxFactory*>( this->GetWidgetFactoryExcept( EditBox::TypeName ) )->CreateEditBox( RendName, RendRect, EditLayerType, EditFont, this );
            this->CheckAndInsertExcept( NewEdit );
            return NewEdit;
        }

        EditBox* Screen::CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, const String& EditFontName)
        {
            EditBox* NewEdit = static_cast<EditBoxFactory*>( this->GetWidgetFactoryExcept( EditBox::TypeName ) )->CreateEditBox( RendName, RendRect, EditLayerType, EditFontName, this );
            this->CheckAndInsertExcept( NewEdit );
            return NewEdit;
        }

        HorizontalScrollbar* Screen::CreateHorizontalScrollbar(const String& RendName, const UI::ScrollbarStyle Style)
        {
            HorizontalScrollbar* NewHScroll = static_cast<HorizontalScrollbarFactory*>( this->GetWidgetFactoryExcept( HorizontalScrollbar::TypeName ) )->CreateHorizontalScrollbar( RendName, Style, this );
            this->CheckAndInsertExcept( NewHScroll );
            return NewHScroll;
        }

        HorizontalScrollbar* Screen::CreateHorizontalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            HorizontalScrollbar* NewHScroll = static_cast<HorizontalScrollbarFactory*>( this->GetWidgetFactoryExcept( HorizontalScrollbar::TypeName ) )->CreateHorizontalScrollbar( RendName, RendRect, Style, this );
            this->CheckAndInsertExcept( NewHScroll );
            return NewHScroll;
        }

        VerticalScrollbar* Screen::CreateVerticalScrollbar(const String& RendName, const UI::ScrollbarStyle Style)
        {
            VerticalScrollbar* NewVScroll = static_cast<VerticalScrollbarFactory*>( this->GetWidgetFactoryExcept( VerticalScrollbar::TypeName ) )->CreateVerticalScrollbar( RendName, Style, this );
            this->CheckAndInsertExcept( NewVScroll );
            return NewVScroll;
        }

        VerticalScrollbar* Screen::CreateVerticalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            VerticalScrollbar* NewVScroll = static_cast<VerticalScrollbarFactory*>( this->GetWidgetFactoryExcept( VerticalScrollbar::TypeName ) )->CreateVerticalScrollbar( RendName, RendRect, Style, this );
            this->CheckAndInsertExcept( NewVScroll );
            return NewVScroll;
        }

        Spinner* Screen::CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont)
        {
            Spinner* NewSpin = static_cast<SpinnerFactory*>( this->GetWidgetFactoryExcept( Spinner::TypeName ) )->CreateSpinner( RendName, SpinStyle, EditFont, this );
            this->CheckAndInsertExcept( NewSpin );
            return NewSpin;
        }

        Spinner* Screen::CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName)
        {
            Spinner* NewSpin = static_cast<SpinnerFactory*>( this->GetWidgetFactoryExcept( Spinner::TypeName ) )->CreateSpinner( RendName, SpinStyle, EditFontName, this );
            this->CheckAndInsertExcept( NewSpin );
            return NewSpin;
        }

        Spinner* Screen::CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont)
        {
            Spinner* NewSpin = static_cast<SpinnerFactory*>( this->GetWidgetFactoryExcept( Spinner::TypeName ) )->CreateSpinner( RendName, RendRect, SpinStyle, EditFont, this );
            this->CheckAndInsertExcept( NewSpin );
            return NewSpin;
        }

        Spinner* Screen::CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName)
        {
            Spinner* NewSpin = static_cast<SpinnerFactory*>( this->GetWidgetFactoryExcept( Spinner::TypeName ) )->CreateSpinner( RendName, RendRect, SpinStyle, EditFontName, this );
            this->CheckAndInsertExcept( NewSpin );
            return NewSpin;
        }

        MenuEntry* Screen::CreateMenuEntry(const String& RendName)
        {
            MenuEntry* NewEntry = static_cast<MenuEntryFactory*>( this->GetWidgetFactoryExcept( MenuEntry::TypeName ) )->CreateMenuEntry( RendName, this );
            this->CheckAndInsertExcept( NewEntry );
            return NewEntry;
        }

        MenuEntry* Screen::CreateMenuEntry(const String& RendName, const UnifiedRect& RendRect)
        {
            MenuEntry* NewEntry = static_cast<MenuEntryFactory*>( this->GetWidgetFactoryExcept( MenuEntry::TypeName ) )->CreateMenuEntry( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewEntry );
            return NewEntry;
        }

        TabSet* Screen::CreateTabSet(const String& RendName)
        {
            TabSet* NewTab = static_cast<TabSetFactory*>( this->GetWidgetFactoryExcept( TabSet::TypeName ) )->CreateTabSet( RendName, this );
            this->CheckAndInsertExcept( NewTab );
            return NewTab;
        }

        TabSet* Screen::CreateTabSet(const String& RendName, const UnifiedRect& RendRect)
        {
            TabSet* NewTab = static_cast<TabSetFactory*>( this->GetWidgetFactoryExcept( TabSet::TypeName ) )->CreateTabSet( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewTab );
            return NewTab;
        }

        ListBox* Screen::CreateListBox(const String& RendName, const UI::ScrollbarStyle Style)
        {
            ListBox* NewList = static_cast<ListBoxFactory*>( this->GetWidgetFactoryExcept( ListBox::TypeName ) )->CreateListBox( RendName, Style, this );
            this->CheckAndInsertExcept( NewList );
            return NewList;
        }

        ListBox* Screen::CreateListBox(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            ListBox* NewList = static_cast<ListBoxFactory*>( this->GetWidgetFactoryExcept( ListBox::TypeName ) )->CreateListBox( RendName, RendRect, Style, this );
            this->CheckAndInsertExcept( NewList );
            return NewList;
        }

        DropDownList* Screen::CreateDropDownList(const String& RendName, const UI::ScrollbarStyle Style)
        {
            DropDownList* NewList = static_cast<DropDownListFactory*>( this->GetWidgetFactoryExcept( DropDownList::TypeName ) )->CreateDropDownList( RendName, Style, this );
            this->CheckAndInsertExcept( NewList );
            return NewList;
        }

        DropDownList* Screen::CreateDropDownList(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            DropDownList* NewList = static_cast<DropDownListFactory*>( this->GetWidgetFactoryExcept( DropDownList::TypeName ) )->CreateDropDownList( RendName, RendRect, Style, this );
            this->CheckAndInsertExcept( NewList );
            return NewList;
        }

        HorizontalContainer* Screen::CreateHorizontalContainer(const String& RendName)
        {
            HorizontalContainer* NewHContain = static_cast<HorizontalContainerFactory*>( this->GetWidgetFactoryExcept( HorizontalContainer::TypeName ) )->CreateHorizontalContainer( RendName, this );
            this->CheckAndInsertExcept( NewHContain );
            return NewHContain;
        }

        HorizontalContainer* Screen::CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect)
        {
            HorizontalContainer* NewHContain = static_cast<HorizontalContainerFactory*>( this->GetWidgetFactoryExcept( HorizontalContainer::TypeName ) )->CreateHorizontalContainer( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewHContain );
            return NewHContain;
        }

        VerticalContainer* Screen::CreateVerticalContainer(const String& RendName)
        {
            VerticalContainer* NewVContain = static_cast<VerticalContainerFactory*>( this->GetWidgetFactoryExcept( VerticalContainer::TypeName ) )->CreateVerticalContainer( RendName, this );
            this->CheckAndInsertExcept( NewVContain );
            return NewVContain;
        }

        VerticalContainer* Screen::CreateVerticalContainer(const String& RendName, const UnifiedRect& RendRect)
        {
            VerticalContainer* NewVContain = static_cast<VerticalContainerFactory*>( this->GetWidgetFactoryExcept( VerticalContainer::TypeName ) )->CreateVerticalContainer( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewVContain );
            return NewVContain;
        }

        GridContainer* Screen::CreateGridContainer(const String& RendName)
        {
            GridContainer* NewGridContain = static_cast<GridContainerFactory*>( this->GetWidgetFactoryExcept( GridContainer::TypeName ) )->CreateGridContainer( RendName, this );
            this->CheckAndInsertExcept( NewGridContain );
            return NewGridContain;
        }

        GridContainer* Screen::CreateGridContainer(const String& RendName, const UnifiedRect& RendRect)
        {
            GridContainer* NewGridContain = static_cast<GridContainerFactory*>( this->GetWidgetFactoryExcept( GridContainer::TypeName ) )->CreateGridContainer( RendName, RendRect, this );
            this->CheckAndInsertExcept( NewGridContain );
            return NewGridContain;
        }


        /*Window* Screen::CreateWidgetWindow(ConstString& Name, const Rect& RendRect)
        {
            return static_cast<Window*>( this->CheckAndInsert( ExtendedRenderableFactory::CreateWidgetWindow(Name,RendRect) ) );
        }// */

        ///////////////////////////////////////////////////////////////////////////////
        // Atlas Query

        void Screen::SetPrimaryAtlas(const String& Atlas)
            { this->PrimaryAtlas = Atlas; }

        String Screen::GetPrimaryAtlas()
            { return this->PrimaryAtlas; }

        Vector2 Screen::GetWhitePixel(const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetWhitePixel(); }

        Sprite* Screen::GetSprite(const String& SpriteName,const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetSprite(SpriteName); }

        FontData* Screen::GetFont(const String& FontName,const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetFont(FontName); }

        Vector2 Screen::GetTextureSize(const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetTextureSize(); }

        TextureAtlas* Screen::GetAtlas(const String& Atlas) const
            { return this->UIMan->GetAtlas(Atlas); }

        Real Screen::GetTexelOffsetX() const
            { return this->SID->RenderSys->getHorizontalTexelOffset(); }

        Real Screen::GetTexelOffsetY() const
            { return this->SID->RenderSys->getVerticalTexelOffset(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Other Query

        bool Screen::IsMarkupParserRegistered(const String& ParserName) const
            { return this->UIMan->IsMarkupParserRegistered(ParserName); }

        MarkupParser* Screen::GetMarkupParser(const String& ParserName) const
            { return this->UIMan->GetMarkupParser(ParserName); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Screen::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( Screen::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("WindowTitle").SetValue( this->GameViewport->GetParentWindow()->GetCaption() ) &&
                PropertiesNode.AppendAttribute("ViewportZOrder").SetValue( this->GameViewport->GetZOrder() ) &&
                PropertiesNode.AppendAttribute("PriAtlas").SetValue( this->PrimaryAtlas ) )
            {
                XML::Node VertexTransformNode = PropertiesNode.AppendChild("VertexTransform");
                this->VertexTransform.ProtoSerialize( VertexTransformNode );
                XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
                this->Scale.ProtoSerialize( ScaleNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",Screen::GetSerializableName() + "Properties",true);
            }
        }

        void Screen::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Screen::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    String WindowTitle;
                    Whole ViewZOrder = 0;

                    // Get the single data type properties
                    CurrAttrib = PropertiesNode.GetAttribute("PriAtlas");
                    if( !CurrAttrib.Empty() )
                        this->PrimaryAtlas = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("WindowTitle");
                    if( !CurrAttrib.Empty() )
                        WindowTitle = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("ViewportZOrder");
                    if( !CurrAttrib.Empty() )
                        ViewZOrder = CurrAttrib.AsWhole();

                    // Get the properties that need their own nodes
                    XML::Node VertexTransformNode = PropertiesNode.GetChild("VertexTransform").GetFirstChild();
                    if( !VertexTransformNode.Empty() )
                        this->VertexTransform.ProtoDeSerialize(VertexTransformNode);

                    XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                    if( !ScaleNode.Empty() )
                        this->Scale.ProtoDeSerialize(ScaleNode);

                    if( !WindowTitle.empty() ) {
                        Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
                        Graphics::GameWindow* NamedWindow = GraphicsMan->GetGameWindowByCaption(WindowTitle);
                        if( NamedWindow != NULL ) {
                            this->GameViewport = NamedWindow->GetViewportByZOrder(ViewZOrder);
                            if( this->GameViewport != NULL ) {
                                this->ActDims.Size.X = (Real)this->GameViewport->GetActualWidth();
                                this->ActDims.Size.Y = (Real)this->GameViewport->GetActualHeight();
                                this->InverseSize.X = 1 / this->ActDims.Size.X;
                                this->InverseSize.Y = 1 / this->ActDims.Size.Y;
                            }else{
                                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"The Viewport specified via ZOrder was not found in the named GameWindow.");
                            }
                        }else{
                            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"The named GameWindow to be used by UI Screen was not found.");
                        }
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"A GameWindow Title/Caption was not specified for UI Screen.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Screen::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Screen::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Screen::GetDerivedSerializableName() const
            { return Screen::GetSerializableName(); }

        String Screen::GetSerializableName()
            { return "Screen"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions

        void Screen::_MarkDirty()
        {
            this->Dirty = true;
            this->MouseStrat->_NotifyScreenDirty();
        }

        void Screen::_MarkAllLayersDirty()
        {
            if( this->Dirty && this->AllLayersDirty )
                return;

            this->Dirty = true;
            this->AllLayersDirty = true;
        }

        void Screen::_RenderScreen()
        {
            Boole Force = false;
            if(Orientation != this->GameViewport->GetOrientationMode() ) {
                this->Orientation = GameViewport->GetOrientationMode();
                if(this->Orientation == Mezzanine::OM_Degree_90)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetHalfPi(),Vector3::Unit_Z()));
                else if(this->Orientation == Mezzanine::OM_Degree_180)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetPi(),Vector3::Unit_Z()));
                else if(this->Orientation == Mezzanine::OM_Degree_270)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetPi() * 1.5,Vector3::Unit_Z()));
                else
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(0,0,0,1));
                Force = true;
            }
            this->CheckViewportSize();
            this->_RenderVertices(Force);
            size_t KnownVertexCount = this->SID->RenderOp.vertexData->vertexCount;
            if(this->SID->RenderOp.vertexData->vertexCount) {
                if(this->TextureByVertex.size() == 0) {
                    AtlasAndPosition MyObject;
                    MyObject.RenderStart = 0;
                    MyObject.RenderEnd = KnownVertexCount;
                    MyObject.Atlas = this->PrimaryAtlas;
                    this->TextureByVertex.push_back(MyObject);
                }
                this->PrepareRenderSystem();
                String CurrAtlas = this->PrimaryAtlas;
                for( Whole TexIndex = 0 ; TexIndex < this->TextureByVertex.size() ; ++TexIndex )
                {
                    String& CurrVertAtlas = this->TextureByVertex[TexIndex].Atlas;
                    if(CurrVertAtlas.empty()) {
                        CurrVertAtlas = this->PrimaryAtlas;
                    }
                    if(CurrVertAtlas != CurrAtlas) {
                        CurrAtlas = CurrVertAtlas;
                        Ogre::TexturePtr TextureUse = this->UIMan->GetAtlas(CurrAtlas)->_GetTexture();
                        this->SID->RenderSys->_setTexture(0,true,TextureUse);
                    }
                    this->SID->RenderOp.vertexData->vertexCount = this->TextureByVertex[TexIndex].RenderEnd - TextureByVertex[TexIndex].RenderStart;
                    this->SID->RenderOp.vertexData->vertexStart = this->TextureByVertex[TexIndex].RenderStart;
                    this->SID->RenderSys->_render(this->SID->RenderOp);
                }
            }
        }

        void Screen::_SetOrientation(const Mezzanine::OrientationMode& Mode)
        {
            this->Orientation = Mode;
            if( this->Orientation == Mezzanine::OM_Degree_90 || this->Orientation == Mezzanine::OM_Degree_270 ) {
                std::swap(this->ActDims.Size.X,this->ActDims.Size.Y);
                std::swap(this->InverseSize.X,this->InverseSize.Y);
            }
        }

        void Screen::_Transform(ScreenRenderData& RenderData, const Whole& Begin, const Whole& End)
        {
            static const Matrix4x4 Iden;
            Whole X;
            if( Begin != End ) {
                for( X = Begin ; X < End ; X++ )
                {
                    RenderData[X].Vert.Position.X = ( ( RenderData[X].Vert.Position.X * this->InverseSize.X ) * 2 ) - 1;
                    RenderData[X].Vert.Position.Y = ( ( RenderData[X].Vert.Position.Y * this->InverseSize.Y ) * -2 ) + 1;
                }
            }
            if( this->VertexTransform != Iden ) {
                for( X = Begin ; X < End ; X++ )
                    RenderData[X].Vert.Position = this->VertexTransform * RenderData[X].Vert.Position;
            }
        }

        void Screen::_RenderVertices(bool Force)
        {
            if( !this->Dirty )
                return;

            Whole KnownVertexCount = 0;
            String CurrentName = this->PrimaryAtlas;
            AtlasAndPosition MyObject(this->PrimaryAtlas);
            this->TextureByVertex.clear();

            ScreenRenderData TempVertexCache;
            this->_AppendRenderDataCascading(TempVertexCache);
            KnownVertexCount = TempVertexCache.Size();
            this->_Transform(TempVertexCache,0,KnownVertexCount);

            this->ResizeVertexBuffer(KnownVertexCount);
            Vertex* WriteIterator = (Vertex*) this->SID->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            for( Whole Index = 0 ; Index < TempVertexCache.Size() ; ++Index )
            {
                if( TempVertexCache[Index].Atlas.empty() ) {
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Null or Empty String Atlas found when rendering UI.");
                }
                if( TempVertexCache[Index].Atlas != CurrentName ) {
                    if( Index != 0 ) {
                        MyObject.RenderEnd = Index;
                        this->TextureByVertex.push_back(MyObject);
                    }
                    MyObject.Atlas = TempVertexCache[Index].Atlas;
                    MyObject.RenderStart = Index;
                    CurrentName = TempVertexCache[Index].Atlas;
                }

                const Vertex& NewVertex = TempVertexCache[Index].Vert;
                *WriteIterator++ = NewVertex;
            }
            MyObject.RenderEnd = KnownVertexCount;
            MyObject.Atlas = CurrentName;
            this->TextureByVertex.push_back(MyObject);

            this->SID->VertexBuffer->unlock();
            this->SID->RenderOp.vertexData->vertexCount = KnownVertexCount;

            this->Dirty = false;
        }
    }//UI
}//Mezzanine

#endif
