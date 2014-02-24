#include "Editor.hpp"

#include <iostream>
#include <QTextStream>
#include <boost/lexical_cast.hpp>


Editor::Editor( ScintillaEditPtr editor ) : editor(editor), isDirty(false) {

	//editor->setViewEOL( true );
	lastFound = QPair<int, int>( 0, 0 );
}


Editor::~Editor( ) {
	
	std::cout << "Editor for " << filePath.fileName().toStdString() << " destroyed." << std::endl;
}


bool Editor::loadFile( QString file ) {
	return loadFile( QFileInfo( file ) );
}

bool Editor::loadFile( QFileInfo file ) {

	if( file.fileName().isEmpty() ) {
		return false;
	}
	
	filePath = file;

	// file is unique and can be open/created:
	QFile newFile( file.filePath() );
	if( !newFile.open( QIODevice::ReadOnly | QIODevice::Text) ) {
		
		std::cout << "could not open file... " << std::endl;
		editor->setReadOnly( true );
		return false;
	}

	// load file to editor:
	// TODO: for now IO is done on same thread. We need a worker thread...
	//ILoader *loader = editor->createLoader(  )
	
	QTextStream in( &newFile );
	//QString line = in.readLine();
	while( !in.atEnd() ) {
		
		QString buffer = in.read( 100 );
		
		// process line:
		editor->addText( buffer.length(), buffer.toAscii() );
		//editor->addStyledText( buffer.length(), buffer.toAscii() );
	}
	
	editor->setSavePoint();
	
	newFile.close();
	
	return true;
}

/**
 *
 *
 */
bool Editor::saveFile( ) {
	
	QFile file( filePath.filePath() );
	if( !file.open( QIODevice::WriteOnly | QIODevice::Text) ) {
		
		std::cout << "Could not open file for saving..." << std::endl;
		editor->setReadOnly( true );
		return false;
	}
	
	editor->setReadOnly( true );
	int length = editor->get_doc()->length();
	QByteArray data = editor->getText( length + 1 );
	
	QTextStream out( &file );
	out << data;
	
	//std::cout << "Text at 0 " << data.data() << std::endl;
	
	editor->setSavePoint();
	editor->setReadOnly( false );
	
	return true;
}

/**
 *
 *
 */
void Editor::setFile( QString newName ) {
	
	if( newName.isEmpty() ) {
		return;
	}
	
	filePath = QFileInfo( newName );
}

/**
 *
 *
 */
void Editor::setFile( QFileInfo newFilePath ) {
	
	filePath = newFilePath;
}

/**
 *
 *
 */
QString Editor::getFileName( ) {
	
	return filePath.fileName();
}

/**
 *
 *
 */
QString Editor::getFilePath( ) {
	
	return filePath.filePath();
}

/**
 *
 *
 */
QPair<int, int> Editor::findNext( QString searchString ) {
	
	int docLength = editor->get_doc()->length();
	
	lastFound = editor->find_text( 0, searchString.toStdString().data(), lastFound.second, docLength );
	
	if( lastFound.first != -1 ) {
		
		editor->setSelectionStart( lastFound.first );
		editor->setSelectionEnd( lastFound.second );
		
	} else {
		
		// allow the find to loop:
		lastFound.second = 0;
	}
	
	return lastFound;
}

/**
 *
 *
 */
QPair<int, int> Editor::findPrevious( QString searchString ) {
	
	lastFound = editor->find_text( 0, searchString.toStdString().data(), lastFound.first, 0 );
	
	if( lastFound.first != -1 ) {
		
		editor->setSelectionStart( lastFound.first );
		editor->setSelectionEnd( lastFound.second );
	} else {
		
		// allow the find to loop:
		lastFound.first = editor->get_doc()->length();
		return QPair<int, int>( -1, 0);
	}
	
	return lastFound;
}

/**
 *
 *
 */
QPair<int, int> Editor::replaceNext( QString search, QString replace ) {
	
	int docLength = editor->get_doc()->length();
	
	lastFound = editor->find_text( 0, search.toStdString().data(), lastFound.second, docLength );
	
	if( lastFound.first != -1 ) {
		
		editor->setTargetStart( lastFound.first );
		editor->setTargetEnd( lastFound.second );
		editor->replaceTarget( replace.length(), replace.toStdString().data() );
		
		editor->setSelectionStart( lastFound.first );
		editor->setSelectionEnd( lastFound.first + replace.length() );
	} else {
		
		// allow for looping:
		lastFound.second = 0;
	}
	
	return lastFound;
}

/**
 *
 *
 */
ScintillaEditPtr Editor::getScintillaEdit( ) {

	return editor;
}