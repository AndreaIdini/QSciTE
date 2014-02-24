#pragma once

#ifndef QSCITE_MAINWINDOW_HPP
#define QSCITE_MAINWINDOW_HPP

#include <boost/shared_ptr.hpp>
#include <QtGui>
#include <ui_QSciTE.h>

#include "EditorManager.hpp"


typedef enum { LEFT_SPLITTER, RIGHT_SPLITTER } splitter;
typedef enum { SINGLE, DOUBLE_H, DOUBLE_V } splitterConfig;


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

	void setSingleSplitter( );
	void setDoubleHSplitter( );
	void setDoubleVSplitter( );
	
protected slots:
	
	void editorModified( bool );
	
protected:
	
	void setupLexing( EditorPtr );
	void setupCPPLexing( EditorPtr );
	void setupFortranLexing( EditorPtr );
	
	void closeEvent( QCloseEvent *event );
	void dragEnterEvent( QDragEnterEvent *event );
	void dropEvent( QDropEvent *event );
	
	void moveTab( QWidget *toMove, QTabWidget *from, QTabWidget *to );
	splitter getCurrentSplitter( );
	
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
	
	splitterConfig currentSplitterConf;
	
	// Menus:
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *findMenu;
	QMenu *viewMenu;
	
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
	
	QAction *singleSplitterAction;
	QAction *doubleHorizontalSplitterAction;
	QAction *doubleVerticalSplitterAction;
	
};

typedef boost::shared_ptr<MainWindow> MainWindowPtr;

#endif