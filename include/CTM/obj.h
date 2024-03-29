//-----------------------------------------------------------------------------
// Product:     OpenCTM tools
// File:        obj.h
// Description: Interface for the OBJ file format importer/exporter.
//-----------------------------------------------------------------------------
// Copyright (c) 2009-2010 Marcus Geelnard
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not
//     be misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//     distribution.
//-----------------------------------------------------------------------------

#ifndef __OBJ_H_
#define __OBJ_H_

#include "mesh.h"
#include "convoptions.h"

namespace CTM{
	/// Import a mesh from an OBJ file.
	void Import_OBJ(const char * aFileName, CTM::Mesh * aMesh);

	/// Export a mesh to an OBJ file.
	void Export_OBJ(const char * aFileName, CTM::Mesh * aMesh, CTM::Options &aOptions);
}

#endif // __OBJ_H_
