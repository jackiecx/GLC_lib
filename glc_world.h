/****************************************************************************

 This file is part of the GLC-lib library.
 Copyright (C) 2005-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 0.9.8, packaged on January, 2008.

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

//! \file glc_world.h interface for the GLC_World class.

#ifndef GLC_WORLD_H_
#define GLC_WORLD_H_

#include "glc_collection.h"
#include "glc_product.h"
#include "glc_part.h"

//////////////////////////////////////////////////////////////////////
//! \class GLC_World
/*! \brief GLC_World : */
//////////////////////////////////////////////////////////////////////
class GLC_World
{
//////////////////////////////////////////////////////////////////////
/*! @name Constructor / Destructor */
//@{
//////////////////////////////////////////////////////////////////////	
public:
	//! Default constructor
	GLC_World();
	//! Copy constructor
	GLC_World(const GLC_World&);
	//! Destructor
	virtual ~GLC_World();
//@}

//////////////////////////////////////////////////////////////////////
/*! \name Get Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Get the root product of the world
	inline GLC_Product* rootProduct() const {return m_pRoot;}
	//! Get the world collection
	inline GLC_Collection* collection() {return m_pCollection;}
	//! return true if the world is empty
	inline bool isEmpty() const {return  m_pCollection->isEmpty();}
	//! Get number of faces
	inline int getNumberOfFaces() const {return m_pRoot->getNumberOfFaces();}
	//! Get number of vertex
	inline int getNumberOfVertex() const{return m_pRoot->getNumberOfVertex();}

//@}

//////////////////////////////////////////////////////////////////////
/*! \name Set Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Merge this world with another world
	void mergeWithAnotherWorld(const GLC_World &);
	//! Reverse worlds part normal
	inline void reversePartNormal() {m_pRoot->reverseChildPartNormal();}
//@}
	
//////////////////////////////////////////////////////////////////////
/*! @name Operator Overload */
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Assignement operator
	GLC_World& operator=(const GLC_World&);
//@}

//////////////////////////////////////////////////////////////////////
/*! \name OpenGL Functions*/
//@{
//////////////////////////////////////////////////////////////////////
public:
	//! Display the world
	inline void glExecute(void) {m_pCollection->glExecute();}

//@}
//////////////////////////////////////////////////////////////////////
// private members
//////////////////////////////////////////////////////////////////////
private:
	//! The instance Collection
	GLC_Collection* m_pCollection;
	//! The root of the product structure
	GLC_Product* m_pRoot;	
	//! Number of this world
	int* m_pNumberOfWorld;
	
};

#endif /*GLC_WORLD_H_*/
