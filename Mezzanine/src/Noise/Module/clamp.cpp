// � Copyright 2010 - 2017 BlackTopp Studios Inc.
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
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef noisemoduleclamp_cpp
#define noisemoduleclamp_cpp

#include "Noise/Module/clamp.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            Clamp::Clamp() :
                ModuleBase(this->GetSourceModuleCount()),
                m_lowerBound(Module::DEFAULT_CLAMP_LOWER_BOUND),
                m_upperBound(Module::DEFAULT_CLAMP_UPPER_BOUND)
                {  }

            PreciseReal Clamp::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                assert( this->m_pSourceModule[0] != NULL );

                PreciseReal value = this->m_pSourceModule[0]->GetValue(X, Y, Z);
                if( value < this->m_lowerBound ) {
                    return this->m_lowerBound;
                }else if( value > this->m_upperBound ) {
                    return this->m_upperBound;
                }else{
                    return value;
                }
            }

            void Clamp::SetBounds(const PreciseReal LowerBound, const PreciseReal UpperBound)
            {
                assert( LowerBound < UpperBound );

                this->m_lowerBound = LowerBound;
                this->m_upperBound = UpperBound;
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
