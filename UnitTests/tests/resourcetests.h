//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _resourcetests_h
#define _resourcetests_h

#include "mezztest.h"
#include "resourcemanager.h"

/// @file
/// @brief A few tests of the resource system, only partial coverage

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering primarily directory management.
class resourcetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Resource"
        virtual String Name()
            { return String("Resource"); }

        /// @brief Test only directory creation and removal
        /// @param TestDirString the directory to attempt to create and remove
        /// @details The functions under test are required to test one another. This creates an unusual
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void CreateRemoveDirectory(const String& TestDirString)
        {
            Bool Exists = Mezzanine::ResourceManager::DoesDirectoryExist(TestDirString);
            cout << "Does testing directory exists: " << Exists << endl;
            if(Exists)
            {
                cout << "Removing " << TestDirString << endl;
                Mezzanine::ResourceManager::RemoveDirectory(TestDirString);
            }else{
                cout << "No cleanup required" << endl;
            }

            Exists = Mezzanine::ResourceManager::DoesDirectoryExist(TestDirString);
            if(Exists)
            {
                TEST(false, "RemoveDirectory");
                TEST_RESULT(Testing::Skipped, "CreateDirectory");
            }else{
                cout << "Attempting to create a directory: " << TestDirString << endl;
                Mezzanine::ResourceManager::CreateDirectory(TestDirString);
                Exists = Mezzanine::ResourceManager::DoesDirectoryExist(TestDirString);
                TEST(Exists, "CreateDirectory");
                cout << "Did it work: " << Exists << endl;

                if(Exists)
                {
                    cout << "Testing removal of: " << TestDirString << endl;
                    Mezzanine::ResourceManager::RemoveDirectory(TestDirString);
                    Exists = Mezzanine::ResourceManager::DoesDirectoryExist(TestDirString);
                    TEST(!Exists, "RemoveDirectory");
                    cout << "Did it work: " << !Exists << endl;
                }else{
                    TEST_RESULT(Testing::Skipped, "RemoveDirectory");
                }
            }
        }

        /// @brief Verify the basename and dirname functions does their jobs right
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void TestBaseAndDirName()
        {
            cout << "Testing DirName: " << endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::ResourceManager::DirName("File.txt") << "\"" << endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"c:\\\"\n\tActual:\t\"" << Mezzanine::ResourceManager::DirName("c:\\untitled doc.txt") << "\"" << endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::ResourceManager::DirName("") << "\"" << endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"/a/b/\"\n\tActual:\t\"" << Mezzanine::ResourceManager::DirName("/a/b/c") << "\"" << endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"/a/b/c/\"\n\tActual:\t\"" << Mezzanine::ResourceManager::DirName("/a/b/c/") << "\"" << endl
                 << endl;
            Test(Mezzanine::ResourceManager::DirName("File.txt")=="","DirNameShouldBeEmpty");
            Test(Mezzanine::ResourceManager::DirName("c:\\untitled doc.txt")=="c:\\","DirNamewindows");
            Test(Mezzanine::ResourceManager::DirName("")=="","DirNameEmptyNotCrash");
            Test(Mezzanine::ResourceManager::DirName("/a/b/c")=="/a/b/","DirNameUnix");
            Test(Mezzanine::ResourceManager::DirName("/a/b/c/")=="/a/b/c/","DirNameUnixDir");

            cout << "Testing BaseName: " << endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"File.txt\"\n\tActual:\t\"" << Mezzanine::ResourceManager::BaseName("File.txt") << "\"" << endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"untitled doc.txt\"\n\tActual:\t\"" << Mezzanine::ResourceManager::BaseName("c:\\untitled doc.txt") << "\"" << endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::ResourceManager::BaseName("") << "\"" << endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"c\"\n\tActual:\t\"" << Mezzanine::ResourceManager::BaseName("/a/b/c") << "\"" << endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::ResourceManager::BaseName("/a/b/c/") << "\"" << endl
                 << endl;
            Test(Mezzanine::ResourceManager::BaseName("File.txt")=="File.txt","BaseNameShouldCopy");
            Test(Mezzanine::ResourceManager::BaseName("c:\\untitled doc.txt")=="untitled doc.txt","BaseNamewindows");
            Test(Mezzanine::ResourceManager::BaseName("")=="","BaseNameEmptyNotCrash");
            Test(Mezzanine::ResourceManager::BaseName("/a/b/c")=="c","BaseNameUnix");
            Test(Mezzanine::ResourceManager::BaseName("/a/b/c/")=="","BaseNameUnixDir");
        }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            String TestDirString(TestingScratchDir);
            TestDirString += "resourcetesting";

            try
            {
                CreateRemoveDirectory(TestDirString);
            } catch (Mezzanine::IOException& e) {
                cout << "Error testing directory creation and removal: " << e.what() << endl;
            }
            cout << endl;

            TestBaseAndDirName();

            ArgC;
            ArgV;


            //cout << "Attempting to create a Mezzanine::ResourceManager" << endl;
            //Mezzanine::ResourceManager res;
            //TEST(true,"ConstructionDidNotFailMiserably");

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


        /// @brief This is called when Interactive tests are run
        void RunInteractiveTests()
        {
            TestResult Answer = GetTestAnswerFromStdin( "Is this a question? ");
            TEST_RESULT(Answer, "Interactive");
        }

        /// @brief Since RunInteractiveTests is implemented so is this.
        /// @return returns true
        virtual bool HasInteractiveTests() const
            { return true; }

};

#endif

