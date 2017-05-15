#----------------------------------------------------------------------------
# QSciTE cmake file
#----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

#----------------------------------------------------------------------------
# set up compile flags
#----------------------------------------------------------------------------
if( NOT DEFINED CMAKE_USER_MAKE_RULES_OVERRIDE )
	set( CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_SOURCE_DIR}/CompilerDefaultFlags.cmake" )
endif( NOT DEFINED CMAKE_USER_MAKE_RULES_OVERRIDE )

project( QSciTE )

set (QT_QMAKE_EXECUTABLE /usr/bin/qmake-qt4)

#----------------------------------------------------------------------------
# Project wide cmake configuration:
#  - Use tools/cmake submodule find modules
#  - Place compilation results in:
#   - ${PROJECT_SOURCE_DIR}/build folder IF BUILD_PATH not set.
#	- ${BUILD_PATH}/build
#----------------------------------------------------------------------------
set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tools/cmake )

if( BUILD_PATH )
	set( _BUILD_PATH ${BUILD_PATH} )
else( BUILD_PATH )
	set( _BUILD_PATH ${PROJECT_SOURCE_DIR} )
endif( BUILD_PATH )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/obj )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/lib )

#----------------------------------------------------------------------------
# QScite optionals:
#----------------------------------------------------------------------------
#option( WITH_???? 					"Compile with ???? support" 	OFF )

#----------------------------------------------------------------------------
# Find Dependencies:
#----------------------------------------------------------------------------
find_package( Qt4 4.6.2 COMPONENTS QtCore QtGui QtOpenGL QtMain REQUIRED )
include(${QT_USE_FILE})

# Take care of Boost:
set( _BOOST_ROOT $ENV{BOOST_ROOT} )
if( NOT BOOST_ROOT )
	if( _BOOST_ROOT )
		set( BOOST_ROOT $ENV{BOOST_ROOT} )
	endif( _BOOST_ROOT )
endif( NOT BOOST_ROOT )
set( Boost_ADDITIONAL_VERSIONS 1.52_0 1.51_0 1.50 1.49 )

# We might need diferent version/modules from boost depending on system
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	find_package(Boost REQUIRED thread system filesystem date_time regex serialization iostreams )
	# Let Boost always link dynamically
	add_definitions( -DBOOST_ALL_DYN_LINK )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )	
	find_package(Boost REQUIRED thread system filesystem date_time regex serialization iostreams)
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	find_package(Boost REQUIRED system thread filesystem date_time regex serialization iostreams)
	# We need to link pthread on linux:
	set( CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -lpthread -lm" )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )

# for windows we need the list of dlls to copy to binary path and for install package
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	
	# because find boost is an external cmake module, assemble the DLL list:
	foreach( dll ${Boost_LIBRARIES} )
		if( dll MATCHES "(.*/)lib(.*).lib$" )
			set( LIB_NAME ${CMAKE_MATCH_1}lib/${CMAKE_MATCH_2}.dll )
			if(  ${LIB_NAME} MATCHES ".*-gd-.*" )
				set( DEBUG_DLL_LIST ${DEBUG_DLL_LIST} ${LIB_NAME} )
			else( ${LIB_NAME} MATCHES ".*-gd-.*" )
				set( RELEASE_DLL_LIST ${RELEASE_DLL_LIST} ${LIB_NAME} )
			endif(  ${LIB_NAME} MATCHES ".*-gd-.*" )
			unset( LIB_NAME )
		endif( dll MATCHES "(.*/)lib(.*).lib$" )
	endforeach( dll ${Boost_LIBRARIES} )
	
	# because find Qt is an external cmake module, assemble the DLL list:
	foreach( dll ${QT_LIBRARIES} )
		if( dll MATCHES "(.*/)lib(.*).lib$" )
			set( LIB_NAME ${CMAKE_MATCH_1}bin${CMAKE_MATCH_2}.dll )
			if( NOT ${LIB_NAME} MATCHES "qtmain.*.dll" )	# qt set a lib we do not need (and does not exist)
				if( ${LIB_NAME} MATCHES ".*d.dll$" OR ${LIB_NAME} MATCHES ".*d4.dll$" )
					set( DEBUG_DLL_LIST ${DEBUG_DLL_LIST} ${LIB_NAME} )
				else( ${LIB_NAME} MATCHES ".*d.dll$" OR ${LIB_NAME} MATCHES ".*d4.dll$" )
					set( RELEASE_DLL_LIST ${RELEASE_DLL_LIST} ${LIB_NAME} )
				endif( ${LIB_NAME} MATCHES ".*d.dll$" OR ${LIB_NAME} MATCHES ".*d4.dll$" )
			endif( NOT ${LIB_NAME} MATCHES "qtmain.*.dll" )
			unset( LIB_NAME )
		endif( dll MATCHES "(.*/)lib(.*).lib$" )
	endforeach( dll ${QT_LIBRARIES} )

endif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )


# Scintilla stuff:
set( SCINTILLA_INSTALL_DIR ${PROJECT_SOURCE_DIR}/3rdparty/scintilla )
find_package( Scintilla REQUIRED )


#----------------------------------------------------------------------------
# Pretty output:
#----------------------------------------------------------------------------
message( "\n====================== QSciTE =======================")
message( "      Building for : " ${CMAKE_SYSTEM_NAME} )
message( "        BUILD_PATH : " ${_BUILD_PATH} )
message( "        BOOST_ROOT : " ${BOOST_ROOT} )
message( "    QT_PLUGIN_PATH : " $ENV{QT_PLUGIN_PATH} )
message( "=========================================================\n")


#----------------------------------------------------------------------------
# Create target QScite
#----------------------------------------------------------------------------
# files which need to be moc'd by Qt
set( QSciTE_MOC_SRCS	src/MainWindow.hpp )
						
# files wich need to be uic by Qt (empty for now)
set( QSciTE_UI_SRCS	 )

# Project code files
set( QSciTE_SRCS_CXX 	src/main.cpp
						src/MainWindow.cpp
						src/MainWindow.hpp
						src/Editor.hpp
						src/Editor.cpp
						src/EditorManager.hpp
						src/EditorManager.cpp

						#src/Buffer.hpp
						#src/Buffer.cpp
						#src/BufferManager.hpp
						#src/BufferManager.cpp
						)

# build ui_XXX files from the XML-style .ui files
QT4_WRAP_UI( QSciTE_UI_CXX resources/QSciTE.ui )
# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP( QSciTE_SRCS_CXX ${QSciTE_MOC_SRCS} )
# generate rules for building source files from the resources
QT4_ADD_RESOURCES( QSciTE_RCC_SRCS resources/resources.qrc )


include_directories( 	${Boost_INCLUDE_DIRS}
						${QT_INCLUDE_DIR}
						${QT_QTDESIGNER_INCLUDE_DIR}
						${QSciTE_BINARY_DIR}
						${SCINTILLA_INCLUDE_DIRS}
												
						resources )

add_executable( QSciTE WIN32 MACOSX_BUNDLE 	${QSciTE_SRCS_CXX} ${QSciTE_UI_CXX}	${QSciTE_RCC_SRCS} ) 

#----------------------------------------------------------------------------
# In Mac OS X, QSciTE is a Bundle. Need to set extra properties
#----------------------------------------------------------------------------
if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
	set_target_properties( QSciTE PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/resources/Info.plist )
	#set_source_files_properties( resources/background.png PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	#set_source_files_properties( resources/package_icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	#set_source_files_properties( resources/qscite_icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	#set_source_files_properties( resources/DS_Store_custom PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )

target_link_libraries( QSciTE	${QT_LIBRARIES} ${Boost_LIBRARIES} ${SCINTILLA_LIBRARIES} )

#----------------------------------------------------------------------------
# Register some post Build commands:
# If the folder does not exists cmake will make a mess of the filesystem...
#----------------------------------------------------------------------------
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	file( MAKE_DIRECTORY ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/bin/Debug )
	foreach( dll ${DEBUG_DLL_LIST} )
		add_custom_command( TARGET QSciTE POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy ${dll} ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/bin/Debug )
	endforeach( dll )
	
	file( MAKE_DIRECTORY ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/bin/Release )
	foreach( dll ${RELEASE_DLL_LIST} )
		add_custom_command( TARGET QSciTE POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy ${dll} ${_BUILD_PATH}/build/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/bin/Release )
	endforeach( dll )
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )



