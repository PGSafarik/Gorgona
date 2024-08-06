CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )
PROJECT( Gorgona VERSION 1.5 DESCRIPTION "Gorgona its manager of your game library" )

# The project settings
SET( BIN_NAME "gorgona" )                   # Executable binnary filename
SET( CMAKE_INSTALL_PREFIX "/usr" )          # Install target directory
SET( LIB_FOX   "/opt/foxlib/1.7/84" )       # Fox toolkit path
SET( LIB_FXGHI "/opt/foxghi/0.4/1"  )       # Fox GHI path
SET( CMAKE_CXX_STANDARD 11 )                # C++ Standard
SET( CMAKE_CXX_STANDARD_REQUIRED ON )       # C++ Standard
#add_compile_options(-Wall -Wextra -pedantic -Werror)
#add_compile_options(-Wall -pedantic)

# Configure project
#CONFIGURE_FILE( MakeConfig.h.in MakeConfig.h )
FILE( GLOB SOURCES src/*.cpp src/Perseus/*.cpp src/Echidna/*.cpp )   # List of implementation files  
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )

# Add directories for project dependecies
INCLUDE_DIRECTORIES( "${LIB_FOX}/include/" )
LINK_DIRECTORIES(    "${LIB_FOX}/lib/" )
INCLUDE_DIRECTORIES( "${LIB_FXGHI}/include/" )
LINK_DIRECTORIES(    "${LIB_FXGHI}/lib/" )

# Build test  
#ADD_SUBDIRECTORY( "${PROJECT_SOURCE_DIR}/tests/id" )

# Add to the executable
ADD_EXECUTABLE( ${BIN_NAME} ${SOURCES} )
TARGET_LINK_LIBRARIES( ${BIN_NAME} FOX-1.7 FXGHI tinyxml2 lua5.1 )

# Install build project
INSTALL( TARGETS ${BIN_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )              # application 
INSTALL( DIRECTORY modules DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME} )		# Module files
INSTALL( FILES integration/medusa-head.png DESTINATION /usr/share/pixmaps/ )                # Gorgona ikon
INSTALL( FILES integration/Gorgona.desktop DESTINATION /usr/share/applications/ )           # Gorgona desktop launch file


