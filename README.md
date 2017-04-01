# JFramework
JFramework is a 100% free, 100% component based framework that currently operates on Windows, MacOSX (10 and above, whenever that becomes a thing.), and Linux (Debian and Redhat). This README is intended to help you get started with the basics of operating with the framework.

- Architecture overview
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
  - ChemistryMaterial
  - Controller
  - PhysicsObject
  - StateObject
  - Surface
  - Transform
  
- JFramework File Format
  - Parser
    - All files that are read in using JFramework's formatting should be loaded in via the Parser class. Once loaded in, you can access elements in a file by calling the Find() method.
    - Calling Find() will return a Root object, which is a node that contains name to value pairs, as well as children (also stored as Root) belonging to it.
    - To get a value from a Root, simply call GetValue(), which is the value as a HashString. HashString includes helpful methods to convert itself into an int, float, std::string, char const*, arrays and etc.
  - Formatting
    - TODO
  
- Assets
  - The assets folder can be configured in a file called SystemProperties.ini with a setting as such:
    - AssetsDirectory = /home/jimmy/Documents/ZombieStealth/Assets/
  - The SystemProperties.ini file must be in the same directory as your game executable.
  
- Levels (cpp stuff)

- Input

- Menus

- Helpful classes
  - Common
  - Interpolators
  - HashString
  - LUA
  - MathExt
  - Constants
  - SystemProperties
  - StateMachine
