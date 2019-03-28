CMAKE_MINIMUM_REQUIRED( VERSION 3.7 )
PROJECT( FXRunner )

# The project settings
set( CMAKE_INSTALL_PREFIX "/opt/Gorgona" )

# Configure project
# CONFIGURE_FILE(
#   "${PROJECT_SOURCE_DIR}/MakeConfig.h.in"
#   "${PROJECT_BINARY_DIR}/MakeConfig.h"
# )
# EMPTY

# Add directories for project
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )
#INCLUDE_DIRECTORIES( "/opt/foxlib/1.7/64/include/fox-1.7" )
#LINK_DIRECTORIES(    "/opt/foxlib/1.7/64/lib/" )

# Add to the executable
ADD_EXECUTABLE( gorgona src/main.cpp src/Process.cpp src/IconsTheme.cpp src/Utils.cpp src/FXGameItem.cpp
src/FXGameLauncher.cpp src/FXLaunchEditor.cpp src/FXLauncherLua.cpp src/GO_Keywords.cpp src/FXListPane.cpp )
TARGET_LINK_LIBRARIES( gorgona FOX-1.7 CHART-1.7 tinyxml lua5.1 )

# Install build project
INSTALL( TARGETS gorgona RUNTIME DESTINATION . )
INSTALL( DIRECTORY modules DESTINATION . )
INSTALL( FILES integration/medusa-head.png DESTINATION /usr/share/pixmaps/ )
INSTALL( FILES integration/Gorgona.desktop DESTINATION /usr/share/applications/ )
