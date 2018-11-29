##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=JFramework
ConfigurationName      :=Debug
WorkspacePath          :=/home/jimmy/Documents/ZombieStealth/Linux/ZombieStealth
ProjectPath            :=/home/jimmy/Documents/JFramework/Linux/JFramework
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Jimmy Spencer
Date                   :=28/11/18
CodeLitePath           :=/home/jimmy/.codelite
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
Preprocessors          :=$(PreprocessorSwitch)_DEBUG $(PreprocessorSwitch)_DEBUG_DRAW $(PreprocessorSwitch)SLB_USE_EXCEPTIONS 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="JFramework.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../Source $(IncludeSwitch)../../Source/Core $(IncludeSwitch)../../Source/Core/Interpolations $(IncludeSwitch)../../Source/Logic $(IncludeSwitch)../../Source/Logic/StateMachine $(IncludeSwitch)../../Source/Logic/Effects $(IncludeSwitch)../../Source/Chemistry $(IncludeSwitch)../../Source/Components $(IncludeSwitch)../../Source/Physics $(IncludeSwitch)../../Source/Sound $(IncludeSwitch)../../Source/Sound/SoundSystems $(IncludeSwitch)../../Source/Sound/DSPs $(IncludeSwitch)../../Source/Sound/DSPs/FMOD $(IncludeSwitch)../../Source/Managers $(IncludeSwitch)../../Source/Graphics $(IncludeSwitch)../../Source/Graphics/Screens $(IncludeSwitch)../../Source/Graphics/Surfaces $(IncludeSwitch)../../Source/Graphics/Framebuffers $(IncludeSwitch)../../Source/LUA $(IncludeSwitch)../../Source/Parsers $(IncludeSwitch)../../Source/Network $(IncludeSwitch)../../Source/TinyXML $(IncludeSwitch)../../Source/Input $(IncludeSwitch)../../Source/Core/Messages $(IncludeSwitch)../../Linux/fmod/api/studio/inc $(IncludeSwitch)../../Linux/fmod/api/lowlevel/inc 
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
Objects0=$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_GameApp.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(ObjectSuffix) \
	

Objects1=$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_LUA_lua.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(ObjectSuffix) 



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
	@$(MakeDirCommand) "/home/jimmy/Documents/ZombieStealth/Linux/ZombieStealth/.build-debug"
	@echo rebuilt > "/home/jimmy/Documents/ZombieStealth/Linux/ZombieStealth/.build-debug/JFramework"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(ObjectSuffix): ../../Source/Sound/SoundSystems/FMODSoundSystem.cpp $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/SoundSystems/FMODSoundSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(DependSuffix): ../../Source/Sound/SoundSystems/FMODSoundSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(DependSuffix) -MM ../../Source/Sound/SoundSystems/FMODSoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(PreprocessSuffix): ../../Source/Sound/SoundSystems/FMODSoundSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODSoundSystem.cpp$(PreprocessSuffix) ../../Source/Sound/SoundSystems/FMODSoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(ObjectSuffix): ../../Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(DependSuffix): ../../Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(DependSuffix) -MM ../../Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(PreprocessSuffix): ../../Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_FMODStudioSoundSystem.cpp$(PreprocessSuffix) ../../Source/Sound/SoundSystems/FMODStudioSoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(ObjectSuffix): ../../Source/Logic/Effects/DefaultEffectsFactory.cpp $(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Effects/DefaultEffectsFactory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(DependSuffix): ../../Source/Logic/Effects/DefaultEffectsFactory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(DependSuffix) -MM ../../Source/Logic/Effects/DefaultEffectsFactory.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(PreprocessSuffix): ../../Source/Logic/Effects/DefaultEffectsFactory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_Effects_DefaultEffectsFactory.cpp$(PreprocessSuffix) ../../Source/Logic/Effects/DefaultEffectsFactory.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(ObjectSuffix): ../../Source/Logic/Effects/Effect.cpp $(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Effects/Effect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(DependSuffix): ../../Source/Logic/Effects/Effect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(DependSuffix) -MM ../../Source/Logic/Effects/Effect.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(PreprocessSuffix): ../../Source/Logic/Effects/Effect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_Effects_Effect.cpp$(PreprocessSuffix) ../../Source/Logic/Effects/Effect.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/State.cpp $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(DependSuffix): ../../Source/Logic/StateMachine/State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(DependSuffix) -MM ../../Source/Logic/StateMachine/State.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_State.cpp$(PreprocessSuffix) ../../Source/Logic/StateMachine/State.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/StateMachine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(DependSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(DependSuffix) -MM ../../Source/Logic/StateMachine/StateMachine.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/StateMachine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateMachine.cpp$(PreprocessSuffix) ../../Source/Logic/StateMachine/StateMachine.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(ObjectSuffix): ../../Source/Graphics/Framebuffers/GLFramebuffer.cpp $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Framebuffers/GLFramebuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(DependSuffix): ../../Source/Graphics/Framebuffers/GLFramebuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(DependSuffix) -MM ../../Source/Graphics/Framebuffers/GLFramebuffer.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(PreprocessSuffix): ../../Source/Graphics/Framebuffers/GLFramebuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffers_GLFramebuffer.cpp$(PreprocessSuffix) ../../Source/Graphics/Framebuffers/GLFramebuffer.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(ObjectSuffix): ../../Source/Logic/StateMachine/StateLink.cpp $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/StateMachine/StateLink.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(DependSuffix): ../../Source/Logic/StateMachine/StateLink.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(DependSuffix) -MM ../../Source/Logic/StateMachine/StateLink.cpp

$(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(PreprocessSuffix): ../../Source/Logic/StateMachine/StateLink.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_StateMachine_StateLink.cpp$(PreprocessSuffix) ../../Source/Logic/StateMachine/StateLink.cpp

$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(ObjectSuffix): ../../Source/Sound/SoundSystems/SoundSystem.cpp $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/SoundSystems/SoundSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(DependSuffix): ../../Source/Sound/SoundSystems/SoundSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(DependSuffix) -MM ../../Source/Sound/SoundSystems/SoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(PreprocessSuffix): ../../Source/Sound/SoundSystems/SoundSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Sound_SoundSystems_SoundSystem.cpp$(PreprocessSuffix) ../../Source/Sound/SoundSystems/SoundSystem.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(ObjectSuffix): ../../Source/Graphics/Surfaces/PCShaderSurface.cpp $(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Surfaces/PCShaderSurface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(DependSuffix): ../../Source/Graphics/Surfaces/PCShaderSurface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(DependSuffix) -MM ../../Source/Graphics/Surfaces/PCShaderSurface.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(PreprocessSuffix): ../../Source/Graphics/Surfaces/PCShaderSurface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_Surfaces_PCShaderSurface.cpp$(PreprocessSuffix) ../../Source/Graphics/Surfaces/PCShaderSurface.cpp

$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(ObjectSuffix): ../../Source/Chemistry/DefaultChemicalFactory.cpp $(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Chemistry/DefaultChemicalFactory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(DependSuffix): ../../Source/Chemistry/DefaultChemicalFactory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(DependSuffix) -MM ../../Source/Chemistry/DefaultChemicalFactory.cpp

$(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(PreprocessSuffix): ../../Source/Chemistry/DefaultChemicalFactory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Chemistry_DefaultChemicalFactory.cpp$(PreprocessSuffix) ../../Source/Chemistry/DefaultChemicalFactory.cpp

$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(ObjectSuffix): ../../Source/Input/InputHandler.cpp $(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Input/InputHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(DependSuffix): ../../Source/Input/InputHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(DependSuffix) -MM ../../Source/Input/InputHandler.cpp

$(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(PreprocessSuffix): ../../Source/Input/InputHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Input_InputHandler.cpp$(PreprocessSuffix) ../../Source/Input/InputHandler.cpp

$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(ObjectSuffix): ../../Source/Physics/Joint.cpp $(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/Joint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(DependSuffix): ../../Source/Physics/Joint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(DependSuffix) -MM ../../Source/Physics/Joint.cpp

$(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(PreprocessSuffix): ../../Source/Physics/Joint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_Joint.cpp$(PreprocessSuffix) ../../Source/Physics/Joint.cpp

$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(ObjectSuffix): ../../Source/Physics/PotentialPair.cpp $(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/PotentialPair.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(DependSuffix): ../../Source/Physics/PotentialPair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(DependSuffix) -MM ../../Source/Physics/PotentialPair.cpp

$(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(PreprocessSuffix): ../../Source/Physics/PotentialPair.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_PotentialPair.cpp$(PreprocessSuffix) ../../Source/Physics/PotentialPair.cpp

$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(ObjectSuffix): ../../Source/Physics/CollisionChecker.cpp $(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/CollisionChecker.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(DependSuffix): ../../Source/Physics/CollisionChecker.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(DependSuffix) -MM ../../Source/Physics/CollisionChecker.cpp

$(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(PreprocessSuffix): ../../Source/Physics/CollisionChecker.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_CollisionChecker.cpp$(PreprocessSuffix) ../../Source/Physics/CollisionChecker.cpp

$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(ObjectSuffix): ../../Source/Network/PCNetworkPort.cpp $(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Network/PCNetworkPort.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(DependSuffix): ../../Source/Network/PCNetworkPort.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(DependSuffix) -MM ../../Source/Network/PCNetworkPort.cpp

$(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(PreprocessSuffix): ../../Source/Network/PCNetworkPort.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Network_PCNetworkPort.cpp$(PreprocessSuffix) ../../Source/Network/PCNetworkPort.cpp

$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(ObjectSuffix): ../../Source/Sound/DSP.cpp $(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/DSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(DependSuffix): ../../Source/Sound/DSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(DependSuffix) -MM ../../Source/Sound/DSP.cpp

$(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(PreprocessSuffix): ../../Source/Sound/DSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Sound_DSP.cpp$(PreprocessSuffix) ../../Source/Sound/DSP.cpp

$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(ObjectSuffix): ../../Source/Physics/ForceRegistry.cpp $(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/ForceRegistry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(DependSuffix): ../../Source/Physics/ForceRegistry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(DependSuffix) -MM ../../Source/Physics/ForceRegistry.cpp

$(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(PreprocessSuffix): ../../Source/Physics/ForceRegistry.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_ForceRegistry.cpp$(PreprocessSuffix) ../../Source/Physics/ForceRegistry.cpp

$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(ObjectSuffix): ../../Source/Physics/ForceGenerator.cpp $(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/ForceGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(DependSuffix): ../../Source/Physics/ForceGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(DependSuffix) -MM ../../Source/Physics/ForceGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(PreprocessSuffix): ../../Source/Physics/ForceGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_ForceGenerator.cpp$(PreprocessSuffix) ../../Source/Physics/ForceGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(ObjectSuffix): ../../Source/Physics/GravityGenerator.cpp $(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/GravityGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(DependSuffix): ../../Source/Physics/GravityGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(DependSuffix) -MM ../../Source/Physics/GravityGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(PreprocessSuffix): ../../Source/Physics/GravityGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_GravityGenerator.cpp$(PreprocessSuffix) ../../Source/Physics/GravityGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(ObjectSuffix): ../../Source/Parsers/ParserFactory.cpp $(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/ParserFactory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(DependSuffix): ../../Source/Parsers/ParserFactory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(DependSuffix) -MM ../../Source/Parsers/ParserFactory.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(PreprocessSuffix): ../../Source/Parsers/ParserFactory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Parsers_ParserFactory.cpp$(PreprocessSuffix) ../../Source/Parsers/ParserFactory.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(ObjectSuffix): ../../Source/Parsers/ParserNode.cpp $(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/ParserNode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(DependSuffix): ../../Source/Parsers/ParserNode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(DependSuffix) -MM ../../Source/Parsers/ParserNode.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(PreprocessSuffix): ../../Source/Parsers/ParserNode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Parsers_ParserNode.cpp$(PreprocessSuffix) ../../Source/Parsers/ParserNode.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(ObjectSuffix): ../../Source/Parsers/TextParser.cpp $(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/TextParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(DependSuffix): ../../Source/Parsers/TextParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(DependSuffix) -MM ../../Source/Parsers/TextParser.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(PreprocessSuffix): ../../Source/Parsers/TextParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Parsers_TextParser.cpp$(PreprocessSuffix) ../../Source/Parsers/TextParser.cpp

$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(ObjectSuffix): ../../Source/Network/NetworkPort.cpp $(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Network/NetworkPort.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(DependSuffix): ../../Source/Network/NetworkPort.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(DependSuffix) -MM ../../Source/Network/NetworkPort.cpp

$(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(PreprocessSuffix): ../../Source/Network/NetworkPort.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Network_NetworkPort.cpp$(PreprocessSuffix) ../../Source/Network/NetworkPort.cpp

$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(ObjectSuffix): ../../Source/Managers/InputManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/InputManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(DependSuffix): ../../Source/Managers/InputManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(DependSuffix) -MM ../../Source/Managers/InputManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(PreprocessSuffix): ../../Source/Managers/InputManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_InputManager.cpp$(PreprocessSuffix) ../../Source/Managers/InputManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(ObjectSuffix): ../../Source/Managers/PhysicsWorld.cpp $(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/PhysicsWorld.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(DependSuffix): ../../Source/Managers/PhysicsWorld.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(DependSuffix) -MM ../../Source/Managers/PhysicsWorld.cpp

$(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(PreprocessSuffix): ../../Source/Managers/PhysicsWorld.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_PhysicsWorld.cpp$(PreprocessSuffix) ../../Source/Managers/PhysicsWorld.cpp

$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(ObjectSuffix): ../../Source/Managers/DebugManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/DebugManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(DependSuffix): ../../Source/Managers/DebugManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(DependSuffix) -MM ../../Source/Managers/DebugManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(PreprocessSuffix): ../../Source/Managers/DebugManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_DebugManager.cpp$(PreprocessSuffix) ../../Source/Managers/DebugManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(ObjectSuffix): ../../Source/Managers/SoundManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/SoundManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(DependSuffix): ../../Source/Managers/SoundManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(DependSuffix) -MM ../../Source/Managers/SoundManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(PreprocessSuffix): ../../Source/Managers/SoundManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_SoundManager.cpp$(PreprocessSuffix) ../../Source/Managers/SoundManager.cpp

$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(ObjectSuffix): ../../Source/Components/Surface.cpp $(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Surface.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(DependSuffix): ../../Source/Components/Surface.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(DependSuffix) -MM ../../Source/Components/Surface.cpp

$(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(PreprocessSuffix): ../../Source/Components/Surface.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Surface.cpp$(PreprocessSuffix) ../../Source/Components/Surface.cpp

$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(ObjectSuffix): ../../Source/Components/PhysicsObject.cpp $(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/PhysicsObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(DependSuffix): ../../Source/Components/PhysicsObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(DependSuffix) -MM ../../Source/Components/PhysicsObject.cpp

$(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(PreprocessSuffix): ../../Source/Components/PhysicsObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_PhysicsObject.cpp$(PreprocessSuffix) ../../Source/Components/PhysicsObject.cpp

$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(ObjectSuffix): ../../Source/Input/PCInputHandler.cpp $(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Input/PCInputHandler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(DependSuffix): ../../Source/Input/PCInputHandler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(DependSuffix) -MM ../../Source/Input/PCInputHandler.cpp

$(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(PreprocessSuffix): ../../Source/Input/PCInputHandler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Input_PCInputHandler.cpp$(PreprocessSuffix) ../../Source/Input/PCInputHandler.cpp

$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(ObjectSuffix): ../../Source/GameApp.cpp $(IntermediateDirectory)/up_up_Source_GameApp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/GameApp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(DependSuffix): ../../Source/GameApp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(DependSuffix) -MM ../../Source/GameApp.cpp

$(IntermediateDirectory)/up_up_Source_GameApp.cpp$(PreprocessSuffix): ../../Source/GameApp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_GameApp.cpp$(PreprocessSuffix) ../../Source/GameApp.cpp

$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(ObjectSuffix): ../../Source/Managers/LevelManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/LevelManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(DependSuffix): ../../Source/Managers/LevelManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(DependSuffix) -MM ../../Source/Managers/LevelManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(PreprocessSuffix): ../../Source/Managers/LevelManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_LevelManager.cpp$(PreprocessSuffix) ../../Source/Managers/LevelManager.cpp

$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(ObjectSuffix): ../../Source/Core/DefaultComponentFactory.cpp $(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/DefaultComponentFactory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(DependSuffix): ../../Source/Core/DefaultComponentFactory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(DependSuffix) -MM ../../Source/Core/DefaultComponentFactory.cpp

$(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(PreprocessSuffix): ../../Source/Core/DefaultComponentFactory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_DefaultComponentFactory.cpp$(PreprocessSuffix) ../../Source/Core/DefaultComponentFactory.cpp

$(IntermediateDirectory)/up_up_Source_main.cpp$(ObjectSuffix): ../../Source/main.cpp $(IntermediateDirectory)/up_up_Source_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_main.cpp$(DependSuffix): ../../Source/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_main.cpp$(DependSuffix) -MM ../../Source/main.cpp

$(IntermediateDirectory)/up_up_Source_main.cpp$(PreprocessSuffix): ../../Source/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_main.cpp$(PreprocessSuffix) ../../Source/main.cpp

$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(ObjectSuffix): ../../Source/Components/Camera.cpp $(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(DependSuffix): ../../Source/Components/Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(DependSuffix) -MM ../../Source/Components/Camera.cpp

$(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(PreprocessSuffix): ../../Source/Components/Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Camera.cpp$(PreprocessSuffix) ../../Source/Components/Camera.cpp

$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(ObjectSuffix): ../../Source/Components/SoundEmitter.cpp $(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/SoundEmitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(DependSuffix): ../../Source/Components/SoundEmitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(DependSuffix) -MM ../../Source/Components/SoundEmitter.cpp

$(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(PreprocessSuffix): ../../Source/Components/SoundEmitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_SoundEmitter.cpp$(PreprocessSuffix) ../../Source/Components/SoundEmitter.cpp

$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(ObjectSuffix): ../../Source/Managers/NetworkManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/NetworkManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(DependSuffix): ../../Source/Managers/NetworkManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(DependSuffix) -MM ../../Source/Managers/NetworkManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(PreprocessSuffix): ../../Source/Managers/NetworkManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_NetworkManager.cpp$(PreprocessSuffix) ../../Source/Managers/NetworkManager.cpp

$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(ObjectSuffix): ../../Source/Core/GameObject.cpp $(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/GameObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(DependSuffix): ../../Source/Core/GameObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(DependSuffix) -MM ../../Source/Core/GameObject.cpp

$(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(PreprocessSuffix): ../../Source/Core/GameObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_GameObject.cpp$(PreprocessSuffix) ../../Source/Core/GameObject.cpp

$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(ObjectSuffix): ../../Source/Core/FileCache.cpp $(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/FileCache.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(DependSuffix): ../../Source/Core/FileCache.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(DependSuffix) -MM ../../Source/Core/FileCache.cpp

$(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(PreprocessSuffix): ../../Source/Core/FileCache.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_FileCache.cpp$(PreprocessSuffix) ../../Source/Core/FileCache.cpp

$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(ObjectSuffix): ../../Source/Core/HashString.cpp $(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/HashString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(DependSuffix): ../../Source/Core/HashString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(DependSuffix) -MM ../../Source/Core/HashString.cpp

$(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(PreprocessSuffix): ../../Source/Core/HashString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_HashString.cpp$(PreprocessSuffix) ../../Source/Core/HashString.cpp

$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(ObjectSuffix): ../../Source/Managers/GraphicsManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/GraphicsManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(DependSuffix): ../../Source/Managers/GraphicsManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(DependSuffix) -MM ../../Source/Managers/GraphicsManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(PreprocessSuffix): ../../Source/Managers/GraphicsManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_GraphicsManager.cpp$(PreprocessSuffix) ../../Source/Managers/GraphicsManager.cpp

$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(ObjectSuffix): ../../Source/Core/MathExt.cpp $(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/MathExt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(DependSuffix): ../../Source/Core/MathExt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(DependSuffix) -MM ../../Source/Core/MathExt.cpp

$(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(PreprocessSuffix): ../../Source/Core/MathExt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_MathExt.cpp$(PreprocessSuffix) ../../Source/Core/MathExt.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(ObjectSuffix): ../../Source/Managers/ChemistryManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/ChemistryManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(DependSuffix): ../../Source/Managers/ChemistryManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(DependSuffix) -MM ../../Source/Managers/ChemistryManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(PreprocessSuffix): ../../Source/Managers/ChemistryManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_ChemistryManager.cpp$(PreprocessSuffix) ../../Source/Managers/ChemistryManager.cpp

$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(ObjectSuffix): ../../Source/Components/CustomScript.cpp $(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/CustomScript.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(DependSuffix): ../../Source/Components/CustomScript.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(DependSuffix) -MM ../../Source/Components/CustomScript.cpp

$(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(PreprocessSuffix): ../../Source/Components/CustomScript.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_CustomScript.cpp$(PreprocessSuffix) ../../Source/Components/CustomScript.cpp

$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(ObjectSuffix): ../../Source/Components/Transform.cpp $(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Transform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(DependSuffix): ../../Source/Components/Transform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(DependSuffix) -MM ../../Source/Components/Transform.cpp

$(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(PreprocessSuffix): ../../Source/Components/Transform.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Transform.cpp$(PreprocessSuffix) ../../Source/Components/Transform.cpp

$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(ObjectSuffix): ../../Source/LUA/lua.c $(IntermediateDirectory)/up_up_Source_LUA_lua.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/lua.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(DependSuffix): ../../Source/LUA/lua.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(DependSuffix) -MM ../../Source/LUA/lua.c

$(IntermediateDirectory)/up_up_Source_LUA_lua.c$(PreprocessSuffix): ../../Source/LUA/lua.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_LUA_lua.c$(PreprocessSuffix) ../../Source/LUA/lua.c

$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(ObjectSuffix): ../../Source/Components/ChemistryMaterial.cpp $(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/ChemistryMaterial.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(DependSuffix): ../../Source/Components/ChemistryMaterial.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(DependSuffix) -MM ../../Source/Components/ChemistryMaterial.cpp

$(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(PreprocessSuffix): ../../Source/Components/ChemistryMaterial.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_ChemistryMaterial.cpp$(PreprocessSuffix) ../../Source/Components/ChemistryMaterial.cpp

$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(ObjectSuffix): ../../Source/Sound/DSPs/FMOD/FMOD_DSP.cpp $(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Sound/DSPs/FMOD/FMOD_DSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(DependSuffix): ../../Source/Sound/DSPs/FMOD/FMOD_DSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(DependSuffix) -MM ../../Source/Sound/DSPs/FMOD/FMOD_DSP.cpp

$(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(PreprocessSuffix): ../../Source/Sound/DSPs/FMOD/FMOD_DSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Sound_DSPs_FMOD_FMOD_DSP.cpp$(PreprocessSuffix) ../../Source/Sound/DSPs/FMOD/FMOD_DSP.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(ObjectSuffix): ../../Source/Graphics/SurfaceProperty.cpp $(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/SurfaceProperty.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(DependSuffix): ../../Source/Graphics/SurfaceProperty.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(DependSuffix) -MM ../../Source/Graphics/SurfaceProperty.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(PreprocessSuffix): ../../Source/Graphics/SurfaceProperty.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_SurfaceProperty.cpp$(PreprocessSuffix) ../../Source/Graphics/SurfaceProperty.cpp

$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(ObjectSuffix): ../../Source/Physics/CollisionPair.cpp $(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/CollisionPair.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(DependSuffix): ../../Source/Physics/CollisionPair.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(DependSuffix) -MM ../../Source/Physics/CollisionPair.cpp

$(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(PreprocessSuffix): ../../Source/Physics/CollisionPair.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_CollisionPair.cpp$(PreprocessSuffix) ../../Source/Physics/CollisionPair.cpp

$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(ObjectSuffix): ../../Source/Core/ShapeMath.cpp $(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/ShapeMath.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(DependSuffix): ../../Source/Core/ShapeMath.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(DependSuffix) -MM ../../Source/Core/ShapeMath.cpp

$(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(PreprocessSuffix): ../../Source/Core/ShapeMath.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_ShapeMath.cpp$(PreprocessSuffix) ../../Source/Core/ShapeMath.cpp

$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(ObjectSuffix): ../../Source/LUA/LUATypes.cpp $(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/LUATypes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(DependSuffix): ../../Source/LUA/LUATypes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(DependSuffix) -MM ../../Source/LUA/LUATypes.cpp

$(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(PreprocessSuffix): ../../Source/LUA/LUATypes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_LUA_LUATypes.cpp$(PreprocessSuffix) ../../Source/LUA/LUATypes.cpp

$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(ObjectSuffix): ../../Source/Logic/SystemProperties.cpp $(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/SystemProperties.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(DependSuffix): ../../Source/Logic/SystemProperties.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(DependSuffix) -MM ../../Source/Logic/SystemProperties.cpp

$(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(PreprocessSuffix): ../../Source/Logic/SystemProperties.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_SystemProperties.cpp$(PreprocessSuffix) ../../Source/Logic/SystemProperties.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(ObjectSuffix): ../../Source/Graphics/Framebuffer.cpp $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Framebuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(DependSuffix): ../../Source/Graphics/Framebuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(DependSuffix) -MM ../../Source/Graphics/Framebuffer.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(PreprocessSuffix): ../../Source/Graphics/Framebuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_Framebuffer.cpp$(PreprocessSuffix) ../../Source/Graphics/Framebuffer.cpp

$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(ObjectSuffix): ../../Source/LUA/SLB.cpp $(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/LUA/SLB.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(DependSuffix): ../../Source/LUA/SLB.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(DependSuffix) -MM ../../Source/LUA/SLB.cpp

$(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(PreprocessSuffix): ../../Source/LUA/SLB.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_LUA_SLB.cpp$(PreprocessSuffix) ../../Source/LUA/SLB.cpp

$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(ObjectSuffix): ../../Source/Physics/Resolver.cpp $(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Physics/Resolver.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(DependSuffix): ../../Source/Physics/Resolver.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(DependSuffix) -MM ../../Source/Physics/Resolver.cpp

$(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(PreprocessSuffix): ../../Source/Physics/Resolver.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Physics_Resolver.cpp$(PreprocessSuffix) ../../Source/Physics/Resolver.cpp

$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(ObjectSuffix): ../../Source/Core/Common.cpp $(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Common.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(DependSuffix): ../../Source/Core/Common.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(DependSuffix) -MM ../../Source/Core/Common.cpp

$(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(PreprocessSuffix): ../../Source/Core/Common.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_Common.cpp$(PreprocessSuffix) ../../Source/Core/Common.cpp

$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(ObjectSuffix): ../../Source/Managers/EffectsManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/EffectsManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(DependSuffix): ../../Source/Managers/EffectsManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(DependSuffix) -MM ../../Source/Managers/EffectsManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(PreprocessSuffix): ../../Source/Managers/EffectsManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_EffectsManager.cpp$(PreprocessSuffix) ../../Source/Managers/EffectsManager.cpp

$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(ObjectSuffix): ../../Source/Core/Shape.cpp $(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Shape.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(DependSuffix): ../../Source/Core/Shape.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(DependSuffix) -MM ../../Source/Core/Shape.cpp

$(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(PreprocessSuffix): ../../Source/Core/Shape.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_Shape.cpp$(PreprocessSuffix) ../../Source/Core/Shape.cpp

$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(ObjectSuffix): ../../Source/Components/StateObject.cpp $(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/StateObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(DependSuffix): ../../Source/Components/StateObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(DependSuffix) -MM ../../Source/Components/StateObject.cpp

$(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(PreprocessSuffix): ../../Source/Components/StateObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_StateObject.cpp$(PreprocessSuffix) ../../Source/Components/StateObject.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(ObjectSuffix): ../../Source/Managers/ObjectManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/ObjectManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(DependSuffix): ../../Source/Managers/ObjectManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(DependSuffix) -MM ../../Source/Managers/ObjectManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(PreprocessSuffix): ../../Source/Managers/ObjectManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_ObjectManager.cpp$(PreprocessSuffix) ../../Source/Managers/ObjectManager.cpp

$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(ObjectSuffix): ../../Source/Components/ChemistryElement.cpp $(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/ChemistryElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(DependSuffix): ../../Source/Components/ChemistryElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(DependSuffix) -MM ../../Source/Components/ChemistryElement.cpp

$(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(PreprocessSuffix): ../../Source/Components/ChemistryElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_ChemistryElement.cpp$(PreprocessSuffix) ../../Source/Components/ChemistryElement.cpp

$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(ObjectSuffix): ../../Source/Components/FollowComponent.cpp $(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/FollowComponent.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(DependSuffix): ../../Source/Components/FollowComponent.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(DependSuffix) -MM ../../Source/Components/FollowComponent.cpp

$(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(PreprocessSuffix): ../../Source/Components/FollowComponent.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_FollowComponent.cpp$(PreprocessSuffix) ../../Source/Components/FollowComponent.cpp

$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(ObjectSuffix): ../../Source/Components/Constraint.cpp $(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Constraint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(DependSuffix): ../../Source/Components/Constraint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(DependSuffix) -MM ../../Source/Components/Constraint.cpp

$(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(PreprocessSuffix): ../../Source/Components/Constraint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Constraint.cpp$(PreprocessSuffix) ../../Source/Components/Constraint.cpp

$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(ObjectSuffix): ../../Source/Components/Controller.cpp $(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(DependSuffix): ../../Source/Components/Controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(DependSuffix) -MM ../../Source/Components/Controller.cpp

$(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(PreprocessSuffix): ../../Source/Components/Controller.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Controller.cpp$(PreprocessSuffix) ../../Source/Components/Controller.cpp

$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(ObjectSuffix): ../../Source/Components/Button.cpp $(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/Button.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(DependSuffix): ../../Source/Components/Button.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(DependSuffix) -MM ../../Source/Components/Button.cpp

$(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(PreprocessSuffix): ../../Source/Components/Button.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_Button.cpp$(PreprocessSuffix) ../../Source/Components/Button.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(ObjectSuffix): ../../Source/Graphics/ShaderLoader.cpp $(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/ShaderLoader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(DependSuffix): ../../Source/Graphics/ShaderLoader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(DependSuffix) -MM ../../Source/Graphics/ShaderLoader.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(PreprocessSuffix): ../../Source/Graphics/ShaderLoader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_ShaderLoader.cpp$(PreprocessSuffix) ../../Source/Graphics/ShaderLoader.cpp

$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(ObjectSuffix): ../../Source/Components/SoundListener.cpp $(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Components/SoundListener.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(DependSuffix): ../../Source/Components/SoundListener.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(DependSuffix) -MM ../../Source/Components/SoundListener.cpp

$(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(PreprocessSuffix): ../../Source/Components/SoundListener.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Components_SoundListener.cpp$(PreprocessSuffix) ../../Source/Components/SoundListener.cpp

$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(ObjectSuffix): ../../Source/Core/Component.cpp $(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Component.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(DependSuffix): ../../Source/Core/Component.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(DependSuffix) -MM ../../Source/Core/Component.cpp

$(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(PreprocessSuffix): ../../Source/Core/Component.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_Component.cpp$(PreprocessSuffix) ../../Source/Core/Component.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(ObjectSuffix): ../../Source/Graphics/QuadTree.cpp $(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/QuadTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(DependSuffix): ../../Source/Graphics/QuadTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(DependSuffix) -MM ../../Source/Graphics/QuadTree.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(PreprocessSuffix): ../../Source/Graphics/QuadTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_QuadTree.cpp$(PreprocessSuffix) ../../Source/Graphics/QuadTree.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(ObjectSuffix): ../../Source/Graphics/TextureCoordinates.cpp $(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/TextureCoordinates.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(DependSuffix): ../../Source/Graphics/TextureCoordinates.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(DependSuffix) -MM ../../Source/Graphics/TextureCoordinates.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(PreprocessSuffix): ../../Source/Graphics/TextureCoordinates.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_TextureCoordinates.cpp$(PreprocessSuffix) ../../Source/Graphics/TextureCoordinates.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(ObjectSuffix): ../../Source/Graphics/Screen.cpp $(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Screen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(DependSuffix): ../../Source/Graphics/Screen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(DependSuffix) -MM ../../Source/Graphics/Screen.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(PreprocessSuffix): ../../Source/Graphics/Screen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_Screen.cpp$(PreprocessSuffix) ../../Source/Graphics/Screen.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(ObjectSuffix): ../../Source/Graphics/ZRenderSorter.cpp $(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/ZRenderSorter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(DependSuffix): ../../Source/Graphics/ZRenderSorter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(DependSuffix) -MM ../../Source/Graphics/ZRenderSorter.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(PreprocessSuffix): ../../Source/Graphics/ZRenderSorter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_ZRenderSorter.cpp$(PreprocessSuffix) ../../Source/Graphics/ZRenderSorter.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(ObjectSuffix): ../../Source/Logic/Level.cpp $(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Level.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(DependSuffix): ../../Source/Logic/Level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(DependSuffix) -MM ../../Source/Logic/Level.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(PreprocessSuffix): ../../Source/Logic/Level.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_Level.cpp$(PreprocessSuffix) ../../Source/Logic/Level.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(ObjectSuffix): ../../Source/Graphics/BatchRenderSorter.cpp $(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/BatchRenderSorter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(DependSuffix): ../../Source/Graphics/BatchRenderSorter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(DependSuffix) -MM ../../Source/Graphics/BatchRenderSorter.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(PreprocessSuffix): ../../Source/Graphics/BatchRenderSorter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_BatchRenderSorter.cpp$(PreprocessSuffix) ../../Source/Graphics/BatchRenderSorter.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(ObjectSuffix): ../../Source/Parsers/Parser.cpp $(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Parsers/Parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(DependSuffix): ../../Source/Parsers/Parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(DependSuffix) -MM ../../Source/Parsers/Parser.cpp

$(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(PreprocessSuffix): ../../Source/Parsers/Parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Parsers_Parser.cpp$(PreprocessSuffix) ../../Source/Parsers/Parser.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(ObjectSuffix): ../../Source/Logic/Constants.cpp $(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/Constants.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(DependSuffix): ../../Source/Logic/Constants.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(DependSuffix) -MM ../../Source/Logic/Constants.cpp

$(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(PreprocessSuffix): ../../Source/Logic/Constants.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_Constants.cpp$(PreprocessSuffix) ../../Source/Logic/Constants.cpp

$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(ObjectSuffix): ../../Source/Logic/TileMapGenerator.cpp $(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/TileMapGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(DependSuffix): ../../Source/Logic/TileMapGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(DependSuffix) -MM ../../Source/Logic/TileMapGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(PreprocessSuffix): ../../Source/Logic/TileMapGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_TileMapGenerator.cpp$(PreprocessSuffix) ../../Source/Logic/TileMapGenerator.cpp

$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(ObjectSuffix): ../../Source/Logic/DefaultGameObjectFactory.cpp $(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Logic/DefaultGameObjectFactory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(DependSuffix): ../../Source/Logic/DefaultGameObjectFactory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(DependSuffix) -MM ../../Source/Logic/DefaultGameObjectFactory.cpp

$(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(PreprocessSuffix): ../../Source/Logic/DefaultGameObjectFactory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Logic_DefaultGameObjectFactory.cpp$(PreprocessSuffix) ../../Source/Logic/DefaultGameObjectFactory.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(ObjectSuffix): ../../Source/Graphics/Screens/PCShaderScreen.cpp $(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Graphics/Screens/PCShaderScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(DependSuffix): ../../Source/Graphics/Screens/PCShaderScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(DependSuffix) -MM ../../Source/Graphics/Screens/PCShaderScreen.cpp

$(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(PreprocessSuffix): ../../Source/Graphics/Screens/PCShaderScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Graphics_Screens_PCShaderScreen.cpp$(PreprocessSuffix) ../../Source/Graphics/Screens/PCShaderScreen.cpp

$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(ObjectSuffix): ../../Source/Core/Threading.cpp $(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Core/Threading.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(DependSuffix): ../../Source/Core/Threading.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(DependSuffix) -MM ../../Source/Core/Threading.cpp

$(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(PreprocessSuffix): ../../Source/Core/Threading.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Core_Threading.cpp$(PreprocessSuffix) ../../Source/Core/Threading.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(ObjectSuffix): ../../Source/Managers/ControllerManager.cpp $(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/jimmy/Documents/JFramework/Source/Managers/ControllerManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(DependSuffix): ../../Source/Managers/ControllerManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(DependSuffix) -MM ../../Source/Managers/ControllerManager.cpp

$(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(PreprocessSuffix): ../../Source/Managers/ControllerManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_Source_Managers_ControllerManager.cpp$(PreprocessSuffix) ../../Source/Managers/ControllerManager.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


