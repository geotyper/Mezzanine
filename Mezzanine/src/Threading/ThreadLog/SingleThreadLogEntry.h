// The DAGFrameScheduler is a Multi-Threaded lock free and wait free scheduling library.
// © Copyright 2010 - 2016 BlackTopp Studios Inc.
/* This file is part of The DAGFrameScheduler.

    The DAGFrameScheduler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DAGFrameScheduler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The DAGFrameScheduler.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the DAGFrameScheduler to anyone, including companies who wish to
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
#ifndef ThreadLog_SingleThreadLogEntry_H
#define ThreadLog_SingleThreadLogEntry_H

#include "ThreadIndex.h"

namespace Mezzanine {
namespace Threading {

    /// @brief A simple fixed sized log entry that is created when log entries are made for a single thread's log.
    class MEZZ_LIB SingleThreadLogEntry
    {
    public:
        ////////////////////////////////////////
        // Types
        using Clock = std::chrono::high_resolution_clock;

        ////////////////////////////////////////
        // Data members

        /// @brief When was this logged?
        Clock::time_point When;

        /// @brief A pointer to the logged message.
        const char* What;

        ////////////////////////////////////////
        // Constructors and semantics

        /// @brief Preferred constructor
        /// @param StartingWhat A pointer to the message describing what is logged, this is never dereferenced buy this.
        /// @param StartingWhen Default to capturing the current time, but can accept other times if needed.
        SingleThreadLogEntry(const char* StartingWhat,
                             Clock::time_point&& StartingWhen = Clock::now());

        /// @brief Default move constructor
        SingleThreadLogEntry(SingleThreadLogEntry&& ) = default;

        /// @brief Default copy constructor
        SingleThreadLogEntry(const SingleThreadLogEntry& ) = default;

        /// @brief Default destructor
        ~SingleThreadLogEntry() = default;

        /// @brief Default assignment operator
        SingleThreadLogEntry& operator=(const SingleThreadLogEntry&) = default;
    }; //SingleThreadLogEntry

    /// @brief Compare two Entries chronologically to see if the one on the left happened before the one on the right.
    /// @param LeftHand A SingleThreadLogEntry to check if it is before than RightHand.
    /// @param RightHand Compared to LeftHand to see if this is after than it.
    /// @return True if the Entry on the Left hand index came before the Right Entry.
    Boole MEZZ_LIB operator< (const SingleThreadLogEntry& LeftHand, const SingleThreadLogEntry& RightHand);

    /// @brief Serialize this log entry.
    /// @param OutputStream The stream to insert this into.
    /// @param ToStream The SingleThreadLogEntry to insert into the stream.
    /// @return A reference to the stream passed as OutputStream.
    std::ostream& MEZZ_LIB operator<<(std::ostream& Output, const SingleThreadLogEntry& ToStream);

} //Threading
} //Mezzanine

#endif
