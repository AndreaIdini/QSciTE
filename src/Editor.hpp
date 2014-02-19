#pragma once

#ifndef QSCITE_EDITOR_HPP
#define QSCITE_EDITOR_HPP

#include <boost/shared_ptr.hpp>
#include <ScintillaEdit.h>

#include <QFileInfo>


typedef boost::shared_ptr< ScintillaEdit > ScintillaEditPtr;


/**
 * class Editor
 * Wrapper for a ScintillaEdit qt widget.
 * We will have a 1:1 between open file and editor.
 *
 * author: jhenriques 2014
 */
class Editor {
	
private:

	ScintillaEditPtr editor;
	QFileInfo filePath;

public:
	
	bool isDirty;
	
public:
	
	Editor( ScintillaEditPtr );
	virtual ~Editor( );
	
	bool loadFile( std::string );
	bool loadFile( QFileInfo );
	
	QString getFileName( );
	
	ScintillaEditPtr getScintillaEdit( );
	
};

typedef boost::shared_ptr< Editor > EditorPtr;


//// From Scintilla
//class ILoader {
//public:
//	virtual int SCI_METHOD Release() = 0;
//	// Returns a status code from SC_STATUS_*
//	virtual int SCI_METHOD AddData(char *data, int length) = 0;
//	virtual void * SCI_METHOD ConvertToDocument() = 0;
//};

#endif