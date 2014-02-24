#include "MainWindow.hpp"

#include <iostream>
#include <SciLexer.h>


MainWindow::MainWindow( QApplication &app ) : QMainWindow(), windowModified(false) {
	
	// read ui:
	ui.setupUi( this );
	
	// refine ui:
	
	// init tabBarLefts:
	ui.tabBarLeft->clear();
	connect( ui.tabBarLeft, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)) );
	
	// init tabBarRight:
	ui.tabBarRight->clear();
	connect( ui.tabBarRight, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)) );
	ui.tabBarRight->hide();
	
	currentSplitterConf = SINGLE;
	
	// Find/Replace config:
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
		
		// find out what is the active splitter if any. default is LEFT_SPLITTER:
		splitter active = getCurrentSplitter( );
		
		switch( active ) {
			case LEFT_SPLITTER: {
				
				int index = ui.tabBarLeft->addTab( editorWidget.get(), tr("untitled") );
				ui.tabBarLeft->setCurrentIndex( index );
				editorWidget->grabFocus();
				break;
			}
			case RIGHT_SPLITTER: {
				
				int index = ui.tabBarRight->addTab( editorWidget.get(), tr("untitled") );
				ui.tabBarRight->setCurrentIndex( index );
				editorWidget->grabFocus();
				break;
			}
		}
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
	
		// must check if we already have editor open for this file:
		EditorPtr existingEditor = editorManager->getEditorForFile( fileName );
		if( existingEditor ) {
			
			// we will move focus to open editor if they are on the same splitter
			// and if not, we will open a new editor and share the scintilla document:
			splitter active = getCurrentSplitter( );
			QTabWidget *currentTabBar = ( active == LEFT_SPLITTER ? ui.tabBarLeft : ui.tabBarRight );
			
			// move focus to editor:
			ScintillaEditPtr widget = editorManager->getWidget( existingEditor );
			if( widget ) {
				
				QTabWidget *tabBar = (QTabWidget *) widget->parentWidget()->parentWidget();
				
				// case when in the same splitter:
				if( tabBar == currentTabBar ) {
					tabBar->setCurrentWidget( widget.get() );
					widget->grabFocus();
					return;
				}
				
				// if we get here we need a new editor and to share scintilla document:
				EditorPtr editor = editorManager->createEditor( );
				
				ScintillaEditPtr editorWidget = editor->getScintillaEdit( );
				setupEditor( editorWidget );
				registerEditorListeners( editorWidget );
				
				editor->setFile( existingEditor->getFilePath() );
				editor->getScintillaEdit()->set_doc( widget->get_doc() );
				
				setupLexing( editor );
				
				int index = currentTabBar->addTab( editorWidget.get(), editor->getFileName() );
				currentTabBar->setCurrentIndex( index );
				editorWidget->grabFocus();
				return;
			
			} else {
				
				// TODO: check if possible to get here...
				std::cout << "Could not get tab widget for open editor for : " << fileName.toStdString() << std::endl;
			}
		}
		
		// If we get here the file is not open. Open and add:
		EditorPtr editor = editorManager->createEditor( );
		
		if( editor ) {
			
			ScintillaEditPtr editorWidget = editor->getScintillaEdit( );
			setupEditor( editorWidget );
			registerEditorListeners( editorWidget );
			
			editor->loadFile( fileName );
			editorWidget->gotoPos( 0 );
			editorWidget->emptyUndoBuffer();
			
			setupLexing( editor );
			
			// find out what is the active splitter if any. default is LEFT_SPLITTER:
			splitter active = getCurrentSplitter( );
			switch( active ) {
				case LEFT_SPLITTER: {
					
					int index = ui.tabBarLeft->addTab( editorWidget.get(), editor->getFileName() );
					ui.tabBarLeft->setCurrentIndex( index );
					editorWidget->grabFocus();
					break;
				}
				case RIGHT_SPLITTER: {
					
					int index = ui.tabBarRight->addTab( editorWidget.get(), editor->getFileName() );
					ui.tabBarRight->setCurrentIndex( index );
					editorWidget->grabFocus();
					break;
				}
			}
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
		editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
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
		editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
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
			
			editor->setFile( filename );
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
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
	
	if( editor->isDirty ) {
		int res = QMessageBox::warning( this, tr("QSciTE"), tr("This session has been modified.\n Do you want to save your changes?"),
										   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
		if( res == QMessageBox::Yes ) {
			saveFile();
		}
	}
	
	// Finally remove tab:
	ui.tabBarLeft->removeTab( index );

	editorManager->removeEditor( editor );
}

/**
 * MainWindow::undo
 * This is simply a pass trough because editor handles undos/redos
 * @author: jhenriques 2014
 */
void MainWindow::undo( ) {
	
	// get current editor:
	ScintillaEdit *editor = (ScintillaEdit *) ui.tabBarLeft->currentWidget();
	
	if( editor && editor->canUndo() ) {
		editor->undo();
	} else {
		ui.statusbar->showMessage( tr("Nothing to undo..."), 2000 );
	}
}

/**
 * MainWindow::redo
 * This is simply a pass trough because editor handles undos/redos
 * @author: jhenriques 2014
 */
void MainWindow::redo( ) {

	// get current editor:
	ScintillaEdit *editor = (ScintillaEdit *) ui.tabBarLeft->currentWidget();

	if( editor && editor->canRedo() ) {
		editor->redo();
	} else {
		ui.statusbar->showMessage( tr("Nothing to redo..."), 2000 );
	}
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
		//ui.searchField->activateWindow();
		ui.searchField->setFocus();
		//ui.searchField->grabKeyboard();
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
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
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
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
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
		//ui.replaceSearchField->activateWindow();
		ui.replaceSearchField->setFocus();
		//ui.replaceSearchField->grabKeyboard();
		ui.replaceSearchField->selectAll();
	}
}

/**
 * MainWindow::replaceNext
 * @author: jhenriques 2014
 */
void MainWindow::replaceNext( ) {

	QString searchString = ui.replaceSearchField->text();
	QString replaceString = ui.replaceField->text();
	
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
	editor->replaceNext( searchString, replaceString );
}

/**
 * MainWindow::setSingleSplitter
 * @author: jhenriques 2014
 */
void MainWindow::setSingleSplitter( ) {
	
	switch( currentSplitterConf ) {
		case SINGLE: { break; }	// Nothing to do here;
		case DOUBLE_H:
		case DOUBLE_V: {	// Got to move all buffers to left tab bar...
			
			QTabWidget *left = ui.tabBarLeft;
			QTabWidget *right = ui.tabBarRight;
			
			for( int i = 0, iCount = right->count(); i < iCount; ++i ) {
				QWidget *toMove = right->widget( i );
				moveTab( toMove, right, left );
			}

			break;
		}
	}
	
	ui.tabBarRight->hide();
	currentSplitterConf = SINGLE;
}

/**
 * MainWindow::setDoubleHSplitter
 * @author: jhenriques 2014
 */
void MainWindow::setDoubleHSplitter( ) {

	ui.splitter->setOrientation( Qt::Horizontal );
	ui.tabBarRight->show();
	
	switch( currentSplitterConf ) {
		case SINGLE: { // if coming from single layout, move focus to right layout:
			ui.tabBarRight->setFocus();
			break;
		}
		case DOUBLE_H:
		case DOUBLE_V: { break; } // Nothing to do here;
	}
	
	currentSplitterConf = DOUBLE_H;
}

/**
 * MainWindow::setDoubleVSplitter
 * @author: jhenriques 2014
 */
void MainWindow::setDoubleVSplitter( ) {

	ui.splitter->setOrientation( Qt::Vertical );
	ui.tabBarRight->show();
	
	switch( currentSplitterConf ) {
		case SINGLE: { // if coming from single layout, move focus to right layout:
			ui.tabBarRight->setFocus();
			break;
		}
		case DOUBLE_H:
		case DOUBLE_V: { break; } // Nothing to do here;
	}
	
	currentSplitterConf = DOUBLE_V;
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
	
	addAction( undoAction );
	addAction( redoAction );
	
	addAction( findAction );
	addAction( findNextAction );
	addAction( findPreviousAction );
	addAction( replaceAction );
	addAction( replaceNextAction );
	
	addAction( singleSplitterAction );
	addAction( doubleHorizontalSplitterAction );
	addAction( doubleVerticalSplitterAction );
	
}

/**
 * MainWindow::editorModified
 * Called when scintilla signals a dirty flag on editor.
 * @author: jhenriques 2014
 */
void MainWindow::editorModified( bool dirty ) {
	
	ScintillaEdit *modifiedEditor = (ScintillaEdit *) QObject::sender();
	if( !modifiedEditor || !modifiedEditor->parentWidget() ) {
		return;
	}
	
	QTabWidget *tabBar = (QTabWidget *) modifiedEditor->parentWidget()->parentWidget();
	if( !tabBar ) {
		return;
	}
	
	EditorPtr editor = editorManager->getEditor( modifiedEditor );
	if( editor ) {
		
		editor->isDirty = dirty;
		
		QString tabLabel = ( editor->getFileName() == "" ? tr("untitled") : editor->getFileName() );
		
		// must look for index of on tab bar:
		int index = tabBar->currentIndex();
		for( int i = 0, iCount = tabBar->count(); i < iCount; ++i ) {
			if( tabBar->widget( i ) == (QWidget *) modifiedEditor ) {
				index = i;
				break;
			}
		}

		if( dirty ) {
			tabBar->setTabText( index, tabLabel + "*" );
		} else {
			tabBar->setTabText( index, tabLabel );
		}
	}
}

/**
 * MainWindow::setupLexing
 * @author: jhenriques 2014
 */
void MainWindow::setupLexing( EditorPtr editor ) {

	// detect file extension:
	int language = SCLEX_NULL;
	QString fileName = editor->getFileName();
	if( fileName.endsWith( ".cpp", Qt::CaseInsensitive ) || fileName.endsWith( ".hpp", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".c", Qt::CaseInsensitive ) || fileName.endsWith( ".h", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".cc", Qt::CaseInsensitive ) || fileName.endsWith( ".hh", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".c++", Qt::CaseInsensitive ) || fileName.endsWith( ".h++", Qt::CaseInsensitive ) ||
  		fileName.endsWith( ".cp", Qt::CaseInsensitive ) || fileName.endsWith( ".hp", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".cxx", Qt::CaseInsensitive ) || fileName.endsWith( ".hxx", Qt::CaseInsensitive ) ) {
		language = SCLEX_CPP;
	}
	
	switch( language ) {
		case SCLEX_CPP:		{ setupCPPLexing( editor ); break; }
		case SCLEX_FORTRAN: { setupFortranLexing( editor ); break; }
		default: {
			break;
		}
	}
}
																			  
/**
 * MainWindow::setupCPPLexing
 * @author: jhenriques 2014
 */
void MainWindow::setupCPPLexing( EditorPtr editor ) {
	
	ScintillaEditPtr editorWidget = editor->getScintillaEdit();
	
	// CPP lexer
	editorWidget->setLexer( SCLEX_CPP );
	
	// Set number of style bits to use
	//editorWidget->setStyleBits( 5 );
	
	// Set tab width
	editorWidget->setTabWidth( 3 );
	
	// Use CPP keywords
	QString keywords =	"asm auto bool break case catch char class const "
						"const_cast continue default delete do double "
						"dynamic_cast else enum explicit extern false finally "
						"float for friend goto if inline int long mutable "
						"namespace new operator private protected public "
						"register reinterpret_cast register return short signed "
						"sizeof static static_cast struct switch template "
						"this throw true try typedef typeid typename "
						"union unsigned using virtual void volatile "
						"wchar_t while "
						"__asm __asume __based __box __cdecl __declspec "
						"__delegate delegate depreciated dllexport dllimport "
						"event __event __except __fastcall __finally __forceinline "
						"__int8 __int16 __int32 __int64 __int128 __interface "
						"interface __leave naked noinline __noop noreturn "
						"nothrow novtable nullptr safecast __stdcall "
						"__try __except __finally __unaligned uuid __uuidof "
						"__virtual_inheritance";
	editorWidget->setKeyWords( 0, keywords.toAscii() );
	
	// Set up the global default style. These attributes
	// are used wherever no explicit choices are made.
	editorWidget->styleSetFont( STYLE_DEFAULT, "Courier New" );
	editorWidget->styleSetSize( STYLE_DEFAULT, 10 );
	editorWidget->styleSetFore( STYLE_DEFAULT, createColor( 100, 100, 100 ) );
	editorWidget->styleSetBack( STYLE_DEFAULT, createColor( 255, 255, 255 ) );
	
	// Set caret foreground color
	editorWidget->setCaretFore( createColor( 100, 100, 100 ) );
	
	// Set all styles
	editorWidget->styleClearAll();
	
	// Set selection color
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
	
	// set margin style:
	editorWidget->setMarginWidthN( 2, 16 );
	editorWidget->setMarginMaskN( 2, SC_MASK_FOLDERS );
	editorWidget->setMarginSensitiveN( 2, true );
	//editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_SHOW & SC_AUTOMATICFOLD_CLICK & SC_AUTOMATICFOLD_CHANGE );
	editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_CLICK );
	
	editorWidget->setFoldMarginColour( true, createColor(240, 240, 240) );
	
	// set of markers to use for folding:
	editorWidget->markerDefine( SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS );
	editorWidget->markerDefine( SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS );
	editorWidget->markerDefine( SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE );
	editorWidget->markerDefine( SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER );
	editorWidget->markerDefine( SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED );
	editorWidget->markerDefine( SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED );
	editorWidget->markerDefine( SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER );
	
	// colors:
	editorWidget->markerSetFore( SC_MARKNUM_FOLDER, createColor(200, 200, 210) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDER, createColor(100, 100, 100) );
	editorWidget->markerSetFore( SC_MARKNUM_FOLDEROPEN, createColor(200, 200, 210) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDEROPEN, createColor(100, 100, 100) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDERSUB, createColor(200, 200, 200) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDERTAIL, createColor(200, 200, 200) );
	editorWidget->markerSetFore( SC_MARKNUM_FOLDEREND, createColor(200, 200, 210) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDEREND, createColor(100, 100, 100) );
	editorWidget->markerSetFore( SC_MARKNUM_FOLDEROPENMID, createColor(200, 200, 210) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDEROPENMID, createColor(100, 100, 100) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDERTAIL, createColor(200, 200, 200) );
	editorWidget->markerSetBack( SC_MARKNUM_FOLDERMIDTAIL, createColor(200, 200, 200) );
	
	// lexer folding configuration:
	editorWidget->setProperty( "fold", "1" );
	editorWidget->setProperty( "fold.cpp.syntax.based", "1" );
	editorWidget->setProperty( "fold.comment", "1" );
	editorWidget->setProperty( "fold.cpp.comment.multiline", "1" );
	editorWidget->setProperty( "fold.cpp.comment.explicit", "1" );
	editorWidget->setProperty( "fold.cpp.explicit.start", "1" );
	editorWidget->setProperty( "fold.cpp.explicit.anywhere", "1" );
	editorWidget->setProperty( "fold.preprocessor", "1" );
	editorWidget->setProperty( "fold.compact", "1" );
	editorWidget->setProperty( "fold.at.else", "1" );
}

/**
 * MainWindow::setupFortranLexing
 * @author: jhenriques 2014
 */
void MainWindow::setupFortranLexing( EditorPtr ) {
	
}

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
 * MainWindow::moveTab
 * @author: jhenriques 2014
 */
void MainWindow::moveTab( QWidget *toMove, QTabWidget *from, QTabWidget *to ) {
	
	if( !toMove || !from || !to ) {
		return;
	}
	
	// get indexs:
	int indexFrom = from->indexOf( toMove );
	if( indexFrom == -1 ) {
		return;
	}
	
	// check if document is also open on to tab:
	EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) toMove );
	QString filePath = editor->getFilePath();
	
	for( int i = 0, iCount = to->count(); i < iCount; ++i ) {
		
		EditorPtr curEditor = editorManager->getEditor( (ScintillaEdit *) to->widget( i ) );
		if( !filePath.isEmpty() && curEditor->getFilePath() == filePath ) {
			
			// this file is already open on to tab widget.
			// just remove it from from tab:
			from->removeTab( indexFrom );
			return;
		}
	}
	
	// If we get here we only need to remove from from tab and add to to tab:
	to->addTab( toMove, from->tabText( indexFrom ) );
	from->removeTab( indexFrom );
}

/**
 * MainWindow::createActions
 * This is more or less an hack to avoid extending QSplitter...
 * It will try to guess what is the current splitter based on
 * the widget that has application focus;
 * @author: jhenriques 2014
 */
splitter MainWindow::getCurrentSplitter( ) {
		
	QWidget *focusWidget = this->focusWidget();
	
	if( focusWidget ) {
		
		if( focusWidget == (QWidget *)ui.tabBarLeft ) {
			return LEFT_SPLITTER;
		} else if( focusWidget == (QWidget *)ui.tabBarRight ) {
			return RIGHT_SPLITTER;
		}
		
		if( focusWidget->parentWidget() == (QWidget *)ui.tabBarLeft ) {
			return LEFT_SPLITTER;
		} else if( focusWidget->parentWidget() == (QWidget *)ui.tabBarRight ) {
			return RIGHT_SPLITTER;
		} else if( focusWidget->parentWidget() ) {
			
			ScintillaEdit *editor = (ScintillaEdit *) focusWidget->parentWidget()->parentWidget();
			
			if( editor == (QWidget *)ui.tabBarLeft ) {
				return LEFT_SPLITTER;
			} else if( editor == (QWidget *)ui.tabBarRight ) {
				return RIGHT_SPLITTER;
			}
		}
	}

	return LEFT_SPLITTER; // the default;
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
	
	
	// Undo Action:
	undoAction = new QAction( "Undo", this );
	undoAction->setIcon( QIcon(":/icons/undo.png") );
	undoAction->setShortcut( QKeySequence::Undo );
	undoAction->setStatusTip( tr("Undo") );
	connect( undoAction, SIGNAL(triggered()), this, SLOT(undo()) );
	
	// Redo Action:
	redoAction = new QAction( "Redo", this );
	redoAction->setIcon( QIcon(":/icons/redo.png") );
	redoAction->setShortcut( QKeySequence::Redo );
	redoAction->setStatusTip( tr("Redo") );
	connect( redoAction, SIGNAL(triggered()), this, SLOT(redo()) );
	
	
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
	
	
	// splitter actions:
	singleSplitterAction = new QAction( "Single", this );
	singleSplitterAction->setShortcut( tr("Alt+Ctrl+1") );
	singleSplitterAction->setStatusTip( tr("Single") );
	connect( singleSplitterAction, SIGNAL(triggered()), this, SLOT(setSingleSplitter()) );
	
	doubleHorizontalSplitterAction = new QAction( "Dual Horizontal", this );
	doubleHorizontalSplitterAction->setShortcut( tr("Alt+Ctrl+2") );
	doubleHorizontalSplitterAction->setStatusTip( tr("Dual Horizontal") );
	connect( doubleHorizontalSplitterAction, SIGNAL(triggered()), this, SLOT(setDoubleHSplitter()) );
	
	doubleVerticalSplitterAction = new QAction( "Dual Vertical", this );
	doubleVerticalSplitterAction->setShortcut( tr("Alt+Ctrl+3") );
	doubleVerticalSplitterAction->setStatusTip( tr("Dual Vertical") );
	connect( doubleVerticalSplitterAction, SIGNAL(triggered()), this, SLOT(setDoubleVSplitter()) );

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
	
	// Edit menu:
	editMenu = menuBar()->addMenu( tr("&Edit") );
	editMenu->addAction( undoAction );
	editMenu->addAction( redoAction );
	editMenu->addSeparator();
	
	// Find menu:
	findMenu = menuBar()->addMenu( tr("Fin&d") );
	findMenu->addAction( findAction );
	findMenu->addAction( findNextAction );
	findMenu->addAction( findPreviousAction );
	findMenu->addSeparator();
	findMenu->addAction( replaceAction );
	findMenu->addAction( replaceNextAction );
	
	viewMenu = menuBar()->addMenu( tr("View") );
	QMenu *layoutMenu = viewMenu->addMenu( tr("Layout") );
	layoutMenu->addAction( singleSplitterAction );
	layoutMenu->addAction( doubleHorizontalSplitterAction );
	layoutMenu->addAction( doubleVerticalSplitterAction );
	
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
	
	// edit toolbar:
	editToolBar = addToolBar( tr("&Edit") );
	editToolBar->addAction( undoAction );
	editToolBar->addAction( redoAction );
	
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
