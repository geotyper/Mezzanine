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
#ifndef _sphere_cpp
#define _sphere_cpp

/// @file
/// @brief This file contains the implementation for the generic Sphere class for math and spacial query.

#include "sphere.h"
#include "MathTools/mathtools.h"
#include "axisalignedbox.h"
#include "plane.h"
#include "ray.h"
#include "linesegment.h"
#include "exception.h"
#include "serialization.h"

#include <OgreSphere.h>

namespace Mezzanine
{
    Sphere::Sphere() :
        Radius(0)
        {  }

    Sphere::Sphere(const Real SphereRadius) :
        Radius(SphereRadius)
        {  }

    Sphere::Sphere(const Vector3& SphereCenter, const Real SphereRadius) :
        Center(SphereCenter),
        Radius(SphereRadius)
        {  }

    Sphere::Sphere(const Ogre::Sphere& InternalSphere)
        { this->ExtractOgreSphere(InternalSphere); }

    Sphere::~Sphere()
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Boole Sphere::IsInside(const Vector3& ToCheck) const
        { return MathTools::IsInside(*this,ToCheck); }

    Boole Sphere::IsOverlapping(const Sphere& ToCheck) const
        { return MathTools::Overlap(*this,ToCheck); }

    Boole Sphere::IsOverlapping(const AxisAlignedBox& ToCheck) const
        { return MathTools::Overlap(ToCheck,*this); }

    Boole Sphere::IsOverlapping(const Plane& ToCheck) const
        { return MathTools::Overlap(ToCheck,*this); }

    Sphere::RayTestResult Sphere::Intersects(const Ray& ToCheck) const
        { return MathTools::Intersects(*this,ToCheck); }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Methods

    void Sphere::ExtractOgreSphere(const Ogre::Sphere& InternalSphere)
        { this->Center = InternalSphere.getCenter();  this->Radius = InternalSphere.getRadius(); }

    Ogre::Sphere Sphere::GetOgreSphere() const
        { return Ogre::Sphere(this->Center.GetOgreVector3(),this->Radius); }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void Sphere::ProtoSerialize(XML::Node& ParentNode) const
    {
        XML::Node SelfRoot = ParentNode.AppendChild( Sphere::GetSerializableName() );

        if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
            SelfRoot.AppendAttribute("Radius").SetValue( this->Radius ) )
        {
            XML::Node CenterNode = SelfRoot.AppendChild("Center");
            this->Center.ProtoSerialize( CenterNode );

            return;
        }else{
            SerializeError("Create XML Attribute Values",Sphere::GetSerializableName(),true);
        }
    }

    void Sphere::ProtoDeSerialize(const XML::Node& SelfRoot)
    {
        XML::Attribute CurrAttrib;

        if( String(SelfRoot.Name()) == Sphere::GetSerializableName() ) {
            if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                CurrAttrib = SelfRoot.GetAttribute("Radius");
                if( !CurrAttrib.Empty() )
                    this->Radius = CurrAttrib.AsReal();

                // Get the properties that need their own nodes
                XML::Node CenterNode = SelfRoot.GetChild("Center").GetFirstChild();
                if( !CenterNode.Empty() ) {
                    Vector3 Cen(CenterNode);
                    this->Center = Cen;
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Sphere::GetSerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Sphere::GetSerializableName() + " was not found in the provided XML node, which was expected.");
        }
    }

    String Sphere::GetSerializableName()
    {
        return "Sphere";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    void Sphere::operator=(const Ogre::Sphere& InternalSphere)
        { this->ExtractOgreSphere(InternalSphere); }

    Boole Sphere::operator>(const Sphere& Other) const
        { return ( this->Radius > Other.Radius ); }

    Boole Sphere::operator<(const Sphere& Other) const
        { return ( this->Radius < Other.Radius ); }

    Boole Sphere::operator>=(const Sphere& Other) const
        { return ( this->Radius >= Other.Radius ); }

    Boole Sphere::operator<=(const Sphere& Other) const
        { return ( this->Radius <= Other.Radius ); }

    Boole Sphere::operator==(const Sphere& Other) const
        { return ( this->Center == Other.Center && this->Radius == Other.Radius ); }

    Boole Sphere::operator!=(const Sphere& Other) const
        { return ( this->Center != Other.Center || this->Radius != Other.Radius ); }
}//Mezzanine

#endif
