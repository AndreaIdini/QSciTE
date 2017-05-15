#include "BufferManager.hpp"


BufferManager::BufferManager( ) : current(BufferPtr()) {
	
}


BufferManager::~BufferManager( ) {
	
}


bool BufferManager::add( BufferPtr newBuffer ) {
	
	if( !newBuffer ) {
		return false;
	}
	
	buffers.push_back( newBuffer );
	return true;
}

/**
 * Returns true if buffer is handled by this manager;
 * author: jhenriques 2014
 */
bool BufferManager::inList( BufferPtr buffer ) {
	
	std::list< BufferPtr >::iterator iter;
	for( iter = buffers.begin(); iter != buffers.end(); iter++ ) {
		if( *iter == buffer ) {
			return true;
		}
	}
	return false;
}


/**
 * Sets current active buffer to given buffer.
 * If buffer is NULL or not in the list of buffers it fails.
 * author: jhenriques 2014
 */
bool BufferManager::setCurrent( BufferPtr buffer ) {
	
	if( !buffer || !inList( buffer ) ) {
		return false;
	}
	
	current = buffer;
	
	return true;
}

//bool BufferManager::setCurrent( int index ) {
//	
//	if( index >= buffers.size() ) {
//		return false;
//	}
//	
//	std::list< BufferPtr >::iterator iter = buffers.begin();
//	if( buffers)
//	
//	return true;
//}


BufferPtr BufferManager::getCurrent( ) {
	
	return current;
}
