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

//! \file glc_cylinder.cpp implementation of the GLC_Cylinder class.

#include "glc_cylinder.h"
#include "../glc_openglexception.h"
#include "../shading/glc_selectionmaterial.h"
#include "../glc_state.h"

#include <QVector>

using namespace glc;
//////////////////////////////////////////////////////////////////////
// Constructor destructor
//////////////////////////////////////////////////////////////////////


GLC_Cylinder::GLC_Cylinder(double dRadius, double dLength)
:GLC_Mesh()
, m_Radius(dRadius)
, m_Length(dLength)
, m_Discret(GLC_POLYDISCRET)	// Default discretion
, m_EndedIsCaped(true)			// Cylinder ended are closed
{
	Q_ASSERT((m_Radius > 0.0) && (m_Length > 0.0));
}

GLC_Cylinder::GLC_Cylinder(const GLC_Cylinder& sourceCylinder)
:GLC_Mesh(sourceCylinder)
, m_Radius(sourceCylinder.m_Radius)
, m_Length(sourceCylinder.m_Length)
, m_Discret(sourceCylinder.m_Discret)
, m_EndedIsCaped(sourceCylinder.m_EndedIsCaped)
{
	Q_ASSERT((m_Radius > 0.0) && (m_Length > 0.0) && (m_Discret > 0));

}
GLC_Cylinder::~GLC_Cylinder()
{

}

//////////////////////////////////////////////////////////////////////
// Get Functions
//////////////////////////////////////////////////////////////////////

// Return a copy of the current geometry
GLC_Geometry* GLC_Cylinder::clone() const
{
	return new GLC_Cylinder(*this);
}

// return the cylinder bounding box
GLC_BoundingBox& GLC_Cylinder::boundingBox()
{
	if (numberOfVertex() == 0)
	{
		createMesh();
	}
	return GLC_Mesh::boundingBox();
}

//////////////////////////////////////////////////////////////////////
// Set Functions
//////////////////////////////////////////////////////////////////////
// Set Cylinder length
void GLC_Cylinder::setLength(double Length)
{
	Q_ASSERT(Length > 0.0);
	m_Length= Length;

	delete m_pBoundingBox;
	m_pBoundingBox= NULL;
	m_GeometryIsValid= false;
}

// Set Cylinder radius
void GLC_Cylinder::setRadius(double Radius)
{
	Q_ASSERT(Radius > 0.0);
	m_Radius= Radius;

	delete m_pBoundingBox;
	m_pBoundingBox= NULL;
	m_GeometryIsValid= false;
}

// Set Discretion
void GLC_Cylinder::setDiscretion(int TargetDiscret)
{
	Q_ASSERT(TargetDiscret > 0);
	if (TargetDiscret != m_Discret)
	{
		m_Discret= TargetDiscret;
		if (m_Discret < 6) m_Discret= 6;

		delete m_pBoundingBox;
		m_pBoundingBox= NULL;

		m_GeometryIsValid= false;
	}
}

// End Caps
void GLC_Cylinder::setEndedCaps(bool CapsEnded)
{
	if (m_EndedIsCaped != CapsEnded)
	{
		m_EndedIsCaped= CapsEnded;

		delete m_pBoundingBox;
		m_pBoundingBox= NULL;

		m_GeometryIsValid= false;

	}
}

//////////////////////////////////////////////////////////////////////
// Private Opengl functions
//////////////////////////////////////////////////////////////////////

// Dessin du GLC_Cylinder
void GLC_Cylinder::glDraw(bool transparent)
{

	if (numberOfVertex() == 0)
	{
		createMesh();
	}

	GLC_Mesh::glDraw(transparent);

}

// Create the cylinder mesh
void GLC_Cylinder::createMesh()
{
	qDebug() << "GLC_Cylinder::createMesh";
	// Clear the mesh of the cylinder
	GLC_Mesh::clearOnlyMesh();

	// Create cosinus and sinus array according to the discretion and radius
	const int vertexNumber= m_Discret + 1;
	// Normals values
	QVector<float> cosNormalArray(vertexNumber);
	QVector<float> sinNormalArray(vertexNumber);

	QVector<float> cosArray(vertexNumber);
	QVector<float> sinArray(vertexNumber);

	const double angle= (2.0 * PI) / static_cast<double>(m_Discret);

	for (int i= 0; i < vertexNumber; ++i)
	{
		const double cosValue= cos(static_cast<double>(i) * angle);
		const double sinValue= sin(static_cast<double>(i) * angle);

		cosNormalArray[i]= static_cast<GLfloat>(cosValue);
		sinNormalArray[i]= static_cast<GLfloat>(sinValue);

		cosArray[i]= static_cast<GLfloat>(m_Radius * cosValue);
		sinArray[i]= static_cast<GLfloat>(m_Radius * sinValue);
	}

	GLfloatVector verticeVector;
	GLfloatVector normalsVector;
	GLfloatVector texelVector;

	if (m_EndedIsCaped)
	{
		const int size= vertexNumber * 4;
		verticeVector.resize(3 * size);
		normalsVector.resize(3 * size);
		texelVector.resize(2 * size);
	}
	else
	{
		const int size= vertexNumber * 2;
		verticeVector.resize(3 * size);
		normalsVector.resize(3 * size);
		texelVector.resize(2 * size);
	}
	for (int i= 0; i < vertexNumber; ++i)
	{
		// Bottom
		verticeVector[3 * i]= cosArray[i];
		verticeVector[3 * i + 1]= sinArray[i];
		verticeVector[3 * i + 2]= 0.0f;

		normalsVector[3 * i]= cosNormalArray[i];
		normalsVector[3 * i + 1]= sinNormalArray[i];
		normalsVector[3 * i + 2]= 0.0f;

		texelVector[2 * i]= static_cast<float>(i) / static_cast<float>(m_Discret);
		texelVector[2 * i + 1]= 0.0f;

		// Top
		verticeVector[3 * i + 3 * vertexNumber]= cosArray[i];
		verticeVector[3 * i + 1 + 3 * vertexNumber]= sinArray[i];
		verticeVector[3 * i + 2 + 3 * vertexNumber]= static_cast<float>(m_Length);

		normalsVector[3 * i + 3 * vertexNumber]= cosNormalArray[i];
		normalsVector[3 * i + 1 + 3 * vertexNumber]= sinNormalArray[i];
		normalsVector[3 * i + 2 + 3 * vertexNumber]= 0.0f;

		texelVector[2 * i + 2 * vertexNumber]= texelVector[i];
		texelVector[2 * i + 1 + 2 * vertexNumber]= 1.0f;

		if (m_EndedIsCaped)
		{
			// Bottom Cap ends
			verticeVector[3 * i + 2 * 3 * vertexNumber]= cosArray[i];
			verticeVector[3 * i + 1 + 2 * 3 * vertexNumber]= sinArray[i];
			verticeVector[3 * i + 2 + 2 * 3 * vertexNumber]= 0.0f;

			normalsVector[3 * i + 2 * 3 * vertexNumber]= 0.0f;
			normalsVector[3 * i + 1 + 2 * 3 * vertexNumber]= 0.0f;
			normalsVector[3 * i + 2 + 2 * 3 * vertexNumber]= -1.0f;

			texelVector[2 * i + 2 * 2 * vertexNumber]= texelVector[i];
			texelVector[2 * i + 1 + 2 * 2 * vertexNumber]= 0.0f;

			// Top Cap ends
			verticeVector[3 * i + 3 * 3 * vertexNumber]= cosArray[i];
			verticeVector[3 * i + 1 + 3 * 3 * vertexNumber]= sinArray[i];
			verticeVector[3 * i + 2 + 3 * 3 * vertexNumber]= static_cast<float>(m_Length);

			normalsVector[3 * i + 3 * 3 * vertexNumber]= 0.0f;
			normalsVector[3 * i + 1 + 3 * 3 * vertexNumber]= 0.0f;
			normalsVector[3 * i + 2 + 3 * 3 * vertexNumber]= 1.0f;

			texelVector[2 * i + 3 * 2 * vertexNumber]= texelVector[i];
			texelVector[2 * i + 1 + 3 * 2 * vertexNumber]= 0.0f;
		}
	}

	// Add bulk data in to the mesh
	addVertices(verticeVector);
	addNormals(normalsVector);
	addTexels(texelVector);

	// Set the material to use
	GLC_Material* pCylinderMaterial;
	if (hasMaterial())
	{
		pCylinderMaterial= this->firstMaterial();
	}
	else
	{
		pCylinderMaterial= new GLC_Material();
	}

	IndexList circumferenceStrips;
	// Create the index
	for (int i= 0; i < vertexNumber; ++i)
	{
		circumferenceStrips.append(i);
		circumferenceStrips.append(i + vertexNumber);
	}
	addTrianglesStrip(pCylinderMaterial, circumferenceStrips);

	if (m_EndedIsCaped)
	{
		IndexList bottomCap;
		IndexList topCap;
		int id1= 0;
		int id2= m_Discret - 1;
		const int size= m_Discret / 2 + (m_Discret % 2);
		for (int i= 0; i < size; ++i)
		{
			bottomCap.append(id1 + 2 * vertexNumber);
			topCap.append(id1 + 3 * vertexNumber);

			bottomCap.append(id2 + 2 * vertexNumber);
			topCap.append(id2 + 3 * vertexNumber);

			id1+= 1;
			id2-= 1;
		}
		addTrianglesStrip(pCylinderMaterial, bottomCap);
		addTrianglesStrip(pCylinderMaterial, topCap);
	}

	finish();
}
