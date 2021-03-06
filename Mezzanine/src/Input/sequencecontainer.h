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
#ifndef _inputsequencecontainer_h
#define _inputsequencecontainer_h

#include "trie.h"
#include "timer.h"
#include "Input/metacode.h"

namespace Mezzanine
{
    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class stores and provides utilities for sequenced inputs and their operations.
        ///////////////////////////////////////
        class MEZZ_LIB SequenceContainer
        {
        public:
            /// @brief Container type for recognized input sequences.
            using SequencedInputContainer = Trie<Input::MetaCode,Int32>;
            /// @brief Iterator type for recognized input sequences.
            using SequencedInputIterator = SequencedInputContainer::iterator;
            /// @brief Const Iterator type for recognized input sequences.
            using ConstSequencedInputIterator = SequencedInputContainer::const_iterator;
            /// @brief Convenience type for a time-stamp from std::chrono.
            using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
        protected:
            /// @brief Container storing all the recognized input sequences.
            SequencedInputContainer SequencedInputs;
            /// @brief Container storing the current input sequence.
            MetaCodeContainer CurrSequence;
            /// @brief Timer used to help detect input sequences.
            Timer SequenceTimer;
            /// @brief The maximum amount of time allowed to pass between inputs in the sequence.
            Whole SequenceTimeout;
            /// @brief The maximum number of inputs to keep cached for detecting sequenced inputs.
            UInt16 MaxSequenceSize;
            /// @brief The index of the device to detect sequenced inputs for.
            UInt32 DeviceID;

            /// @brief Checks the amount of time since our last update to see if our input cache is still valid.
            void CheckTimeout();
            /// @brief Removes any codes from the start of the sequence cache that we don't need to track anymore.
            void TruncateSequenceCache();
            /// @brief Performs a single sequence test to see if it matches any configured sequences.
            /// @param TestBegin An iterator to the start of the range of new codes to test.
            /// @param TestEnd An iterator to the end of the range of new codes to test.
            /// @return Returns a MetaValue for a completed sequence, or an InvalidSequence if no sequence was completed.
            Int32 TestSequenceSingle(ConstMetaCodeIterator TestBegin, ConstMetaCodeIterator TestEnd);
            /// @brief Tests the currently tracked set of inputs for sequences that have been completed.
            /// @remarks This will also consume all the cached inputs that were identified as a sequence, as well as inputs prior to the sequence.
            /// @return Returns a MetaValue for a completed sequence, or an InvalidSequence if no sequence was completed.
            Int32 TestCurrentSequence();
        public:
            /// @brief Class constructor.
            SequenceContainer();
            /// @brief Copy constructor.
            /// @param Other The other sequence container to be copied.
            SequenceContainer(const SequenceContainer& Other) = default;
            /// @brief Move constructor.
            /// @param Other the other sequence container to be moved.
            SequenceContainer(SequenceContainer&& Other) = default;
            /// @brief Controller constructor.
            /// @param Device The index of the controller device this container belongs to.
            SequenceContainer(const UInt32 Device);
            /// @brief Class destructor.
            ~SequenceContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other sequence container to be copied.
            /// @return Returns a reference to this.
            SequenceContainer& operator=(const SequenceContainer& Other) = default;
            /// @brief Move assignment operator.
            /// @param Other The other sequence container to be moved.
            /// @return Returns a reference to this.
            SequenceContainer& operator=(SequenceContainer&& Other) = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @brief Adds a custom sequence of inputs that this system will look for and generate MetaCode's for when they occur.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.  An exception can also be thrown if the ID provided is the max value of an Int32.
            /// @param Codes A vector containing the sequence of MetaCode's to be added.
            /// @param SequenceID A unique UInt32 to be used as the identifier for this sequence when a MetaCode is generated.
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32 SequenceID);
            /// @brief Checks to see if the provided sequence of MetaCode's is already being checked for.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to check for.
            /// @return Returns true if the sequence already exists, false otherwise.
            Boole InputSequenceExists(const MetaCodeContainer& Codes);
            /// @brief Gets the ID of the provided sequence of MetaCode's.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to get the ID for.
            /// @return Returns the ID of the provided sequence, or the max value of an Int32 if the sequence does not exist.
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes);
            /// @brief Removes the specified custom sequence of MetaCode's.
            /// @exception If the vector of MetaCode's doesn't end with a null MetaCode, an exception will be thrown.
            /// @param Codes A vector containing the sequence of MetaCode's to be removed.
            void RemoveInputSequence(const MetaCodeContainer& Codes);
            /// @brief Removes all stored input sequences.
            void RemoveAllInputSequences();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the amount of time that the input cache will hold on to previous inputs for sequence detection.
            /// @param Timeout The amount of time in milliseconds to allow before clearing the input cache for a new sequence.  Default: 300 milliseconds.
            void SetSequenceTimeout(const Whole Timeout);
            /// @brief Gets the amount of time that the input cache will hold on to previous inputs for sequence detection.
            /// @return Returns how much time this will permit between inputs before clearing the cache in milliseconds.
            Whole GetSequenceTimeout() const;

            /// @brief Gets the number of input sequences stored in this sequence container.
            /// @return Returns a UInt32 containing the number of sequences stored.
            UInt32 GetNumInputSequences() const;
            /// @brief Adds a single code to the cache and checks for sequences that have been met.
            /// @param Delta The new code to be added.
            /// @return Returns a MetaCode with the generated sequence code if a sequence was met, or a NullMetaCode if none were met.
            MetaCode DetectSequence(const MetaCode& Delta);
            /// @brief Adds a range of codes to the cache and checks for sequences that have been met.
            /// @param DeltaBegin An iterator to the start of the range of new codes to update with.
            /// @param DeltaEnd An iterator to the end of the range of new codes to update with.
            /// @return Returns a vector of the codes generated during the update.
            MetaCodeContainer DetectSequence(ConstMetaCodeIterator DeltaBegin, ConstMetaCodeIterator DeltaEnd);
        };//SequenceContainer
    }//Input
}//Mezzanine

#endif
