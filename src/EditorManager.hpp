#pragma once

#ifndef QSCITE_EDITORMANAGER_HPP
#define QSCITE_EDITORMANAGER_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include "Editor.hpp"


/**
 * class EditorManager
 * Handles a list of scintilla editors. Works as the manager of all open
 * documents in the main editor.
 *
 * author: jhenriques 2014
 */
class EditorManager {

private:
	
	std::vector< EditorPtr > editors;
	std::map< ScintillaEditPtr, EditorPtr > editorsMap;
	std::map< ScintillaEdit *, EditorPtr > editorsPtrMap;
	
protected:
	
public:
	
	EditorManager( );
	virtual ~EditorManager( );
	
	EditorPtr createEditor( );
	
	EditorPtr getEditor( ScintillaEditPtr );
	EditorPtr getEditor( ScintillaEdit * );

};

typedef boost::shared_ptr< EditorManager > EditorManagerPtr;

#endif