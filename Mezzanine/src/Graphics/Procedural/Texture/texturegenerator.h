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
#ifndef _graphicsproceduraltexturegenerator_h
#define _graphicsproceduraltexturegenerator_h

#include "Graphics/Procedural/proceduralenumerations.h"
#include "Graphics/Procedural/Texture/texturebuffer.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A base class for texture generators that use the TextureBuffer class.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB TextureGenerator
            {
            protected:
            public:
                /// @brief Blank constructor.
                TextureGenerator();
                /// @brief Class destructor.
                virtual ~TextureGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Generates the Texture.
                /// @param SquareSize The width and height of the Texture to create.  This value will be clamped to be at least 8.
                /// @param TexName The name to give to the generated Texture.
                /// @param TexGroup The asset group to place the Texture in.
                /// @param Format The pixel format for the generated Texture.
                /// @return Returns a pointer to the newly created Texture.
                Texture* GenerateTexture(const Whole SquareSize, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;
                /// @brief Generates the Texture.
                /// @param TexWidth The pixel width of the Texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the Texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexName The name to give to the generated Texture.
                /// @param TexGroup The asset group to place the Texture in.
                /// @param Format The pixel format for the generated Texture.
                /// @return Returns a pointer to the newly created Texture.
                Texture* GenerateTexture(const Whole TexWidth, const Whole TexHeight, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;

                /// @brief Generates the Image.
                /// @remarks The generated image is not owned by this generator or anything.  It is up to the user/caller of this method to clean it up.
                /// @param SquareSize The width and height of the Image to create.  This value will be clamped to be at least 8.
                /// @param Format The pixel format for the generated Image.
                /// @return Returns a pointer to the newly created Image.
                Image* GenerateImage(const Whole SquareSize, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;
                /// @brief Generates the Image.
                /// @remarks The generated image is not owned by this generator or anything.  It is up to the user/caller of this method to clean it up.
                /// @param TexWidth The pixel width of the Image to be generated.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the Image to be generated.  This value will be clamped to be at least 8.
                /// @param Format The pixel format for the generated Image.
                /// @return Returns a pointer to the newly created Image.
                Image* GenerateImage(const Whole TexWidth, const Whole TexHeight, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;

                /// @brief Creates a TextureBuffer with the pixels as configured in this generator.
                /// @param SquareSize The width and height of the Texture to create.  This value will be clamped to be at least 8.
                /// @return Returns a new TextureBuffer with the generated pixels of this generator.
                TextureBuffer BuildTextureBuffer(const Whole SquareSize) const;
                /// @brief Creates a TextureBuffer with the pixels as configured in this generator.
                /// @param TexWidth The pixel width of the Texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the Texture to be generated.  This value will be clamped to be at least 8.
                /// @return Returns a new TextureBuffer with the generated pixels of this generator.
                TextureBuffer BuildTextureBuffer(const Whole TexWidth, const Whole TexHeight) const;
                /// @brief Replaces and populates the pixels as configured in this generator to a TextureBuffer.
                /// @param Buffer The buffer to place this generators pixels in.
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const = 0;

                /// @brief Gets the name of this generator.
                /// @return Returns a String containing the name of this generator.
                virtual String GetName() const = 0;
            };//TextureGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
