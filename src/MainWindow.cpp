#include "MainWindow.hpp"

#include <iostream>
#include <SciLexer.h>

const int base03 = 0 | (43 << 8) | (54 << 16);
const int base02 = 7 | (54 << 8) | (66 << 16);
const int base01 = 88 | (110 << 8) | (117 << 16);
const int base00 = 101 | (123 << 8) | (131 << 16);
const int base0 = 112 | (130 << 8) | (132 << 16);
const int base1 = 129 | (144 << 8) | (144 << 16);
const int base2 = 238 | (232 << 8) | (213 << 16);
const int base3 = 253 | (246 << 8) | (227 << 16);
const int yellow = 115 | (138 << 8) | (5 << 16);
const int orange = 203 | (75 << 8) | (22 << 16);
const int gold = 254 | (231 << 8) | (146 << 16);
const int red = 220 | (50 << 8) | (47 << 16);
const int magenta = 211 | (54 << 8) | (130 << 16);
const int violet = 108 | (113 << 8) | (196 << 16);
const int blue = 38 | (139 << 8) | (210 << 16);
const int cyan = 33 | (118 << 8) | (199 << 16);
const int green = 37 | (146 << 8) | (134 << 16);


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
	findSelectedSplitter = LEFT_SPLITTER;
	
	// Find/Replace config:
	ui.searchFrame->hide();
	connect( ui.searchField, SIGNAL(textChanged(QString)), this, SLOT(findNext(QString)) );
	connect( ui.nextButton, SIGNAL(clicked()), this, SLOT(findNext()) );
	connect( ui.previousButton, SIGNAL(clicked()), this, SLOT(findPrevious()) );
	
	ui.replaceFrame->hide();
	connect( ui.replaceSearchField, SIGNAL(textChanged(QString)), this, SLOT(findNext(QString)) );
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
	
}

/**
 * MainWindow::newFile
 * Creates a new empty buffer;
 * @author: jhenriques 2014
 */
void MainWindow::newFile( ) {
	
	EditorPtr editor = editorManager->createEditor( );
	
	if( editor ) {
		
		ScintillaEdit *editorWidget = editor->getScintillaEdit( );
		
		setupEditor( editorWidget );
		registerEditorListeners( editorWidget );
		
		// find out what is the active splitter if any. default is LEFT_SPLITTER:
		splitter active = getCurrentSplitter( );
		
		switch( active ) {
			case LEFT_SPLITTER: {
				
				int index = ui.tabBarLeft->addTab( editorWidget, tr("untitled") );
				ui.tabBarLeft->setCurrentIndex( index );
				editorWidget->grabFocus();
				break;
			}
			case RIGHT_SPLITTER: {
				
				int index = ui.tabBarRight->addTab( editorWidget, tr("untitled") );
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
			ScintillaEdit *widget = existingEditor->getScintillaEdit();
			if( widget ) {
				
				QTabWidget *tabBar = (QTabWidget *) widget->parentWidget()->parentWidget();
				
				// case when in the same splitter:
				if( tabBar == currentTabBar ) {
					tabBar->setCurrentWidget( widget );
					widget->grabFocus();
					return;
				}
				
				// if we get here we need a new editor and to share scintilla document:
				EditorPtr editor = editorManager->createEditor( );
				
				ScintillaEdit *editorWidget = editor->getScintillaEdit( );
				setupEditor( editorWidget );
				registerEditorListeners( editorWidget );
				
				editor->setFile( existingEditor->getFilePath() );
				editor->getScintillaEdit()->set_doc( widget->get_doc() );
				
				setupLexing( editor );
				
				int index = currentTabBar->addTab( editorWidget, editor->getFileName() );
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
			
			ScintillaEdit *editorWidget = editor->getScintillaEdit( );
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
					
					int index = ui.tabBarLeft->addTab( editorWidget, editor->getFileName() );
					ui.tabBarLeft->setCurrentIndex( index );
					editorWidget->grabFocus();
					break;
				}
				case RIGHT_SPLITTER: {
					
					int index = ui.tabBarRight->addTab( editorWidget, editor->getFileName() );
					ui.tabBarRight->setCurrentIndex( index );
					editorWidget->grabFocus();
					break;
				}
			}
		}
	}
	
	this->activateWindow();
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
		
		// find out what is the active splitter if any. default is LEFT_SPLITTER:
		splitter active = getCurrentSplitter( );
		switch( active ) {
			case LEFT_SPLITTER: {
				editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
				break;
			}
			case RIGHT_SPLITTER: {
				editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
				break;
			}
		}
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
		
		// find out what is the active splitter if any. default is LEFT_SPLITTER:
		splitter active = getCurrentSplitter( );
		switch( active ) {
			case LEFT_SPLITTER: {
				editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
				break;
			}
			case RIGHT_SPLITTER: {
				editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
				break;
			}
		}
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
	
	EditorPtr editor;
	
	// get current editor:
	splitter active = getCurrentSplitter( );
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->widget( index ) );
			break;
		}
		case RIGHT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->widget( index ) );
			break;
		}
	}
	
	if( !editor ) {
		return;
	}
	
	if( editor->isDirty ) {
		int res = QMessageBox::warning( this, tr("QSciTE"), tr("This session has been modified.\n Do you want to save your changes?"),
										   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
		if( res == QMessageBox::Yes ) {
			saveFile();
		}
	}
	
	// Finally remove tab:
	switch( active ) {
		case LEFT_SPLITTER: {
			ui.tabBarLeft->removeTab( index );
			break;
		}
		case RIGHT_SPLITTER: {
			ui.tabBarRight->removeTab( index );
			break;
		}
	}

	editorManager->removeEditor( editor );
}

/**
 * MainWindow::undo
 * This is simply a pass trough because editor handles undos/redos
 * @author: jhenriques 2014
 */
void MainWindow::undo( ) {
	
	// get current editor:
	ScintillaEdit *editor;
	splitter active = getCurrentSplitter( );
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = (ScintillaEdit *) ui.tabBarLeft->currentWidget();
			break;
		}
		case RIGHT_SPLITTER: {
			editor = (ScintillaEdit *) ui.tabBarRight->currentWidget();
			break;
		}
	}
	
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
	ScintillaEdit *editor;
	splitter active = getCurrentSplitter( );
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = (ScintillaEdit *) ui.tabBarLeft->currentWidget();
			break;
		}
		case RIGHT_SPLITTER: {
			editor = (ScintillaEdit *) ui.tabBarRight->currentWidget();
			break;
		}
	}

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
	
	splitter active = getCurrentSplitter( );
	
	if( ui.replaceFrame->isVisible() ) {
		ui.replaceFrame->hide();
		
		// copy text between find fields:
		if( !ui.replaceSearchField->text().isEmpty() ) {
			ui.searchField->setText( ui.replaceSearchField->text() );
		}
	}
	
	if( ui.searchFrame->isVisible() ) {
		ui.searchFrame->hide();
		
		switch( active ) {
			case LEFT_SPLITTER: {
				EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
				if( editor ) { editor->clearSearchIndicators(); }
				break;
			}
			case RIGHT_SPLITTER: {
				EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
				if( editor ) { editor->clearSearchIndicators(); }
				break;
			}
		}
	} else {
		ui.searchFrame->show();
		ui.searchField->setFocus();
		ui.searchField->selectAll();
	}
}

/**
 * MainWindow::findNext
 *
 * @author: jhenriques 2014
 */
void MainWindow::findNext( QString search ) {
	
	QString searchString = search;
	QObject *sender = QObject::sender();
	
	if( searchString == "" ) {
		if( sender ) {
			if( sender == ui.nextButton ) {
				searchString = ui.searchField->text();
			} else if( sender == ui.replaceNextButton ) {
				searchString = ui.replaceSearchField->text();
			}
		}
	}
	
	EditorPtr editor;
	splitter active = getCurrentSplitter( );
	if( active != findSelectedSplitter ) {
		active = findSelectedSplitter;
		findSelectedSplitter = getCurrentSplitter( );
	}
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
			break;
		}
		case RIGHT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
			break;
		}
	}
	
	if( editor ) {
		
		QPair<int, int> found = editor->findNext( searchString );
		
		if( found.first == -1 ) {
			ui.statusbar->showMessage( tr("Reached end of document. Will loop with next find."), 3000 );
		}
	}
}

/**
 * MainWindow::findPrevious
 *
 * @author: jhenriques 2014
 */
void MainWindow::findPrevious( ) {

	QString searchString = ui.searchField->text();
	
	EditorPtr editor;
	splitter active = getCurrentSplitter( );
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
			break;
		}
		case RIGHT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
			break;
		}
	}
	
	if( editor ) {
		
		QPair<int, int> found = editor->findPrevious( searchString );
		
		if( found.first == -1 ) {
			ui.statusbar->showMessage( tr("Reached end of document. Will loop with next find."), 3000 );
		}
	}
}

/**
 * MainWindow::replace
 *
 * @author: jhenriques 2014
 */
void MainWindow::replace( ) {
	
	splitter active = getCurrentSplitter( );

	if( ui.searchFrame->isVisible() ) {
		ui.searchFrame->hide();
		
		// copy text between find fields:
		if( !ui.searchField->text().isEmpty() ) {
			ui.replaceSearchField->setText( ui.searchField->text() );
		}
	}
	
	if( ui.replaceFrame->isVisible() ) {
		ui.replaceFrame->hide();
		
		switch( active ) {
			case LEFT_SPLITTER: {
				EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
				if( editor ) { editor->clearSearchIndicators(); }
				break;
			}
			case RIGHT_SPLITTER: {
				EditorPtr editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
				if( editor ) { editor->clearSearchIndicators(); }
				break;
			}
		}
	} else {
		ui.replaceFrame->show();
		ui.replaceSearchField->setFocus();
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
	
	EditorPtr editor;
	splitter active = getCurrentSplitter( );
	switch( active ) {
		case LEFT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarLeft->currentWidget() );
			break;
		}
		case RIGHT_SPLITTER: {
			editor = editorManager->getEditor( (ScintillaEdit *) ui.tabBarRight->currentWidget() );
			break;
		}
	}

	if( editor) {
		
		QPair<int, int> found = editor->replaceNext( searchString, replaceString );
		
		if( found.first == -1 ) {
			ui.statusbar->showMessage( tr("Reached end of document. Will loop with next find."), 3000 );
		}
	}
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
				QWidget *toMove = right->currentWidget( );
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
void MainWindow::setupEditor( ScintillaEdit *editor ) {

	if( !editor ) {
		return;
	}

	// Margins:
	editor->setMarginTypeN( 0, SC_MARGIN_NUMBER );
	editor->setMarginMaskN( 0, ~SC_MASK_FOLDERS );
	editor->setMarginWidthN( 0, 20 );
	editor->styleSetBack( STYLE_LINENUMBER, base3 );
	editor->styleSetFont( STYLE_LINENUMBER, "DejaVu Sans Mono" );
	editor->styleSetSize( STYLE_LINENUMBER, 10 );
	editor->styleSetFore( STYLE_LINENUMBER, base0 );

	// hide this margin
	editor->setMarginWidthN( 1, 0 );
	
	// Set up the global default style. These attributes
	// are used wherever no explicit choices are made.
	editor->styleSetFont( STYLE_DEFAULT, "DejaVu Sans Mono" );
	editor->styleSetSize( STYLE_DEFAULT, 11 );
	editor->styleSetFore( STYLE_DEFAULT, base0 );
	editor->styleSetBack( STYLE_DEFAULT, base3 );
	
	// plain text:
	editor->styleSetFont( 0, "DejaVu Sans Mono" );
	editor->styleSetSize( 0, 11 );
	editor->styleSetFore( 0, base0 );
	editor->styleSetBack( 0, base3 );
	
	// Set caret foreground color
	editor->setCaretFore( base0 );
	
	// Set selection color
	editor->setSelBack( 1, base2 );
	
	// identation:
	editor->setTabWidth( 3 );
	editor->setUseTabs( true );
	editor->setIndent( 0 );
	editor->setIndentationGuides( SC_IV_LOOKBOTH );
	
	//editor->setCaretSticky( SC_CARETSTICKY_WHITESPACE );
	
	// indicators setup:
	// INDIC_0 -> box around the match
	editor->indicSetStyle( 0, INDIC_ROUNDBOX );
	editor->indicSetFore( 0, base0 );
	editor->indicSetAlpha( 0, 0 );
	editor->indicSetOutlineAlpha( 0, 230 );
	
	// INDIC_0 -> filled box around current match
	editor->indicSetStyle( 1, INDIC_ROUNDBOX );
	editor->indicSetFore( 1, gold );
	editor->indicSetAlpha( 1, 50 );
	editor->indicSetOutlineAlpha( 1, 0 );
}

/**
 * MainWindow::registerEditorListeners
 * Registers to listen to ScintillaEdit widget important signals;
 * @author: jhenriques 2014
 */
void MainWindow::registerEditorListeners( ScintillaEdit *editor ) {
	
	if( !editor ) {
		return;
	}

	connect( editor, SIGNAL(savePointChanged(bool)), this, SLOT(editorModified(bool)) );
	
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
		fileName.endsWith( ".ipp", Qt::CaseInsensitive ) || fileName.endsWith( ".m", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".mm", Qt::CaseInsensitive ) || fileName.endsWith( ".sma", Qt::CaseInsensitive ) ||
		fileName.endsWith( ".cxx", Qt::CaseInsensitive ) || fileName.endsWith( ".hxx", Qt::CaseInsensitive ) ) {
		language = SCLEX_CPP;
	} else if(	fileName.endsWith( ".f", Qt::CaseInsensitive ) || fileName.endsWith( ".for", Qt::CaseInsensitive ) ||
				fileName.endsWith( ".f90", Qt::CaseInsensitive ) || fileName.endsWith( ".f95", Qt::CaseInsensitive ) ||
				fileName.endsWith( ".f2k", Qt::CaseInsensitive ) ) {
		language = SCLEX_FORTRAN;
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
	
	ScintillaEdit *editorWidget = editor->getScintillaEdit( );
	
	// CPP lexer
	editorWidget->setLexer( SCLEX_CPP );
	
	// Use CPP keywords
	QString keywords = "and and_eq asm auto bitand bitor bool break \
						case catch char class compl const const_cast continue \
						default delete do double dynamic_cast else enum explicit export extern false float for \
						friend goto if inline int long mutable namespace new not not_eq \
						operator or or_eq private protected public \
						register reinterpret_cast return short signed sizeof static static_cast struct switch \
						template this throw true try typedef typeid typename union unsigned using \
						virtual void volatile wchar_t while xor xor_eq\
						@class @defs @protocol @required @optional @end \
						@interface @public @package @protected @private @property \
						@implementation @synthesize @dynamic \
						@throw @try @catch @finally \
						@synchronized @autoreleasepool \
						@selector @encode \
						@compatibility_alias";
	editorWidget->setKeyWords( 0, keywords.toAscii() );
	
	keywords = 'a addindex addtogroup anchor arg attention \
				author b brief bug c class code date def defgroup deprecated dontinclude \
				e em endcode endhtmlonly endif endlatexonly endlink endverbatim enum example exception \
				f$ f[ f] file fn hideinitializer htmlinclude htmlonly \
				if image include ingroup internal invariant interface latexonly li line link \
				mainpage name namespace nosubgrouping note overload \
				p page par param param[in] param[out] \
				post pre ref relates remarks return retval \
				sa section see showinitializer since skip skipline struct subsection \
				test throw throws todo typedef union until \
				var verbatim verbinclude version warning weakgroup $ @ \ & < > # { }';
	editorWidget->setKeyWords( 3, keywords.toAscii() );
	
	// Set caret foreground color
	editorWidget->setCaretFore( base0 );
	
	// Set all styles
	editorWidget->styleClearAll();
	
	// Set selection color
	editorWidget->setSelBack( 1, base2 );
	
	// Set syntax colors
	editorWidget->styleSetFore( SCE_C_DEFAULT,					base0 );
	editorWidget->styleSetFore( SCE_C_COMMENT,					base1 );
	editorWidget->styleSetFore( SCE_C_COMMENTLINE,				base1 );
	editorWidget->styleSetFore( SCE_C_COMMENTDOC,				base1 );
	editorWidget->styleSetFore( SCE_C_NUMBER,					green );
	editorWidget->styleSetFore( SCE_C_WORD,						yellow );
	editorWidget->styleSetFore( SCE_C_STRING,					green );
	editorWidget->styleSetFore( SCE_C_CHARACTER,				green );
	editorWidget->styleSetFore( SCE_C_UUID,						cyan );
	editorWidget->styleSetFore( SCE_C_PREPROCESSOR,				red );
	editorWidget->styleSetFore( SCE_C_OPERATOR,					base0 );
	editorWidget->styleSetFore( SCE_C_IDENTIFIER,				blue );
	editorWidget->styleSetFore( SCE_C_STRINGEOL,				base0 );
	editorWidget->styleSetFore( SCE_C_VERBATIM,					green );
	editorWidget->styleSetFore( SCE_C_REGEX,					red );
	editorWidget->styleSetFore( SCE_C_COMMENTLINEDOC,			base1 );
	editorWidget->styleSetFore( SCE_C_WORD2,					green );
	editorWidget->styleSetFore( SCE_C_COMMENTDOCKEYWORD,		red );
	editorWidget->styleSetFore( SCE_C_COMMENTDOCKEYWORDERROR,	red );
	editorWidget->styleSetFore( SCE_C_GLOBALCLASS,				yellow );
	editorWidget->styleSetFore( SCE_C_STRINGRAW,				green );
	editorWidget->styleSetFore( SCE_C_TRIPLEVERBATIM,			green );
	editorWidget->styleSetFore( SCE_C_HASHQUOTEDSTRING,			red );
	editorWidget->styleSetFore( SCE_C_PREPROCESSORCOMMENT,		base1 );
	editorWidget->styleSetFore( SCE_C_PREPROCESSORCOMMENTDOC,	base1 );
	editorWidget->styleSetFore( SCE_C_USERLITERAL,				green );
	
	// set margin style:
	editorWidget->setMarginWidthN( 2, 16 );
	editorWidget->setMarginMaskN( 2, SC_MASK_FOLDERS );
	editorWidget->setMarginSensitiveN( 2, true );
	//editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_SHOW & SC_AUTOMATICFOLD_CLICK & SC_AUTOMATICFOLD_CHANGE );
	editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_CLICK );
	
	// Margins:
	editorWidget->setMarginTypeN( 0, SC_MARGIN_NUMBER );
	editorWidget->setMarginMaskN( 0, ~SC_MASK_FOLDERS );
	editorWidget->setMarginWidthN( 0, 20 );
	editorWidget->styleSetBack( STYLE_LINENUMBER, base3 );
	editorWidget->styleSetFont( STYLE_LINENUMBER, "DejaVu Sans Mono" );
	editorWidget->styleSetSize( STYLE_LINENUMBER, 10 );
	editorWidget->styleSetFore( STYLE_LINENUMBER, base0 );
	
	// hide this margin
	editorWidget->setMarginWidthN( 1, 0 );
	
	editorWidget->setFoldMarginColour( true, base3 );
	
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
void MainWindow::setupFortranLexing( EditorPtr editor ) {
	
	ScintillaEdit *editorWidget = editor->getScintillaEdit( );
	
	// CPP lexer
	editorWidget->setLexer( SCLEX_FORTRAN );
	
	// Set tab width
	editorWidget->setTabWidth( 3 );
	
	// Use CPP keywords
	QString keywords =	"access action advance allocatable allocate \
	apostrophe assign assignment associate asynchronous backspace \
	bind blank blockdata call case character class close common \
	complex contains continue cycle data deallocate decimal delim \
	default dimension direct do dowhile double doubleprecision else \
	elseif elsewhere encoding end endassociate endblockdata enddo \
	endfile endforall endfunction endif endinterface endmodule endprogram \
	endselect endsubroutine endtype endwhere entry eor equivalence \
	err errmsg exist exit external file flush fmt forall form format \
	formatted function go goto id if implicit in include inout \
	integer inquire intent interface intrinsic iomsg iolength \
	iostat kind len logical module name named namelist nextrec nml \
	none nullify number only open opened operator optional out pad \
	parameter pass pause pending pointer pos position precision \
	print private program protected public quote read readwrite \
	real rec recl recursive result return rewind save select \
	selectcase selecttype sequential sign size stat status stop stream \
	subroutine target then to type unformatted unit use value \
	volatile wait where while write";
	editorWidget->setKeyWords( 0, keywords.toAscii() );
	
	keywords = "abs achar acos acosd adjustl adjustr \
	aimag aimax0 aimin0 aint ajmax0 ajmin0 akmax0 akmin0 all allocated alog \
	alog10 amax0 amax1 amin0 amin1 amod anint any asin asind associated \
	atan atan2 atan2d atand bitest bitl bitlr bitrl bjtest bit_size bktest break \
	btest cabs ccos cdabs cdcos cdexp cdlog cdsin cdsqrt ceiling cexp char \
	clog cmplx conjg cos cosd cosh count cpu_time cshift csin csqrt dabs \
	dacos dacosd dasin dasind datan datan2 datan2d datand date \
	date_and_time dble dcmplx dconjg dcos dcosd dcosh dcotan ddim dexp \
	dfloat dflotk dfloti dflotj digits dim dimag dint dlog dlog10 dmax1 dmin1 \
	dmod dnint dot_product dprod dreal dsign dsin dsind dsinh dsqrt dtan dtand \
	dtanh eoshift epsilon errsns exp exponent float floati floatj floatk floor fraction \
	free huge iabs iachar iand ibclr ibits ibset ichar idate idim idint idnint ieor ifix \
	iiabs iiand iibclr iibits iibset iidim iidint iidnnt iieor iifix iint iior iiqint iiqnnt iishft \
	iishftc iisign ilen imax0 imax1 imin0 imin1 imod index inint inot int int1 int2 int4 \
	int8 iqint iqnint ior ishft ishftc isign isnan izext jiand jibclr jibits jibset jidim jidint \
	jidnnt jieor jifix jint jior jiqint jiqnnt jishft jishftc jisign jmax0 jmax1 jmin0 jmin1 \
	jmod jnint jnot jzext kiabs kiand kibclr kibits kibset kidim kidint kidnnt kieor kifix \
	kind kint kior kishft kishftc kisign kmax0 kmax1 kmin0 kmin1 kmod knint knot kzext \
	lbound leadz len len_trim lenlge lge lgt lle llt log log10 logical lshift malloc matmul \
	max max0 max1 maxexponent maxloc maxval merge min min0 min1 minexponent minloc \
	minval mod modulo mvbits nearest nint not nworkers number_of_processors pack popcnt \
	poppar precision present product radix random random_number random_seed range real \
	repeat reshape rrspacing rshift scale scan secnds selected_int_kind \
	selected_real_kind set_exponent shape sign sin sind sinh size sizeof sngl snglq spacing \
	spread sqrt sum system_clock tan tand tanh tiny transfer transpose trim ubound unpack verify";
	editorWidget->setKeyWords( 2, keywords.toAscii() );
	
	keywords = "cdabs cdcos cdexp cdlog cdsin cdsqrt cotan cotand \
	dcmplx dconjg dcotan dcotand decode dimag dll_export dll_import doublecomplex dreal \
	dvchk encode find flen flush getarg getcharqq getcl getdat getenv gettim hfix ibchng \
	identifier imag int1 int2 int4 intc intrup invalop iostat_msg isha ishc ishl jfix \
	lacfar locking locnear map nargs nbreak ndperr ndpexc offset ovefl peekcharqq precfill \
	prompt qabs qacos qacosd qasin qasind qatan qatand qatan2 qcmplx qconjg qcos qcosd \
	qcosh qdim qexp qext qextd qfloat qimag qlog qlog10 qmax1 qmin1 qmod qreal qsign qsin \
	qsind qsinh qsqrt qtan qtand qtanh ran rand randu rewrite segment setdat settim system \
	timer undfl unlock union val virtual volatile zabs zcos zexp zlog zsin zsqrt";
	editorWidget->setKeyWords( 3, keywords.toAscii() );
	
	// Set all styles
	editorWidget->styleClearAll();
	
	// Set syntax colors
	editorWidget->styleSetFore( SCE_F_DEFAULT,		base0 );
	editorWidget->styleSetFore( SCE_F_COMMENT,		base1 );
	editorWidget->styleSetFore( SCE_F_NUMBER,		green );
	editorWidget->styleSetFore( SCE_F_STRING1,		green );
	editorWidget->styleSetFore( SCE_F_STRING2,		green );
	editorWidget->styleSetFore( SCE_F_STRINGEOL,	green );
	editorWidget->styleSetFore( SCE_F_OPERATOR,		base0 );
	editorWidget->styleSetFore( SCE_F_IDENTIFIER,	red );
	editorWidget->styleSetFore( SCE_F_WORD,			yellow );
	editorWidget->styleSetFore( SCE_F_WORD2,		yellow );
	editorWidget->styleSetFore( SCE_F_WORD3,		yellow );
	editorWidget->styleSetFore( SCE_F_PREPROCESSOR,	red );
	editorWidget->styleSetFore( SCE_F_OPERATOR2,	base0 );
	editorWidget->styleSetFore( SCE_F_LABEL,		green );
	editorWidget->styleSetFore( SCE_F_CONTINUATION,	red );
	
	// set margin style:
	editorWidget->setMarginWidthN( 2, 16 );
	editorWidget->setMarginMaskN( 2, SC_MASK_FOLDERS );
	editorWidget->setMarginSensitiveN( 2, true );
	//editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_SHOW & SC_AUTOMATICFOLD_CLICK & SC_AUTOMATICFOLD_CHANGE );
	editorWidget->setAutomaticFold( SC_AUTOMATICFOLD_CLICK );
	
	// Margins:
	editorWidget->setMarginTypeN( 0, SC_MARGIN_NUMBER );
	editorWidget->setMarginMaskN( 0, ~SC_MASK_FOLDERS );
	editorWidget->setMarginWidthN( 0, 20 );
	editorWidget->styleSetBack( STYLE_LINENUMBER, base3 );
	editorWidget->styleSetFont( STYLE_LINENUMBER, "DejaVu Sans Mono" );
	editorWidget->styleSetSize( STYLE_LINENUMBER, 10 );
	editorWidget->styleSetFore( STYLE_LINENUMBER, base0 );
	
	// hide this margin
	editorWidget->setMarginWidthN( 1, 0 );
	
	editorWidget->setFoldMarginColour( true, base3 );
	
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
	editorWidget->setProperty( "fold.comment.fortran", "1" );
	editorWidget->setProperty( "fold.quotes.fortran", "1" );
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
