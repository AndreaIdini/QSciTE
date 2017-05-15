#pragma once

#ifndef QSCITE_BUFFERMANAGER_HPP
#define QSCITE_BUFFERMANAGER_HPP

#include <list>
#include <boost/shared_ptr.hpp>
#include <Scintilla.h>

#include "Buffer.hpp"


/**
 * class BufferManager
 * Handles a list of buffers. Works as the manager of all buffers 
 * in the editor.
 *
 * author: jhenriques 2014
 */
class BufferManager {

private:
	
	std::list< BufferPtr > buffers;
	
	BufferPtr current;
	
protected:
	
	bool inList( BufferPtr );
	
public:
	
	// --== member vars ==--
	
	// --== member methods ==--
	BufferManager( );
	virtual ~BufferManager( );
	
	bool add( BufferPtr );
	
	bool setCurrent( BufferPtr );
	//bool setCurrent( int );
	BufferPtr getCurrent( );
	
};

typedef boost::shared_ptr< BufferManager > BufferManagerPtr;

#endif