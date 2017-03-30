# JFramework
JFramework is a 100% free, 100% component based framework that currently operates on Windows, MacOSX (10 and above, whenever that becomes a thing.), and Linux (Debian and Redhat). This README is intended to help you get started with the basics of operating with the framework.

- Architecture overview
  - It all starts at the GameApp class. Whenever you make a new game, be sure to derive from this class. Initializing this class creates all of the managers needed by JFramework to function. In your own app constructor, feel free to create new managers or register new classes in LUA.
  - Every frame, JFramework iterates over its managers and calls Update(), updating the current level, objects within that level, etc.
  - Your custom manager update loops will NOT be called unless you call AddManager() on your GameApp.
  - Calling Start from your app will enable the engine to run, once everything has been initialized.

- Managers
  - ObjectManager
  - LevelManager
  - ChemistryManager
  - PhysicsWorld
  - DebugManager
  - GraphicsManager
  - ControllerManager
  - NetworkManager
  - SoundManager
  
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
  
- Assets
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
