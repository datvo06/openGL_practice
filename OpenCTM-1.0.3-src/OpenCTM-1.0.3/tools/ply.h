//-----------------------------------------------------------------------------
// Product:     OpenCTM tools
// File:        ply.h
// Description: Interface for the PLY file format importer/exporter.
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

#ifndef __PLY_H_
#define __PLY_H_

#include "mesh.h"
#include "convoptions.h"

namespace CTM{
	/// Import a PLY file from a file.
	void Import_PLY(const char * aFileName, CTM::Mesh * aMesh);

	/// Export a PLY file to a file.
	void Export_PLY(const char * aFileName, CTM::Mesh * aMesh, Options &aOptions);
}

#endif // __PLY_H_