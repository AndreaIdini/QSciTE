#############################################################################
# - Find Scintilla 
#
# This module defines
#  - SCINTILLA_FOUND
#  - SCINTILLA_INCLUDE_DIRS
#  - SCINTILLA_LIBRARIES
#
# This modules supposes that the user might have provided (and looks at those places)
#  - SCINTILLA_INSTALL_DIR
#
#############################################################################

IF( SCINTILLA_INCLUDE_PATH AND SCINTILLA_LIBRARIES )
  SET( SCINTILLA_FIND_QUIETLY TRUE ) # Already in cache, be silent.
ENDIF( SCINTILLA_INCLUDE_PATH AND SCINTILLA_LIBRARIES)

set( ENV_INSTALL_DIR $ENV{SCINTILLA_INSTALL_DIR} )

#----------------------------------------------------------------------------
# Construct potential include and lib paths
#----------------------------------------------------------------------------
set( SCINTILLA_POTENTIAL_INCPATH ${SCINTILLA_POTENTIAL_INCPATH}

    ${ENV_INSTALL_DIR}/include
    ${SCINTILLA_INSTALL_DIR}/include

	${ENV_INSTALL_DIR}/qt/ScintillaEditBase/
	${SCINTILLA_INSTALL_DIR}/qt/ScintillaEditBase/

	${ENV_INSTALL_DIR}/qt/ScintillaEdit/
	${SCINTILLA_INSTALL_DIR}/qt/ScintillaEdit/

    #Mac os X

    #linux:

)

set( SCINTILLA_POTENTIAL_LIBPATH ${SCINTILLA_POTENTIAL_LIBPATH}
    
    HINTS   

    ${ENV_INSTALL_DIR}/bin
    ${SCINTILLA_INSTALL_DIR}/bin

    #Mac OS X:

    #linux:

)

#message( "LIB PATH:  ${SCINTILLA_POTENTIAL_LIBPATH}" )

#----------------------------------------------------------------------------
# Set SCINTILLA_INCLUDE_DIRS. (The path to the include folder)
#----------------------------------------------------------------------------
find_path( SCINTILLA_INCLUDE_MAIN_DIRS NAMES Scintilla.h PATHS ${SCINTILLA_POTENTIAL_INCPATH} )
find_path( SCINTILLA_INCLUDE_QT_EDITBASE_DIRS NAMES ScintillaEditBase.h PATHS ${SCINTILLA_POTENTIAL_INCPATH} )
find_path( SCINTILLA_INCLUDE_QT_EDIT_DIRS NAMES ScintillaEdit.h PATHS ${SCINTILLA_POTENTIAL_INCPATH} )

set( SCINTILLA_INCLUDE_DIRS ${SCINTILLA_INCLUDE_MAIN_DIRS} ${SCINTILLA_INCLUDE_QT_EDITBASE_DIRS} ${SCINTILLA_INCLUDE_QT_EDIT_DIRS} )

#message( "\n SCINTILLA_INCLUDE_DIRS: ${SCINTILLA_INCLUDE_DIRS}" )

#----------------------------------------------------------------------------
# Set SCINTILLA_LIBRARIES.
#----------------------------------------------------------------------------
find_library( SCINTILLA_LIBRARY_RELEASE NAMES ScintillaEdit PATHS ${SCINTILLA_POTENTIAL_LIBPATH} )

set( SCINTILLA_LIBRARIES "optimized" ${SCINTILLA_LIBRARY_RELEASE} "debug" ${SCINTILLA_LIBRARY_RELEASE} )

#message( "\n SCINTILLA_LIBRARIES: ${SCINTILLA_LIBRARIES}" )

#----------------------------------------------------------------------------
# Set OVR_FOUND.
#----------------------------------------------------------------------------
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Scintilla DEFAULT_MSG SCINTILLA_LIBRARIES SCINTILLA_INCLUDE_DIRS)

MARK_AS_ADVANCED( SCINTILLA_INCLUDE_DIRS SCINTILLA_INCLUDE_DIRS SCINTILLA_FOUND )
