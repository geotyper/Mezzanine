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
#ifndef _boilerplatetests_h
#define _boilerplatetests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

const String SubSubMessage("This is output during a child process.");

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class boilerplatetests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "BoilerPlate".
    virtual String Name()
        { return String("BoilerPlate"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        // The TEST macro will capture Line, function file Metadata while
        TEST(true,"AutomaticTest");
        //TEST(false,"AutomaticFail");

        TEST_WARN(true,"AutomaticDoNotWarn");
        //TEST_WARN(false,"AutomaticWarn");

        TestResult Answer = Testing::Success;
        TEST_RESULT(Answer, "AutomaticTestResult");

        // Multiline example
        TEST_THROW  (
                        std::runtime_error&,
                        throw std::runtime_error("oh noes!");
                        , "AutomaticTestThrow"
                    );
        //TEST_THROW(std::runtime_error, throw "oh noes!";, "AutomaticTestThrow"); //Throws unexpected type so it fails

        TEST_NO_THROW( int x = 0; x++; , "ShouldNotThrow");


        String SubProcessOutput(LaunchSubProcessTest());
        TestOutput  << "Expecting: " << SubSubMessage << std::endl
                    << "Actual:    " << SubProcessOutput << std::endl;
        TEST(SubSubMessage==SubProcessOutput, "SubProcess");
    }
    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasAutomaticTests() const
        { return true; }


    /// @brief This is called when Interactive tests are run.
    void RunInteractiveTests()
    {
        TestResult Answer = GetTestAnswerFromStdin( "Is this a question? ");
        TEST_RESULT(Answer, "Interactive");
    }
    /// @brief Since RunInteractiveTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasInteractiveTests() const
        { return true; }


    /// @brief This will be launched in a sub sub process by the unit test framework.
    /// @param Arg Unused in this example.
    virtual void RunSubprocessTest(const String&)
    {
        // Please note that test macros mean nothing here. This is run in a subprocess
        // with as little interference from the Unit Test framework as possible
        std::cout << SubSubMessage;
    }
    /// @brief Indicates this test will launch a subprocess.
    /// @return Returns true to indicate this test will launch a subprocess of it's own.
    virtual bool HasSubprocessTest() const
        { return true; }
};//boilerplatetests

#endif
