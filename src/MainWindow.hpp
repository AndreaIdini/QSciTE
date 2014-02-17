#pragma once

#ifndef QSCITE_MAINWINDOW_HPP
#define QSCITE_MAINWINDOW_HPP

#include <boost/shared_ptr.hpp>

#include <QtGui>

#include <ui_QSciTE.h>


class MainWindow : public QMainWindow {

	Q_OBJECT
	
signals:
	
	
public:
	
	MainWindow( QApplication &app );
	virtual ~MainWindow( );
	
	void initialize( );
	
public slots:

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
	
	bool windowModified;
	QString currentFileName;
	
	QByteArray savedGeometry;
	
	// Menus:
	QMenu *fileMenu;
	
	// ToolBars:
	QToolBar *fileToolBar;
	
	// Actions:
	QAction *openAction;
	
};

typedef boost::shared_ptr<MainWindow> MainWindowPtr;

#endif