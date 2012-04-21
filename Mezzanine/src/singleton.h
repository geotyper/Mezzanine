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
#ifndef _singleton_h
#define _singleton_h

#include "exception.h"

//#define THROW_ON_FETCH_FAIL

#if defined(_MSC_VER)
// Turn off warnings generated by this singleton implementation
#   pragma warning (disable : 4311)
#   pragma warning (disable : 4312)
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Singleton
    /// @headerfile singleton.h
    /// @brief This is a convenient base class intended to be used with classes that need to be singletons.
    /// @details This is based on the Ogre Singleton class.
    ///////////////////////////////////////
    template<class Type>
    class MEZZ_LIB Singleton
    {
        private:
            /// @brief Made private because copying a singleton is a no-no.
            Singleton(const Singleton<Type> &) {};
            /// @brief Made private because copying a singleton is a no-no.
            Singleton& operator=(const Singleton<Type> &) {};
        protected:
            static Type* SingletonPtr;
        public:
            /// @brief Class constructor.
            Singleton()
            {
                if(SingletonPtr)
                    throw(Exception("Singleton class already exists. Cannot make multiple singletons."));
                #if defined( _MSC_VER ) && _MSC_VER < 1200
                int offset = (int)(Type*)1 - (int)(Singleton <Type>*)(Type*)1;
                SingletonPtr = (Type*)((int)this + offset);
                #else
                SingletonPtr = static_cast<Type*>(this);
                #endif
            };
            /// @brief Class destructor.
            ~Singleton()
            {
                SingletonPtr = 0;
            };
            /// @brief Fetches a pointer to the singleton.
            /// @return Returns a pointer to the type of singleton this is.
            static Type* GetSingletonPtr()
            {
                #ifdef THROW_ON_FETCH_FAIL
                if(!SingletonPtr)
                    throw(Exception("Attempting to fetch invalid Singleton pointer."));
                #endif
                return SingletonPtr;
            };
            /// @brief Checks to see if the singleton pointer is valid.
            /// @return Returns true if the singleton is valid, false otherwise.
            static bool SingletonValid()
            {
                return SingletonPtr != NULL;
            };
    };//Singleton
}//Mezzanine

#endif
