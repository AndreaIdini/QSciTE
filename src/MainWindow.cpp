#include "MainWindow.hpp"

#include <SciLexer.h>


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
	
	editorManager = EditorManagerPtr( new EditorManager( ) );
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
	
	EditorPtr editor = editorManager->createEditor( );
	
	if( editor ) {
		
		ScintillaEditPtr editorWidget = editor->getScintillaEdit( );
		
		setupEditor( editorWidget );
		registerEditorListeners( editorWidget );
				
		// create new tab entry:
		ui.tabBar->addTab( editorWidget.get(), tr("untitled") );
	}
}

/**
 * MainWindow::openFile
 * Opend existing file
 * @author: jhenriques 2014
 */
void MainWindow::openFile( QString fileName ) {
	
	// show open file dialog:
	if( fileName.isEmpty() )
		fileName = QFileDialog::getOpenFileName( this, tr("Open file"), settings.value( "path/load", "." ).toString() );
	
	if( !fileName.isEmpty() ) {
		
		// detect file extension:
		int language = SCLEX_NULL;
		if( fileName.endsWith( ".cpp", Qt::CaseInsensitive ) || fileName.endsWith( ".hpp", Qt::CaseInsensitive ) ) {
			language = SCLEX_CPP;
		}
	
		EditorPtr editor = editorManager->createEditor( );
		
		if( editor ) {
			
			ScintillaEditPtr editorWidget = editor->getScintillaEdit( );
			setupEditor( editorWidget );
			registerEditorListeners( editorWidget );
			
			editorWidget->setLexer( language );
			editorWidget->clearDocumentStyle();
			
			editor->loadFile( fileName );
			
			editorWidget->startStyling( 0, 0x1f );
			
			// create new tab entry:
			int index = ui.tabBar->addTab( editorWidget.get(), editor->getFileName() );
			ui.tabBar->setCurrentIndex( index );
		}
	}
}

/**
 * MainWindow::closeFile
 * closes file at given index;
 * @author: jhenriques 2014
 */
void MainWindow::closeFile( int index ) {
	
	std::cout << "closing file " << index << std::endl;
	
//	// get the editor to remove:
//	ScintillaEdit *toRemove = (ScintillaEdit *)ui.tabBar->widget( index );
//	
//	// check if editor needs saving:
//	toRemove->setReadOnly( 1 );
//	
//	for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
//		if( toRemove == editors[i] ) {
//			editors.erase( editors.begin() + i );
//			break;
//		}
//	}
	
	// Finally remove tab:
	ui.tabBar->removeTab( index );
}

/**
 * MainWindow::setupEditor
 * Setups default options for the editor.
 * @author: jhenriques 2014
 */
void MainWindow::setupEditor( ScintillaEditPtr editor ) {

	// Margins:
	editor->setMarginTypeN( 1, 1 );
	editor->setMarginWidthN( 1, 20 );
}

/**
 * MainWindow::registerEditorListeners
 * Registers to listen to ScintillaEdit widget important signals;
 * @author: jhenriques 2014
 */
void MainWindow::registerEditorListeners( ScintillaEditPtr editor ) {
	
	connect( editor.get(), SIGNAL(savePointChanged(bool)), this, SLOT(editorModified(bool)) );
	//connect( editor.get(), SIGNAL(styleNeeded(int)), this, SLOT(styleNeeded(int) ) );
}


/**
 * MainWindow::editorModified
 * Called when scintilla signals a dirty flag on editor.
 * @author: jhenriques 2014
 */
void MainWindow::editorModified( bool dirty ) {
	
	ScintillaEdit *modifiedEditor = (ScintillaEdit *) QObject::sender();
	
	EditorPtr editor = editorManager->getEditor( modifiedEditor );
	if( editor ) {
		
		editor->isDirty = dirty;
		
		int index = ui.tabBar->indexOf( modifiedEditor );
		if( dirty ) {
			ui.tabBar->setTabText( index, editor->getFileName() + "*" );
		} else {
			ui.tabBar->setTabText( index, editor->getFileName() );
		}
	}
}
			
//void MainWindow::styleNeeded( int position ) {
//	
//	ScintillaEdit *modifiedEditor = (ScintillaEdit *) QObject::sender();
//	
//	std::cout << "style needed for position " << position << std::endl;
//}


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
	openFileAction = new QAction( "&Open File", this );
	openFileAction->setIcon( QIcon(":/icons/open.png") );
	openFileAction->setShortcut( QKeySequence::Open );
	openFileAction->setStatusTip( tr("Open file") );
	connect( openFileAction, SIGNAL(triggered()), this, SLOT(openFile()) );
	
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
	fileMenu->addAction( openFileAction );
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
	fileToolBar->addAction( openFileAction );
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
