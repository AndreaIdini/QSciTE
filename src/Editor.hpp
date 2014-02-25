#pragma once

#ifndef QSCITE_EDITOR_HPP
#define QSCITE_EDITOR_HPP

#include <boost/shared_ptr.hpp>
#include <ScintillaEdit.h>

#include <QFileInfo>


/**
 * class Editor
 * Wrapper for a ScintillaEdit qt widget.
 * We will have a 1:1 between open file and editor.
 *
 * author: jhenriques 2014
 */
class Editor {
	
private:

	ScintillaEdit *editor;
	QString filePath;
	
	QPair<int, int> lastFound;
	
protected:
	
	void tagAllMatches( QString );
	
public:
	
	bool isDirty;
	
public:
	
	Editor( ScintillaEdit * );
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
	void clearSearchIndicators( );
	
	ScintillaEdit * getScintillaEdit( );
	void setScintillaEdit( ScintillaEdit * );
	
};

typedef boost::shared_ptr< Editor > EditorPtr;

#endif