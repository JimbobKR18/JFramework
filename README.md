# JFramework
JFramework is a 100% free, 100% component based framework that currently operates on Windows, MacOSX (10 and above, whenever that becomes a thing.), and Linux (Debian and Redhat). This README is intended to help you get started with the basics of operating with the framework.

- Required Libraries
  - SDL2 (https://www.libsdl.org/download-2.0.php)
  - SDL2_mixer (https://www.libsdl.org/projects/SDL_mixer)
  - SDL2_ttf (https://www.libsdl.org/projects/SDL_ttf)
  - SDL2_net (https://www.libsdl.org/projects/SDL_net)
  - SDL2_image (https://www.libsdl.org/projects/SDL_image)
  - GLEW (http://glew.sourceforge.net/index.html)

- Architecture Overview
  - It all starts at the GameApp class. Whenever you make a new game, be sure to derive from this class. Initializing this class creates all of the managers needed by JFramework to function. In your own app constructor, feel free to create new managers or register new classes in LUA.
  - Every frame, JFramework iterates over its managers and calls Update(), updating the current level, objects within that level, etc.
  - Your custom manager update loops will NOT be called unless you call AddManager() on your GameApp.
  - Calling Start from your app will enable the engine to run, once everything has been initialized.

- Managers
  - ObjectManager
    - Iterates over GameObjects in the current active level, updates their components.
  - LevelManager
    - Change levels, update level, etc.
  - ChemistryManager
    - Manage chemical materials and elements. Has a factory to create custom elements and materials. Updates chemicals and adjusts temperature.
  - PhysicsWorld
    - Manage physics objects, collisions, calls scripts for collision, and passes messages around.
  - DebugManager
    - Mainly unused, but can be used to track objects created and deleted and various other debug functionality. I would like to add AntTweakBar or something here at some point.
  - GraphicsManager
    - Manage surfaces and screens to render GameObjects on screen. Also has functionality to convert from world space to screen space and vice versa.
  - ControllerManager
    - Create and delete controllers.
  - NetworkManager
    - Networking functionality, untested.
  - SoundManager
    - Create, play, and delete sounds.
  
- Components
  - ChemistryElement
    - Emits a temperature or wattage (or both!) to hand off to ChemistryMaterials, does not change properties over time.
  - ChemistryMaterial
    - Stores and emits temperature and wattage (or both!), temperature falls off or increases over time, based on environment factors.
  - Controller
    - Define behaviors for objects, meant to be abstracted from, due to the variety of bahaviors an object can exhibit.
  - PhysicsObject
    - Details how an object should interact with the environment, define shapes, mass, restitution, and various other factors to dictate how the object should move.
  - StateObject
    - Represents a state machine.
  - Surface
    - Dictates how an object should be drawn, including coordinate system, color, texture, shader, etc.
  - Transform
    - Details the objects orientation in the game, can be in view or world space (dictated by surface view mode.)
  
- JFramework File Format
  - Parser
    - All files that are read in using JFramework's formatting should be loaded in via the Parser class. Once loaded in, you can access elements in a file by calling the Find() method.
    - Calling Find() will return a Root object, which is a node that contains name to value pairs, as well as children (also stored as Root) belonging to it.
    - To get a value from a Root, simply call GetValue(), which is the value as a HashString. HashString includes helpful methods to convert itself into an int, float, std::string, char const*, arrays and etc.
  - Formatting (Insert newlines)
    - <code>Start
{
	Name
	{
		Value = Oliver
	}
	PhysicsObject
	{
		Mass = 50
		Static = false
		Gravity = false
		Passable = false
		Damping = 0.00000000000000001
		Restitution = 0
		Shape_0
		{
			Type = SPHERE
			Radius = 12
			PositionX = 0
			PositionY = 20
			PositionZ = 0
		}
	}
	Surface
	{
		TextureName = nesswalk.png
		AnimationCount = 24
		AnimationSpeeds
		{
			Animation_0 = 0.15,0.15
			Animation_1 = 0.15,0.15
			Animation_10 = 0.15,0.15
			Animation_11 = 0.15,0.15
			Animation_12 = 0.15,0.15
			Animation_13 = 0.15,0.15
			Animation_14 = 0.15,0.15
			Animation_15 = 0.15,0.15
			Animation_16 = 0.1,0.1
			Animation_17 = 0.1,0.1
			Animation_18 = 0.1,0.1
			Animation_19 = 0.1,0.1
			Animation_2 = 0.15,0.15
			Animation_20 = 0.1,0.1
			Animation_21 = 0.1,0.1
			Animation_22 = 0.1,0.1
			Animation_23 = 0.1,0.1
			Animation_3 = 0.15,0.15
			Animation_4 = 0.15,0.15
			Animation_5 = 0.15,0.15
			Animation_6 = 0.15,0.15
			Animation_7 = 0.15,0.15
			Animation_8 = 0.15,0.15
			Animation_9 = 0.15,0.15
		}
		FrameNumbers = 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
		Animated = true
		VertexShader = Basic.vs
		FragmentShader = Basic.fs
	}
	Transform
	{
		PositionX = 0
		PositionY = 0
		PositionZ = 0
		ScaleX = 1
		ScaleY = 1
		ScaleZ = 1
		SizeX = 26
		SizeY = 36
		SizeZ = 32
		RotationX = 0
    RotationY = 0
    RotationZ = 0
		AlignX = CENTER
		AlignY = CENTER
		AlignZ = CENTER
	}
	Focus
	{
		IsFocus = true
	}
}</code>
  
- Assets
  - The assets folder can be configured in a file called SystemProperties.ini with a setting as such:
    - <code>AssetsDirectory = /home/jimmy/Documents/ZombieStealth/Assets/</code>
  - The SystemProperties.ini file must be in the same directory as your game executable.
  
- Levels (cpp stuff)

- Input

- Menus

- Helpful Classes
  - Common
    - Helper functions that are used throughout the engine.
  - Interpolators
    - Helper class where you dictate an object must equal a value within a time limit, this class will handle the rest, so long as you call Update().
  - HashString
    - Handy class that stores strings and can has faster string comparison. Class knows how to convert itself to various objects (int, float, arrays). Use this whenever possible.
  - LUA
    - Various LUA functionality stated in this folder, check out LUATypes.h for full details.
  - MathExt
    - Contains various math helper classes. (Vector, Matrix, etc.)
  - Constants
    - Define values in a Constants.txt file in Assets/Game and JFramework will allow you to get these values (as float, int, string, array, etc.) for use in your game. Makes magic numbers less of a thing, and data drives the engine.
  - SystemProperties
    - For now, define where the Assets folder for you game is.
  - StateMachine
    - Dictate how an object should go between states.
    
- Common Issues
  - "SDL_main not defined" <-- Don't include any other files named "main", don't have any functions named "main".
  - "Cannot start program (0xc000022) <-- Verify file permissions on ALL folders and files.
