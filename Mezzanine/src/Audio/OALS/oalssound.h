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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalssound_h
#define _audiooalssound_h

#include "Audio/sound.h"

// OALS forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCcontext;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Filter;
            class Effect;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for a 2D sound.
            ///////////////////////////////////////
            class MEZZ_LIB Sound : public iSound
            {
            public:
                /// @brief Basic container type for the storage of @ref iEffect instances by this class.
                typedef std::vector<OALS::Effect*>       EffectContainer;
                /// @brief Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::iterator        EffectIterator;
                /// @brief Const Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::const_iterator  ConstEffectIterator;
                /// @brief Basic container type for the storage of internal buffer handles.
                typedef std::vector<UInt32>              BufferContainer;
                /// @brief Iterator type for buffer handles stored by this class.
                typedef BufferContainer::iterator        BufferIterator;
                /// @brief Const Iterator type for buffer handles stored by this class.
                typedef BufferContainer::const_iterator  ConstBufferIterator;
            protected:
                /// @brief The OALS context this sound belongs to.
                ALCcontext* Context;
                /// @brief The filter processing audio emitted by this sound.
                OALS::Filter* SoundFilter;
                /// @brief The decoder responsible for tracking this sounds position in the audio stream and decompressing into something we can populate buffers with.
                iDecoder* SoundDecoder;
                /// @brief The type of sound this is.
                UInt16 SType;
                /// @brief This is a bitfield storing the the current state of this sound.
                UInt16 State;
                /// @brief The speed of playback modifier for the sound.  Higher values make playback sound more high pitched.
                Real SoundPitch;
                /// @brief The base volume applied to this sound alone, before external modifiers.
                Real BaseVolume;
                /// @brief The minimum volume this sound is allowed to reach, regardless of other settings.
                Real MinVolume;
                /// @brief The maximum volume this sound is allowed to reach, regardless of other settings.
                Real MaxVolume;
                /// @brief Handle to the internal OALS source representing this sound.
                UInt32 InternalSource;
                /// @brief This stores the handles to all the OALS buffers owned by this sound.
                BufferContainer Buffers;
                /// @brief This stores the effects currently being applied to audio emitted by this sound.
                EffectContainer Effects;

                /// @brief Makes the context this listener belongs to the current context.
                void MakeCurrent();
                /// @brief Streams the content from the decoder into an OALS buffer.
                /// @param BufferID The ID of the buffer to be streamed to.
                /// @return Returns true if the streaming was successful, false if there was a failure.
                Boole StreamToBuffer(const UInt32 BufferID);
                /// @brief Updates the internal data of the attached filter to reflect any changes made to the filter externally.
                void RefreshFilter();
                /// @brief Updates the internal data of the attached effects to reflect any changes made to the effects externally.
                void RefreshEffects();
            public:
                /// @brief Class constructor.
                /// @param Type The type of sound to initialize.  See @ref Audio::SoundType enum for the basic values.
                /// @param Decode A pointer to the decoder assigned to this sound.
                /// @param PlayContext A pointer to the context responsible for decoder audio streams for this sound.
                Sound(const UInt16 Type, iDecoder* Decode, ALCcontext* PlayContext);
                /// @brief Class destructor.
                virtual ~Sound();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iSound::IsValid() const
                virtual Boole IsValid() const;
                /// @copydoc iSound::GetType() const
                virtual UInt16 GetType() const;
                /// @copydoc iSound::GetDecoder() const
                virtual iDecoder* GetDecoder() const;

                /// @copydoc iSound::SetPitch(const Real)
                virtual void SetPitch(const Real Pitch);
                /// @copydoc iSound::GetPitch() const
                virtual Real GetPitch() const;
                /// @copydoc iSound::SetStream(DataStreamPtr, const Audio::Encoding)
                virtual void SetStream(DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc iSound::SetStream(const UInt16, DataStreamPtr, const Audio::Encoding)
                virtual void SetStream(const UInt16 Type, DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc iSound::SetStream(iDecoder*)
                virtual void SetStream(iDecoder* Decode);
                /// @copydoc iSound::SetStream(const UInt16, iDecoder*)
                virtual void SetStream(const UInt16 Type, iDecoder* Decode);

                ///////////////////////////////////////////////////////////////////////////////
                // Playback

                /// @copydoc iSound::Play()
                virtual Boole Play();
                /// @copydoc iSound::IsPlaying() const
                virtual Boole IsPlaying() const;
                /// @copydoc iSound::Pause()
                virtual void Pause();
                /// @copydoc iSound::IsPaused() const
                virtual Boole IsPaused() const;
                /// @copydoc iSound::Stop()
                virtual void Stop();
                /// @copydoc iSound::IsStopped() const
                virtual Boole IsStopped() const;
                /// @copydoc iSound::Loop(Boole ToLoop)
                virtual void Loop(Boole ToLoop);
                /// @copydoc iSound::IsLooping() const
                virtual Boole IsLooping() const;

                /// @copydoc iSound::Seek(const Real, Boole)
                virtual Boole Seek(const Real Seconds, Boole Relative = false);

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Control

                /// @copydoc iSound::GetVolume() const
                virtual Real GetVolume() const;
                /// @copydoc iSound::SetBaseVolume(const Real)
                virtual void SetBaseVolume(const Real Base);
                /// @copydoc iSound::GetBaseVolume() const
                virtual Real GetBaseVolume() const;
                /// @copydoc iSound::SetMinVolume(const Real)
                virtual void SetMinVolume(const Real MinVol);
                /// @copydoc iSound::GetMinVolume() const
                virtual Real GetMinVolume() const;
                /// @copydoc iSound::SetMaxVolume(const Real)
                virtual void SetMaxVolume(const Real MaxVol);
                /// @copydoc iSound::GetMaxVolume() const
                virtual Real GetMaxVolume() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Effects Methods

                /// @copydoc iSound::AttachEffect(const UInt32, iEffect*)
                virtual Boole AttachEffect(const UInt32 Slot, iEffect* Eff);
                /// @copydoc iSound::GetEffect(const UInt32 Slot) const
                virtual iEffect* GetEffect(const UInt32 Slot) const;
                /// @copydoc iSound::GetMaxEffectSlots() const
                virtual UInt32 GetMaxEffectSlots() const;
                /// @copydoc iSound::GetNumEffectSlotsAvailable() const
                virtual UInt32 GetNumEffectSlotsAvailable() const;
                /// @copydoc iSound::RemoveEffect(const UInt32)
                virtual void RemoveEffect(const UInt32 Slot);

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Methods

                /// @copydoc iSound::AttachFilter(iFilter*)
                virtual Boole AttachFilter(iFilter* Fil);
                /// @copydoc iSound::GetFilter() const
                virtual iFilter* GetFilter() const;
                /// @copydoc iSound::RemoveFilter()
                virtual void RemoveFilter();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Updates all the necessary components of this sound.
                virtual void _Update();
                /// @internal
                /// @brief Updates the buffers for this sound as necessary for seemless playback.
                /// @return Returns true of there were no errors updating, false if there were.
                virtual Boole _UpdateBuffers();
            };//Sound
        }//OALS
    }//Audio
}//Mezzanine

#endif
