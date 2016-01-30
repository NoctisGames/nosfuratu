NDKROOT := /Applications/adt-bundle-mac-x86_64-20140624/android-ndk-r10e
LOCAL_PATH := $(call my-dir)
PROJECT_ROOT_PATH := $(LOCAL_PATH)/../../../../../../
CORE_RELATIVE_PATH := ../../../../../../core/
CORE_FRAMEWORK_RELATIVE_PATH := $(CORE_RELATIVE_PATH)/framework/
CORE_FRAMEWORK_ENTITY_RELATIVE_PATH := $(CORE_FRAMEWORK_RELATIVE_PATH)/entity/
CORE_FRAMEWORK_MATH_RELATIVE_PATH := $(CORE_FRAMEWORK_RELATIVE_PATH)/math/
CORE_FRAMEWORK_STATE_RELATIVE_PATH := $(CORE_FRAMEWORK_RELATIVE_PATH)/state/
CORE_FRAMEWORK_UI_RELATIVE_PATH := $(CORE_FRAMEWORK_RELATIVE_PATH)/ui/
CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH := $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/opengles/
CORE_FRAMEWORK_UTIL_RELATIVE_PATH := $(CORE_FRAMEWORK_RELATIVE_PATH)/util/
CORE_GAME_RELATIVE_PATH := $(CORE_RELATIVE_PATH)/game/
CORE_GAME_UI_RELATIVE_PATH := $(CORE_GAME_RELATIVE_PATH)/ui/
CORE_GAME_UI_OPENGLES_RELATIVE_PATH := $(CORE_GAME_UI_RELATIVE_PATH)/opengles/
CORE_GAME_UI_OPENGLES_ANDROID_RELATIVE_PATH := $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/android/

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra -DGGD_OPENGL_ES -DGGD_ANDROID -DGL_GLEXT_PROTOTYPES=1

# Android
LOCAL_SRC_FILES := platform_asset_utils.c
LOCAL_SRC_FILES += game_renderer.cpp

# Game Framework
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/entity/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/math/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/state/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/ui/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/ui/opengles/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/framework/util/

LOCAL_SRC_FILES += $(CORE_FRAMEWORK_ENTITY_RELATIVE_PATH)/Entity.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_ENTITY_RELATIVE_PATH)/EntityManager.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_ENTITY_RELATIVE_PATH)/PhysicalEntity.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/Circle.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/Line.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/OverlapTester.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/Rectangle.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/Triangle.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_MATH_RELATIVE_PATH)/Vector2D.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/Animation.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/CircleBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/DummyGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/Font.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/GpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/LineBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/RectangleBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/SpriteBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/TextureRegion.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_RELATIVE_PATH)/TouchEvent.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/asset_utils.c
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/image.c
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/ColorProgram.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/FramebufferToScreenProgram.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESCircleBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESFramebufferToScreenGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESGeometryGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESLineBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESManager.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESRectangleBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESSpriteBatcher.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/OpenGLESTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/platform_file_utils.c
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/shader.c
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/texture.c
LOCAL_SRC_FILES += $(CORE_FRAMEWORK_UI_OPENGLES_RELATIVE_PATH)/TextureProgram.cpp

# Game
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/game/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/game/ui/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/game/ui/opengles/
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/core/game/ui/opengles/android/

LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/BackgroundCave.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/BackgroundSky.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/BackgroundTrees.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Carrot.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/CaveExit.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/CaveExitCover.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/DestructiblePhysicalEntity.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/DustCloud.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/EndSign.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/EntityUtils.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Game.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/GoldenCarrot.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Ground.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/GroundPlatform.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Hole.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/HoleCover.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Jon.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/JumpSpring.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/LandPhysicalEntity.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/LogVerticalShort.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/LogVerticalTall.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Rock.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/SideSpike.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/SnakeEnemy.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/SnakeGrunt.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/SnakeHorned.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/SnakeSpirit.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Stump.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Thorns.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/Tree.cpp
LOCAL_SRC_FILES += $(CORE_GAME_RELATIVE_PATH)/UpwardSpike.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/Assets.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/BackButton.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/Chapter1Levels.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/GameScreen.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/GameScreenStates.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/LevelEditor.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/LevelEditorActionsPanel.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/LevelEditorEntitiesPanel.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/LevelSelectorPanel.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/Renderer.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/ShockwaveTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/SinWaveTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/SnakeDeathTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/TransDeathGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/TransitionGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_RELATIVE_PATH)/TrashCan.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESFramebufferTintGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESFramebufferRadialBlurGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESGameScreen.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESRenderer.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESShockwaveTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESSinWaveTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESSnakeDeathTextureGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESTransDeathGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESTransTitleToWorldMapGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/OpenGLESTransWorldMapToLevelGpuProgramWrapper.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/ShockwaveTextureProgram.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/SinWaveTextureProgram.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/SnakeDeathTextureProgram.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/TransDeathProgram.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_RELATIVE_PATH)/TransitionProgram.cpp
LOCAL_SRC_FILES += $(CORE_GAME_UI_OPENGLES_ANDROID_RELATIVE_PATH)/AndroidOpenGLESGameScreen.cpp

# 3rd Party
LOCAL_C_INCLUDES += $(PROJECT_ROOT_PATH)/3rdparty/
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_LDLIBS := -lGLESv2 -landroid

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(PROJECT_ROOT_PATH)/3rdparty)
$(call import-module, libpng)