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

	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
		
		ScintillaEdit *scintillaEdit = editors[i]->getScintillaEdit( );
		if( scintillaEdit ) {
			delete scintillaEdit;
		}
	}
	
	editors.clear();
}

/**
 * EditorManager::createEditor
 * A new Editor will be created to manage empty file;
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::createEditor( ) {
	
	// create a new ScintillaEdit widget:
	ScintillaEdit *scintillaEdit = new ScintillaEdit( );
	scintillaEdit->setFocusPolicy( Qt::StrongFocus );
	scintillaEdit->QWidget::setFocus();
	EditorPtr newEditor = EditorPtr( new Editor( scintillaEdit ) );
	
	if( newEditor ) {
		editors.push_back( newEditor );
	}
	
	return newEditor;
}

/**
 * EditorManager::getEditor
 * Returns the internal Editor for the scintillaEdit widget
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::getEditor( ScintillaEdit *widget ) {

	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
		if( editors[i]->getScintillaEdit() == widget ) {
			return editors[i];
		}
	}	
	
	return EditorPtr();
}

/**
 * EditorManager::getEditorForFile
 *
 * author: jhenriques 2014
 */
EditorPtr EditorManager::getEditorForFile( QString fileName ) {
	
	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
		if( editors[i]->getFilePath() == fileName ) {
			return editors[i];
		}
	}
	return EditorPtr();
}

/**
 * EditorManager::getEditors
 *
 * author: jhenriques 2014
 */
const std::vector< EditorPtr > &EditorManager::getEditors( ) {

	return editors;
}


/**
 * EditorManager::removeEditor
 * Stops tracking the given editor. If no other component holds a reference
 * this will end up destroying the editor;
 *
 * author: jhenriques 2014
 */
bool EditorManager::removeEditor( EditorPtr editor ) {
	
	if( !editor ) {
		return false;
	}

	// remove it from editors list:
	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
		if( editors[i] == editor ) {
			editors.erase( editors.begin() + i );
			break;
		}
	}

	return true;
}

void EditorManager::cleanRemoved( ) {

	editorsRemoved.clear();
}

/**
 * EditorManager::areEditorsDirty
 * Checks all editors for the dirty flag;
 *
 * author: jhenriques 2014
 */
bool EditorManager::areEditorsDirty( ) {
	
	bool flag = false;
	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
		if( editors[i]->isDirty ) {
			flag = true;
			break;
		}
	}
	return flag;
}

