// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

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
#ifndef _graphicsproceduraltrianglebuffer_cpp
#define _graphicsproceduraltrianglebuffer_cpp

#include "Graphics/Procedural/Mesh/trianglebuffer.h"

#include "Graphics/meshmanager.h"
#include "Graphics/mesh.h"

#include "Ogre.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            TriangleBuffer::TriangleBuffer() :
                CurrentVertex(NULL),
                EstimatedVertexCount(0),
                EstimatedIndexCount(0),
                GlobalOffset(0)
                {  }

            TriangleBuffer::~TriangleBuffer()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void TriangleBuffer::AppendBuffer(const TriangleBuffer& Other)
            {
                this->RebaseOffset();
                for( ConstIndexIterator IndIt = Other.Indices.begin() ; IndIt != Other.Indices.end() ; ++IndIt )
                    { this->Indices.push_back( this->GlobalOffset + (*IndIt) ); }

                for( ConstVertexIterator VertIt = Other.Vertices.begin() ; VertIt != Other.Vertices.end() ; ++VertIt )
                    { this->Vertices.push_back( *VertIt ); }
            }

            void TriangleBuffer::AppendBufferAsSection(const TriangleBuffer& Other, const String& MatName, const String& MatGroup, const Whole RenderOp)
            {
                this->BeginSection(MatName,MatGroup,RenderOp);
                this->AppendBuffer(Other);
                this->EndSection();
            }

            Mesh* TriangleBuffer::GenerateMesh(const String& MeshName, const String& MeshGroup, const String& MatName, const String& MatGroup) const
            {
                Ogre::ManualObject* TempMan = new Ogre::ManualObject("TempMan");

                if( this->IsUsingSections() ) {
                    for( ConstSectionIterator SectIt = this->Sections.begin() ; SectIt != this->Sections.end() ; ++SectIt )
                    {
                        TempMan->begin( (*SectIt).MaterialName, Ogre::RenderOperation::OT_TRIANGLE_LIST, (*SectIt).MaterialGroup );
                        const ConstVertexIterator VertEndIt = this->Vertices.begin() + (*SectIt).LastVertex;
                        for( ConstVertexIterator VertIt = this->Vertices.begin() + (*SectIt).FirstVertex ; VertIt != VertEndIt ; ++VertIt )
                        {
                            TempMan->position( VertIt->Position.GetOgreVector3() );
                            TempMan->textureCoord( VertIt->UV.GetOgreVector2() );
                            TempMan->normal( VertIt->Normal.GetOgreVector3() );
                        }
                        const ConstIndexIterator IndicEndIt = this->Indices.begin() + (*SectIt).LastIndex;
                        for( ConstIndexIterator IndicIt = this->Indices.begin() + (*SectIt).FirstIndex ; IndicIt != IndicEndIt ; ++IndicIt )
                            { TempMan->index( *IndicIt ); }
                        TempMan->end();
                    }
                }else{
                    TempMan->begin(MatName,Ogre::RenderOperation::OT_TRIANGLE_LIST,MatGroup);
                    for( ConstVertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                    {
                        TempMan->position( VertIt->Position.GetOgreVector3() );
                        TempMan->textureCoord( VertIt->UV.GetOgreVector2() );
                        TempMan->normal( VertIt->Normal.GetOgreVector3() );
                    }
                    for( ConstIndexIterator IndIt = this->Indices.begin() ; IndIt != this->Indices.end() ; ++IndIt )
                        { TempMan->index( *IndIt ); }
                    TempMan->end();
                }

                // ManualObjects will automatically pad AABBs that they generate.  This can be circumvented by changing some parameters
                // but those are internal to the method we are calling with no control.  So we must save the AABB and set it after creation.
                Ogre::AxisAlignedBox MeshAABB = TempMan->getBoundingBox();
                Mesh* NewMesh = MeshManager::GetSingletonPtr()->_WrapInternalMesh( TempMan->convertToMesh(MeshName,MeshGroup) );
                NewMesh->_GetInternalMesh()->_setBounds(MeshAABB,false);
                delete TempMan;
                return NewMesh;
            }

            void TriangleBuffer::EstimateVertexCount(const Whole VertexCount)
            {
                this->EstimatedVertexCount += VertexCount;
                this->Vertices.reserve( this->EstimatedVertexCount );
            }

            void TriangleBuffer::EstimateIndexCount(const Whole IndexCount)
            {
                this->EstimatedIndexCount += IndexCount;
                this->Indices.reserve( this->EstimatedIndexCount );
            }

            void TriangleBuffer::RebaseOffset()
                { this->GlobalOffset = Vertices.size(); }

            TriangleBuffer::SectionContainer& TriangleBuffer::GetSections()
                { return this->Sections; }

            const TriangleBuffer::SectionContainer& TriangleBuffer::GetSections() const
                { return this->Sections; }

            VertexContainer& TriangleBuffer::GetVertices()
                { return this->Vertices; }

            const VertexContainer& TriangleBuffer::GetVertices() const
                { return this->Vertices; }

            IndexContainer& TriangleBuffer::GetIndices()
                { return this->Indices; }

            const IndexContainer& TriangleBuffer::GetIndices() const
                { return this->Indices; }

            ///////////////////////////////////////////////////////////////////////////////
            // Section Utility

            void TriangleBuffer::BeginSection(const String& MatName, const String& MatGroup, const Whole RenderOp)
            {
                // Close any sections we may be working on.
                this->EndSection();
                // If this check succeeds then we have no sections and are starting a new one with data that needs to be in a section.
                if( this->Sections.empty() && !this->Vertices.empty() ) {
                    this->CurrentSection.FirstIndex = 0;
                    this->CurrentSection.FirstVertex = 0;
                    this->CurrentSection.RenderOp = Graphics::RO_TriangleList;
                    this->CurrentSection.LastIndex = this->Indices.size() - 1;
                    this->CurrentSection.LastVertex = this->Vertices.size() - 1;

                    this->Sections.push_back( this->CurrentSection );
                    this->CurrentSection.Clear();
                }

                this->RebaseOffset();
                this->CurrentSection.FirstIndex = this->Indices.size();
                this->CurrentSection.FirstVertex = this->Vertices.size();
                this->CurrentSection.RenderOp = RenderOp;
                this->CurrentSection.MaterialName = MatName;
                this->CurrentSection.MaterialGroup = MatGroup;
            }

            void TriangleBuffer::EndSection()
            {
                if( this->IsWorkingOnSection() ) {
                    this->CurrentSection.LastIndex = this->Indices.size() - 1;
                    this->CurrentSection.LastVertex = this->Vertices.size() - 1;

                    this->Sections.push_back( this->CurrentSection );
                    this->CurrentSection.Clear();
                }
            }

            Boole TriangleBuffer::IsUsingSections() const
                { return ( !this->Sections.empty() || this->IsWorkingOnSection() ); }

            Boole TriangleBuffer::IsWorkingOnSection() const
                { return ( this->CurrentSection.FirstVertex != 0 && this->CurrentSection.FirstIndex != 0 ); }

            ///////////////////////////////////////////////////////////////////////////////
            // Vertex Management

            TriangleBuffer& TriangleBuffer::AddVertex(const Vertex& Vert)
            {
                this->Vertices.push_back(Vert);
                this->CurrentVertex = &this->Vertices.back();
                return *this;
            }

            TriangleBuffer& TriangleBuffer::AddVertex(const Vector3& Loc, const Vector3& Norm, const Vector2& UV)
            {
                Vertex Vert;
                Vert.Position = Loc;
                Vert.Normal = Norm;
                Vert.UV = UV;
                this->Vertices.push_back(Vert);
                this->CurrentVertex = &this->Vertices.back();
                return *this;
            }

            TriangleBuffer& TriangleBuffer::AddVertex(const Vector3& Loc)
            {
                Vertex Vert;
                Vert.Position = Loc;
                this->Vertices.push_back(Vert);
                this->CurrentVertex = &this->Vertices.back();
                return *this;
            }

            TriangleBuffer& TriangleBuffer::AddVertex(const Real X, const Real Y, const Real Z)
            {
                Vertex Vert;
                Vert.Position.SetValues(X,Y,Z);
                this->Vertices.push_back(Vert);
                this->CurrentVertex = &this->Vertices.back();
                return *this;
            }

            TriangleBuffer& TriangleBuffer::SetNormal(const Vector3& Norm)
            {
                this->CurrentVertex->Normal = Norm;
                return *this;
            }

            TriangleBuffer& TriangleBuffer::SetTextureCoord(const Real U, const Real V)
            {
                this->CurrentVertex->UV.SetValues(U,V);
                return *this;
            }

            TriangleBuffer& TriangleBuffer::SetTextureCoord(const Vector2& UV)
            {
                this->CurrentVertex->UV = UV;
                return *this;
            }

            TriangleBuffer& TriangleBuffer::ApplyTransform(const Matrix4x4& Mat)
            {
                for( VertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                {
                    VertIt->Position = Mat * VertIt->Position;
                    VertIt->Normal = Mat * VertIt->Normal;
                    VertIt->Normal.Normalize();
                }
                return *this;
            }

            TriangleBuffer& TriangleBuffer::TranslateVertices(const Vector3& Trans)
            {
                for( VertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                {
                    VertIt->Position = VertIt->Position + Trans;
                }
                return *this;
            }

            TriangleBuffer& TriangleBuffer::TranslateVertices(const Real X, const Real Y, const Real Z)
            {
                return this->TranslateVertices( Vector3(X,Y,Z) );
            }

            TriangleBuffer& TriangleBuffer::RotateVertices(const Quaternion& Rot)
            {
                for( VertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                {
                    VertIt->Position = Rot * VertIt->Position;
                    VertIt->Normal = Rot * VertIt->Normal;
                    VertIt->Normal.Normalize();
                }
                return *this;
            }

            TriangleBuffer& TriangleBuffer::ScaleVertices(const Vector3& Scale)
            {
                for( VertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                {
                    VertIt->Position = Scale * VertIt->Position;
                }
                return *this;
            }

            TriangleBuffer& TriangleBuffer::ScaleVertices(const Real X, const Real Y, const Real Z)
            {
                return this->ScaleVertices( Vector3(X,Y,Z) );
            }

            TriangleBuffer& TriangleBuffer::InvertNormals()
            {
                for( VertexIterator VertIt = this->Vertices.begin() ; VertIt != this->Vertices.end() ; ++VertIt )
                {
                    VertIt->Normal = -(VertIt->Normal);
                }
                for( Whole Index = 0 ; Index < this->Indices.size() ; ++Index )
                {
                    if( Index % 3 == 1 ) {
                        std::swap(this->Indices[Index],this->Indices[Index - 1]);
                    }
                }
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Index Management

            TriangleBuffer& TriangleBuffer::AddIndex(const Integer Index)
            {
                this->Indices.push_back( this->GlobalOffset + Index );
                return *this;
            }

            TriangleBuffer& TriangleBuffer::AddTriangle(const Integer Index1, const Integer Index2, const Integer Index3)
            {
                this->Indices.push_back( this->GlobalOffset + Index1 );
                this->Indices.push_back( this->GlobalOffset + Index2 );
                this->Indices.push_back( this->GlobalOffset + Index3 );
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
