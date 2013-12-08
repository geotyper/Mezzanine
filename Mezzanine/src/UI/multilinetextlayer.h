//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#ifndef _uimultilinetextlayer_h
#define _uimultilinetextlayer_h

#include "UI/textlayer.h"
#include "UI/character.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLine;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a render layer specializing in multi-line text.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MultiLineTextLayer : public TextLayer
        {
        protected:
            /// @copydoc TextLayer::PopulateTextLinesImpl()
            virtual void PopulateTextLinesImpl();
            /// @copydoc TextLayer::GetCharacterIndexAtPositionImpl(const Vector2& Offset)
            virtual CharIndexPair GetIndexAtOffsetImpl(const Vector2& Offset);
            /// @copydoc TextLayer::GetOffsetAtIndex(const Integer Index)
            virtual CharOffsetPair GetOffsetAtIndexImpl(const Integer Index);
            /// @internal
            /// @brief Gets the TextLine at the requested index, or creates one if it doesn't exist.
            virtual TextLine* GetOrCreateTextLine(const UInt32& Index);
        public:
            /// @brief No-Font constructor.
            /// @param ParentRenderable The renderable that created this layer.
            MultiLineTextLayer(QuadRenderable* ParentRenderable);
            /// @brief Class constructor.
            /// @param FontName The name of the font to use for this layer.
            /// @param ParentRenderable The renderable that created this layer.
            MultiLineTextLayer(const String& FontName, QuadRenderable* ParentRenderable);
            /// @brief Text-Scaling constructor.
            /// @param LineHeight The height of the text to display relative to the parent renderable's height.
            /// @param ParentRenderable The renderable that created this layer.
            MultiLineTextLayer(const Real& LineHeight, QuadRenderable* ParentRenderable);
            /// @brief Class destructor.
            virtual ~MultiLineTextLayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc RenderLayer::GetLayerType() const
            virtual RenderLayer::RenderLayerType GetLayerType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Text Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc SimpleRenderer::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc SimpleRenderer::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc SimpleRenderer::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc SimpleRenderer::GetSerializableName()
            static String GetSerializableName();
        };//DialogLayer
    }//UI
}//Mezzanine

#endif
