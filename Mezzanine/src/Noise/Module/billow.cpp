// � Copyright 2010 - 2014 BlackTopp Studios Inc.
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
// Copyright (C) 2004 Jason Bevins
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

#ifndef noisemodulebillow_cpp
#define noisemodulebillow_cpp

#include "Noise/Module/billow.h"

#include "Noise/noisegen.h"

#include "MathTools/arithmetic.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            Billow::Billow() :
                ModuleBase     (this->GetSourceModuleCount()),
                m_frequency    (Module::DEFAULT_BILLOW_FREQUENCY   ),
                m_lacunarity   (Module::DEFAULT_BILLOW_LACUNARITY  ),
                m_noiseQuality (Module::DEFAULT_BILLOW_QUALITY     ),
                m_octaveCount  (Module::DEFAULT_BILLOW_OCTAVE_COUNT),
                m_persistence  (Module::DEFAULT_BILLOW_PERSISTENCE ),
                m_seed         (Module::DEFAULT_BILLOW_SEED)
                {  }

            PreciseReal Billow::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                PreciseReal value = 0.0;
                PreciseReal signal = 0.0;
                PreciseReal curPersistence = 1.0;
                PreciseReal nx, ny, nz;
                Integer Seed;

                PreciseReal CurrX = X * this->m_frequency;
                PreciseReal CurrY = Y * this->m_frequency;
                PreciseReal CurrZ = Z * this->m_frequency;

                for( Integer curOctave = 0 ; curOctave < this->m_octaveCount ; curOctave++ )
                {
                    // Make sure that these Realing-point values have the same range as a 32-
                    // bit integer so that we can pass them to the coherent-noise functions.
                    nx = Noise::MakeInt32Range(CurrX);
                    ny = Noise::MakeInt32Range(CurrY);
                    nz = Noise::MakeInt32Range(CurrZ);

                    // Get the coherent-noise value from the input value and add it to the
                    // final result.
                    Seed = ( this->m_seed + curOctave ) & 0xffffffff;
                    signal = Noise::GradientCoherentNoise3D(nx, ny, nz, Seed, this->m_noiseQuality);
                    signal = 2.0 * MathTools::Abs(signal) - 1.0;
                    value += signal * curPersistence;

                    // Prepare the next octave.
                    CurrX *= this->m_lacunarity;
                    CurrY *= this->m_lacunarity;
                    CurrZ *= this->m_lacunarity;
                    curPersistence *= this->m_persistence;
                }
                value += 0.5;

                return value;
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
