/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 1.2.0, packaged on September 2009.

 http://glc-lib.sourceforge.net

 GLC-lib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 GLC-lib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GLC-lib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/

//! \file glc_enum.h provide usefull utilities

#ifndef GLC_ENUM_H_
#define GLC_ENUM_H_

#include <QMutex>
#include <QtOpenGL>

#include "glc_config.h"

// Type for UID
typedef unsigned int GLC_uint;

namespace glc
{
	//! Simple ID generation
	GLC_LIB_EXPORT GLC_uint GLC_GenID(void);

	//! Simple Geom ID generation
	GLC_LIB_EXPORT GLC_uint GLC_GenGeomID(void);

	//! Simple User ID generation
	GLC_LIB_EXPORT GLC_uint GLC_GenUserID(void);

	//! Return the GLC_uint decoded ID from RGB encoded ID
	inline GLC_LIB_EXPORT GLC_uint decodeRgbId(const GLubyte*);

	//! Return the encoded color of the id
	inline GLC_LIB_EXPORT void encodeRgbId(GLC_uint, GLubyte*);

	const int GLC_DISCRET= 70;
	const int GLC_POLYDISCRET= 60;

	extern QMutex iDMutex;
	extern QMutex geomIdMutex;
	extern QMutex userIdMutex;
};

// Return the GLC_uint decoded ID from RGBA encoded ID
GLC_uint glc::decodeRgbId(const GLubyte* pcolorId)
{
	GLC_uint returnId= 0;
	returnId|= (GLC_uint)pcolorId[0] << (0 * 8);
	returnId|= (GLC_uint)pcolorId[1] << (1 * 8);
	returnId|= (GLC_uint)pcolorId[2] << (2 * 8);
	// Only get first 24 bits
	//returnId|= (GLC_uint)pcolorId[3] << (3 * 8);

	return returnId;
}

// Encode Id to RGBA color
void glc::encodeRgbId(GLC_uint id, GLubyte* colorId)
{
	colorId[0]= static_cast<GLubyte>((id >> (0 * 8)) & 0xFF);
	colorId[1]= static_cast<GLubyte>((id >> (1 * 8)) & 0xFF);
	colorId[2]= static_cast<GLubyte>((id >> (2 * 8)) & 0xFF);
	colorId[3]= static_cast<GLubyte>((id >> (3 * 8)) & 0xFF);
}

// GLC_Lib version

#define GLC_VERSION "1.2.0"
#define GLC_DESCRIPTION "GLC_lib is a Open Source C++ class library that enables the quick creation of an OpenGL application based on QT4."


#endif //GLC_ENUM_H_


