#include "MainWindow.hpp"


MainWindow::MainWindow( QApplication &app ) : QMainWindow(), windowModified(false) {
	
	// read ui:
	ui.setupUi( this );
	
	// create and set the scintilla editor as main widget:
//	sciEditor = boost::shared_ptr< ScintillaEdit >( new ScintillaEdit( this ) );
//	sciEditor->hide();
	
	// refine ui:
	ui.tabBar->clear();
	connect( ui.tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)) );
	
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
	
//	// TODO
//	
//	ScintillaDocument *pDoc = sciEditor->get_doc();
//
//	sciEditor->addText( 14, "Jose Henriques" );
//
//	//sciEditor->setLexer( SCLEX_CPP );
//	
//	QByteArray buffer = sciEditor->getText( pDoc->length() );
//	std::cout << "Current Text: " << QString( buffer ).toStdString() << std::endl;
//	
//	
//	//create a new document:
////	ScintillaDocument *pDoc2 = new ScintillaDocument( );
////	sciEditor->setDocPointer( pDoc2 );
//	
//	sptr_t newDoc = sciEditor->createDocument( );
//	sciEditor->setDocPointer( newDoc );
//	
//	ScintillaDocument *pDoc2 = sciEditor->get_doc();
//	
//	sciEditor->addText( 6, "Amaral" );
//	buffer = sciEditor->getText( pDoc2->length( ) );
//	std::cout << "Current Text 2: " << QString( buffer ).toStdString() << std::endl;

}

/**
 * MainWindow::newFile
 * Creates a new empty buffer;
 * @author: jhenriques 2014
 */
void MainWindow::newFile( ) {
	
	std::cout << "creating new file..." << std::endl;
	
	// create new tab entry:
	ui.tabBar->addTab( new ScintillaEdit( this ), tr("untitled") );
	
	
	
//	BufferPtr newBuffer = BufferPtr( new Buffer( ) );
//	
//	bufferManager->add( newBuffer );
//	bufferManager->setCurrent( newBuffer );
}

/**
 * MainWindow::closeFile
 * closes file at given index;
 * @author: jhenriques 2014
 */
void MainWindow::closeFile( int index ) {
	
	std::cout << "closing file " << index << std::endl;
	
	ui.tabBar->removeTab( index );
}

/**
 * MainWindow::createActions
 * Creates extra Qt Actions not created in Designer.
 * @author: jhenriques 2014
 */
void MainWindow::createActions() {
	
	// New File Action:
	newFileAction = new QAction( "&New", this );
	newFileAction->setIcon( QIcon( ":/icons/new.png" ) );
	newFileAction->setShortcut( QKeySequence::New );
	newFileAction->setStatusTip( tr("New") );
	connect( newFileAction, SIGNAL(triggered()), this, SLOT(newFile()) );
	
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
	fileMenu->addAction( newFileAction );
	fileMenu->addAction( openAction );
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
	fileToolBar->addAction( newFileAction );
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
