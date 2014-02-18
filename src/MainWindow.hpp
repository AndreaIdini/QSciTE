#pragma once

#ifndef QSCITE_MAINWINDOW_HPP
#define QSCITE_MAINWINDOW_HPP

#include <boost/shared_ptr.hpp>

#include <QtGui>

#include <ui_QSciTE.h>

#include <ScintillaEdit.h>

#include "BufferManager.hpp"


class MainWindow : public QMainWindow {

	Q_OBJECT
	
signals:
	
	
public:
	
	MainWindow( QApplication &app );
	virtual ~MainWindow( );
	
	void initialize( );
	
public slots:
	
	void newFile( );
	void closeFile( int );

protected slots:
	
protected:
	
private slots:
	
private:
	
	
	// --== methods ==--
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
	boost::shared_ptr< ScintillaEdit > sciEditor;
	
	BufferManagerPtr bufferManager;
	
	bool windowModified;
	QString currentFileName;
	
	QByteArray savedGeometry;
	
	// Menus:
	QMenu *fileMenu;
	
	// ToolBars:
	QToolBar *fileToolBar;
	
	// Actions:
	QAction *newFileAction;
	QAction *openAction;
	
};

typedef boost::shared_ptr<MainWindow> MainWindowPtr;

#endif