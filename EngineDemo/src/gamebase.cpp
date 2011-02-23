#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include <physgame.h>       //Physgame include
#include <sstream>          //STL includes

#include <string>
#include <iostream>
#include <fstream>            //Only used for testing
#define PHYSDEBUG

using namespace phys;
using namespace std;

//Create the World.... Globally! and set it to hold some actors
World *TheWorld;

const Plane PlaneOfPlay( Vector3(2.0,1.0,-5.0), Vector3(1.0,2.0,-5.0), Vector3(1.0,1.0,-5.0));

int main(int argc, char **argv)
{
    try
    {
        TheWorld = new World( Vector3(-30000.0,-30000.0,-30000.0), Vector3(30000.0,30000.0,30000.0), SceneManager::Generic, 30);
    }catch( exception x){
        cerr << "Could not create world: " << x.what();
        return 1;
        //could not create the perfect worldending program
    }

    #ifdef PHYSDEBUG
    TheWorld->Log("World Created:");
    TheWorld->Log(PlaneOfPlay);
    #endif

    // Set the Title
    TheWorld->SetWindowName("Catch!... The Game!");
    TheWorld->SetTargetFrameRate(60);
    #ifdef PHYSDEBUG
    TheWorld->Log("Framerate and Title set");
    #endif

    //Give the world functions to run before and after input and physics
    TheWorld->GetEventManager()->SetPreMainLoopItems(&PreInput);
    TheWorld->GetEventManager()->SetPostMainLoopItems(&PostInput);
    TheWorld->GetPhysicsManager()->SetPreMainLoopItems(&PrePhysics);
    TheWorld->GetPhysicsManager()->SetPostMainLoopItems(&PostPhysics);
    TheWorld->GetGraphicsManager()->SetPostMainLoopItems(&PostRender);
    #ifdef PHYSDEBUG
    TheWorld->Log("Managers Created");
    #endif

    //Set the Make the RenderWindow and load system stuff
	TheWorld->GameInit(false);
    #ifdef PHYSDEBUG
    TheWorld->Log("Initialized games");
    #endif

    //Set up polling for the letter Q and middle mouse button, and the mouse X and Y locations
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::KEY_q) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_3) );
    TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_1) );
    TheWorld->GetEventManager()->RemovePollingCheck( MetaCode(0, MetaCode::MOUSEBUTTON_3) );
    //TheWorld->GetEventManager()->AddPollingCheck( MetaCode(0, MetaCode::MOUSEABSOLUTEHORIZONTAL) );   //Not supported

    //Actually Load the game stuff
    LoadContent();

    //Generate the UI
    MakeGUI();

    //Configure the wireframe Drawer
    TheWorld->GetPhysicsManager()->SetDebugPhysicsWireCount(2);
    TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(0);

    //Setup some camera tricks
    WorldNode* CameraNode = TheWorld->GetSceneManager()->CreateOrbitingNode( "Orbit1", Vector3(0,0,0), Vector3(0.0,200.0,750.0), true );
    CameraNode->AttachElement(TheWorld->GetCameraManager()->GetDefaultCamera());

	//Start the Main Loop
	TheWorld->MainLoop();

    //delete TheWorld;
	return 0;
}

bool PostRender()
{
	//Lets set a variable for the time
	static Whole gametime = 0;

	TheWorld->Log(String("---------- Starting PosterRender CallBack -------------"));
    TheWorld->Log(String("Current Game Time "));

	//getting a message from the event manager)
	EventRenderTime* CurrentTime = TheWorld->GetEventManager()->PopNextRenderTimeEvent();
    Whole LastFrame = 0;

    // Is CurrentTime a valid event?
    while(0 != CurrentTime)
    {
        LastFrame = CurrentTime->getMilliSecondsSinceLastFrame();

        TheWorld->Log(gametime);
        gametime+=CurrentTime->getMilliSecondsSinceLastFrame();

        delete CurrentTime;
        CurrentTime = TheWorld->GetEventManager()->GetNextRenderTimeEvent();
    }

    //Play around with the title bar
    std::stringstream timestream;
    timestream << "Catch!... " << gametime;
    TheWorld->SetWindowName( timestream.str() );

    ActorBase* Act1 = TheWorld->GetActorManager()->FindActor("RobotWayUpFrontLeft");
    ActorBase* Act2 = TheWorld->GetActorManager()->FindActor("RobotWayUpFrontRight");
    if (Act1->IsAnimated())
    {
        Act1->AdvanceAnimation((Real)0.001 * LastFrame);
    }

    if (Act2->IsAnimated())
    {
        Act2->AdvanceAnimation((Real)0.0001 * LastFrame);
    }

    static bool notplayed=true;
    //if (1000<gametime && notplayed)
    if (notplayed)
    {
        notplayed=false;
        Sound* Welcome = NULL;
        Welcome = TheWorld->GetSoundManager()->GetSoundByName("Welcome");
        if(Welcome)
        {
            Welcome->Play2d(false);
        }
        #ifdef PHYSDEBUG
        TheWorld->Log("Played Welcome Fun:");
        #endif

    }

    // Update Stat information
    UI::Caption* CurFPS = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("StatsLayer")->GetCaption("CurFPS");
    UI::Caption* AvFPS = TheWorld->GetUIManager()->GetScreen("DefaultScreen")->GetLayer("StatsLayer")->GetCaption("AvFPS");
    std::stringstream CFPSstream;
    std::stringstream AFPSstream;
    CFPSstream << TheWorld->GetGraphicsManager()->GetLastFPS();
    AFPSstream << TheWorld->GetGraphicsManager()->GetAverageFPS();
    String CFPS = CFPSstream.str();
    String AFPS = AFPSstream.str();
    CurFPS->SetText(CFPS);
    AvFPS->SetText(AFPS);

    // Turn on the Wireframe
    if (30000<gametime)
        { TheWorld->GetPhysicsManager()->SetDebugPhysicsRendering(1); }

    //IF the game has gone on for 150 or more seconds close it.
	if (150000<gametime || (TheWorld->GetEventManager()->GetNextQuitEvent()!=0) )
        { return false; }

    return true;
}

bool PrePhysics()
{
    TheWorld->Log("Object Locations");
    //Replace this with something that uses the actor container and logs the location of everything
    TheWorld->Log(TheWorld->GetActorManager()->FindActor("MetalSphere2")->GetLocation());
    return true;
}

bool PostPhysics()
{
    //// Updating functions to be used when a suitable mesh is found/created.
    //ActorSoft* ActS = static_cast< ActorSoft* > (TheWorld->Actors->FindActor("Column1"));
    //ActS->UpdateSoftBody();
    return true;
}

bool PreUI()
{
    return true;
}

bool PostUI()
{
    return true;
}

bool PreInput()
{
    return true;
}

bool PostInput()
{
    //User Input through a WorldQueryTool
    static WorldQueryTool Queryer;

    Queryer.GatherEvents();
    TheWorld->Log("Mouse location From WorldQueryTool X/Y");
    TheWorld->Log(Queryer.GetMouseX());
    TheWorld->Log(Queryer.GetMouseY());

//    if(320>Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
//        {TheWorld->Cameras->IncrementYOrbit(-0.01, TheWorld->Cameras->GetNodeAttachedToCamera() );}

//    if(320<Queryer.GetMouseX() && Queryer.IsMouseButtonPushed(3))
//        {TheWorld->Cameras->IncrementYOrbit(0.01, TheWorld->Cameras->GetNodeAttachedToCamera() );}

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_LEFT) )
        { TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(-0.01); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_RIGHT) )
        { TheWorld->GetSceneManager()->GetNode("Orbit1")->IncrementOrbit(0.01); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_UP) )
        { TheWorld->GetCameraManager()->GetDefaultCamera()->ZoomCamera( -12.0 ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_DOWN) )
        { TheWorld->GetCameraManager()->GetDefaultCamera()->ZoomCamera( 12.0 ); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_SPACE) )
        { TheWorld->GetCameraManager()->GetDefaultCamera()->ResetZoom(); }

    if( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_m) )
    {
        Sound* Theme = TheWorld->GetSoundManager()->GetSoundByName("Theme2");
        if(!Theme->IsPlaying())
        {
            Theme->Play2d(false);
        }
    }

    //Resize the window
    static bool videobuttonpushed = false;
    if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_f) && !videobuttonpushed )
    {
        videobuttonpushed = true;
        //TheWorld->GetGraphicsManager()->setRenderOptions(1366,768,true);
        //TheWorld->GetGraphicsManager()->setRenderOptions(1280,1024,true);
        TheWorld->GetGraphicsManager()->setFullscreen(true);
        //TheWorld->GetGraphicsManager()->setFullscreen(false);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_g)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setFullscreen(false);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_r)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setRenderResolution(1280,1024);
    }
    else if ( Queryer.IsKeyboardButtonPushed(MetaCode::KEY_t)  && !videobuttonpushed )
    {
        videobuttonpushed = true;
        TheWorld->GetGraphicsManager()->setRenderResolution(800,600);
    }
    else { videobuttonpushed = false; }

    // Make a declaration for a static constrain so it survives the function lifetime
    // static *constraint MouseDragger = 0;

    //static Generic6DofConstraint* Dragger=NULL;
    static Point2PointConstraint* Dragger=NULL;

    if( Queryer.IsMouseButtonPushed(1) )
    {
        /// @todo determine whether this next snippt should be a function on the UIScreen
        UI::Button* MouseButton = NULL;
        UIScreen* Screen = TheWorld->GetUIManager()->GetScreen("DefaultScreen");
        for(Whole x=0 ; x != Screen->GetNumLayers() ; x++ )
        {
            UILayer* Layer = Screen->GetLayer(x);
            MouseButton = Layer->CheckButtonMouseIsOver();
            if(MouseButton)
            { break; }
        }
        if(MouseButton)
        {
            if("Menu" == MouseButton->GetName())
            {
                UILayer* Layer = Screen->GetLayer("MenuLayer");
                Layer->Show();
            }
            if("Return" == MouseButton->GetName())
            {
                UILayer* Layer = Screen->GetLayer("MenuLayer");
                Layer->Hide();
            }
            if("Exit" == MouseButton->GetName())
            {
                return false;
            }
        }else{
            #ifdef PHYSDEBUG
            TheWorld->Log("Gamebase CLICK:");
            TheWorld->LogStream << "Camera Location: " << TheWorld->GetCameraManager()->GetDefaultCamera()->GetCameraGlobalLocation() << endl;
            #endif

            Ray *MouseRay = Queryer.GetMouseRay(5000);
            // *MouseRay *= 1000;
            //Ray *MouseRay = new Ray(Vector3(500.0, 0.0, 0.0),Vector3(-500.0, 0.0, 0.0));

            Vector3WActor *ClickOnActor = Queryer.GetFirstActorOnRayByPolygon( *MouseRay );
            #ifdef PHYSDEBUG
            TheWorld->LogStream << "MouseRay: " << *MouseRay << "| Length: " << MouseRay->Length() << endl;
            #endif

            //ActorBase *temp = ClickOnActor->Actor;

            bool firstframe=false;
            if (0 == ClickOnActor || 0 == ClickOnActor->Actor)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("No Actor Clicked on");
                #endif
            }else{
                #ifdef PHYSDEBUG
                TheWorld->Log("Actor Clicked on"); TheWorld->Log(*ClickOnActor);
                TheWorld->Log("MouseRay"); TheWorld->Log(*MouseRay);
                TheWorld->Log("PlaneOfPlay"); TheWorld->Log(PlaneOfPlay);
                TheWorld->Log("ClickOnActor"); TheWorld->Log(*ClickOnActor);
                #endif
                if(!(ClickOnActor->Actor->IsStaticOrKinematic()))
                {
                    if(!Dragger) //If we have a dragger, then this is dragging, not clicking
                    {
                        if(ClickOnActor->Actor->GetType()==ActorBase::Actorrigid) //This is Dragging let's do some checks for sanity
                        {
                            Vector3 LocalPivot = ClickOnActor->Vector;
                            ActorRigid* rigid = static_cast<ActorRigid*>(ClickOnActor->Actor);
                            rigid->DisableDeactivation();
                            //Dragger = new Generic6DofConstraint(rigid, LocalPivot, Quaternion(0,0,0,1), false);
                            Dragger = new Point2PointConstraint(rigid, LocalPivot);
                            Dragger->SetTAU(0.001);
                            //Dragger->SetLinearLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetLinearUpperLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularLowerLimit(Vector3(0.f,0.f,0.f));
                            //Dragger->SetAngularUpperLimit(Vector3(0.f,0.f,0.f));
                            TheWorld->GetPhysicsManager()->AddConstraint(Dragger);
                            Dragger->SetParam(4,0.8,0); Dragger->SetParam(4,0.8,1); Dragger->SetParam(4,0.8,2); //Dragger->SetParam(4,0.8,3); Dragger->SetParam(4,0.8,4); Dragger->SetParam(4,0.8,5);
                            Dragger->SetParam(2,0.1,0); Dragger->SetParam(2,0.1,1); Dragger->SetParam(2,0.1,2); //Dragger->SetParam(2,0.1,3); Dragger->SetParam(2,0.1,4); Dragger->SetParam(2,0.1,5);
                            firstframe=true;
                        }else{  // since we don't
                            #ifdef PHYSDEBUG
                            TheWorld->Log("Actor is not an ActorRigid.  Aborting.");
                            #endif
                        }
                    }
                }else{
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Actor is Static/Kinematic.  Aborting.");
                    #endif
                }
            }

            // This chunk of code calculates the 3d point that the actor needs to be dragged to
            Vector3 *DragTo = Queryer.RayPlaneIntersection(*MouseRay, PlaneOfPlay);
            if (0 == DragTo)
            {
                #ifdef PHYSDEBUG
                TheWorld->Log("PlaneOfPlay Not Clicked on");
                #endif
            }else{
                if(Dragger && !firstframe)
                {
                    #ifdef PHYSDEBUG
                    TheWorld->Log("Dragged To");
                    TheWorld->Log(*DragTo);
                    #endif
                    //Dragger->SetOffsetALocation(*DragTo);
                    Dragger->SetPivotB(*DragTo);
                }
            }

            // Here we cleanup everything we needed for the clicking/dragging
            delete DragTo;
            delete MouseRay;
        }

    }else{  //Since we are no longer clicking we need to setup for the next clicking
        if(Dragger)
        {
            ActorRigid* Act = Dragger->GetActorA();
            TheWorld->GetPhysicsManager()->RemoveConstraint(Dragger);
            delete Dragger;
            Dragger=NULL;
            Act->RestoreActivation();
        }
    }

    // using the Raw Event Manager, and deleting the events
    if( !CheckForEsc() )
        return false;
    return true;
}

///////////////////
//Non-Callbacks
bool CheckForEsc()
{
    //this will either set the pointer to 0 or return a valid pointer to work with.
    EventUserInput* OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();

    //We check each Event
    while(0 != OneInput)
    {
        #ifdef PHYSDEBUG
        TheWorld->Log("Input Events Processed");
        #endif

        //we check each MetaCode in each Event
        for (unsigned int c=0; c<OneInput->GetMetaCodeCount(); c++ )
        {
            #ifdef PHYSDEBUG
            TheWorld->LogStream << "Metacode (" << c << ")" << OneInput->GetMetaCode(c) << endl << "Escape is: " << MetaCode::KEY_ESCAPE << endl;

            #endif
            //Is the key we just pushed ESCAPE
            if(MetaCode::KEY_ESCAPE == OneInput->GetMetaCode(c).GetCode() && MetaCode::BUTTON_PRESSING == OneInput->GetMetaCode(c).GetMetaValue())
                { return false; }
        }

        delete OneInput;
        OneInput = TheWorld->GetEventManager()->PopNextUserInputEvent();
    }

    return true;
}

void LoadContent()
{
    TheWorld->GetSceneManager()->SetAmbientLight(1.0,1.0,1.0,1.0);
    ActorRigid *object1, *object2, *object3, *object4;
    ActorTerrain *object5, *object6;
    ActorRigid *object7;
    //Ogre Setup Code
    String groupname ("Group1");
    String filerobot ("robot.mesh");
    String robotprefix ("Robot");

    std::stringstream zipname;
    zipname << crossplatform::GetDataDirectory() << "test.zip";

    Real mass=5.0;
    TheWorld->GetResourceManager()->AddResourceLocation(crossplatform::GetDataDirectory(), "FileSystem", groupname, false);
    TheWorld->GetResourceManager()->AddResourceLocation(zipname.str(), "Zip", groupname, false);
    TheWorld->GetResourceManager()->AddResourceLocation("", "FileSystem", groupname, false);

    TheWorld->GetResourceManager()->DeclareResource(filerobot, "Mesh", groupname);
    TheWorld->GetResourceManager()->InitResourceGroup(groupname);

    //Test the Resource input stream here
    #ifdef PHYSDEBUG
    TheWorld->Log("Trying to open test.xml and test.txt");
    #endif
    {
        ResourceInputStream* XMLptr = TheWorld->GetResourceManager()->GetResourceStream("test.xml");
        ResourceInputStream* Zippedptr = TheWorld->GetResourceManager()->GetResourceStream("test.txt");

        // Test reading by character
        Whole Howmany=10000;
        char chararray[Howmany+1];
        for (Whole c=0; c<Howmany+1; c++)
            { chararray[c]='\0'; }
        XMLptr->read(chararray, Howmany);
        String ShouldHaveXML( chararray );

        for (Whole c=0; c<Howmany+1; c++)
            { chararray[c]='\0'; }
        Zippedptr->read(chararray, Howmany);
        String ZippedFileContents( chararray );

        char filearray[Howmany+1];
        for (Whole c=0; c<Howmany+1; c++)
            { filearray[c]='\0'; }

        ifstream TestFile("data/common/test.txt");
        TestFile.read(filearray, Howmany);
        String TestFileString(filearray);

        String StreamExtractionTest;
        TestFile >> StreamExtractionTest;

        #ifdef PHYSDEBUG
        TheWorld->LogStream << "ShouldHaveXML: " << endl << ShouldHaveXML << endl
                            << "ZippedFileContents: " << endl << ZippedFileContents << endl
                            << "File read from fstream: " << endl << TestFileString << endl ;
        #endif

        //if the above stuff was logged this should just work
        if( !Zippedptr->good() )
            { TheWorld->Log("Zippedptr corrupted/broken by read call"); }

        //this should just work
        Zippedptr->unget();
        if( !Zippedptr->good() )
            { TheWorld->Log("Zippedptr corrupted/broken by unget() call"); }
        else
        {
            Whole howfew = 500;
            char temp[howfew+1];
            for (Whole c=0; c<=howfew+1; c++)
                { temp[c]='\0'; }
            for (Whole c=0; c<howfew-1; c++)      //we already did one above -(1), we do not want to pad anything with a \0 (no +1), and we want exactly this many (< instead of <=)
                { Zippedptr->unget(); }
            Zippedptr->read(temp, howfew);
            TheWorld->LogStream << howfew << " ungets and " << howfew << " characters :" << temp;
        }

        TheWorld->LogStream << endl << "End first streaming test" << endl;

        try
            { XMLptr->putback('X'); }// as in :X
        catch (Exception E)
            { TheWorld->Log( E.what() ); }

        delete XMLptr;
        delete Zippedptr;
    }

    {
        #ifdef PHYSDEBUG
        TheWorld->Log("Testing streaming on test.xml and zipped test.txt, Making fresh set of stream pointers");
        #endif

        ResourceInputStream* XMLptr = TheWorld->GetResourceManager()->GetResourceStream("test.xml");
        ResourceInputStream* Zippedptr = TheWorld->GetResourceManager()->GetResourceStream("test.txt");

        String Content("");
        (*XMLptr) >> Content;
        TheWorld->LogStream << "Extracted: " << Content << endl;

        if( !XMLptr->good() )
            { TheWorld->Log("XMLptr corrupted/broken by >> call"); }
        else
        {
            TheWorld->Log("XMLptr fine after >> call");
            TheWorld->Log("Getting some words with >> from Zippedptr:");
            for (Whole c=0; c<=5; c++)
            {
                String ZippedContent;
                (*Zippedptr) >> ZippedContent;
                TheWorld->LogStream << "Word " << c << ": " << ZippedContent;
                TheWorld->Log();
            }
        }

        delete XMLptr;
        delete Zippedptr;

        #ifdef PHYSDEBUG
        TheWorld->Log("End streaming test2");
        #endif
    }

    {
        #ifdef PHYSDEBUG
        TheWorld->Log("Testing streaming on test.xml with phys::xml::Document");
        #endif

        ResourceInputStream* XMLptr = TheWorld->GetResourceManager()->GetResourceStream("test.xml");
        xml::Document TestDoc;
        xml::ParseResult ParsedXML = TestDoc.Load( *XMLptr );

        TheWorld->LogStream << "xml::StatusOk :" << xml::StatusOk << endl
                            << "xml::StatusFileNotFound :" << xml::StatusFileNotFound << endl
                            << "xml::StatusIOError :" << xml::StatusIOError << endl
                            << "xml::StatusOutOfMemory :" << xml::StatusOutOfMemory << endl
                            << "xml::StatusInternalError :" << xml::StatusInternalError << endl
                            << "xml::StatusUnrecognizedTag :" << xml::StatusUnrecognizedTag << endl
                            << "xml::StatusBadPi :" << xml::StatusBadPi << endl
                            << "xml::StatusBadComment :" << xml::StatusBadComment << endl
                            << "xml::StatusBadCdata :" << xml::StatusBadCdata << endl
                            << "xml::StatusBadDoctype :" << xml::StatusBadDocType << endl
                            << "xml::StatusBadPcdata :" << xml::StatusBadPcdata << endl
                            << "xml::StatusBadStartElement :" << xml::StatusBadStartElement << endl
                            << "xml::StatusBadAttribute :" << xml::StatusBadAttribute << endl
                            << "xml::StatusBadEndElement :" << xml::StatusBadEndElement << endl
                            << "xml::StatusEndElementMismatch :" << xml::StatusEndElementMismatch << endl
                            ;



        if(ParsedXML.Status)
        {
            TheWorld->Log("Successfully loaded and parsed test.xml");
        }else{
            TheWorld->LogStream << "Failed to load and parse test.xml. With Status :" << ParsedXML.Status;
        }

        TheWorld->Log("XML Streaming Test");

        Vector2 ASinglePoint(1,2);
        TheWorld->Log("ASinglePoint:");
        TheWorld->Log(ASinglePoint);

        TheWorld->Log("Streaming ASinglePoint to String Stream:");
        stringstream XMLStringStream;
        XMLStringStream << ASinglePoint;
        TheWorld->Log(XMLStringStream.str());

        Vector2 ASecondPoint(0,0);
        TheWorld->Log("ASecondPoint:");
        TheWorld->Log(ASecondPoint);

        TheWorld->Log("ReStreaming ASinglePoint from stringStream, to ASecondPoint:");
        XMLStringStream >> ASecondPoint;
        TheWorld->Log(ASecondPoint);


        Plane ASinglePlane(Vector3(1.34,23,1.004),4.5);
        TheWorld->Log("ASinglePlane:");
        TheWorld->Log(ASinglePlane);

        TheWorld->Log("Streaming ASinglePlane to String Stream:");
        stringstream XMLStringStream2;
        XMLStringStream2 << ASinglePlane;
        TheWorld->Log(XMLStringStream2.str());

        Plane ASecondPlane(Vector3(0,0,0),0);
        TheWorld->Log("ASecondPlane:");
        TheWorld->Log(ASecondPlane);

        TheWorld->Log("ReStreaming ASinglePlane from stringStream, to ASecondPlane:");
        XMLStringStream2 >> ASecondPlane;
        TheWorld->Log(ASecondPlane);


        MetaCode ASingleCode(MetaCode(1,MetaCode::MOUSEBUTTON));
        TheWorld->Log("ASingleCode:");
        TheWorld->Log(ASingleCode);

        TheWorld->Log("Streaming ASingleCode to String Stream:");
        stringstream XMLStringStream3;
        XMLStringStream3 << ASingleCode;
        TheWorld->Log(XMLStringStream3.str());

        MetaCode ASecondCode(MetaCode(0,MetaCode::KEY_FIRST));
        TheWorld->Log("ASecondCode:");
        TheWorld->Log(ASecondCode);

        TheWorld->Log("ReStreaming ASingleCode from stringStream, to ASingleCode:");
        XMLStringStream3 >> ASecondCode;
        TheWorld->Log(ASecondCode);


        #ifdef PHYSDEBUG
        TheWorld->Log("End of testing XML and Streaming test 3");
        #endif
    }

    // Now Lets make some bowling pins
    Real PinSpacing=75.0;           //This is how far apart we want the pins
    for(unsigned int c=0; c<4; c++)     //make the back row
    {
        std::stringstream namestream;
        namestream << robotprefix << c;
        TheWorld->GetActorManager()->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname) );
        TheWorld->GetActorManager()->LastActorAdded()->CreateShapeFromMeshDynamic(1);
        TheWorld->GetActorManager()->LastActorAdded()->SetInitLocation(Vector3( (-2.0*PinSpacing)+(c*PinSpacing), -90.0, 0));
    }

    for(unsigned int c=0; c<3; c++)     //the row with three pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+4);
        TheWorld->GetActorManager()->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname) );
        TheWorld->GetActorManager()->LastActorAdded()->CreateShapeFromMeshDynamic(1);
        //TheWorld->GetResourceManager()->ImportShapeData(TheWorld->GetActorManager()->LastActorAdded(), "data/common/RobotDecomp3.bullet");
        TheWorld->GetActorManager()->LastActorAdded()->SetInitLocation(Vector3( (-1.5*PinSpacing)+(c*PinSpacing), -66.0, -PinSpacing));
    }
    //TheWorld->Resources->ImportShapeData(TheWorld->GetActorManager()->LastActorAdded(), "RobotDecomp3.bullet");

    for(unsigned int c=0; c<2; c++)     //the row with 2 pins
    {
        std::stringstream namestream;
        namestream << robotprefix << (c+7);
        TheWorld->GetActorManager()->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname) );
        TheWorld->GetActorManager()->LastActorAdded()->CreateShapeFromMeshDynamic(1);
        TheWorld->GetActorManager()->LastActorAdded()->SetInitLocation(Vector3( (-PinSpacing)+(c*PinSpacing), -30.0, -PinSpacing*2));
    }

    std::stringstream namestream;           //make the front pin
    namestream << robotprefix << 9;
    TheWorld->GetActorManager()->AddActor( new ActorRigid (mass,namestream.str(),filerobot,groupname) );
    TheWorld->GetActorManager()->LastActorAdded()->CreateShapeFromMeshDynamic(1);
    TheWorld->GetActorManager()->LastActorAdded()->SetInitLocation(Vector3( (-0.5*PinSpacing), 0.0, -PinSpacing*3));

    GravityField* Reverse = new GravityField(String("UpField"), Vector3(0.0,-100.0,0.0));
    Reverse->CreateCylinderShape(Vector3(100.0,200.0,100));
    Reverse->SetLocation(Vector3(200,50,-5.0));
    TheWorld->GetPhysicsManager()->AddAreaEffect(Reverse); // Now that we have passed it, we can forget about it

    //// The simulations soft body, to be used once a suitable mesh is found/created.
    //TheWorld->Actors->AddActor( new ActorSoft (51,"Column1","column.mesh",groupname) );
    //ActorSoft* Act9 = static_cast < ActorSoft* > (TheWorld->Actors->LastActorAdded());
    //Act9->SetInitLocation(Vector3( (-0.5*PinSpacing), 100.0, -PinSpacing*4));

    object5 = new ActorTerrain (Vector3(0.0,-100,-300.0),"Plane","Plane.mesh",groupname);
    object5->CreateShapeFromMeshStatic();
    //object5->SetInitLocation(Vector3(0.0,-100,-300.0));

    object6 = new ActorTerrain (Vector3(00.0,300.0,-1100.0),"Ramp","Plane.mesh",groupname);
    object6->CreateShapeFromMeshStatic();
    //object6->SetInitLocation(Vector3(00.0,300.0,-1100.0));
    object6->SetInitOrientation(Quaternion(0.5, 0.0, 0.0, -0.25));

    object1 = new ActorRigid (mass,"RobotWayUpFrontRight",filerobot,groupname);
    object1->CreateShapeFromMeshDynamic(1);
    object1->SetInitLocation(Vector3(400,70,100));
    object1->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object1->SetAnimation("Idle", true);
    object1->EnableAnimation(true);

    object2 = new ActorRigid (150.0f,"WoodSphere","Sphere_Wood.mesh",groupname);
    object2->CreateSphereShapeFromMesh();
    object2->SetActorScaling(Vector3(0.5,0.5,0.5));
    object2->SetInitLocation(Vector3(-140.0,2800.0,-1150.0));

    object3 = new ActorRigid (200.0f,"MetalSphere","Sphere_Metal.mesh",groupname);
    object3->CreateSphereShapeFromMesh();
    object3->SetActorScaling(Vector3(0.7,0.7,0.7));
    object3->SetInitLocation(Vector3(150.0,1800.0,-1300.0));

    object4 = new ActorRigid (mass,"RobotWayUpFrontLeft",filerobot,groupname);
    object4->CreateShapeFromMeshDynamic(3);
    object4->SetInitLocation(Vector3(-400,10, 100));
    object4->SetInitOrientation(Quaternion(0.5, 0.5, 0.0, 0.9));
    object4->SetAnimation("Idle", true);
    object4->EnableAnimation(true);

    object7 = new ActorRigid (200.0f,"MetalSphere2","Sphere_Metal.mesh",groupname);
    object7->CreateSphereShapeFromMesh();
    object7->SetActorScaling(Vector3(0.3,0.3,0.3));
    object7->SetInitLocation(Vector3(10.0,25000.0,-1300.0));
    object7->SetDamping(0.3,0.0);

    //Final Steps
    Vector3 grav;
    grav.X=0.0;
    grav.Y=-10000.0;
    grav.Z=0.0;

    TheWorld->GetActorManager()->AddActor(object1);
    TheWorld->GetActorManager()->AddActor(object2);
    TheWorld->GetActorManager()->AddActor(object3);
    TheWorld->GetActorManager()->AddActor(object4);
    TheWorld->GetActorManager()->AddActor(object5);
    TheWorld->GetActorManager()->AddActor(object6);
    TheWorld->GetActorManager()->AddActor(object7);

    AreaEffect* TestField = new TestAE("Tester", Vector3(0,0,150));
    TestField->CreateBoxShape(Vector3(500,80,80));
    TheWorld->GetPhysicsManager()->AddAreaEffect(TestField);

    Sound *sound1, *music1, *music2;
    TheWorld->GetSoundManager()->CreateSoundSet("Announcer");
    sound1 = TheWorld->GetSoundManager()->CreateSound("Welcome", "data/common/sounds/welcomefun-1.ogg", true);
    TheWorld->GetSoundManager()->AddSoundToSoundSet("Announcer", sound1);

    TheWorld->GetSoundManager()->CreateSoundSet("SoundTrack");
    music1 = TheWorld->GetSoundManager()->CreateSound("Theme1", "data/common/music/cAudioTheme1.ogg", true);
    TheWorld->GetSoundManager()->AddSoundToSoundSet("SoundTrack", music1);
    music2 = TheWorld->GetSoundManager()->CreateSound("Theme2", "data/common/music/cAudioTheme2.ogg", true);
    TheWorld->GetSoundManager()->AddSoundToSoundSet("SoundTrack", music2);

    TheWorld->Log("Actor Count");
    TheWorld->Log( TheWorld->GetActorManager()->GetActorCount() );

    TheWorld->GetPhysicsManager()->SetGravity(grav);
    TheWorld->GetPhysicsManager()->SetSoftGravity(grav);
}

void MakeGUI()
{
    String DefaultScreen = "DefaultScreen";
    String MenuLayer = "MenuLayer";
    String HUDLayer = "HUDLayer";
    String ItemShopLayer = "ItemShopLayer";
    String StatsLayer = "StatsLayer";
    UIManager* GUI = TheWorld->GetUIManager();
    GUI->LoadGorilla("dejavu");

    UIScreen* Screen = GUI->CreateScreen(DefaultScreen, "dejavu");
    UILayer* Menu = Screen->CreateLayer(MenuLayer, 3);
    UILayer* ItemShop = Screen->CreateLayer(ItemShopLayer, 2);
    UILayer* Stats = Screen->CreateLayer(StatsLayer, 1);
    UILayer* HUD = Screen->CreateLayer(HUDLayer, 0);

    ColourValue Transparent(0.0,0.0,0.0,0.0);
    ColourValue Black(0.0,0.0,0.0,1.0);
    ColourValue TransBlack(0.0,0.0,0.0,0.3);

    //Build the HUD layer
    UI::TextButton* MenuButton = HUD->CreateTextButton( "Menu", Vector2(0.0, 0.92),
                                            Vector2(0.2, 0.08),
                                            24, "Menu");
    MenuButton->HorizontallyAlign(UI::Txt_Middle);
    MenuButton->VerticallyAlign(UI::Txt_Center);
    ColourValue MenuColour(0.1,0.3,0.8,1.0);
    MenuButton->SetBackgroundColour(MenuColour);

    //Build the ItemShop Layer
    ItemShop->Hide();

    //Build the Menu Layer
    UI::Rectangle* MenuBackground = Menu->CreateRectangle( Vector2(0.25, 0.15),
                                                         Vector2(0.5, 0.7));
    ColourValue Colours(0.4,0.8,0.3,1.0);
    MenuBackground->SetBackgroundColour(Colours);

    UI::TextButton* ReturnButton = Menu->CreateTextButton( "Return", Vector2(0.30, 0.61),
                                            Vector2(0.4, 0.08),
                                            24, "Return to Game");
    ReturnButton->HorizontallyAlign(UI::Txt_Middle);
    ReturnButton->VerticallyAlign(UI::Txt_Center);
    Colours = ColourValue(0.6,0.2,0.2,1.0);
    ReturnButton->SetBackgroundColour(Colours);

    UI::TextButton* ExitButton = Menu->CreateTextButton( "Exit", Vector2(0.30, 0.73),
                                            Vector2(0.4, 0.08),
                                            24, "Exit Game");
    ExitButton->HorizontallyAlign(UI::Txt_Middle);
    ExitButton->VerticallyAlign(UI::Txt_Center);
    Colours = ColourValue(0.6,0.2,0.2,1.0);
    ExitButton->SetBackgroundColour(Colours);
    Menu->Hide();

    //Build the Stats Layer
    UI::Caption* CurFPS = Stats->CreateCaption( "CurFPS", Vector2(0.16, 0.06), Vector2(0.06, 0.065), 14, "0.0");
    CurFPS->SetBackgroundColour(Transparent);
    CurFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* CurFPSText = Stats->CreateCaption( "CurFPSText", Vector2(0.008, 0.06), Vector2(0.15, 0.065), 14, "Current FPS: ");
    CurFPSText->SetBackgroundColour(Transparent);
    CurFPSText->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPS = Stats->CreateCaption( "AvFPS", Vector2(0.16, 0.105), Vector2(0.06, 0.065), 14, "0.0");
    AvFPS->SetBackgroundColour(Transparent);
    AvFPS->HorizontallyAlign(UI::Txt_Left);

    UI::Caption* AvFPSText = Stats->CreateCaption( "AvFPSText", Vector2(0.008, 0.105), Vector2(0.15, 0.065), 14, "Average FPS: ");
    AvFPSText->SetBackgroundColour(Transparent);
    AvFPSText->HorizontallyAlign(UI::Txt_Left);
    //End of Stats Layer
}
//*/
#endif
