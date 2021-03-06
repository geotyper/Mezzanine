/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2015 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreHlmsShaderPiecesManager.h"
#include "OgreHlmsShaderCommon.h"

namespace Ogre
{
	//-----------------------------------------------------------------------------------
	ShaderPiecesManager::ShaderPiecesManager(const Ogre::String& piecesFolder)
	{

	}
	//-----------------------------------------------------------------------------------
	ShaderPiecesManager::~ShaderPiecesManager()
	{

	}
	//-----------------------------------------------------------------------------------
	void ShaderPiecesManager::enumeratePieceFiles(void)
	{
		// TODO
		/*
		auto stringVectorPtr = mDataFolder->list(false, false);

		Ogre::StringVector stringVectorLowerCase(*stringVectorPtr);

		{
			Ogre::StringVector::iterator itor = stringVectorLowerCase.begin();
			Ogre::StringVector::iterator end = stringVectorLowerCase.end();
			while (itor != end)
			{
				std::transform(itor->begin(), itor->end(), itor->begin(), ::tolower);
				++itor;
			}
		}

		auto size = sizeof(FilePatterns) / sizeof(*FilePatterns);
		for (size_t i = 0; i < size; ++i)
		{
			auto itLowerCase = stringVectorLowerCase.begin();
			auto itor = stringVectorPtr->begin();
			auto end = stringVectorPtr->end();

			while (itor != end)
			{
				if (itLowerCase->find(FilePatterns[i]) != Ogre::String::npos)
					mPieceFiles.push_back(*itor);

				++itLowerCase;
				++itor;
			}
		}

		auto langIt = mPieceFiles.find(languarge);
		if (langIt != mPieceFiles.end())
		{
			return (*langIt).second[shaderType];
		}*/
	}
	//-----------------------------------------------------------------------------------
	Ogre::StringVector& ShaderPiecesManager::getPieces(Ogre::String languarge, Ogre::GpuProgramType shaderType)
	{
		std::map<Ogre::String, Ogre::StringVector[5]>::iterator langIt = mPieceFiles.find(languarge);
		if (langIt != mPieceFiles.end())
		{
			return (*langIt).second[shaderType];
		}

		return mDefaultStringVector;
	}
	//-----------------------------------------------------------------------------------
}
