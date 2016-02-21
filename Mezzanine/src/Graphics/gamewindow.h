// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _graphicsgamewindow_h
#define _graphicsgamewindow_h

#include "Graphics/windowsettings.h"

namespace Ogre
{
    class RenderWindow;
}

struct SDL_Window;

namespace Mezzanine
{
    namespace Graphics
    {
        class GraphicsManager;
        class Viewport;
        class CameraProxy;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is for creating and managing game windows.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GameWindow
        {
        public:
            /// @brief Basic container type for Viewport storage by this class.
            typedef std::list<Viewport*> ViewportContainer;
            /// @brief Iterator type for Viewport instances stored by this class.
            typedef ViewportContainer::iterator ViewportIterator;
            /// @brief Const Iterator type for Viewport instances stored by this class.
            typedef ViewportContainer::const_iterator ConstViewportIterator;
            /// @brief Reverse Iterator type for Viewport instances stored by this class.
            typedef ViewportContainer::reverse_iterator ReverseViewportIterator;
            /// @brief Const Reverse Iterator type for Viewport instances stored by this class.
            typedef ViewportContainer::const_reverse_iterator ConstReverseViewportIterator;

            /// @brief A listing of potential options for configuring a game window during construction.
            enum WindowFlags
            {
                WF_Fullscreen = 1,    ///< This enables fullscreen on the window.
                WF_Hidden = 2,        ///< This hides the window so that it isn't visible.
                WF_VsyncEnabled = 4,  ///< This enables vsync for the window.
                WF_FSAA_2 = 8,        ///< Enables Fullscreen Anti-Aliasing level 2 for the window.
                WF_FSAA_4 = 16,       ///< Enables Fullscreen Anti-Aliasing level 4 for the window.
                WF_FSAA_8 = 32,       ///< Enables Fullscreen Anti-Aliasing level 8 for the window.
                WF_FSAA_16 = 64,      ///< Enables Fullscreen Anti-Aliasing level 16 for the window.
                WF_Resizeable = 128,  ///< Creates a window with resizable borders, otherwise it is fixed size.
                WF_Maximized = 256,   ///< Maximizes the window immediately after construction.
                WF_Borderless = 512   ///< Removes all window decorations from the window(titlebar, borders, etc.).
            };
        protected:
            /// @internal
            /// @brief A struct storing all the window dimensions and update settings.
            WindowSettings Settings;
            /// @internal
            /// @brief A container storing all the viewports belonging to this window.
            ViewportContainer Viewports;
            /// @internal
            /// @brief A pointer to the manager that created this window.
            GraphicsManager* Manager;
            /// @internal
            /// @brief A pointer to the internal window used for rendering.
            Ogre::RenderWindow* OgreWindow;
            /// @internal
            /// @brief A pointer to the internal window used for collecting input.
            SDL_Window* SDLWindow;
            /// @internal
            /// @brief The last set FSAA level for this gamewindow (used for serialization).
            Whole RequestedFSAA;
            /// @internal
            /// @brief A bit field containing all the flags used in the construction of this GameWindow.
            Whole CreationFlags;

            /// @internal
            /// @brief Convenience method for constructing a window.
            /// @param WindowCaption The caption to be set in the window titlebar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see WindowFlags enum for more info.
            void CreateGameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags);
            /// @internal
            /// @brief Inserts a Viewport into this window based on it's ZOrder.
            /// @param NewVP A pointer to the Viewport being added to this window.
            void AddViewport(Viewport* NewVP);
            /// @internal
            /// @brief Updates all the viewports of this window and the cameras attached to them after a change in window geometry.
            void UpdateViewportsAndCameras();
        public:
            /// @brief Class constructor.
            /// @param WindowCaption The caption to be set in the window titlebar.
            /// @param Width The desired width in pixels.
            /// @param Height The desired height in pixels.
            /// @param Flags Additional misc parameters, see WindowFlags enum for more info.
            GameWindow(const String& WindowCaption, const Whole Width, const Whole Height, const Whole Flags);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            GameWindow(const XML::Node& XMLNode);
            /// @brief Class destructor.
            ~GameWindow();

            ///////////////////////////////////////////////////////////////////////////////
            // Viewport Management

            /// @brief Creates an additional Viewport within a created render window.
            /// @param VeiwportCamera The camera that is to be attached to this Viewport.
            /// @param ZOrder The render order of this viewport relative to other viewports in this window.
            /// @return Returns a pointer to the created Viewport.
            Viewport* CreateViewport(CameraProxy* ViewportCamera, const Integer ZOrder);
            /// @brief Gets a viewport by index.
            /// @param Index The index of the Viewport to retrieve.
            /// @return Returns a pointer to the viewport requested.
            Viewport* GetViewport(const Whole Index) const;
            /// @brief Gets a viewport by ZOrder.
            /// @param ZOrder The ZOrder of the Viewport to retrieve.
            /// @return Returns a pointer at the specified ZOrder, or NULL if no Viewports use that ZOrder.
            Viewport* GetViewportByZOrder(const Integer ZOrder) const;
            /// @brief Gets the number of viewports within this window.
            /// @return Returns a Whole representing the number of viewports within this window.
            Whole GetNumViewports() const;
            /// @brief Destroys a viewport within this window.
            /// @param ToBeDestroyed The viewport that will be destroyed.
            void DestroyViewport(Viewport* ToBeDestroyed);
            /// @brief Destroys every viewport within this window.
            void DestroyAllViewports();

            /// @brief Gets an iterator to the first viewport in this window.
            ViewportIterator BeginViewport();
            /// @brief Gets an iterator to one passed the last viewport in this window.
            ViewportIterator EndViewport();
            /// @brief Gets a const iterator to the first viewport in this window.
            ConstViewportIterator BeginViewport() const;
            /// @brief Gets a const iterator to one passed the last viewport in this window.
            ConstViewportIterator EndViewport() const;

            /// @brief Gets an iterator to the last viewport in this window.
            ReverseViewportIterator ReverseBeginViewport();
            /// @brief Gets an iterator to one before the first viewport in this window.
            ReverseViewportIterator ReverseEndViewport();
            /// @brief Gets a const iterator to the last viewport in this window.
            ConstReverseViewportIterator ReverseBeginViewport() const;
            /// @brief Gets a const iterator to one before the first viewport in this window.
            ConstReverseViewportIterator ReverseEndViewport() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Window Metrics Management

            /// @brief Sets the width of the game window.
            /// @param Width The pixel size of the game window on the X axis.
            void SetWidth(const Whole& Width);
            /// @brief Gets the Width of the game window.
            /// @return Returns a Whole containing the size of the game window on the X axis.
            Whole GetWidth() const;
            /// @brief Sets the height of the game window.
            /// @param Height The pixel size of the game window on the Y axis.
            void SetHeight(const Whole& Height);
            /// @brief Gets the height of the game window.
            /// @return Returns a Whole containing the size of the game window on the Y axis.
            Whole GetHeight() const;

            /// @brief Sets the width and height of the game window.
            /// @param WinRes A Resolution containing the width and height to be applied.
            void SetResolution(const Resolution& WinRes);
            /// @brief Sets the width and height of the game window.
            /// @param Width The pixel size of the game window on the X axis.
            /// @param Height The pixel size of the game window on the Y axis.
            void SetResolution(const Whole& Width, const Whole& Height);
            /// @brief Gets the width and height of the game window.
            /// @return Returns a const Resolution reference containing the pixel size of the window on the X and Y axes.
            const Resolution& GetResolution() const;

            /// @brief Set the Fullscreen Setting.
            /// @param Fullscreen This accepts a Boole. True for fullscreen, false for windowed.
            void SetFullscreen(const Boole Fullscreen);
            /// @brief Gets the Fullscreen Setting.
            /// @return This returns a Boole, true if fullscreen is set, false otherwise
            Boole GetFullscreen() const;

            /// @brief Changes the X Resolution, Y Resolution, and fullscreen at the same time.
            /// @param NewSettings A WindowSettings struct containing all of the window settings to be applied.
            void SetRenderOptions(const WindowSettings& NewSettings);
            /// @brief Gets the current window settings.
            /// @return Returns a WindowSettings struct with the current window settings.
            const WindowSettings& GetSettings();

            ///////////////////////////////////////////////////////////////////////////////
            // Window Settings Methods

            /// @brief Gets the the text in the titlebar.
            /// @return Returns a const string ref of the text in the titlebar.
            const String& GetWindowCaption() const;

            /// @brief Sets the level of Full Scale Anti-Aliasing to be used when rendering to this window.
            /// @note Generally when this gets set it will not be applied.  Anti-Aliasing is a construction time property of a window.
            /// However this will be saved when the game window and it's settings are serialized, which will be used when it is reloaded.
            /// @param FSAA The Anti-Aliasing level to be used, generally one of these values: 0, 2, 4, 8, or 16.
            void SetFSAALevel(const Whole FSAA);
            /// @brief Gets the last set Anti-Aliasing level on this Window.
            /// @note This may or may not be the same as the current FSAA level setting.
            /// @return Returns a Whole indicating which level of AA is set on this window.
            Whole GetFSAALevel() const;
            /// @brief Gets the actual Anti-Aliasing level currently being used by this game window.
            /// @return Returns the FSAA level currently being used to render this window, or 0 if FSAA is disabled.
            Whole GetActualFSAALevel() const;

            /// @brief Enables (or disables) vsync on this window.
            /// @param Enable Whether or not to enable vsync.
            void EnableVsync(Boole Enable);
            /// @brief Gets whether or not vsync is currently enabled on this window.
            /// @return Returns true if vsync is currently enabled, false otherwise.
            Boole VsyncEnabled() const;
            /// @brief Hides (shows) the window.
            /// @param Hidden Whether or not to hide the window.
            void SetHidden(Boole Hidden);
            /// @brief Gets whether this window is hidden.
            /// @return Returns true if this window is being hidden, false otherwise.
            Boole IsHidden() const;
            /// @brief Gets whether or not this window has a resizeable border.
            /// @return Returns true if this window has a border when not fullscreen and it can be resized, false otherwise.
            Boole BorderIsResizeable() const;
            /// @brief Gets whether this window is borderless.
            /// @return Returns true if this window has no border, false otherwise.
            Boole IsBorderless() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Window Stats Methods

            /// @brief Gets the FPS based on the last frame rendered.
            /// @details This essentially determines how many frames could be rendered if all
            /// frames within a second rendered at the same speed.
            /// @return Returns a Real representing the framerate.
            Real GetLastFPS() const;
            /// @brief Gets the Average FPS.
            /// @return Returns a Real representing the average framerate.
            Real GetAverageFPS() const;
            /// @brief Gets the Best FPS.
            /// @return Returns a Real representing the best framerate.
            Real GetBestFPS() const;
            /// @brief Gets the Worst FPS.
            /// @return Returns a Real representing the worst framerate.
            Real GetWorstFPS() const;
            /// @brief Gets the shortest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the best time for rendering a frame.
            Real GetBestFrameTime() const;
            /// @brief Gets the longest amount of time it's taken to render a frame.
            /// @return Returns a Real representing the worst time for rendering a frame.
            Real GetWorstFrameTime() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this GameWindow should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Convert the properties of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the viewports of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            void ProtoSerializeViewports(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the viewports of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerializeViewports(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief This will get a pointer to the Ogre RenderWindow.
            /// @return Returns a pointer to the Ogre RenderWindow class in use.
            Ogre::RenderWindow* _GetOgreWindowPointer();
            /// @internal
            /// @brief This will get a pointer to the SDL Window.
            /// @return Returns a pointer to the SDL Window class in use.
            SDL_Window* _GetSDLWindowPointer();
        };//GameWindow
    }//Graphics
}//Mezzanine

#endif
