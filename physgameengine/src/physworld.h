#ifndef _physworld_h
#define _physworld_h


/// @Todo Make main page in documentation with wiki link and lisitng of required steps (build target and OS define), and dependencies required to build this.


//Includes and Forward Declarations
#include "physcrossplatform.h"
#include "physworldcallbackmanager.h"
#include "physevent.h"
#include "physeventmanager.h"
#include "physdatatypes.h"

#include <string>
#include <queue>



using namespace std;

//forward Declarations so that we do not need #include "physvector.h"
//forward Declarations so that we do not need #include "gamesettings.h"
//Forward declarations to prevent infite loop of includes
class Settings;
class PhysVector3;

//Other forward declarations
//forward Declarations so that we do not need #include "SDL.h"
class SDL_Surface;

//forward Declarations so that we do not need #include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btSoftRigidDynamicsWorld;


//forward Declarations so that we do not need #include <Ogre.h>
namespace Ogre
{
	class Root;
	class RenderSystem;
	class RenderWindow;
    class ResourceGroupManager;
	class SceneManager;
	class Camera;
	class Viewport;
}


///////////////////////////////////////////////////////////////////////////////
/// @class PhysWorld
/// @headerfile physworld.h
/// @brief This is the main entry point for the entire library.
/// The physworld coordinates and integrates all the underlying subsystems,
/// Currently Ogre3d is used for 3d Graphics, Bullet is used for physics, and
/// SDL is used for user input and window management. Games will need a
/// container for all the playing pieces. It makes sense to tie all of this
/// functionality into one world object.
///////////////////////////////////////
class PhysWorld
{
	private:
		//Physics Items
		PhysVector3* GeographyLowerBounds;
		PhysVector3* GeographyUpperbounds;
		unsigned short int  MaxPhysicsProxies;

		btAxisSweep3* BulletBroadphase;
		btDefaultCollisionConfiguration* BulletCollisionConfiguration;
		btCollisionDispatcher* BulletDispatcher;
		btSequentialImpulseConstraintSolver* BulletSolver;
		btSoftRigidDynamicsWorld* BulletDynamicsWorld;

		//SDL Objects
		SDL_Surface *SDLscreen;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* OgreGameWindow;
        Ogre::ResourceGroupManager* OgreResource;
		Ogre::SceneManager* OgreSceneManager;
		Ogre::Camera* OgreCamera;
		Ogre::Viewport* OgreViewport;

		//Players settings
		Settings* PlayerSettings;

		//Try to retrieve ogre settinss
		void LoadOgreSettings();

		//Manages the windows all the graphics will be drawn to
		void CreateRenderWindow();
		void DestroyRenderWindow();

		//Used by the constructors
		void Construct(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies);
        void TestLogger();
        template <class T> void OneLogTest(T Data, string DataType,string Message1 = "Logging and Throwing a ", string Message2 = "Logging a ");

		//SDL specific Items
		//This function will get all the events from SDL and Sort them into one of two Queues
		void PreProcessSDLEvents();
        queue<RawEvent*> SDL_WmEvents;
        queue<RawEvent*> SDL_UserInputEvents;

        /// Do Not Use this, This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        /// @warning This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        friend void RenderPhysWorld(PhysWorld *TheWorld);

	public:
        /// @brief Descriptive constructor
        /// This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
        /// @param GeographyLowerBounds The lower limits for the size of the physics simulation
        /// @param GeographyUpperbounds The Upper limits for the size of the physics simulation
        /// @param MaxPhysicsProxies This is the amount of Adows (Also called Actors or Proxies) allowed in a physics simulation.
		PhysWorld(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies=1024);

        /// @brief Default constructor
        /// This simply performs the same work as the descriptive constructor with some sane, but small, limits. It will give you a world which expands for 100 units from the Origin, and only allows 10 Adows.
		PhysWorld();

        /// @brief
        /// This Tears
		~PhysWorld();

		//I am just extending what ogre provides for a logging system
		template <class T> void Log(T Message);
        template <class T> void LogAndThrow(T Message);

        //I plan on deprecating this thing soon and building our own settings system
        bool ShowSystemSettingDialog();

		//Change the camera angle;
		void MoveCamera(PhysVector3 Position, PhysVector3 LookAt);

        //Starts the Game
        void GameInit();

        //Functions to run during the game loop.
        void DoMainLoopAllItems();
        void DoMainLoopPhysics();
		void DoMainLoopInputBuffering();
		void DoMainLoopWindowManagerBuffering();
		void DoMainLoopRender();

		//used to set callback functions to be run in the main loop
		PhysWorldCallBackManager* CallBacks;

		//This will be used to communicate with underlying sybsystems in a clean way
		PhysEventManager*  Events;
};
#endif
