#include "EditorManager.hpp"


/**
 * author: jhenriques 2014
 */
EditorManager::EditorManager( ) {
	
}

/**
 * author: jhenriques 2014
 */
EditorManager::~EditorManager( ) {
	
}

/**
 * EditorManager::createEditor
 * A new Editor will be created to manage empty file;
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::createEditor( ) {
	
	// create a new ScintillaEdit widget:
	ScintillaEditPtr scintillaEdit = ScintillaEditPtr( new ScintillaEdit( ) );
	EditorPtr newEditor = EditorPtr( new Editor( scintillaEdit ) );
	
	if( newEditor ) {
		editors.push_back( newEditor );
		editorsMap.insert( std::make_pair( scintillaEdit, newEditor ) );
		editorsPtrMap.insert( std::make_pair( scintillaEdit.get(), newEditor ) );
	}
	
	return newEditor;
}

/**
 * EditorManager::getEditor
 * Returns the internal Editor for the scintillaEdit widget
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::getEditor( ScintillaEditPtr widget ) {

	std::map< ScintillaEditPtr, EditorPtr >::iterator found = editorsMap.find( widget );

	return found->second;
}

/**
 * EditorManager::getEditor
 * Returns the internal Editor for the scintillaEdit widget
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::getEditor( ScintillaEdit *widget ) {
	
	std::map< ScintillaEdit *, EditorPtr >::iterator found = editorsPtrMap.find( widget );
	
	return found->second;
}