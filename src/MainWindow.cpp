#include "MainWindow.hpp"


MainWindow::MainWindow( QApplication &app ) : QMainWindow(), windowModified(false) {
	
	// read ui:
	ui.setupUi( this );
	
	// refine ui:
	
	// create ui elements:
	createActions( );
	createMenus( );
	createToolBars( );
	createStatusBar( );
	
	// read QSciTE settings:
	readSettings( );
	
	// accept drag & drops:
	setAcceptDrops( true );
	
	// on mac show unified toolbar:
	setUnifiedTitleAndToolBarOnMac( true );
}

/**
 *
 * @author: jhenriques 2014
 */
MainWindow::~MainWindow() {
}


/**
 * MainWindow::initialize
 *
 * @author: jhenriques 2014
 */
void MainWindow::initialize( ) {
	
	// TODO
}

/**
 * MainWindow::createActions
 * Creates extra Qt Actions not created in Designer.
 * @author: jhenriques 2014
 */
void MainWindow::createActions() {
	
	// Open Action:
	openAction = new QAction( "&Open File", this );
	openAction->setIcon( QIcon(":/icons/open.png") );
	openAction->setShortcut( QKeySequence::Open );
	openAction->setStatusTip( tr("Open file") );

	// TODO:
	//connect( openAction, SIGNAL(triggered()), this, SLOT(loadScene()) );
	
}

/**
 * MainWindow::createMenus
 * Creates all editor Menus.
 * @author: jhenriques 2014
 */
void MainWindow::createMenus() {
	
	// File menu:
	fileMenu = menuBar()->addMenu( tr("&File") );
//	fileMenu->addAction( openAction );
//	fileMenu->addAction( saveAction );
//	fileMenu->addAction( saveAsAction );
//	fileMenu->addSeparator();
//	fileMenu->addAction( closeAction );

}

/**
 * MainWindow::createToolBars
 * Creates all Editor ToolBars.
 * @author: jhenriques 2014
 */
void MainWindow::createToolBars() {
	
	// create file toolBar:
	fileToolBar = addToolBar( tr("&File") );
	fileToolBar->addAction( openAction );
//	fileToolBar->addAction( saveAction );
//	fileToolBar->addAction( saveAsAction );

}

/**
 * MainWindow::createStatusBar
 * Creates editor's status Bar. It is empty for now but will be used.
 * @author: jhenriques 2014
 */
void MainWindow::createStatusBar() {
	
}

/**
 * MainWindow::okToContinue
 *  This method checks if current file has been modified and if modified
 *  shows a dialog so the user can decide if to save modifications before proceding.
 * @author: jhenriques 2014
 */
bool MainWindow::okToContinue() {
	
	if( isWindowModified() ) {
		
		int res = QMessageBox::warning( this, tr("QSciTE"),
										tr("This session has been modified.\n"
										   "Do you want to save your changes?"),
									   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
		if( res == QMessageBox::Yes ) {
			
			// TODO: create saveFile();
			//return saveFile();
		} else if( res == QMessageBox::Cancel ) {
			return false;
		}
	}
	return true;
}

/**
 * MainWindow::writeSettings
 *  Stores the application settings using Qt mechanisms.
 * @author: jhenriques 2014
 */
void MainWindow::writeSettings() {
	
	settings.setValue( "window/geometry", saveGeometry() );
}

/**
 * MainWindow::readSettings
 *  Reads the application settings and applies them.
 * @author: jhenriques 2014
 */
void MainWindow::readSettings() {
	
	restoreGeometry( settings.value("window/geometry").toByteArray() );
}
