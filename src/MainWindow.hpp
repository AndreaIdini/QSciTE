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
	void closeFile( int );

protected slots:
	
	void editorModified( bool );
	//void styleNeeded( int );
	
protected:
	
private slots:
	
private:
	
	
	// --== methods ==--
	void setupEditor( ScintillaEditPtr );
	void registerEditorListeners( ScintillaEditPtr );
	
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	
	bool okToContinue();
	void writeSettings();
	void readSettings();
	
	
	// --== variables ==--
	QSettings settings;
	Ui::QSciTE ui;

	EditorManagerPtr editorManager;
	
	bool windowModified;
	QString currentFileName;
	
	QByteArray savedGeometry;
	
	// Menus:
	QMenu *fileMenu;
	
	// ToolBars:
	QToolBar *fileToolBar;
	
	// Actions:
	QAction *newFileAction;
	QAction *openFileAction;
	
};

typedef boost::shared_ptr<MainWindow> MainWindowPtr;

#endif