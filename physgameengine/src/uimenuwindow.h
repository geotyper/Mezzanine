//� Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _uimenuwindow_h
#define _uimenuwindow_h

#include "uiwindow.h"

#include <vector>
#include <utility>

namespace phys
{
    namespace UI
    {
        class UIlayer;
        class Menu;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MenuWindow
        /// @headerfile uimenuwindow.h
        /// @brief This class is an extended version of the window class for use exclusively with the menu widget.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB MenuWindow : public Window
        {
            protected:
                MenuWindow* ParentWindow;
                UI::Menu* MasterMenu;
                Button* BackButton;
                bool AutoHideWindow;
                std::vector<std::pair<Button*,MenuWindow*> > ChildWindows;
            public:
                /// @brief Standard initialization constructor.
                /// @param Name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                /// @param TheMenu The menu this window belongs to.
                /// @param Layer The parent layer this window belongs to.
                MenuWindow(ConstString& Name, const Vector2& Position, const Vector2& Size, UI::Menu* TheMenu, Layer* PLayer);
                /// @brief Standard destructor.
                virtual ~MenuWindow();
                /// @brief Sets whether or not thie window should auto hide when another window is added to the menu stack.
                /// @param AutoHide whether or not to enable auto hiding for this menu window.
                virtual void SetAutoHide(bool AutoHide);
                /// @brief Gets wether or not this window is set to auto hide when another window is added to the menu stack.
                /// @return Returns a bool indicating whether or not AutoHide is enabled on this menu window.
                virtual bool GetAutoHide();
                /// @brief Gets the parent window of this window.
                /// @return Returns a pointer to the parent window of this window, or NULL if this is the root window.
                virtual MenuWindow* GetParentWindow();
                /// @brief Creates a back button for this window.
                /// @details The name is autogenerated to be the name of the window + "back".
                /// This function will do nothing if the back button has already been created.
                /// @param Position The relative screen position to place the button at.
                /// @param Size The relative(to the screen) size the button should be.
                /// @return Returns a pointer to the created Back Button.
                virtual Button* CreateBackButton(const Vector2& Position, const Vector2& Size);
                /// @brief Creates a back button that is a text button for this window.
                /// @details The name is autogenerated to be the name of the window + "back".
                /// This function will do nothing if the back button has already been created.
                /// @param Position The relative screen position to place the button at.
                /// @param Size The relative(to the screen) size the button should be.
                /// @param Glyph The glyph set to use when generating the text for the button.
                /// @param Text The text to set for the button.
                /// @return Returns a pointer to the created Back Button.
                virtual TextButton* CreateBackButton(const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String& Text);
                /// @brief Gets the back button of this window.
                /// @return Returns a pointer to the Back Button, or NULL if it hasn't been created.
                virtual Button* GetBackButton();
                /// @brief Creates a button to be used for accessing child windows.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The position of the button.
                /// @param Size The size of the button.
                virtual Button* CreateAccessorButton(ConstString& Name, const Vector2& Position, const Vector2& Size);
                /// @brief Creates a text button to be used for accessing child windows.
                /// @return Returns a pointer to the created text button.
                /// @param Name The name of the button.
                /// @param Position The position of the button.
                /// @param Size The size of the button.
                /// @param Glyph The glyph set to use when generating the text for the button.
                /// @param Text The text to set for the button.
                virtual TextButton* CreateAccessorButton(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String& Text);
                /// @brief Gets an Accessor button by name.
                /// @return Returns a pointer to the named accessor button.
                /// @param Name The name of the accessor button you want to find.
                virtual Button* GetAccessorButton(ConstString& Name);
                /// @brief Gets the Window corresponding to the access button.
                /// @param Accessor Pointer to the Button of which you want to get the window for.
                /// @return Returns A pointer to the Menu window the button provided accesses.
                virtual MenuWindow* GetWindowOfAccessButton(Button* Accessor);
                /// @brief Adds a child window to this window.
                /// @return Returns a pointer to the created child window.
                /// @param Name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                virtual MenuWindow* CreateChildMenuWindow(ConstString& Name, const Vector2 Position, const Vector2 Size, Button* Accessor);
                /// @brief Gets an already created menu window by name.
                /// @return Returns a pointer to the menu window of the specified name.
                virtual MenuWindow* GetChildMenuWindow(ConstString& Name);
                /// @brief Gets an already created menu window by index.
                /// @return Returns a pointer to the menu window at the specified index.
                virtual MenuWindow* GetChildMenuWindow(const Whole& Index);
                /// @brief Gets the number of menu windows created and stored in this class.
                /// @return Returns the number of menu windows this class is storing.
                virtual Whole GetNumChildMenuWindows();
                /// @brief Destroys a menu window.
                /// @param ToBeDestroyed Pointer to the menu window you want destroyed.
                virtual void DestroyChildMenuWindow(MenuWindow* ToBeDestroyed);
        };
    }
}

#endif
