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

#include "OgreHlmsShaderManager.h"
#include "OgreHlmsShaderGenerator.h"
#include "OgreHlmsShaderPiecesManager.h"
#include "OgreHlmsDatablock.h"
#include "OgreHlmsShaderCommon.h"

namespace Ogre
{
	//-----------------------------------------------------------------------------------
	ShaderManager::ShaderManager()
	{
		mShaderPiecesManager = new ShaderPiecesManager("c:/");
	}
	//-----------------------------------------------------------------------------------
	ShaderManager::~ShaderManager()
	{
		delete mShaderPiecesManager;
		mShaderPiecesManager = NULL;
	}
	//-----------------------------------------------------------------------------------
	Ogre::GpuProgramPtr ShaderManager::getGpuProgram(HlmsDatablock* dataBlock)
	{
		Ogre::uint32 hash = dataBlock->getHash();

		std::map<Ogre::uint32, Ogre::GpuProgramPtr>::iterator it = mShaderCache.find(hash);
		if (it != mShaderCache.end())
		{
			return (*it).second;
		}
		else
		{
			Ogre::String typeStr = FilePatterns[dataBlock->getShaderType()];

			std::stringstream sstream;
			sstream << std::hex << hash;
			std::string hashString = sstream.str();

			Ogre::String name = hashString + typeStr;

			// generate the shader code
			Ogre::String code = dataBlock->getTemplate()->getTemplate();
			code = ShaderGenerator::parse(code, *(dataBlock->getPropertyMap()), mShaderPiecesManager->getPieces(dataBlock->getLanguarge(), dataBlock->getShaderType()));

			HighLevelGpuProgramPtr gpuProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(name,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, dataBlock->getLanguarge(), dataBlock->getShaderType());

			gpuProgram->setSource(code);
			gpuProgram->setParameter("entry_point", "main");

			if (dataBlock->getLanguarge() == "hlsl")
			{
				// HLSL program requires specific target profile settings - we have to split the profile string.
				const Ogre::StringVector& profilesList = dataBlock->getProfileList();
				Ogre::StringVector::const_iterator it = profilesList.begin();
				Ogre::StringVector::const_iterator itEnd = profilesList.end();

				for (; it != itEnd; ++it)
				{
					if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported(*it))
					{
						gpuProgram->setParameter("target", *it);
						break;
					}
				}

				//gpuProgram->setParameter("enable_backwards_compatibility", "false");
				//gpuProgram->setParameter("column_major_matrices", Ogre::StringConverter::toString(shaderProgram->getUseColumnMajorMatrices()));
			}

			//gpuProgram->setParameter("profiles", profiles);
			gpuProgram->load();

			// Case an error occurred.
			if (gpuProgram->hasCompileError())
			{
				gpuProgram.reset();
				return Ogre::GpuProgramPtr(gpuProgram);
			}

			mShaderCache[hash] = gpuProgram;

			return gpuProgram;
		}

	}
	//-----------------------------------------------------------------------------------
	void clearCache()
	{
		// TODO
		//mShaderCache
	}
    //-----------------------------------------------------------------------------------
	/*
    const HlmsCache* Hlms::createShaderCacheEntry( uint32 renderableHash, const HlmsCache &passCache,
                                                   uint32 finalHash,
                                                   const QueuedRenderable &queuedRenderable )
    {
        //Set the properties by merging the cache from the pass, with the cache from renderable
        mSetProperties.clear();
        //If retVal is null, we did something wrong earlier
        //(the cache should've been generated by now)
        const RenderableCache &renderableCache = getRenderableCache( renderableHash );
        mSetProperties.reserve( passCache.setProperties.size() + renderableCache.setProperties.size() );
        //Copy the properties from the renderable
        mSetProperties.insert( mSetProperties.end(), renderableCache.setProperties.begin(),
                                                     renderableCache.setProperties.end() );
        {
            //Now copy the properties from the pass (one by one, since be must maintain the order)
            HlmsPropertyVec::const_iterator itor = passCache.setProperties.begin();
            HlmsPropertyVec::const_iterator end  = passCache.setProperties.end();

            while( itor != end )
            {
                setProperty( itor->keyName, itor->value );
                ++itor;
            }
        }

        GpuProgramPtr shaders[NumShaderTypes];
        //Generate the shaders
        for( size_t i=0; i<NumShaderTypes; ++i )
        {
            //Collect pieces
            mPieces = renderableCache.pieces[i];

            if( mShaderProfile == "glsl" ) //TODO: String comparision
                setProperty( HlmsBaseProp::GL3Plus, 330 );

            setProperty( HlmsBaseProp::HighQuality, mHighQuality );

            StringVector::const_iterator itor = mPieceFiles[i].begin();
            StringVector::const_iterator end  = mPieceFiles[i].end();

            while( itor != end )
            {
                DataStreamPtr inFile = mDataFolder->open( *itor );

                String inString;
                String outString;

                inString.resize( inFile->size() );
                inFile->read( &inString[0], inFile->size() );

                this->parseMath( inString, outString );
                this->parseForEach( outString, inString );
                this->parseProperties( inString, outString );
                this->collectPieces( outString, inString );
                ++itor;
            }

            //Generate the shader file. TODO: Identify the file extension at runtime
            const String filename = ShaderFiles[i] + ".glsl";
            if( mDataFolder->exists( filename ) )
            {
                DataStreamPtr inFile = mDataFolder->open( filename );

                String inString;
                String outString;

                inString.resize( inFile->size() );
                inFile->read( &inString[0], inFile->size() );

                this->parseMath( inString, outString );
                this->parseForEach( outString, inString );
                this->parseProperties( inString, outString );
                this->collectPieces( outString, inString );
                this->insertPieces( inString, outString );
                this->parseCounter( outString, inString );

                outString.swap( inString );

                if( mDebugOutput )
                {
                    std::ofstream outFile( (mOutputPath + "./" +
                                           StringConverter::toString( finalHash ) +
                                           ShaderFiles[i]).c_str(),
                                           std::ios::out | std::ios::binary );
                    outFile.write( &outString[0], outString.size() );
                }

                HighLevelGpuProgramManager *gpuProgramManager =
                                                        HighLevelGpuProgramManager::getSingletonPtr();

                HighLevelGpuProgramPtr gp = gpuProgramManager->createProgram(
                                    StringConverter::toString( finalHash ) + ShaderFiles[i],
                                    ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME,
                                    mShaderProfile, static_cast<GpuProgramType>(i) );
                gp->setSource( outString );

                gp->setSkeletalAnimationIncluded( getProperty( HlmsBaseProp::Skeleton ) != 0 );
                gp->setMorphAnimationIncluded( false );
                gp->setPoseAnimationIncluded( getProperty( HlmsBaseProp::Pose ) );
                gp->setVertexTextureFetchRequired( false );

                gp->load();

                shaders[i] = gp;
            }
        }

        const HlmsCache* retVal = addShaderCache( finalHash, shaders[VertexShader],
                                                  shaders[GeometryShader], shaders[HullShader],
                                                  shaders[DomainShader], shaders[PixelShader] );
        return retVal;
    }
    //-----------------------------------------------------------------------------------
    void Hlms::calculateHashFor( Renderable *renderable, uint32 &outHash, uint32 &outCasterHash )
    {
        mSetProperties.clear();

        setProperty( HlmsBaseProp::Skeleton, renderable->hasSkeletonAnimation() );

        RenderOperation op;
        renderable->getRenderOperation( op );
        VertexDeclaration *vertexDecl = op.vertexData->vertexDeclaration;
        const VertexDeclaration::VertexElementList &elementList = vertexDecl->getElements();
        VertexDeclaration::VertexElementList::const_iterator itor = elementList.begin();
        VertexDeclaration::VertexElementList::const_iterator end  = elementList.end();

        uint numTexCoords = 0;
        while( itor != end )
        {
            const VertexElement &vertexElem = *itor;
            switch( vertexElem.getSemantic() )
            {
            case VES_NORMAL:
                if( VertexElement::getTypeCount( vertexElem.getType() ) < 4 )
                {
                    setProperty( HlmsBaseProp::Normal, 1 );
                }
                else
                {
                    setProperty( HlmsBaseProp::QTangent, 1 );
                }
                break;
            case VES_TANGENT:
                setProperty( HlmsBaseProp::Tangent, 1 );
                break;
            case VES_DIFFUSE:
                setProperty( HlmsBaseProp::Colour, 1 );
                break;
            case VES_TEXTURE_COORDINATES:
                numTexCoords = std::max<uint>( numTexCoords, vertexElem.getIndex() + 1 );
                setProperty( *HlmsBaseProp::UvCountPtrs[vertexElem.getIndex()],
                              VertexElement::getTypeCount( vertexElem.getType() ) );
                break;
            case VES_BLEND_WEIGHTS:
                setProperty( HlmsBaseProp::BonesPerVertex,
                             VertexElement::getTypeCount( vertexElem.getType() ) );
                break;
            default:
                break;
            }

            vertexElem.getType();
            ++itor;
        }

        setProperty( HlmsBaseProp::UvCount, numTexCoords );

        HlmsDatablock *datablock = renderable->getDatablock();

        setProperty( HlmsBaseProp::AlphaTest, datablock->getAlphaTest() );

        PiecesMap pieces[NumShaderTypes];
        calculateHashForPreCreate( renderable, pieces );

        uint32 renderableHash = this->addRenderableCache( mSetProperties, pieces );

        //For shadow casters, turn normals off. UVs & diffuse also off unless there's alpha testing.
        setProperty( HlmsBaseProp::Normal, 0 );
        setProperty( HlmsBaseProp::QTangent, 0 );
        if( !datablock->getAlphaTest() )
        {
            setProperty( HlmsBaseProp::UvCount, 0 );
        }
        PiecesMap piecesCaster[NumShaderTypes];
        calculateHashForPreCaster( renderable, piecesCaster );
        uint32 renderableCasterHash = this->addRenderableCache( mSetProperties, piecesCaster );

        outHash         = renderableHash;
        outCasterHash   = renderableCasterHash;
    }
    //-----------------------------------------------------------------------------------
    HlmsCache Hlms::preparePassHash( const CompositorShadowNode *shadowNode, bool casterPass,
                                     bool dualParaboloid, SceneManager *sceneManager )
    {
        mSetProperties.clear();

        if( !casterPass )
        {
            if( shadowNode )
            {
                int32 numPssmSplits = 0;
                const vector<Real>::type *pssmSplits = shadowNode->getPssmSplits( 0 );
                if( pssmSplits )
                    numPssmSplits = static_cast<int32>( pssmSplits->size() - 1 );
                setProperty( HlmsBaseProp::PssmSplits, numPssmSplits );

                size_t numShadowMaps = shadowNode->getNumShadowCastingLights();
                if( numPssmSplits )
                    numShadowMaps += numPssmSplits - 1;
                setProperty( HlmsBaseProp::NumShadowMaps, numShadowMaps );
            }

            uint numLightsPerType[Light::NUM_LIGHT_TYPES];
            memset( numLightsPerType, 0, sizeof( numLightsPerType ) );

            if( mLightGatheringMode == LightGatherForwardPlus )
            {
                if( shadowNode )
                {
                    //Gather shadow casting lights, regardless of their type.
                    const LightClosestArray &lights = shadowNode->getShadowCastingLights();
                    LightClosestArray::const_iterator itor = lights.begin();
                    LightClosestArray::const_iterator end  = lights.end();
                    while( itor != end )
                    {
                        ++numLightsPerType[itor->light->getType()];
                        ++itor;
                    }
                }

                //Always gather directional lights.
                numLightsPerType[Light::LT_DIRECTIONAL] = 0;
                {
                    const LightListInfo &globalLightList = sceneManager->getGlobalLightList();
                    LightArray::const_iterator itor = globalLightList.lights.begin();
                    LightArray::const_iterator end  = globalLightList.lights.end();

                    while( itor != end )
                    {
                        if( (*itor)->getType() == Light::LT_DIRECTIONAL )
                            ++numLightsPerType[Light::LT_DIRECTIONAL];
                        ++itor;
                    }
                }
            }
            else if( mLightGatheringMode == LightGatherForward )
            {
                //Gather all lights.
                const LightListInfo &globalLightList = sceneManager->getGlobalLightList();
                LightArray::const_iterator itor = globalLightList.lights.begin();
                LightArray::const_iterator end  = globalLightList.lights.end();

                size_t numTotalLights = 0;

                while( itor != end && numTotalLights < mNumLightsLimit )
                {
                    ++numLightsPerType[(*itor)->getType()];
                    ++numTotalLights;
                    ++itor;
                }
            }

            setProperty( HlmsBaseProp::LightsAttenuation, numLightsPerType[Light::LT_POINT] +
                                                          numLightsPerType[Light::LT_SPOTLIGHT] );
            setProperty( HlmsBaseProp::LightsSpotParams,  numLightsPerType[Light::LT_SPOTLIGHT] );


            numLightsPerType[Light::LT_POINT]       += numLightsPerType[Light::LT_DIRECTIONAL];
            numLightsPerType[Light::LT_SPOTLIGHT]   += numLightsPerType[Light::LT_POINT];

            //The value is cummulative for each type (order: Directional, point, spot)
            setProperty( HlmsBaseProp::LightsDirectional, numLightsPerType[Light::LT_DIRECTIONAL] );
            setProperty( HlmsBaseProp::LightsPoint,       numLightsPerType[Light::LT_POINT] );
            setProperty( HlmsBaseProp::LightsSpot,        numLightsPerType[Light::LT_SPOTLIGHT] );
        }
        else
        {
            setProperty( HlmsBaseProp::ShadowCaster, casterPass );
            setProperty( HlmsBaseProp::DualParaboloidMapping, dualParaboloid );

            setProperty( HlmsBaseProp::NumShadowMaps, 0 );
            setProperty( HlmsBaseProp::PssmSplits, 0 );
            setProperty( HlmsBaseProp::LightsAttenuation, 0 );
            setProperty( HlmsBaseProp::LightsSpotParams,  0 );
            setProperty( HlmsBaseProp::LightsDirectional, 0 );
            setProperty( HlmsBaseProp::LightsPoint,       0 );
            setProperty( HlmsBaseProp::LightsSpot,        0 );
        }

        uint32 hash = getProperty( HlmsBaseProp::DualParaboloidMapping ) |
                (getProperty( HlmsBaseProp::NumShadowMaps )         << 1 )|
                (getProperty( HlmsBaseProp::PssmSplits )            << 5 )|
                (getProperty( HlmsBaseProp::LightsDirectional )     << 8 )|
                (getProperty( HlmsBaseProp::LightsPoint )           << 12)|
                (getProperty( HlmsBaseProp::LightsSpot )            << 16)|
                (getProperty( HlmsBaseProp::ShadowCaster )          << 20);

        HlmsCache retVal( hash, mType );
        retVal.setProperties = mSetProperties;

        return retVal;
    }
    //-----------------------------------------------------------------------------------
    const HlmsCache* Hlms::getMaterial( HlmsCache const *lastReturnedValue,
                                        const HlmsCache &passCache,
                                        const QueuedRenderable &queuedRenderable, bool casterPass )
    {
        uint32 finalHash;
        uint32 hash[2];
        hash[0] = casterPass ? queuedRenderable.renderable->getHlmsCasterHash() :
                               queuedRenderable.renderable->getHlmsHash();
        hash[1] = passCache.hash &
                        (queuedRenderable.movableObject->getCastShadows() ? 0xffffffff : 0xffffffe1 );

        MurmurHash3_x86_32( hash, sizeof( hash ), IdString::Seed, &finalHash );

        if( lastReturnedValue->hash != finalHash )
        {
            lastReturnedValue = this->getShaderCache( finalHash );

            if( !lastReturnedValue )
            {
                lastReturnedValue = createShaderCacheEntry( hash[0], passCache, finalHash,
                                                            queuedRenderable );
            }
        }

        return lastReturnedValue;
    }*/

}
