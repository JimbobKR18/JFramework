##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=JFramework
ConfigurationName      :=Debug
WorkspacePath          := "/home/jimmy/Documents/project-chang-e/Linux/Chang-e"
ProjectPath            := "/home/jimmy/Documents/JFramework/Linux/JFramework"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Jimmy Spencer
Date                   :=05/03/16
CodeLitePath           :="/home/jimmy/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=$(PreprocessorSwitch)ASSET_DIRECTORY=/home/jimmy/Documents/project-chang-e/Assets/ $(PreprocessorSwitch)_DEBUG $(PreprocessorSwitch)_DEBUG_DRAW $(PreprocessorSwitch)SLB_USE_EXCEPTIONS 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="JFramework.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../Source $(IncludeSwitch)../../Source/Core $(IncludeSwitch)../../Source/Logic $(IncludeSwitch)../../Source/Logic/StateMachine $(IncludeSwitch)../../Source/Components $(IncludeSwitch)../../Source/Physics $(IncludeSwitch)../../Source/Sound $(IncludeSwitch)../../Source/Managers $(IncludeSwitch)../../Source/Graphics $(IncludeSwitch)../../Source/LUA $(IncludeSwitch)../../Source/Parsers $(IncludeSwitch)../../Source/TinyXML $(IncludeSwitch)../../Source/Core/Messages 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/lib $(LibraryPathSwitch)/usr/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -std=c++11 -Wall -fno-inline -fno-eliminate-unused-debug-types $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -fno-inline -fno-eliminate-unused-debug-types $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/LUA_lua.c$(ObjectSuffix) $(IntermediateDirectory)/LUA_SLB.cpp$(ObjectSuffix) $(IntermediateDirectory)/LUA_LUATypes.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_MathExt.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_AutoCrit.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_Common.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_GameObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_HashString.cpp$(ObjectSuffix) $(IntermediateDirectory)/Core_Shape.cpp$(ObjectSuffix) $(IntermediateDirectory)/Source_main.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Source_GameApp.cpp$(ObjectSuffix) $(IntermediateDirectory)/Components_Surface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Components_Controller.cpp$(ObjectSuffix) $(IntermediateDirectory)/Components_Transform.cpp$(ObjectSuffix) $(IntermediateDirectory)/Components_PhysicsObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_View.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_Constants.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Logic_Level.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_MenuElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_MenuImage.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_Menu.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_MenuText.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/Logic_MenuButton.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_ObjectManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_ControllerManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_LevelManager.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Managers_SoundManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_DebugManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_GraphicsManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Managers_InputManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/Parsers_Parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/Parsers_TextParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/Parsers_Root.cpp$(ObjectSuffix) $(IntermediateDirectory)/Parsers_XMLParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_GravityGenerator.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Physics_Resolver.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_CollisionPair.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_ForceGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_ForceRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_CollisionChecker.cpp$(ObjectSuffix) $(IntermediateDirectory)/Physics_PotentialPair.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/Sound_Sound.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sound_PCSound.cpp$(ObjectSuffix) $(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_PCSurface.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_PCScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/Graphics_Screen.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine_StateMachine.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine_State.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine_StateLink.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/jimmy/Documents/project-chang-e/Linux/Chang-e/.build-debug"
	@echo rebuilt > "/home/jimmy/Documents/project-chang-e/Linux/Chang-e/.build-debug/JFramework"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/LUA_lua.c$(ObjectSuffix): ../../Source/LUA/lua.c $(IntermediateDirectory)/LUA_lua.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/lua.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LUA_lua.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LUA_lua.c$(DependSuffix): ../../Source/LUA/lua.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LUA_lua.c$(ObjectSuffix) -MF$(IntermediateDirectory)/LUA_lua.c$(DependSuffix) -MM "../../Source/LUA/lua.c"

$(IntermediateDirectory)/LUA_lua.c$(PreprocessSuffix): ../../Source/LUA/lua.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LUA_lua.c$(PreprocessSuffix) "../../Source/LUA/lua.c"

$(IntermediateDirectory)/LUA_SLB.cpp$(ObjectSuffix): ../../Source/LUA/SLB.cpp $(IntermediateDirectory)/LUA_SLB.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/SLB.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LUA_SLB.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LUA_SLB.cpp$(DependSuffix): ../../Source/LUA/SLB.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LUA_SLB.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LUA_SLB.cpp$(DependSuffix) -MM "../../Source/LUA/SLB.cpp"

$(IntermediateDirectory)/LUA_SLB.cpp$(PreprocessSuffix): ../../Source/LUA/SLB.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LUA_SLB.cpp$(PreprocessSuffix) "../../Source/LUA/SLB.cpp"

$(IntermediateDirectory)/LUA_LUATypes.cpp$(ObjectSuffix): ../../Source/LUA/LUATypes.cpp $(IntermediateDirectory)/LUA_LUATypes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/LUATypes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LUA_LUATypes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LUA_LUATypes.cpp$(DependSuffix): ../../Source/LUA/LUATypes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LUA_LUATypes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LUA_LUATypes.cpp$(DependSuffix) -MM "../../Source/LUA/LUATypes.cpp"

$(IntermediateDirectory)/LUA_LUATypes.cpp$(PreprocessSuffix): ../../Source/LUA/LUATypes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LUA_LUATypes.cpp$(PreprocessSuffix) "../../Source/LUA/LUATypes.cpp"

$(IntermediateDirectory)/Core_MathExt.cpp$(ObjectSuffix): ../../Source/Core/MathExt.cpp $(IntermediateDirectory)/Core_MathExt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/MathExt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_MathExt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_MathExt.cpp$(DependSuffix): ../../Source/Core/MathExt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_MathExt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_MathExt.cpp$(DependSuffix) -MM "../../Source/Core/MathExt.cpp"

$(IntermediateDirectory)/Core_MathExt.cpp$(PreprocessSuffix): ../../Source/Core/MathExt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_MathExt.cpp$(PreprocessSuffix) "../../Source/Core/MathExt.cpp"

$(IntermediateDirectory)/Core_AutoCrit.cpp$(ObjectSuffix): ../../Source/Core/AutoCrit.cpp $(IntermediateDirectory)/Core_AutoCrit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/AutoCrit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_AutoCrit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_AutoCrit.cpp$(DependSuffix): ../../Source/Core/AutoCrit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_AutoCrit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_AutoCrit.cpp$(DependSuffix) -MM "../../Source/Core/AutoCrit.cpp"

$(IntermediateDirectory)/Core_AutoCrit.cpp$(PreprocessSuffix): ../../Source/Core/AutoCrit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_AutoCrit.cpp$(PreprocessSuffix) "../../Source/Core/AutoCrit.cpp"

$(IntermediateDirectory)/Core_Common.cpp$(ObjectSuffix): ../../Source/Core/Common.cpp $(IntermediateDirectory)/Core_Common.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Common.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_Common.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_Common.cpp$(DependSuffix): ../../Source/Core/Common.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_Common.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_Common.cpp$(DependSuffix) -MM "../../Source/Core/Common.cpp"

$(IntermediateDirectory)/Core_Common.cpp$(PreprocessSuffix): ../../Source/Core/Common.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_Common.cpp$(PreprocessSuffix) "../../Source/Core/Common.cpp"

$(IntermediateDirectory)/Core_GameObject.cpp$(ObjectSuffix): ../../Source/Core/GameObject.cpp $(IntermediateDirectory)/Core_GameObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/GameObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_GameObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_GameObject.cpp$(DependSuffix): ../../Source/Core/GameObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_GameObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_GameObject.cpp$(DependSuffix) -MM "../../Source/Core/GameObject.cpp"

$(IntermediateDirectory)/Core_GameObject.cpp$(PreprocessSuffix): ../../Source/Core/GameObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_GameObject.cpp$(PreprocessSuffix) "../../Source/Core/GameObject.cpp"

$(IntermediateDirectory)/Core_HashString.cpp$(ObjectSuffix): ../../Source/Core/HashString.cpp $(IntermediateDirectory)/Core_HashString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/HashString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_HashString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_HashString.cpp$(DependSuffix): ../../Source/Core/HashString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_HashString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_HashString.cpp$(DependSuffix) -MM "../../Source/Core/HashString.cpp"

$(IntermediateDirectory)/Core_HashString.cpp$(PreprocessSuffix): ../../Source/Core/HashString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_HashString.cpp$(PreprocessSuffix) "../../Source/Core/HashString.cpp"

$(IntermediateDirectory)/Core_Shape.cpp$(ObjectSuffix): ../../Source/Core/Shape.cpp $(IntermediateDirectory)/Core_Shape.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Shape.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Core_Shape.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Core_Shape.cpp$(DependSuffix): ../../Source/Core/Shape.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Core_Shape.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Core_Shape.cpp$(DependSuffix) -MM "../../Source/Core/Shape.cpp"

$(IntermediateDirectory)/Core_Shape.cpp$(PreprocessSuffix): ../../Source/Core/Shape.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Core_Shape.cpp$(PreprocessSuffix) "../../Source/Core/Shape.cpp"

$(IntermediateDirectory)/Source_main.cpp$(ObjectSuffix): ../../Source/main.cpp $(IntermediateDirectory)/Source_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_main.cpp$(DependSuffix): ../../Source/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_main.cpp$(DependSuffix) -MM "../../Source/main.cpp"

$(IntermediateDirectory)/Source_main.cpp$(PreprocessSuffix): ../../Source/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_main.cpp$(PreprocessSuffix) "../../Source/main.cpp"

$(IntermediateDirectory)/Source_GameApp.cpp$(ObjectSuffix): ../../Source/GameApp.cpp $(IntermediateDirectory)/Source_GameApp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/GameApp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Source_GameApp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Source_GameApp.cpp$(DependSuffix): ../../Source/GameApp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Source_GameApp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_GameApp.cpp$(DependSuffix) -MM "../../Source/GameApp.cpp"

$(IntermediateDirectory)/Source_GameApp.cpp$(PreprocessSuffix): ../../Source/GameApp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Source_GameApp.cpp$(PreprocessSuffix) "../../Source/GameApp.cpp"

$(IntermediateDirectory)/Components_Surface.cpp$(ObjectSuffix): ../../Source/Components/Surface.cpp $(IntermediateDirectory)/Components_Surface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Surface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Components_Surface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Components_Surface.cpp$(DependSuffix): ../../Source/Components/Surface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Components_Surface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Components_Surface.cpp$(DependSuffix) -MM "../../Source/Components/Surface.cpp"

$(IntermediateDirectory)/Components_Surface.cpp$(PreprocessSuffix): ../../Source/Components/Surface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Components_Surface.cpp$(PreprocessSuffix) "../../Source/Components/Surface.cpp"

$(IntermediateDirectory)/Components_Controller.cpp$(ObjectSuffix): ../../Source/Components/Controller.cpp $(IntermediateDirectory)/Components_Controller.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Components_Controller.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Components_Controller.cpp$(DependSuffix): ../../Source/Components/Controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Components_Controller.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Components_Controller.cpp$(DependSuffix) -MM "../../Source/Components/Controller.cpp"

$(IntermediateDirectory)/Components_Controller.cpp$(PreprocessSuffix): ../../Source/Components/Controller.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Components_Controller.cpp$(PreprocessSuffix) "../../Source/Components/Controller.cpp"

$(IntermediateDirectory)/Components_Transform.cpp$(ObjectSuffix): ../../Source/Components/Transform.cpp $(IntermediateDirectory)/Components_Transform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Transform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Components_Transform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Components_Transform.cpp$(DependSuffix): ../../Source/Components/Transform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Components_Transform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Components_Transform.cpp$(DependSuffix) -MM "../../Source/Components/Transform.cpp"

$(IntermediateDirectory)/Components_Transform.cpp$(PreprocessSuffix): ../../Source/Components/Transform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Components_Transform.cpp$(PreprocessSuffix) "../../Source/Components/Transform.cpp"

$(IntermediateDirectory)/Components_PhysicsObject.cpp$(ObjectSuffix): ../../Source/Components/PhysicsObject.cpp $(IntermediateDirectory)/Components_PhysicsObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/PhysicsObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Components_PhysicsObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Components_PhysicsObject.cpp$(DependSuffix): ../../Source/Components/PhysicsObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Components_PhysicsObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Components_PhysicsObject.cpp$(DependSuffix) -MM "../../Source/Components/PhysicsObject.cpp"

$(IntermediateDirectory)/Components_PhysicsObject.cpp$(PreprocessSuffix): ../../Source/Components/PhysicsObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Components_PhysicsObject.cpp$(PreprocessSuffix) "../../Source/Components/PhysicsObject.cpp"

$(IntermediateDirectory)/Graphics_View.cpp$(ObjectSuffix): ../../Source/Graphics/View.cpp $(IntermediateDirectory)/Graphics_View.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/View.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_View.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_View.cpp$(DependSuffix): ../../Source/Graphics/View.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_View.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_View.cpp$(DependSuffix) -MM "../../Source/Graphics/View.cpp"

$(IntermediateDirectory)/Graphics_View.cpp$(PreprocessSuffix): ../../Source/Graphics/View.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_View.cpp$(PreprocessSuffix) "../../Source/Graphics/View.cpp"

$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(ObjectSuffix): ../../Source/Graphics/TextureCoordinates.cpp $(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/TextureCoordinates.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(DependSuffix): ../../Source/Graphics/TextureCoordinates.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(DependSuffix) -MM "../../Source/Graphics/TextureCoordinates.cpp"

$(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(PreprocessSuffix): ../../Source/Graphics/TextureCoordinates.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_TextureCoordinates.cpp$(PreprocessSuffix) "../../Source/Graphics/TextureCoordinates.cpp"

$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(ObjectSuffix): ../../Source/Graphics/ZRenderSorter.cpp $(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/ZRenderSorter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(DependSuffix): ../../Source/Graphics/ZRenderSorter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(DependSuffix) -MM "../../Source/Graphics/ZRenderSorter.cpp"

$(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(PreprocessSuffix): ../../Source/Graphics/ZRenderSorter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_ZRenderSorter.cpp$(PreprocessSuffix) "../../Source/Graphics/ZRenderSorter.cpp"

$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(ObjectSuffix): ../../Source/Graphics/BatchRenderSorter.cpp $(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/BatchRenderSorter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(DependSuffix): ../../Source/Graphics/BatchRenderSorter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(DependSuffix) -MM "../../Source/Graphics/BatchRenderSorter.cpp"

$(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(PreprocessSuffix): ../../Source/Graphics/BatchRenderSorter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_BatchRenderSorter.cpp$(PreprocessSuffix) "../../Source/Graphics/BatchRenderSorter.cpp"

$(IntermediateDirectory)/Logic_Constants.cpp$(ObjectSuffix): ../../Source/Logic/Constants.cpp $(IntermediateDirectory)/Logic_Constants.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Constants.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_Constants.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_Constants.cpp$(DependSuffix): ../../Source/Logic/Constants.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_Constants.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_Constants.cpp$(DependSuffix) -MM "../../Source/Logic/Constants.cpp"

$(IntermediateDirectory)/Logic_Constants.cpp$(PreprocessSuffix): ../../Source/Logic/Constants.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_Constants.cpp$(PreprocessSuffix) "../../Source/Logic/Constants.cpp"

$(IntermediateDirectory)/Logic_Level.cpp$(ObjectSuffix): ../../Source/Logic/Level.cpp $(IntermediateDirectory)/Logic_Level.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Level.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_Level.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_Level.cpp$(DependSuffix): ../../Source/Logic/Level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_Level.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_Level.cpp$(DependSuffix) -MM "../../Source/Logic/Level.cpp"

$(IntermediateDirectory)/Logic_Level.cpp$(PreprocessSuffix): ../../Source/Logic/Level.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_Level.cpp$(PreprocessSuffix) "../../Source/Logic/Level.cpp"

$(IntermediateDirectory)/Logic_MenuElement.cpp$(ObjectSuffix): ../../Source/Logic/MenuElement.cpp $(IntermediateDirectory)/Logic_MenuElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/MenuElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_MenuElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_MenuElement.cpp$(DependSuffix): ../../Source/Logic/MenuElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_MenuElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_MenuElement.cpp$(DependSuffix) -MM "../../Source/Logic/MenuElement.cpp"

$(IntermediateDirectory)/Logic_MenuElement.cpp$(PreprocessSuffix): ../../Source/Logic/MenuElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_MenuElement.cpp$(PreprocessSuffix) "../../Source/Logic/MenuElement.cpp"

$(IntermediateDirectory)/Logic_MenuImage.cpp$(ObjectSuffix): ../../Source/Logic/MenuImage.cpp $(IntermediateDirectory)/Logic_MenuImage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/MenuImage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_MenuImage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_MenuImage.cpp$(DependSuffix): ../../Source/Logic/MenuImage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_MenuImage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_MenuImage.cpp$(DependSuffix) -MM "../../Source/Logic/MenuImage.cpp"

$(IntermediateDirectory)/Logic_MenuImage.cpp$(PreprocessSuffix): ../../Source/Logic/MenuImage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_MenuImage.cpp$(PreprocessSuffix) "../../Source/Logic/MenuImage.cpp"

$(IntermediateDirectory)/Logic_Menu.cpp$(ObjectSuffix): ../../Source/Logic/Menu.cpp $(IntermediateDirectory)/Logic_Menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_Menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_Menu.cpp$(DependSuffix): ../../Source/Logic/Menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_Menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_Menu.cpp$(DependSuffix) -MM "../../Source/Logic/Menu.cpp"

$(IntermediateDirectory)/Logic_Menu.cpp$(PreprocessSuffix): ../../Source/Logic/Menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_Menu.cpp$(PreprocessSuffix) "../../Source/Logic/Menu.cpp"

$(IntermediateDirectory)/Logic_MenuText.cpp$(ObjectSuffix): ../../Source/Logic/MenuText.cpp $(IntermediateDirectory)/Logic_MenuText.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/MenuText.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_MenuText.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_MenuText.cpp$(DependSuffix): ../../Source/Logic/MenuText.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_MenuText.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_MenuText.cpp$(DependSuffix) -MM "../../Source/Logic/MenuText.cpp"

$(IntermediateDirectory)/Logic_MenuText.cpp$(PreprocessSuffix): ../../Source/Logic/MenuText.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_MenuText.cpp$(PreprocessSuffix) "../../Source/Logic/MenuText.cpp"

$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(ObjectSuffix): ../../Source/Logic/TileMapGenerator.cpp $(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/TileMapGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(DependSuffix): ../../Source/Logic/TileMapGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(DependSuffix) -MM "../../Source/Logic/TileMapGenerator.cpp"

$(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(PreprocessSuffix): ../../Source/Logic/TileMapGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_TileMapGenerator.cpp$(PreprocessSuffix) "../../Source/Logic/TileMapGenerator.cpp"

$(IntermediateDirectory)/Logic_MenuButton.cpp$(ObjectSuffix): ../../Source/Logic/MenuButton.cpp $(IntermediateDirectory)/Logic_MenuButton.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/MenuButton.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Logic_MenuButton.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Logic_MenuButton.cpp$(DependSuffix): ../../Source/Logic/MenuButton.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Logic_MenuButton.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Logic_MenuButton.cpp$(DependSuffix) -MM "../../Source/Logic/MenuButton.cpp"

$(IntermediateDirectory)/Logic_MenuButton.cpp$(PreprocessSuffix): ../../Source/Logic/MenuButton.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Logic_MenuButton.cpp$(PreprocessSuffix) "../../Source/Logic/MenuButton.cpp"

$(IntermediateDirectory)/Managers_ObjectManager.cpp$(ObjectSuffix): ../../Source/Managers/ObjectManager.cpp $(IntermediateDirectory)/Managers_ObjectManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/ObjectManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_ObjectManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_ObjectManager.cpp$(DependSuffix): ../../Source/Managers/ObjectManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_ObjectManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_ObjectManager.cpp$(DependSuffix) -MM "../../Source/Managers/ObjectManager.cpp"

$(IntermediateDirectory)/Managers_ObjectManager.cpp$(PreprocessSuffix): ../../Source/Managers/ObjectManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_ObjectManager.cpp$(PreprocessSuffix) "../../Source/Managers/ObjectManager.cpp"

$(IntermediateDirectory)/Managers_ControllerManager.cpp$(ObjectSuffix): ../../Source/Managers/ControllerManager.cpp $(IntermediateDirectory)/Managers_ControllerManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/ControllerManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_ControllerManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_ControllerManager.cpp$(DependSuffix): ../../Source/Managers/ControllerManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_ControllerManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_ControllerManager.cpp$(DependSuffix) -MM "../../Source/Managers/ControllerManager.cpp"

$(IntermediateDirectory)/Managers_ControllerManager.cpp$(PreprocessSuffix): ../../Source/Managers/ControllerManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_ControllerManager.cpp$(PreprocessSuffix) "../../Source/Managers/ControllerManager.cpp"

$(IntermediateDirectory)/Managers_LevelManager.cpp$(ObjectSuffix): ../../Source/Managers/LevelManager.cpp $(IntermediateDirectory)/Managers_LevelManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/LevelManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_LevelManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_LevelManager.cpp$(DependSuffix): ../../Source/Managers/LevelManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_LevelManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_LevelManager.cpp$(DependSuffix) -MM "../../Source/Managers/LevelManager.cpp"

$(IntermediateDirectory)/Managers_LevelManager.cpp$(PreprocessSuffix): ../../Source/Managers/LevelManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_LevelManager.cpp$(PreprocessSuffix) "../../Source/Managers/LevelManager.cpp"

$(IntermediateDirectory)/Managers_SoundManager.cpp$(ObjectSuffix): ../../Source/Managers/SoundManager.cpp $(IntermediateDirectory)/Managers_SoundManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/SoundManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_SoundManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_SoundManager.cpp$(DependSuffix): ../../Source/Managers/SoundManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_SoundManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_SoundManager.cpp$(DependSuffix) -MM "../../Source/Managers/SoundManager.cpp"

$(IntermediateDirectory)/Managers_SoundManager.cpp$(PreprocessSuffix): ../../Source/Managers/SoundManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_SoundManager.cpp$(PreprocessSuffix) "../../Source/Managers/SoundManager.cpp"

$(IntermediateDirectory)/Managers_DebugManager.cpp$(ObjectSuffix): ../../Source/Managers/DebugManager.cpp $(IntermediateDirectory)/Managers_DebugManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/DebugManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_DebugManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_DebugManager.cpp$(DependSuffix): ../../Source/Managers/DebugManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_DebugManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_DebugManager.cpp$(DependSuffix) -MM "../../Source/Managers/DebugManager.cpp"

$(IntermediateDirectory)/Managers_DebugManager.cpp$(PreprocessSuffix): ../../Source/Managers/DebugManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_DebugManager.cpp$(PreprocessSuffix) "../../Source/Managers/DebugManager.cpp"

$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(ObjectSuffix): ../../Source/Managers/PhysicsWorld.cpp $(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/PhysicsWorld.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(DependSuffix): ../../Source/Managers/PhysicsWorld.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(DependSuffix) -MM "../../Source/Managers/PhysicsWorld.cpp"

$(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(PreprocessSuffix): ../../Source/Managers/PhysicsWorld.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_PhysicsWorld.cpp$(PreprocessSuffix) "../../Source/Managers/PhysicsWorld.cpp"

$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(ObjectSuffix): ../../Source/Managers/GraphicsManager.cpp $(IntermediateDirectory)/Managers_GraphicsManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/GraphicsManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(DependSuffix): ../../Source/Managers/GraphicsManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(DependSuffix) -MM "../../Source/Managers/GraphicsManager.cpp"

$(IntermediateDirectory)/Managers_GraphicsManager.cpp$(PreprocessSuffix): ../../Source/Managers/GraphicsManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_GraphicsManager.cpp$(PreprocessSuffix) "../../Source/Managers/GraphicsManager.cpp"

$(IntermediateDirectory)/Managers_InputManager.cpp$(ObjectSuffix): ../../Source/Managers/InputManager.cpp $(IntermediateDirectory)/Managers_InputManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/InputManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Managers_InputManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Managers_InputManager.cpp$(DependSuffix): ../../Source/Managers/InputManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Managers_InputManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Managers_InputManager.cpp$(DependSuffix) -MM "../../Source/Managers/InputManager.cpp"

$(IntermediateDirectory)/Managers_InputManager.cpp$(PreprocessSuffix): ../../Source/Managers/InputManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Managers_InputManager.cpp$(PreprocessSuffix) "../../Source/Managers/InputManager.cpp"

$(IntermediateDirectory)/Parsers_Parser.cpp$(ObjectSuffix): ../../Source/Parsers/Parser.cpp $(IntermediateDirectory)/Parsers_Parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/Parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Parsers_Parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Parsers_Parser.cpp$(DependSuffix): ../../Source/Parsers/Parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Parsers_Parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Parsers_Parser.cpp$(DependSuffix) -MM "../../Source/Parsers/Parser.cpp"

$(IntermediateDirectory)/Parsers_Parser.cpp$(PreprocessSuffix): ../../Source/Parsers/Parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Parsers_Parser.cpp$(PreprocessSuffix) "../../Source/Parsers/Parser.cpp"

$(IntermediateDirectory)/Parsers_TextParser.cpp$(ObjectSuffix): ../../Source/Parsers/TextParser.cpp $(IntermediateDirectory)/Parsers_TextParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/TextParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Parsers_TextParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Parsers_TextParser.cpp$(DependSuffix): ../../Source/Parsers/TextParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Parsers_TextParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Parsers_TextParser.cpp$(DependSuffix) -MM "../../Source/Parsers/TextParser.cpp"

$(IntermediateDirectory)/Parsers_TextParser.cpp$(PreprocessSuffix): ../../Source/Parsers/TextParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Parsers_TextParser.cpp$(PreprocessSuffix) "../../Source/Parsers/TextParser.cpp"

$(IntermediateDirectory)/Parsers_Root.cpp$(ObjectSuffix): ../../Source/Parsers/Root.cpp $(IntermediateDirectory)/Parsers_Root.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/Root.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Parsers_Root.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Parsers_Root.cpp$(DependSuffix): ../../Source/Parsers/Root.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Parsers_Root.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Parsers_Root.cpp$(DependSuffix) -MM "../../Source/Parsers/Root.cpp"

$(IntermediateDirectory)/Parsers_Root.cpp$(PreprocessSuffix): ../../Source/Parsers/Root.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Parsers_Root.cpp$(PreprocessSuffix) "../../Source/Parsers/Root.cpp"

$(IntermediateDirectory)/Parsers_XMLParser.cpp$(ObjectSuffix): ../../Source/Parsers/XMLParser.cpp $(IntermediateDirectory)/Parsers_XMLParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/XMLParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Parsers_XMLParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Parsers_XMLParser.cpp$(DependSuffix): ../../Source/Parsers/XMLParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Parsers_XMLParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Parsers_XMLParser.cpp$(DependSuffix) -MM "../../Source/Parsers/XMLParser.cpp"

$(IntermediateDirectory)/Parsers_XMLParser.cpp$(PreprocessSuffix): ../../Source/Parsers/XMLParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Parsers_XMLParser.cpp$(PreprocessSuffix) "../../Source/Parsers/XMLParser.cpp"

$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(ObjectSuffix): ../../Source/Physics/GravityGenerator.cpp $(IntermediateDirectory)/Physics_GravityGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/GravityGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(DependSuffix): ../../Source/Physics/GravityGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(DependSuffix) -MM "../../Source/Physics/GravityGenerator.cpp"

$(IntermediateDirectory)/Physics_GravityGenerator.cpp$(PreprocessSuffix): ../../Source/Physics/GravityGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_GravityGenerator.cpp$(PreprocessSuffix) "../../Source/Physics/GravityGenerator.cpp"

$(IntermediateDirectory)/Physics_Resolver.cpp$(ObjectSuffix): ../../Source/Physics/Resolver.cpp $(IntermediateDirectory)/Physics_Resolver.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/Resolver.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_Resolver.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_Resolver.cpp$(DependSuffix): ../../Source/Physics/Resolver.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_Resolver.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_Resolver.cpp$(DependSuffix) -MM "../../Source/Physics/Resolver.cpp"

$(IntermediateDirectory)/Physics_Resolver.cpp$(PreprocessSuffix): ../../Source/Physics/Resolver.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_Resolver.cpp$(PreprocessSuffix) "../../Source/Physics/Resolver.cpp"

$(IntermediateDirectory)/Physics_CollisionPair.cpp$(ObjectSuffix): ../../Source/Physics/CollisionPair.cpp $(IntermediateDirectory)/Physics_CollisionPair.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/CollisionPair.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_CollisionPair.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_CollisionPair.cpp$(DependSuffix): ../../Source/Physics/CollisionPair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_CollisionPair.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_CollisionPair.cpp$(DependSuffix) -MM "../../Source/Physics/CollisionPair.cpp"

$(IntermediateDirectory)/Physics_CollisionPair.cpp$(PreprocessSuffix): ../../Source/Physics/CollisionPair.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_CollisionPair.cpp$(PreprocessSuffix) "../../Source/Physics/CollisionPair.cpp"

$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(ObjectSuffix): ../../Source/Physics/ForceGenerator.cpp $(IntermediateDirectory)/Physics_ForceGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/ForceGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(DependSuffix): ../../Source/Physics/ForceGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(DependSuffix) -MM "../../Source/Physics/ForceGenerator.cpp"

$(IntermediateDirectory)/Physics_ForceGenerator.cpp$(PreprocessSuffix): ../../Source/Physics/ForceGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_ForceGenerator.cpp$(PreprocessSuffix) "../../Source/Physics/ForceGenerator.cpp"

$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(ObjectSuffix): ../../Source/Physics/ForceRegistry.cpp $(IntermediateDirectory)/Physics_ForceRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/ForceRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(DependSuffix): ../../Source/Physics/ForceRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(DependSuffix) -MM "../../Source/Physics/ForceRegistry.cpp"

$(IntermediateDirectory)/Physics_ForceRegistry.cpp$(PreprocessSuffix): ../../Source/Physics/ForceRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_ForceRegistry.cpp$(PreprocessSuffix) "../../Source/Physics/ForceRegistry.cpp"

$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(ObjectSuffix): ../../Source/Physics/CollisionChecker.cpp $(IntermediateDirectory)/Physics_CollisionChecker.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/CollisionChecker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(DependSuffix): ../../Source/Physics/CollisionChecker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(DependSuffix) -MM "../../Source/Physics/CollisionChecker.cpp"

$(IntermediateDirectory)/Physics_CollisionChecker.cpp$(PreprocessSuffix): ../../Source/Physics/CollisionChecker.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_CollisionChecker.cpp$(PreprocessSuffix) "../../Source/Physics/CollisionChecker.cpp"

$(IntermediateDirectory)/Physics_PotentialPair.cpp$(ObjectSuffix): ../../Source/Physics/PotentialPair.cpp $(IntermediateDirectory)/Physics_PotentialPair.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/PotentialPair.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Physics_PotentialPair.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Physics_PotentialPair.cpp$(DependSuffix): ../../Source/Physics/PotentialPair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Physics_PotentialPair.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Physics_PotentialPair.cpp$(DependSuffix) -MM "../../Source/Physics/PotentialPair.cpp"

$(IntermediateDirectory)/Physics_PotentialPair.cpp$(PreprocessSuffix): ../../Source/Physics/PotentialPair.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Physics_PotentialPair.cpp$(PreprocessSuffix) "../../Source/Physics/PotentialPair.cpp"

$(IntermediateDirectory)/Sound_Sound.cpp$(ObjectSuffix): ../../Source/Sound/Sound.cpp $(IntermediateDirectory)/Sound_Sound.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/Sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sound_Sound.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sound_Sound.cpp$(DependSuffix): ../../Source/Sound/Sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sound_Sound.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sound_Sound.cpp$(DependSuffix) -MM "../../Source/Sound/Sound.cpp"

$(IntermediateDirectory)/Sound_Sound.cpp$(PreprocessSuffix): ../../Source/Sound/Sound.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sound_Sound.cpp$(PreprocessSuffix) "../../Source/Sound/Sound.cpp"

$(IntermediateDirectory)/Sound_PCSound.cpp$(ObjectSuffix): ../../Source/Sound/PCSound.cpp $(IntermediateDirectory)/Sound_PCSound.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/PCSound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sound_PCSound.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sound_PCSound.cpp$(DependSuffix): ../../Source/Sound/PCSound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sound_PCSound.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sound_PCSound.cpp$(DependSuffix) -MM "../../Source/Sound/PCSound.cpp"

$(IntermediateDirectory)/Sound_PCSound.cpp$(PreprocessSuffix): ../../Source/Sound/PCSound.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sound_PCSound.cpp$(PreprocessSuffix) "../../Source/Sound/PCSound.cpp"

$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(ObjectSuffix): ../../Source/TinyXML/tinyxml2.cpp $(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/TinyXML/tinyxml2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(DependSuffix): ../../Source/TinyXML/tinyxml2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(DependSuffix) -MM "../../Source/TinyXML/tinyxml2.cpp"

$(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(PreprocessSuffix): ../../Source/TinyXML/tinyxml2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TinyXML_tinyxml2.cpp$(PreprocessSuffix) "../../Source/TinyXML/tinyxml2.cpp"

$(IntermediateDirectory)/Graphics_PCSurface.cpp$(ObjectSuffix): ../../Source/Graphics/PCSurface.cpp $(IntermediateDirectory)/Graphics_PCSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/PCSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_PCSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_PCSurface.cpp$(DependSuffix): ../../Source/Graphics/PCSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_PCSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_PCSurface.cpp$(DependSuffix) -MM "../../Source/Graphics/PCSurface.cpp"

$(IntermediateDirectory)/Graphics_PCSurface.cpp$(PreprocessSuffix): ../../Source/Graphics/PCSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_PCSurface.cpp$(PreprocessSuffix) "../../Source/Graphics/PCSurface.cpp"

$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(ObjectSuffix): ../../Source/Graphics/PCShaderSurface.cpp $(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/PCShaderSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(DependSuffix): ../../Source/Graphics/PCShaderSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(DependSuffix) -MM "../../Source/Graphics/PCShaderSurface.cpp"

$(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(PreprocessSuffix): ../../Source/Graphics/PCShaderSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_PCShaderSurface.cpp$(PreprocessSuffix) "../../Source/Graphics/PCShaderSurface.cpp"

$(IntermediateDirectory)/Graphics_PCScreen.cpp$(ObjectSuffix): ../../Source/Graphics/PCScreen.cpp $(IntermediateDirectory)/Graphics_PCScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/PCScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_PCScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_PCScreen.cpp$(DependSuffix): ../../Source/Graphics/PCScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_PCScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_PCScreen.cpp$(DependSuffix) -MM "../../Source/Graphics/PCScreen.cpp"

$(IntermediateDirectory)/Graphics_PCScreen.cpp$(PreprocessSuffix): ../../Source/Graphics/PCScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_PCScreen.cpp$(PreprocessSuffix) "../../Source/Graphics/PCScreen.cpp"

$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(ObjectSuffix): ../../Source/Graphics/PCShaderScreen.cpp $(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/PCShaderScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(DependSuffix): ../../Source/Graphics/PCShaderScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(DependSuffix) -MM "../../Source/Graphics/PCShaderScreen.cpp"

$(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(PreprocessSuffix): ../../Source/Graphics/PCShaderScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_PCShaderScreen.cpp$(PreprocessSuffix) "../../Source/Graphics/PCShaderScreen.cpp"

$(IntermediateDirectory)/Graphics_Screen.cpp$(ObjectSuffix): ../../Source/Graphics/Screen.cpp $(IntermediateDirectory)/Graphics_Screen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Screen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Graphics_Screen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Graphics_Screen.cpp$(DependSuffix): ../../Source/Graphics/Screen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Graphics_Screen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Graphics_Screen.cpp$(DependSuffix) -MM "../../Source/Graphics/Screen.cpp"

$(IntermediateDirectory)/Graphics_Screen.cpp$(PreprocessSuffix): ../../Source/Graphics/Screen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Graphics_Screen.cpp$(PreprocessSuffix) "../../Source/Graphics/Screen.cpp"

$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp $(IntermediateDirectory)/StateMachine_StateMachine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/StateMachine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(DependSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(DependSuffix) -MM "../../Source/Logic/StateMachine/StateMachine.cpp"

$(IntermediateDirectory)/StateMachine_StateMachine.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StateMachine_StateMachine.cpp$(PreprocessSuffix) "../../Source/Logic/StateMachine/StateMachine.cpp"

$(IntermediateDirectory)/StateMachine_State.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/State.cpp $(IntermediateDirectory)/StateMachine_State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StateMachine_State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StateMachine_State.cpp$(DependSuffix): ../../Source/Logic/StateMachine/State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StateMachine_State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StateMachine_State.cpp$(DependSuffix) -MM "../../Source/Logic/StateMachine/State.cpp"

$(IntermediateDirectory)/StateMachine_State.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StateMachine_State.cpp$(PreprocessSuffix) "../../Source/Logic/StateMachine/State.cpp"

$(IntermediateDirectory)/StateMachine_StateLink.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/StateLink.cpp $(IntermediateDirectory)/StateMachine_StateLink.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/StateLink.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StateMachine_StateLink.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StateMachine_StateLink.cpp$(DependSuffix): ../../Source/Logic/StateMachine/StateLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StateMachine_StateLink.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StateMachine_StateLink.cpp$(DependSuffix) -MM "../../Source/Logic/StateMachine/StateLink.cpp"

$(IntermediateDirectory)/StateMachine_StateLink.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/StateLink.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StateMachine_StateLink.cpp$(PreprocessSuffix) "../../Source/Logic/StateMachine/StateLink.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


