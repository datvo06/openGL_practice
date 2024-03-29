//-----------------------------------------------------------------------------
// Product:     OpenCTM tools
// File:        convoptions.h
// Description: Interface for the conversion options class.
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

#ifndef __CONVOPTIONS_H_
#define __CONVOPTIONS_H_

#include <string>
#include <openctm.h>


namespace CTM{
	typedef enum {
		uaX, uaY, uaZ, uaNX, uaNY, uaNZ
	} UpAxis;

	class Options {
		public:
			/// Constructor
			Options();

			/// Get options from the command line arguments
			void GetFromArgs(int argc, char **argv, int aStartIdx);

			CTMfloat mScale;
			UpAxis mUpAxis;
			bool mFlipTriangles;
			bool mCalcNormals;
			bool mNoNormals;
			bool mNoTexCoords;
			bool mNoColors;

			CTMenum mMethod;
			CTMuint mLevel;

			CTMfloat mVertexPrecision;
			CTMfloat mVertexPrecisionRel;
			CTMfloat mNormalPrecision;
			CTMfloat mTexMapPrecision;
			CTMfloat mColorPrecision;

			std::string mComment;
			std::string mTexFileName;
	};
}

#endif // __CONVOPTIONS_H_
