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
	
	QPair<int, int> lastFound;

public:
	
	bool isDirty;
	
public:
	
	Editor( ScintillaEditPtr );
	virtual ~Editor( );
	
	bool loadFile( QString );
	bool loadFile( QFileInfo );
	
	bool saveFile( );
	
	void setFile( QString );
	void setFile( QFileInfo );
	QString getFileName( );
	QString getFilePath( );
	
	// Find handling:
	QPair<int, int> findNext( QString );
	QPair<int, int> findPrevious( QString );
	QPair<int, int> replaceNext( QString, QString );
	
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