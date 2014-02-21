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
	scintillaEdit->setFocusPolicy( Qt::StrongFocus );
	scintillaEdit->QWidget::setFocus();
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
 * EditorManager::getWidget
 *
 * author: jhenriques 2014
 */
ScintillaEditPtr EditorManager::getWidget( EditorPtr editor ) {
	
	std::map< ScintillaEditPtr, EditorPtr >::iterator iter = editorsMap.begin();
	while( iter != editorsMap.end() ) {
		
		if( iter->second == editor ) {
			return iter->first;
		}
		iter++;
	}
	return ScintillaEditPtr();
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
	
	// remove from all mappings:
	std::map< ScintillaEditPtr, EditorPtr >::iterator iter1 = editorsMap.begin();
	while( iter1 != editorsMap.end() ) {
		
		if( iter1->second == editor ) {
			editorsMap.erase( iter1 );
			break;
		}
		iter1++;
	}

	std::map< ScintillaEdit *, EditorPtr >::iterator iter2 = editorsPtrMap.begin();
	while( iter2 != editorsPtrMap.end() ) {
		
		if( iter2->second == editor ) {
			editorsPtrMap.erase( iter2 );
			break;
		}
		iter2++;
	}
	
	return true;
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

