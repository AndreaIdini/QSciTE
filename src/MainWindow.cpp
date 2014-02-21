#include "MainWindow.hpp"

#include <iostream>
#include <SciLexer.h>


// TEMP:::

// C++ keywords
static const char g_cppKeyWords[] =		// Standard
										"asm auto bool break case catch char class const "
										"const_cast continue default delete do double "
										"dynamic_cast else enum explicit extern false finally "
										"float for friend goto if inline int long mutable "
										"namespace new operator private protected public "
										"register reinterpret_cast register return short signed "
										"sizeof static static_cast struct switch template "
										"this throw true try typedef typeid typename "
										"union unsigned using virtual void volatile "
										"wchar_t while "

										// Extended
										"__asm __asume __based __box __cdecl __declspec "
										"__delegate delegate depreciated dllexport dllimport "
										"event __event __except __fastcall __finally __forceinline "
										"__int8 __int16 __int32 __int64 __int128 __interface "
										"interface __leave naked noinline __noop noreturn "
										"nothrow novtable nullptr safecast __stdcall "
										"__try __except __finally __unaligned uuid __uuidof "
										"__virtual_inheritance";


MainWindow::MainWindow( QApplication &app ) : QMainWindow(), windowModified(false) {
	
	// read ui:
	ui.setupUi( this );
	
	// create and set the scintilla editor as main widget:
//	sciEditor = boost::shared_ptr< ScintillaEdit >( new ScintillaEdit( this ) );
//	sciEditor->hide();
	
	// refine ui:
	ui.tabBar->clear();
	connect( ui.tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)) );
	
	ui.searchFrame->hide();
	connect( ui.nextButton, SIGNAL(clicked()), this, SLOT(findNext()) );
	connect( ui.previousButton, SIGNAL(clicked()), this, SLOT(findPrevious()) );
	
	ui.replaceFrame->hide();
	connect( ui.replaceNextButton, SIGNAL(clicked()), this, SLOT(findNext()) );
	connect( ui.replaceButton, SIGNAL(clicked()), this, SLOT(replaceNext()) );
	
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
		int index = ui.tabBar->addTab( editorWidget.get(), tr("untitled") );
		ui.tabBar->setCurrentIndex( index );
		editorWidget->grabFocus();
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
	
		// must check if we already have editor open for this file:
		EditorPtr existingEditor = editorManager->getEditorForFile( fileName );
		if( existingEditor ) {
			
			// move focus to editor:
			ScintillaEditPtr widget = editorManager->getWidget( existingEditor );
			if( widget ) {
				ui.tabBar->setCurrentWidget( widget.get( ) );
				widget->grabFocus();
				return;
			} else {
				// TODO: check if possible to get here...
				std::cout << "Could not get tab widget for open editor for : " << fileName.toStdString() << std::endl;
			}
		}
		
		EditorPtr editor = editorManager->createEditor( );
		
		if( editor ) {
			
			ScintillaEditPtr editorWidget = editor->getScintillaEdit( );
			setupEditor( editorWidget );
			registerEditorListeners( editorWidget );
			
//			editorWidget->setLexer( language );
//			editorWidget->clearDocumentStyle();
			
//			editorWidget->setLexer( SCLEX_CPP );
//			editorWidget->setLexerLanguage( "cpp" );
//			editorWidget->setProperty("lexer.cpp.track.preprocessor", "1" );
//			editorWidget->setProperty("lexer.cpp.update.preprocessor", "1" );
//			
//			editorWidget->setKeyWords( 0, "case if else" );
//			editorWidget->styleSetFore( SCE_C_WORD, 100 | (0 << 8) | (0 << 16) );
//			editorWidget->styleSetFore( SCE_C_COMMENT, 100 | (0 << 8) | (0 << 16) );
//			
//			editorWidget->colourise( 0, -1 );
//			
//			editorWidget->gotoPos( 0 );
			
			
			
			// CPP lexer
			//SendEditor( SCI_SETLEXER, SCLEX_CPP );
			editorWidget->setLexer( SCLEX_CPP );
			
			// Set number of style bits to use
			//SendEditor( SCI_SETSTYLEBITS, 5 );
			editorWidget->setStyleBits( 5 );
			
			// Set tab width
			//SendEditor( SCI_SETTABWIDTH, 4 );
			editorWidget->setTabWidth( 3 );
			
			// Use CPP keywords
			//SendEditor( SCI_SETKEYWORDS, 0, (LPARAM)g_cppKeyWords );
			editorWidget->setKeyWords( 0, g_cppKeyWords );
			
			// Set up the global default style. These attributes
			// are used wherever no explicit choices are made.
			//SetAStyle( STYLE_DEFAULT, white, black, 10, "Courier New" );
			editorWidget->styleSetFont( STYLE_DEFAULT, "Courier New" );
			editorWidget->styleSetSize( STYLE_DEFAULT, 10 );
			editorWidget->styleSetFore( STYLE_DEFAULT, createColor( 100, 100, 100 ) );
			editorWidget->styleSetBack( STYLE_DEFAULT, createColor( 255, 255, 255 ) );
			
			// Set caret foreground color
			//SendEditor( SCI_SETCARETFORE, RGB( 255, 255, 255 ) );
			editorWidget->setCaretFore( createColor( 100, 100, 100 ) );
			
			// Set all styles
			//SendEditor( SCI_STYLECLEARALL );
			editorWidget->styleClearAll();
			
			// Set selection color
			//SendEditor( SCI_SETSELBACK, TRUE, RGB( 0, 0, 255 ) );
			editorWidget->setSelBack( 1, createColor( 10, 10, 100 ) );
			
			// Set syntax colors
			int red = createColor( 200, 10, 10 );
			int green = createColor( 10, 200, 10 );
			int yellow = createColor( 200, 200, 10 );
			int magenta = createColor( 200, 10, 200 );
			int cyan = createColor( 10, 200, 200 );
			editorWidget->styleSetFore( SCE_C_COMMENT,		green );
			editorWidget->styleSetFore( SCE_C_COMMENTLINE,	green );
			editorWidget->styleSetFore( SCE_C_COMMENTDOC,	green );
			editorWidget->styleSetFore( SCE_C_NUMBER,		magenta );
			editorWidget->styleSetFore( SCE_C_STRING,		yellow );
			editorWidget->styleSetFore( SCE_C_CHARACTER,	yellow );
			editorWidget->styleSetFore( SCE_C_UUID,			cyan );
			editorWidget->styleSetFore( SCE_C_OPERATOR,		red );
			editorWidget->styleSetFore( SCE_C_PREPROCESSOR,	cyan );
			editorWidget->styleSetFore( SCE_C_WORD,			cyan );
			
			
			
			editor->loadFile( fileName );
			
			
//			SCE_C_DEFAULT=0
//			SCE_C_COMMENT=1
//			SCE_C_COMMENTLINE=2
//			SCE_C_COMMENTDOC=3
//			SCE_C_NUMBER=4
//			SCE_C_WORD=5
//			SCE_C_STRING=6
//			SCE_C_CHARACTER=7
//			SCE_C_UUID=8
//			SCE_C_PREPROCESSOR=9
//			SCE_C_OPERATOR=10
//			SCE_C_IDENTIFIER=11
//			SCE_C_STRINGEOL=12
//			SCE_C_VERBATIM=13
//			SCE_C_REGEX=14
//			SCE_C_COMMENTLINEDOC=15
//			SCE_C_WORD2=16
//			SCE_C_COMMENTDOCKEYWORD=17
//			SCE_C_COMMENTDOCKEYWORDERROR=18
//			SCE_C_GLOBALCLASS=19
//			SCE_C_STRINGRAW=20
//			SCE_C_TRIPLEVERBATIM=21
//			SCE_C_HASHQUOTEDSTRING=22
//			SCE_C_PREPROCESSORCOMMENT=23
//			SCE_C_PREPROCESSORCOMMENTDOC=24
//			SCE_C_USERLITERAL=25
			
			
			std::cout << "Prop: " << QString( editorWidget->propertyNames() ).toStdString() << std::endl;
			std::cout << "LExer: " << editorWidget->lexer() << std::endl;
			
			// create new tab entry:
			int index = ui.tabBar->addTab( editorWidget.get(), editor->getFileName() );
			ui.tabBar->setCurrentIndex( index );
			editorWidget->activateWindow();
			editorWidget->grabFocus();
		}
	}
}

/**
 * MainWindow::saveFile
 * This slot can be called directly with no argument or with
 * a editor argument. If no argument, the current tab editor will be picked.
 * @author: jhenriques 2014
 */
void MainWindow::saveFile( EditorPtr editor ) {
	
	// If called with no editor get the current activate editor:
	if( !editor ) {
		// get current editor:
		editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	}
	
	if( editor ) {
		
		if( editor->getFileName() == "" ) {
			return saveAsFile( editor );
		}
		
		editor->saveFile( );
	}
}

/**
 * MainWindow::saveAsFile
 * This slot can be called directly with no argument or with
 * a editor argument. If no argument, the current tab editor will be picked.
 * @author: jhenriques 2014
 */
void MainWindow::saveAsFile( EditorPtr editor ) {

	// If called with no editor get the current activate editor:
	if( !editor ) {
		// get current editor:
		editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	}
	
	if( editor ) {
		
		const QString saveDir = settings.value( "path/save", "." ).toString();
		const QString title( tr("Save untitled buffer as...") );
				
		QString filename = QFileDialog::getSaveFileName( 0, title, saveDir, "", 0, QFileDialog::DontUseSheet );

		if( !filename.isEmpty() ) {
			
			// Store last used directory for ease of use:
			QDir directory( filename );
			directory.cdUp();
			settings.setValue( "path/save", directory.path() );
			
			editor->setFileName( filename );
			editor->saveFile();
		} else {
			
			int res = QMessageBox::warning( this, tr("QSciTE"), tr("WARNING: Changes in untitled buffer will be lost!\nContinue?"),
										   QMessageBox::Yes | QMessageBox::No );
			if( res == QMessageBox::No ) {
				return saveAsFile( editor );
			}
		}
	}
}

/**
 * MainWindow::closeFile
 * closes file at given index;
 * @author: jhenriques 2014
 */
void MainWindow::closeFile( int index ) {
	
	// get current editor:
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	
	if( editor->isDirty ) {
		int res = QMessageBox::warning( this, tr("QSciTE"), tr("This session has been modified.\n Do you want to save your changes?"),
										   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
		if( res == QMessageBox::Yes ) {
			saveFile();
		}
	}
	
	// Finally remove tab:
	ui.tabBar->removeTab( index );

	editorManager->removeEditor( editor );
}

/**
 * MainWindow::find
 * shows the find dialogs and sets editor for the search.
 * @author: jhenriques 2014
 */
void MainWindow::find( ) {
	
	if( ui.replaceFrame->isVisible() ) {
		ui.replaceFrame->hide();
		
		// copy text between find fields:
		if( !ui.replaceSearchField->text().isEmpty() ) {
			ui.searchField->setText( ui.replaceSearchField->text() );
		}
	}
	
	if( ui.searchFrame->isVisible() ) {
		ui.searchFrame->hide();
	} else {
		ui.searchFrame->show();
		ui.searchField->activateWindow();
		ui.searchField->setFocus();
		ui.searchField->grabKeyboard();
		ui.searchField->selectAll();
	}
}

/**
 * MainWindow::findNext
 *
 * @author: jhenriques 2014
 */
void MainWindow::findNext( ) {
	
	QString searchString = ui.searchField->text();
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	QPair<int, int> found = editor->findNext( searchString );
	
	//std::cout << "Find Next: " << searchString.toStdString() << ". <" << found.first << ", " << found.second << ">" << std::endl;
}

/**
 * MainWindow::findNext
 *
 * @author: jhenriques 2014
 */
void MainWindow::findPrevious( ) {

	QString searchString = ui.searchField->text();
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	QPair<int, int> found = editor->findPrevious( searchString );
	
	//std::cout << "Find Previous: " << searchString.toStdString() << ". <" << found.first << ", " << found.second << ">" << std::endl;
}

/**
 * MainWindow::replace
 *
 * @author: jhenriques 2014
 */
void MainWindow::replace( ) {

	if( ui.searchFrame->isVisible() ) {
		ui.searchFrame->hide();
		
		// copy text between find fields:
		if( !ui.searchField->text().isEmpty() ) {
			ui.replaceSearchField->setText( ui.searchField->text() );
		}
	}
	
	if( ui.replaceFrame->isVisible() ) {
		ui.replaceFrame->hide();
	} else {
		ui.replaceFrame->show();
		ui.replaceSearchField->activateWindow();
		ui.replaceSearchField->setFocus();
		ui.replaceSearchField->grabKeyboard();
		ui.replaceSearchField->selectAll();
	}
}

/**
 * MainWindow::replaceNext
 *
 * @author: jhenriques 2014
 */
void MainWindow::replaceNext( ) {

	QString searchString = ui.replaceSearchField->text();
	QString replaceString = ui.replaceField->text();
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBar->currentWidget() );
	editor->replaceNext( searchString, replaceString );
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
 * MainWindow::registerMainWindowActions()
 * Register all available action. Don't forget to register new Actions!
 * @author: jhenriques 2014
 */
void MainWindow::registerMainWindowActions() {

	addAction( newFileAction );
	addAction( openFileAction );
	addAction( saveFileAction );
	addAction( saveAsFileAction );
	
	addAction( findAction );
	addAction( findNextAction );
	addAction( findPreviousAction );
	addAction( replaceAction );
	addAction( replaceNextAction );
	
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
		
		QString tabLabel = ( editor->getFileName() == "" ? tr("untitled") : editor->getFileName() );
		
		int index = ui.tabBar->indexOf( modifiedEditor );
		if( dirty ) {
			ui.tabBar->setTabText( index, tabLabel + "*" );
		} else {
			ui.tabBar->setTabText( index, tabLabel );
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
 * MainWindow::closeEvent
 * Overloaded from QWidget in order to intercept closing Editor.
 * @author: jhenriques 2014
 */
void MainWindow::closeEvent( QCloseEvent *event ) {
	
	if( okToContinue() ) {
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

/**
 * MainWindow::dragEnterEvent
 * Overloaded from QWidget
 * @author: jhenriques, DFKI 2010
 */
void MainWindow::dragEnterEvent( QDragEnterEvent *event ) {
	
	if( event->mimeData()->hasFormat( "text/uri-list" ) || event->mimeData()->hasFormat( "text/x-cgsrc" ) )
		event->acceptProposedAction();
}

/**
 * MainWindow::dropEvent
 * Overloaded from QWidget
 * @author: jhenriques, DFKI 2010
 */
void MainWindow::dropEvent( QDropEvent *event ) {
	
	QList<QUrl> urls = event->mimeData()->urls();
	
	if( urls.isEmpty() ) {
		event->ignore();
		return;
	}

	for( int i = 0, iCount = urls.count(); i < iCount; ++i ) {
	
		QFileInfo fileInfo( urls[i].toLocalFile() );
		if( fileInfo.fileName().isEmpty() ) {
			continue;
		}
		
		openFile( fileInfo.filePath() );
	}
	
	event->accept();
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
	openFileAction = new QAction( "&Open", this );
	openFileAction->setIcon( QIcon(":/icons/open.png") );
	openFileAction->setShortcut( QKeySequence::Open );
	openFileAction->setStatusTip( tr("Open file") );
	connect( openFileAction, SIGNAL(triggered()), this, SLOT(openFile()) );
	
	// Save Action:
	saveFileAction = new QAction( "&Save", this );
	saveFileAction->setIcon( QIcon(":/icons/save.png") );
	saveFileAction->setShortcut( QKeySequence::Save );
	saveFileAction->setStatusTip( tr("Save file") );
	connect( saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()) );
	
	// Save As Action:
	saveAsFileAction = new QAction( "&Save As", this );
	saveAsFileAction->setIcon( QIcon(":/icons/save_as.png") );
	saveAsFileAction->setShortcut( QKeySequence::SaveAs );
	saveAsFileAction->setStatusTip( tr("Save file as...") );
	connect( saveAsFileAction, SIGNAL(triggered()), this, SLOT(saveAsFile()) );
	
	
	// Find Action:
	findAction = new QAction( "&Find...", this );
	findAction->setIcon( QIcon(":/icons/find.png") );
	findAction->setShortcut( QKeySequence::Find );
	findAction->setStatusTip( tr("Find...") );
	connect( findAction, SIGNAL(triggered()), this, SLOT(find()) );
	
	findNextAction = new QAction( "&Find Next", this );
	//findNextAction->setIcon( QIcon(":/icons/find.png") );
	findNextAction->setShortcut( QKeySequence::FindNext );
	findNextAction->setStatusTip( tr("Find Next") );
	connect( findNextAction, SIGNAL(triggered()), this, SLOT(findNext()) );
	
	findPreviousAction = new QAction( "&Find Previous", this );
	//findNextAction->setIcon( QIcon(":/icons/find.png") );
	findPreviousAction->setShortcut( QKeySequence::FindPrevious );
	findPreviousAction->setStatusTip( tr("Find Previous") );
	connect( findPreviousAction, SIGNAL(triggered()), this, SLOT(findPrevious()) );
		
	replaceAction = new QAction( "&Replace...", this );
	//replaceAction->setIcon( QIcon(":/icons/find.png") );
	replaceAction->setShortcut( tr("Shift+Ctrl+F") );
	replaceAction->setStatusTip( tr("Replace...") );
	connect( replaceAction, SIGNAL(triggered()), this, SLOT(replace()) );
	
	replaceNextAction = new QAction( "&Replace Next", this );
	//replaceNextAction->setIcon( QIcon(":/icons/find.png") );
	replaceNextAction->setShortcut( QKeySequence::Replace );
	replaceNextAction->setStatusTip( tr("Replace next...") );
	connect( replaceNextAction, SIGNAL(triggered()), this, SLOT(replaceNext()) );

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
	fileMenu->addAction( saveFileAction );
	fileMenu->addAction( saveAsFileAction );
//	fileMenu->addSeparator();
//	fileMenu->addAction( closeAction );
	
	// Find menu:
	findMenu = menuBar()->addMenu( tr("Fin&d") );
	findMenu->addAction( findAction );
	findMenu->addAction( findNextAction );
	findMenu->addAction( findPreviousAction );
	findMenu->addSeparator();
	findMenu->addAction( replaceAction );
	findMenu->addAction( replaceNextAction );
	
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
	fileToolBar->addAction( saveFileAction );
	fileToolBar->addAction( saveAsFileAction );
	
	// find toolbar:
	findToolBar = addToolBar( tr("Find" ) );
	findToolBar->addAction( findAction );
	findToolBar->addAction( findNextAction );
	findToolBar->addAction( findPreviousAction );
	findToolBar->addAction( replaceAction );
	findToolBar->addAction( replaceNextAction );
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
 *  This method checks if any file has been modified and if modified
 *  shows a dialog so the user can decide if to save modifications before proceding.
 * @author: jhenriques 2014
 */
bool MainWindow::okToContinue() {

	if( editorManager->areEditorsDirty() ) {
			
		int res = QMessageBox::warning( this, tr("QSciTE"), tr("There are modified buffers.\n"
										   "Do you want to save your changes before quiting?"),
									   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
		if( res == QMessageBox::Yes ) {
			
			// save modified buffers:
			const std::vector< EditorPtr > &editors = editorManager->getEditors();
			for( int i = 0, iCount = editors.size(); i < iCount; ++i ) {
				if( editors[i]->isDirty ) {
					saveFile( editors[i] );
				}
			}
			
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

/**
 * MainWindow::createColor
 * @author: jhenriques 2014
 */
int MainWindow::createColor( int r, int g, int b ) {
	return r | (g << 8) | (b << 16);
}
