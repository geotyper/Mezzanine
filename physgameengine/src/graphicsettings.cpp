//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _graphicsettings_cpp
#define _graphicsettings_cpp

#include "world.h"
#include "eventmanager.h"
#include "graphicsettings.h"

namespace phys
{

    ///////////////////////////////////////////////////////////////////////////
    // Creation and Deletion functions
    ///////////////////////////////////
    GraphicsSettings::GraphicsSettings()
    {
        Construct(640,480,false);
    }

    GraphicsSettings::GraphicsSettings( const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        Construct( Width_, Height_, FullScreen_ );
    }

    void GraphicsSettings::Construct( const Whole &Width_, const Whole &Height_, const bool &FullScreen_ )
    {
        this->Fullscreen = FullScreen_;
        this->RenderHeight = Height_;
        this->RenderWidth = Width_;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Fullscreen functions
    ///////////////////////////////////
    bool GraphicsSettings::getFullscreen() const
    {
        return this->Fullscreen;
    }

    //returns: false if changes could not be made
    void GraphicsSettings::setFullscreen(const bool &Fullscreen_)
    {
        /// @todo TODO: We really should double check that going into fullscreen worked the way we wanted, this fails in too many games
        this->Fullscreen = Fullscreen_;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Resolution functions
    ///////////////////////////////////
    Whole GraphicsSettings::getRenderHeight() const
    {
        return this->RenderHeight;
    }

    Whole GraphicsSettings::getRenderWidth() const
    {
        return this->RenderWidth;
    }

    void GraphicsSettings::setRenderHeight(const Whole &Height_)
    {
        this->RenderHeight = Height_;
    }

    void GraphicsSettings::setRenderWidth(const Whole &Width_)
    {
        this->RenderWidth = Width_;
    }

}

#endif
