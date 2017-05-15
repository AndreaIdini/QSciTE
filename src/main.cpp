/**
 * QSciTE main 
 * QSciTE is a Qt based editor with a scintilla backend.
 */

#include <QApplication>
#include <QSplashScreen>
#include <QIcon>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "MainWindow.hpp"


//
// This is the main entry point for the editor.
// It will set the Qt App and start the application by
// running the MainWindow.
//
int main( int argc, char* argv[] ) {
	
	QApplication app( argc, argv );
	
	// Set application meta information:
	QCoreApplication::setOrganizationName( "QSciTE by Andrea Idini" );
	QCoreApplication::setOrganizationDomain( "www.phme.it/wilt/" );
	QCoreApplication::setApplicationName( "QSciTE" );
	
	// TODO: Replace icon.
	QApplication::setWindowIcon( QIcon( ":QSciTE_icon.svg" ) );
	
	// create and show our splashscreen:
	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap( QPixmap(":/splash.png") );
	splash->show();
	
	Qt::Alignment alignment = Qt::AlignCenter | Qt::AlignBottom;
	splash->showMessage( QObject::tr("Setting up QSciTE main window..."), alignment, Qt::white );
	app.processEvents();
	
	// small stupid hack to make the splash show for 2 seconds... might remove this...
	boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ) );
	
	// init main window:
	MainWindowPtr mainWindow = MainWindowPtr( new MainWindow( app ) );

	mainWindow->initialize();
	mainWindow->show();

	// TODO: handle command line argument:
//	if( argc > 1 ) {
//		QString file( argv[1] );
//		if ( QFileInfo( file ).suffix() == "exr" )
//			mainWindow->loadSphericalMap( file );
//		else
//			mainWindow->loadScene( file );
//	}
	
	// close and delete splash screen:
	splash->finish( mainWindow.get() );
	delete splash;
	
	// finally run editor:
	return app.exec();
}
