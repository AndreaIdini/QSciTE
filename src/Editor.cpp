#include "Editor.hpp"

#include <QTextStream>
#include <boost/lexical_cast.hpp>


Editor::Editor( ScintillaEditPtr editor ) : editor(editor), isDirty(false) {

	//editor->setViewEOL( true );
}


Editor::~Editor( ) {
	
}


bool Editor::loadFile( std::string file ) {
	return loadFile( QFileInfo( QString::fromStdString( file ) ) );
}

bool Editor::loadFile( QFileInfo file ) {
	
	if( !file.exists() ) {
		
		// handle case when file does not exists and empty file name... New empty buffer?
		if( file.fileName() == "" ) {
			
			QFileInfo newFile;
			int index = 1;
			while( 1 ) {
				newFile.setFile( "./untitled" + QString::fromStdString( boost::lexical_cast<std::string>( index ) ) + ".txt" );
				if( !newFile.exists() ) {
					break;
				} else {
					index++;
				}
			}
			
			std::cout << "Creating new file: " << newFile.fileName().toStdString() << std::endl;
			file = newFile;
		}
	}
	
	filePath = file;

	// file is unique and can be open/created:
	QFile newFile( file.filePath() );
	if( !newFile.open( QIODevice::ReadWrite | QIODevice::Text) ) {
		
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
	
	return true;
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
ScintillaEditPtr Editor::getScintillaEdit( ) {

	return editor;
}