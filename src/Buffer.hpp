#pragma once

#ifndef QSCITE_BUFFER_HPP
#define QSCITE_BUFFER_HPP

#include <boost/shared_ptr.hpp>

#include <Scintilla.h>


/**
 * class Buffer
 * Represents an open file as a working scintilla buffer;
 *
 * author: jhenriques 2014
 */
class Buffer {
	
public:
	
	// --== member vars ==--
	sptr_t doc;
	bool isDirty;
	
	// --== member methods ==--
	Buffer( );
	virtual ~Buffer( );
	
};

typedef boost::shared_ptr< Buffer > BufferPtr;

#endif