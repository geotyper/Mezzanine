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
#ifndef _eventstests_h
#define _eventstests_h

#include "mezztest.h"
#include "eventsubscriber.h"
#include "eventpublisher.h"

#include <stdexcept> //only used to throw for TEST_THROW
#include <ostream>
#include <memory>

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class eventstests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "BoilerPlate"
    virtual String Name()
        { return String("Events"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        {//EventID
            EventID IntegerID(144);
            TEST(IntegerID.ID == 144,"EventID(const_IDType)");

            HashedString32 IDHashed("TestEventHash");
            EventID HashEventID(IDHashed);
            TEST(HashEventID.ID == IDHashed.GetHash(),"EventID(const_HashedString32&)");

            EventID CopyID(IntegerID);
            TEST(CopyID.ID == 144,"EventID(const_EventID&)");

            EventID MoveSourceID(288);
            EventID MoveDestID(std::move(MoveSourceID));
            TEST(MoveDestID.ID == 288,"EventID(EventID&&)");

            EventID AssignSourceID(576);
            EventID AssignDestID(0);
            AssignDestID = AssignSourceID;
            TEST(AssignDestID.ID == 576,"EventID::operator=(const_EventID&&)");

            EventID AssignMoveSourceID(1152);
            EventID AssignMoveDestID(0);
            AssignMoveDestID = std::move(AssignMoveSourceID);
            TEST(AssignMoveDestID.ID == 1152,"EventID::operator=(EventID&&)");

            TEST(EventID(50) == EventID(50),"EventID::operator==(const_EventID&)");
            TEST(EventID(100) != EventID(242),"EventID::operator!=(const_EventID&)");

            StringStream ResultStream;
            EventID StreamID(150);
            ResultStream << StreamID;
            TEST(ResultStream.str() == "150","operator<<(std::ostream&,const_EventID&)");
        }//EventID

        {//EventSubscriberID
            EventSubscriberID IntegerID(144);
            TEST(IntegerID.ID == 144,"EventSubscriberID(const_IDType)");

            EventSubscriberID* SubPtr = &IntegerID;
            EventSubscriberID PointerID(SubPtr);
            TEST(PointerID.ID == reinterpret_cast<EventSubscriberID::IDType>(SubPtr),"EventSubscriberID(PtrType*)");

            EventSubscriberID CopyID(IntegerID);
            TEST(CopyID.ID == 144,"EventSubscriberID(const_EventSubscriberID&)");

            EventSubscriberID MoveSourceID(288);
            EventSubscriberID MoveDestID(std::move(MoveSourceID));
            TEST(MoveDestID.ID == 288,"EventSubscriberID(EventSubscriberID&&)");

            EventSubscriberID AssignSourceID(576);
            EventSubscriberID AssignDestID(0);
            AssignDestID = AssignSourceID;
            TEST(AssignDestID.ID == 576,"EventSubscriberID::operator=(const_EventSubscriberID&&)");

            EventSubscriberID AssignMoveSourceID(1152);
            EventSubscriberID AssignMoveDestID(0);
            AssignMoveDestID = std::move(AssignMoveSourceID);
            TEST(AssignMoveDestID.ID == 1152,"EventSubscriberID::operator=(EventSubscriberID&&)");

            TEST(EventSubscriberID(50) == EventSubscriberID(50),"EventSubscriberID::operator==(const_EventSubscriberID&)");
            TEST(EventSubscriberID(100) != EventSubscriberID(242),"EventSubscriberID::operator!=(const_EventSubscriberID&)");

            StringStream ResultStream;
            EventSubscriberID StreamID(150);
            ResultStream << StreamID;
            TEST(ResultStream.str() == "150","operator<<(std::ostream&,const_EventSubscriberID&)");
        }//EventSubscriberID

        {//FunctionSubscriber
            {//SingleArg
                using TestSubscriberType = FunctionSubscriber<EventSubscriberID,void,EventPtr>;

                Whole DispatchCount = 0;
                EventPtr TestArg = std::make_shared<Event>("TestEvent");
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestSubscriberType IntegerSub(144,TestDelegate);
                TEST(IntegerSub.GetID().ID == 144,"FunctionSubscriber(const_IDType,const_FunctionType&)-SingleArg");

                TestSubscriberType CopySub(IntegerSub);
                TEST(CopySub.GetID().ID == 144,"FunctionSubscriber(const_FunctionSubscriber&)-SingleArg");

                TestSubscriberType MoveSourceSub(288,TestDelegate);
                TestSubscriberType MoveDestSub(std::move(MoveSourceSub));
                TEST(MoveDestSub.GetID().ID == 288,"FunctionSubscriber(FunctionSubscriber&&)-SingleArg");

                TestSubscriberType AssignSourceSub(576,TestDelegate);
                TestSubscriberType AssignDestSub(0,TestDelegate);
                AssignDestSub = AssignSourceSub;
                TEST(AssignDestSub.GetID().ID == 576,"FunctionSubscriber::operator=(const_FunctionSubscriber&&)-SingleArg");

                TestSubscriberType AssignMoveSourceSub(1152,TestDelegate);
                TestSubscriberType AssignMoveDestSub(0,TestDelegate);
                AssignMoveDestSub = std::move(AssignMoveSourceSub);
                TEST(AssignMoveDestSub.GetID().ID == 1152,"FunctionSubscriber::operator=(FunctionSubscriber&&)-SingleArg");

                TestSubscriberType DispatchSub(2304,TestDelegate);
                DispatchSub(TestArg);
                TEST(DispatchCount == 1,"FunctionSubscriber::DispatchEvent(Args...)_const-SingleArg");
            }//SingleArg

            {//MixedArg
                using TestSubscriberType = FunctionSubscriber<EventSubscriberID,void,const Real&, Whole>;

                Whole DispatchCount = 0;
                Real TestArg1 = 3.1415926;
                Whole TestArg2 = 8675309;
                TestSubscriberType::FunctionType TestDelegate([&](const Real& fn, Whole ln){ ++DispatchCount; });

                TestSubscriberType IntegerSub(144,TestDelegate);
                TEST(IntegerSub.GetID().ID == 144,"FunctionSubscriber(const_IDType,const_FunctionType&)-MixedArg");

                TestSubscriberType CopySub(IntegerSub);
                TEST(CopySub.GetID().ID == 144,"FunctionSubscriber(const_FunctionSubscriber&)-MixedArg");

                TestSubscriberType MoveSourceSub(288,TestDelegate);
                TestSubscriberType MoveDestSub(std::move(MoveSourceSub));
                TEST(MoveDestSub.GetID().ID == 288,"FunctionSubscriber(FunctionSubscriber&&)-MixedArg");

                TestSubscriberType AssignSourceSub(576,TestDelegate);
                TestSubscriberType AssignDestSub(0,TestDelegate);
                AssignDestSub = AssignSourceSub;
                TEST(AssignDestSub.GetID().ID == 576,"FunctionSubscriber::operator=(const_FunctionSubscriber&)-MixedArg");

                TestSubscriberType AssignMoveSourceSub(1152,TestDelegate);
                TestSubscriberType AssignMoveDestSub(0,TestDelegate);
                AssignMoveDestSub = std::move(AssignMoveSourceSub);
                TEST(AssignMoveDestSub.GetID().ID == 1152,"FunctionSubscriber::operator=(FunctionSubscriber&&)-MixedArg");

                TestSubscriberType DispatchSub(2304,TestDelegate);
                DispatchSub(TestArg1,TestArg2);
                TEST(DispatchCount == 1,"FunctionSubscriber::DispatchEvent(Args...)_const-MixedArg");
            }//MixedArg
        }//FunctionSubscriber

        {//EventSubscriberBindingImpl
            using TestTableType = DefaultEventBindingTable<DefaultSubscriberType>;
            using TestSubscriberType = TestTableType::SubscriberType;
            using TestBindingType = TestTableType::BindingType;
            using TestBindingPtrType = TestTableType::BindingPtrType;
            using TestBindingImplType = TestTableType::BindingImplType;
            using TestBindingImplPtrType = TestTableType::BindingImplPtrType;

            {//NoTable
                Whole DispatchCount = 0;
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestBindingImplType IntegerBinding(TestSubscriberType(144,TestDelegate),nullptr);
                TEST(IntegerBinding.GetID().ID == 144 &&
                     IntegerBinding.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::ConstructionAndGetters-NoTable");

                TestBindingImplType MoveBinding(std::move(IntegerBinding));
                TEST(MoveBinding.GetID().ID == 144 &&
                     MoveBinding.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::(EventSubscriberBindingImpl&&)-NoTable");

                TestBindingImplType AssignMoveSourceBind(TestSubscriberType(288,TestDelegate),nullptr);
                TestBindingImplType AssignMoveDestBind(TestSubscriberType(0,TestDelegate),nullptr);
                AssignMoveDestBind = std::move(AssignMoveSourceBind);
                TEST(AssignMoveDestBind.GetID().ID == 288 &&
                     AssignMoveDestBind.GetTable() == nullptr,
                     "EventSubscriberBindingImpl::operator=(EventSubscriberBindingImpl&&)-NoTable");

                TestBindingImplType UtilityTestBinding(TestSubscriberType(576,TestDelegate),nullptr);
                TEST( !UtilityTestBinding.IsSubscribed() ,"EventSubscriberBindingImpl::IsSubscribed()_const-NoTable");
            }//NoTable

            {//Table
                Whole DispatchCount = 0;
                HashedString32 FirstTestTableName("FirstTestTable");
                TestTableType FirstTestTable(FirstTestTableName);
                HashedString32 SecondTestTableName("SecondTestTable");
                TestTableType SecondTestTable(SecondTestTableName);
                TestSubscriberType::FunctionType TestDelegate([&](EventPtr Args){ ++DispatchCount; });

                TestBindingImplPtrType IntegerBinding = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(144,TestDelegate)));
                TEST(IntegerBinding->GetID().ID == 144 &&
                     IntegerBinding->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::ConstructionAndGetters-Table");

                TestBindingImplPtrType MoveBinding(IntegerBinding);
                TEST(MoveBinding->GetID().ID == 144 &&
                     MoveBinding->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::(EventSubscriberBindingImpl&&)-Table");

                TestBindingImplPtrType AssignMoveSourceBind = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(288,TestDelegate)));
                TestBindingImplPtrType AssignMoveDestBind = std::static_pointer_cast<TestBindingImplType>(SecondTestTable.Subscribe(TestSubscriberType(578,TestDelegate)));
                *AssignMoveDestBind = std::move(*AssignMoveSourceBind);
                TEST(AssignMoveDestBind->GetID().ID == 288 &&
                     AssignMoveDestBind->GetTable() == &FirstTestTable,
                     "EventSubscriberBindingImpl::operator=(EventSubscriberBindingImpl&&)-Table");

                TestBindingImplPtrType UtilityTestBinding = std::static_pointer_cast<TestBindingImplType>(FirstTestTable.Subscribe(TestSubscriberType(1156,TestDelegate)));
                Boole SubscribedPassTest = UtilityTestBinding->IsSubscribed();

                UtilityTestBinding->UpdateTable(&SecondTestTable);
                TEST(UtilityTestBinding->GetTable() == &SecondTestTable,
                     "EventSubscriberBindingImpl::UpdateTable(TableType*)-Table");

                UtilityTestBinding->Unbind();
                TEST(UtilityTestBinding->GetTable() == nullptr,
                     "EventSubscriberBindingImpl::Unbind()-Table");

                Boole SubscribedFailTest = !UtilityTestBinding->IsSubscribed();
                TEST(SubscribedPassTest && SubscribedFailTest,
                     "EventSubscriberBindingImpl::IsSubscribed()_const-Table");

                UtilityTestBinding->UpdateTable(&FirstTestTable);
                UtilityTestBinding->Unsubscribe();
                TEST(UtilityTestBinding->IsSubscribed() == false &&
                     FirstTestTable.GetBinding(1156) == nullptr,
                     "EventSubscriberBindingImpl::Unsubscribe()-Table");
            }//Table
        }//EventSubscriberBindingImpl

        {//EmptyEventDispatcher

        }//EmptyEventDispatcher

        {//SilenceableEventDispatcher

        }//SilenceableEventDispatcher

        {//EventBindingTable

        }//EventBindingTable

        {//EventSubscriptionTable

        }//EventSubscriptionTable

        {//MonoEventPublisher

        }//MonoEventPublisher

        {//MultiEventPublisher

        }//MultiEventPublisher

        /*
        {//EventSubscriptionTable
            Whole DispatchCount = 0;
            EventNameType TestName("TestName");
            EventSubscriberBinding::CallbackType TestCallback([&](EventPtr Args) { ++DispatchCount; });
            EventPtr TestEvent = std::make_shared<Mezzanine::Event>(TestName);

            EventPublisher TestPublisher;
            TestPublisher.AddSubscriptionTable(TestName);

            EventSubscriptionTable TestTable(TestName.GetHash());
            TEST(TestTable.GetHash() == TestName.GetHash(),
                 "EventSubscriptionTable::ConstructionAndGetters");

            EventSubscriberBindingPtr FirstBinding = TestTable.Subscribe(16,TestCallback,&TestPublisher);
            EventSubscriberBindingPtr SecondBinding = TestTable.Subscribe(32,TestCallback,nullptr);
            EventSubscriberBindingPtr ThirdBinding = TestTable.Subscribe(48,TestCallback,&TestPublisher);
            TEST(FirstBinding->GetSubID() == 16 && FirstBinding->GetPublisher() == &TestPublisher && FirstBinding->GetEventHash() == TestName.GetHash() &&
                 SecondBinding->GetSubID() == 32 && SecondBinding->GetPublisher() == nullptr && SecondBinding->GetEventHash() == TestName.GetHash() &&
                 ThirdBinding->GetSubID() == 48 && ThirdBinding->GetPublisher() == &TestPublisher && ThirdBinding->GetEventHash() == TestName.GetHash(),
                 "EventSubscriptionTable::Subscribe");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION>::Type,
                       TestTable.Subscribe(16,TestCallback,nullptr),
                       "EventSubscriptionTable::Subscribe-Throw");

            EventSubscriberBindingPtr FirstFetchedBinding = TestTable.GetBinding(16);
            EventSubscriberBindingPtr SecondFetchedBinding = TestTable.GetBinding(32);
            EventSubscriberBindingPtr ThirdFetchedBinding = TestTable.GetBinding(48);
            TEST(FirstBinding == FirstFetchedBinding &&
                 SecondBinding == SecondFetchedBinding &&
                 ThirdBinding == ThirdFetchedBinding,
                 "EventSubscriptionTable::GetBinding");

            TestTable.DispatchEvent(TestEvent);
            TEST(DispatchCount == 3,
                 "EventSubscriptionTable::DispatchEvent");

            Whole BeforeRefCount = FirstBinding.use_count();
            TestTable.Unsubscribe(16);
            Whole AfterRefCount = FirstBinding.use_count();
            TEST(BeforeRefCount == 3 && AfterRefCount == 2,
                 "EventSubscriptionTable::Unsubscribe");

            Whole TotalRemoveCount = TestTable.UnsubscribeAll();
            TEST(TotalRemoveCount == 2 &&
                 SecondBinding.use_count() == 2 &&
                 ThirdBinding.use_count() == 2,
                 "EventSubscriptionTable::UnsubscribeAll");
        }//EventSubscriptionTable

        {//EventPublisher
            Whole DispatchCount = 0;
            EventNameType FirstTestName("FirstTest");
            EventNameType SecondTestName("SecondTest");
            EventNameType ThirdTestName("ThirdTest");
            EventSubscriberBinding::CallbackType TestCallback([&](EventPtr Args) { ++DispatchCount; });
            EventPtr FirstTestEvent = std::make_shared<Mezzanine::Event>(FirstTestName);
            EventPtr SecondTestEvent = std::make_shared<Mezzanine::Event>(SecondTestName);

            EventPublisher TestPublisher(2);

            EventPublisher::SubscriptionTableIterator TableIt = TestPublisher.AddSubscriptionTable(FirstTestName);
            Boole FirstTestAdd = (*TableIt).GetHash() == FirstTestName.GetHash();
            TableIt = TestPublisher.AddSubscriptionTable(SecondTestName);
            Boole SecondTestAdd = (*TableIt).GetHash() == SecondTestName.GetHash();
            TableIt = TestPublisher.AddSubscriptionTable(ThirdTestName);
            Boole ThirdTestAdd = (*TableIt).GetHash() == ThirdTestName.GetHash();
            TEST(FirstTestAdd && SecondTestAdd && ThirdTestAdd,
                 "EventPublisher::ConstructionAndAddSubscriptionTable");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION>::Type,
                       TestPublisher.AddSubscriptionTable(FirstTestName),
                       "EventPublisher::AddSubscriptionTable-Throw");

            TEST(TestPublisher.HasSubscriptionTable(FirstTestName) &&
                 TestPublisher.HasSubscriptionTable(SecondTestName.GetHash()) &&
                 TestPublisher.HasSubscriptionTable(ThirdTestName) &&
                 !TestPublisher.HasSubscriptionTable("FailTest"),
                 "EventPublisher::HasSubscriptionTable");

            const EventPublisher& ConstTestPublisher = TestPublisher;
            TEST(TestPublisher.GetSubscriptionTable(FirstTestName.GetHash())->GetHash() == FirstTestName.GetHash() &&
                 TestPublisher.GetSubscriptionTable(SecondTestName.GetHash())->GetHash() == SecondTestName.GetHash() &&
                 TestPublisher.GetSubscriptionTable(ThirdTestName.GetHash())->GetHash() == ThirdTestName.GetHash() &&
                 ConstTestPublisher.GetSubscriptionTable(FirstTestName.GetHash())->GetHash() == FirstTestName.GetHash() &&
                 ConstTestPublisher.GetSubscriptionTable(SecondTestName.GetHash())->GetHash() == SecondTestName.GetHash() &&
                 ConstTestPublisher.GetSubscriptionTable(ThirdTestName.GetHash())->GetHash() == ThirdTestName.GetHash(),
                 "EventPublisher::GetSubscriptionTable");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       TestPublisher.GetSubscriptionTable("FailFetch"),
                       "EventPublisher::GetSubscriptionTable-ThrowString");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       TestPublisher.GetSubscriptionTable(EventNameType::EmptyHash),
                       "EventPublisher::GetSubscriptionTable-ThrowHash");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       ConstTestPublisher.GetSubscriptionTable("FailFetch"),
                       "EventPublisher::GetSubscriptionTable-ThrowStringConst");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       ConstTestPublisher.GetSubscriptionTable(EventNameType::EmptyHash),
                       "EventPublisher::GetSubscriptionTable-ThrowHashConst");

            EventSubscriberBindingPtr FirstBinding = TestPublisher.Subscribe(FirstTestName,18,TestCallback);
            EventSubscriberBindingPtr SecondBinding = TestPublisher.Subscribe(FirstTestName,36,TestCallback);
            EventSubscriberBindingPtr ThirdBinding = TestPublisher.Subscribe(FirstTestName,54,TestCallback);
            EventSubscriberBindingPtr FourthBinding = TestPublisher.Subscribe(SecondTestName,72,TestCallback);
            EventSubscriberBindingPtr FifthBinding = TestPublisher.Subscribe(SecondTestName,90,TestCallback);
            EventSubscriberBindingPtr SixthBinding = TestPublisher.Subscribe(SecondTestName,108,TestCallback);
            TEST(FirstBinding->GetSubID() == 18 && FirstBinding->GetEventHash() == FirstTestName.GetHash() &&
                 SecondBinding->GetSubID() == 36 && SecondBinding->GetEventHash() == FirstTestName.GetHash() &&
                 ThirdBinding->GetSubID() == 54 && ThirdBinding->GetEventHash() == FirstTestName.GetHash() &&
                 FourthBinding->GetSubID() == 72 && FourthBinding->GetEventHash() == SecondTestName.GetHash() &&
                 FifthBinding->GetSubID() == 90 && FifthBinding->GetEventHash() == SecondTestName.GetHash() &&
                 SixthBinding->GetSubID() == 108 && SixthBinding->GetEventHash() == SecondTestName.GetHash(),
                 "EventPublisher::Subscribe");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       TestPublisher.Subscribe("FailTableName",0,TestCallback),
                       "EventPublisher::Subscribe-Throw");

            TestPublisher.DispatchEvent(FirstTestEvent);
            Whole PostFirstDispatchCount = DispatchCount;
            TestPublisher.DispatchEvent(SecondTestEvent);
            Whole PostSecondDispatchCount = DispatchCount;
            TEST(PostFirstDispatchCount == 3 &&
                 PostSecondDispatchCount == 6,
                 "EventPublisher::DispatchEvent");

            TestPublisher.SetMuteEvents(true);
            TestPublisher.DispatchEvent(FirstTestEvent);
            TestPublisher.DispatchEvent(SecondTestEvent);
            TEST(TestPublisher.GetMuteEvents() == true &&
                 DispatchCount == 6,
                 "EventPublisher::GetAndSetMuteEvents");

            TestPublisher.SetMuteEvents(false);
            TestPublisher.Unsubscribe(FirstTestName,36);
            TestPublisher.DispatchEvent(FirstTestEvent);
            TEST(DispatchCount == 8 &&
                 FirstBinding.use_count() == 2 &&
                 SecondBinding.use_count() == 1 &&
                 ThirdBinding.use_count() == 2,
                 "EventPublisher::UnsubscribeFromEvent");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       TestPublisher.Unsubscribe("FailTableName",0),
                       "EventPublisher::UnsubscribeFromEvent-Throw");

            Whole FirstRemoveCount = TestPublisher.UnsubscribeAll(FirstTestName);
            TestPublisher.DispatchEvent(FirstTestEvent);
            TEST(FirstRemoveCount == 2 &&
                 DispatchCount == 8 &&
                 FirstBinding.use_count() == 1 &&
                 SecondBinding.use_count() == 1 &&
                 ThirdBinding.use_count() == 1,
                 "EventPublisher::UnsubscribeAllFromEvent");
            TEST_THROW(ExceptionFactory<ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION>::Type,
                       TestPublisher.UnsubscribeAll("FailTableName"),
                       "EventPublisher::UnsubscribeAllFromEvent-Throw");

            TestPublisher.Subscribe(FirstTestName,108,TestCallback);
            Whole PreRemoveAllUseCount = SixthBinding.use_count();
            TestPublisher.Unsubscribe(108);
            Whole PostRemoveAllUseCount = SixthBinding.use_count();
            TestPublisher.DispatchEvent(SecondTestEvent);
            TEST(DispatchCount == 10 &&
                 PreRemoveAllUseCount == 2 &&
                 PostRemoveAllUseCount == 1,
                 "EventPublisher::UnsubscribeFromAllEvents");

            TestPublisher.Subscribe(FirstTestName,108,TestCallback);
            Whole SecondRemoveCount = TestPublisher.UnsubscribeAll();
            TestPublisher.DispatchEvent(SecondTestEvent);
            TEST(SecondRemoveCount == 3 &&
                 DispatchCount == 10 &&
                 FourthBinding.use_count() == 1 &&
                 FifthBinding.use_count() == 1 &&
                 SixthBinding.use_count() == 1,
                 "EventPublisher::UnsubscribeAllFromAllEvents");

            TestPublisher.RemoveSubscriptionTable(SecondTestName);
            TEST(TestPublisher.HasSubscriptionTable(FirstTestName) &&
                 !TestPublisher.HasSubscriptionTable(SecondTestName) &&
                 TestPublisher.HasSubscriptionTable(ThirdTestName),
                 "EventPublisher::RemoveSubscriptionTable");

            TestPublisher.RemoveAllSubscriptionTables();
            TEST(!TestPublisher.HasSubscriptionTable(FirstTestName) &&
                 !TestPublisher.HasSubscriptionTable(SecondTestName) &&
                 !TestPublisher.HasSubscriptionTable(ThirdTestName),
                 "EventPublisher::RemoveAllSubscriptionTables");
        }//EventPublisher
        */
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//eventstests

#endif

