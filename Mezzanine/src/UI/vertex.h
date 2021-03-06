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
#ifndef _uivertex_h
#define _uivertex_h

#include "vector3.h"
#include "vector2.h"
#include "colourvalue.h"

namespace Mezzanine
{
    namespace UI
    {
        /// @brief Basic class describing a vertex in the UI to be rendered.
        /// @details This is a very low level class used internally by most UI classes.  The values here should never need to be tempered with.
        struct MEZZ_LIB Vertex
        {
            /// @brief The 3D position of this vertex for placement in front of the frustrum.
            Vector3 Position;
            /// @brief The colour of the vertex.
            ColourValue Colour;
            /// @brief The UV position (or texture coordinates) of this vertex if a texture is being applied to it.
            Vector2 UV;
        };//Vertex

        /// @brief Basic class describing a vertex in the UI to be rendered.
        /// @details This class is necessary to exist since vertex declarations can't contain Atlas names.
        struct MEZZ_LIB VertexData
        {
            /// @brief The actual vertex data relevant to rendering.
            Vertex Vert;
            /// @brief Name of the Atlas the texture belongs to if a texture is being applied to it.
            String Atlas;

            /// @brief Assignment operator.
            void operator=(const VertexData& Other)
            {
                this->Vert.Position = Other.Vert.Position;
                this->Vert.Colour = Other.Vert.Colour;
                this->Vert.UV = Other.Vert.UV;
                this->Atlas = Other.Atlas;
            }
        };//Vertex
    }//UI
}//Mezzanine

#endif
