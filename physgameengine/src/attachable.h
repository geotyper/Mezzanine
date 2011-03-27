//� Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _attachable_h
#define _attachable_h

#include "crossplatformexport.h"
#include "datatypes.h"


namespace phys
{
    //Forward Declarations
    class WorldNode;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Attachable
    /// @headerfile attachable.h
    /// @brief This is just a base class to be used by elements that are attachable to worldnodes.
    /// @details This class is useless on it's own and should not be created manually.
    ///////////////////////////////////////
    class PHYS_LIB Attachable
    {
        public:
            friend class WorldNode;

            /// @brief A list of possible types of this that could be attached to a WorldNode
            enum AttachableElement
            {
                None            = 0, ///< This is no-type of object, only used in error conditions
                Camera          = 1, ///< This is a phys::Camera
                Light           = 2, ///< This is a phys::Light
                ParticleEffect  = 3  ///< This is a phys::ParticleEffect
            };
        protected:
            /// @brief Enum value representing the type of element this is.
            Attachable::AttachableElement ElementType;

            /// @brief Sets the type of element this class is.
            /// @param Type Value representing the type of element to be set.
            void SetElementType(Attachable::AttachableElement Type);

            /// @brief The WorldNode this is attached to or null if not attached.
            WorldNode* AttachedTo;

            /// @brief This changes the WorldNode that this thinks it is attached to.
            /// @param NextWorldNode A pointer to the WorldNode this should think it is attached to.
            void SetAttachedTo(WorldNode* NextWorldNode);

        public:
            /// @brief No initialization class constructor.
            Attachable();

            /// @brief Class destructor.
            virtual ~Attachable();

            /// @brief Gets the type of element this is.
            /// @return Returns an enum value indicating what type of element this is.
            Attachable::AttachableElement GetElementType() const;

            /// @brief Gets the WorldNode this thinks it is attached to.
            /// @return Returns a pointer to the WorldNode this is attached to.
            WorldNode* GetAttachedTo() const;

            /// @brief Gets the name of the attachable element.
            virtual ConstString& GetName() const = 0;
    };
}//phys

#endif
