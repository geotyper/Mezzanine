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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduraltextilegenerator_h
#define _graphicsproceduraltextilegenerator_h

#include "Graphics/Procedural/Texture/texturegenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a textile structured image.
            /// @details Creates a textile structure from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB TextileGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The background colour of the textile to be generated.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
            public:
                /// @brief Class constructor.
                TextileGenerator();
                /// @brief Class destructor.
                virtual ~TextileGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The colour to be applied to the textile to be generated.
                /// @return Returns a reference to this.
                TextileGenerator& SetColour(const ColourValue& Colour);
                /// @brief Set the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                TextileGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                TextileGenerator& SetSeed(const Whole Seed);
            };//TextileGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
