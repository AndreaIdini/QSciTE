#pragma once

#ifndef QSCITE_MAINWINDOW_HPP
#define QSCITE_MAINWINDOW_HPP

#include <boost/shared_ptr.hpp>
#include <QtGui>
#include <ui_QSciTE.h>

#include "EditorManager.hpp"


class MainWindow : public QMainWindow {

	Q_OBJECT
	
signals:
	
	
public:
	
	MainWindow( QApplication &app );
	virtual ~MainWindow( );
	
	void initialize( );
	
public slots:
	
	void newFile( );
	void openFile( QString fileName = "" );
	void saveFile( EditorPtr = EditorPtr() );
	void saveAsFile( EditorPtr = EditorPtr() );
	void closeFile( int );
	
	void undo( );
	void redo( );
	
	void find( );
	void findNext( );
	void findPrevious( );
	void replace( );
	void replaceNext( );

protected slots:
	
	void editorModified( bool );
	//void styleNeeded( int );
	
	
protected:
	
	void closeEvent( QCloseEvent *event );
	void dragEnterEvent( QDragEnterEvent *event );
	void dropEvent( QDropEvent *event );
	
private slots:
	
private:
	
	
	// --== methods ==--
	void setupEditor( ScintillaEditPtr );
	void registerEditorListeners( ScintillaEditPtr );
	void registerMainWindowActions();
	
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	
	bool okToContinue();
	void writeSettings();
	void readSettings();
	
	int createColor( int, int, int );
	
	
	// --== variables ==--
	QSettings settings;
	Ui::QSciTE ui;

	EditorManagerPtr editorManager;
	
	bool windowModified;
	QString currentFileName;
	
	QByteArray savedGeometry;
	
	// Menus:
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *findMenu;
	
	// ToolBars:
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *findToolBar;
	
	// Actions:
	QAction *newFileAction;
	QAction *openFileAction;
	QAction *saveFileAction;
	QAction *saveAsFileAction;
	
	QAction *undoAction;
	QAction *redoAction;
	
	QAction *findAction;
	QAction *findNextAction;
	QAction *findPreviousAction;
	
	QAction *replaceAction;
	QAction *replaceNextAction;
	
};

typedef boost::shared_ptr<MainWindow> MainWindowPtr;

#endif