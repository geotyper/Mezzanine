//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _collisionshape_cpp
#define _collisionshape_cpp

#include "collisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"
#include "serialization.h"
#include "world.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletSoftBody/btSoftBodyInternals.h"

namespace Mezzanine
{
    /////////////////////////////////////////
    // CollisionShape Functions

    CollisionShape::CollisionShape()
        : ShapeBase(NULL)
    {
    }

    CollisionShape::~CollisionShape()
    {
    }

    const String& CollisionShape::GetName() const
    {
        return Name;
    }

    void CollisionShape::SetMargin(const Real& Margin)
    {
        ShapeBase->setMargin(Margin);
    }

    Real CollisionShape::GetMargin() const
    {
        return ShapeBase->getMargin();
    }

    void CollisionShape::SetScaling(const Vector3& Scaling)
    {
        ShapeBase->setLocalScaling(Scaling.GetBulletVector3());
    }

    Vector3 CollisionShape::GetScaling() const
    {
        return Vector3(ShapeBase->getLocalScaling());
    }

    btCollisionShape* CollisionShape::GetBulletShape() const
    {
        return ShapeBase;
    }

#ifdef MEZZXML
    void CollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Attribute NameAttr = CollisionNode.AppendAttribute("Name");
        if(!NameAttr)
            { SerializeError("Create Name Attribute", SerializableName()); }
        NameAttr.SetValue(this->GetName());

        xml::Attribute MarginAttr = CollisionNode.AppendAttribute("Margin");
        if(!MarginAttr)
            { SerializeError("Create Margin Attribute", SerializableName()); }
        MarginAttr.SetValue(this->GetMargin());

        xml::Node ScalingNode = CollisionNode.AppendChild("Scaling");
        if (!ScalingNode) { SerializeError("Create Name Attribute", SerializableName()); }
        this->GetScaling().ProtoSerialize(ScalingNode);
    }

    void CollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->CollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                if(OneNode.GetAttribute("Name"))
                    { this->Name = OneNode.GetAttribute("Name").AsString(); }
                this->SetMargin(OneNode.GetAttribute("Margin").AsReal());

                xml::Node ScalingNode = OneNode.GetChild("Scaling").GetFirstChild();
                if(!ScalingNode)
                    { DeSerializeError("locate Scaling node",SerializableName()); }
                this->SetScaling(Vector3(ScalingNode));
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CollisionShape::SerializableName()
        {   return String("CollisionShape"); }
#endif

    /////////////////////////////////////////
    // PrimitiveCollisionShape Functions

    PrimitiveCollisionShape::PrimitiveCollisionShape()
    {
    }

    PrimitiveCollisionShape::~PrimitiveCollisionShape()
    {
    }

    void PrimitiveCollisionShape::SetPointers(btConvexInternalShape* Shape)
        { ShapeBase = Shape; }

    btConvexInternalShape* PrimitiveCollisionShape::GetBulletConvexShape() const
        { return static_cast<btConvexInternalShape*>(ShapeBase); }

#ifdef MEZZXML
    void PrimitiveCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->PrimitiveCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->PrimitiveCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (!Version)
            { SerializeError("Create Version Attribute", SerializableName()); }
        Version.SetValue(1);

        xml::Node ImplicitShape = CollisionNode.AppendChild("ImplicitShape");
        if (!ImplicitShape)
            { SerializeError("Create ImplicitShape Node", SerializableName()); }
        Vector3(GetBulletConvexShape()->getImplicitShapeDimensions()).ProtoSerialize(ImplicitShape);

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void PrimitiveCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->PrimitiveCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);

                xml::Node ImplicitShape = OneNode.GetChild("ImplicitShape").GetFirstChild();
                if(!ImplicitShape)
                    { DeSerializeError("locate ImplicitShape node",SerializableName()); }
                GetBulletConvexShape()->setImplicitShapeDimensions(Vector3(ImplicitShape).GetBulletVector3());
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String PrimitiveCollisionShape::SerializableName()
        {   return String("PrimitiveCollisionShape"); }
#endif

    /////////////////////////////////////////
    // FieldCollisionShape Functions
    FieldCollisionShape::FieldCollisionShape()
    {
    }

    FieldCollisionShape::~FieldCollisionShape()
    {
    }

    void FieldCollisionShape::SetPointers(btConcaveShape* Shape)
        { ShapeBase = Shape; }

    btConcaveShape* FieldCollisionShape::GetBulletConcaveShape() const
        { return static_cast<btConcaveShape*>(ShapeBase); }

#ifdef MEZZXML
    void FieldCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->FieldCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->FieldCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void FieldCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->FieldCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String FieldCollisionShape::SerializableName()
        {   return String("FieldCollisionShape"); }
#endif

    /////////////////////////////////////////
    // MeshCollisionShape Functions

    MeshCollisionShape::MeshCollisionShape()
    {
    }

    MeshCollisionShape::~MeshCollisionShape()
    {
    }

    void MeshCollisionShape::SetPointers(btConcaveShape* Shape)
        { ShapeBase = Shape; }

    btConcaveShape* MeshCollisionShape::GetBulletConcaveShape() const
        { return static_cast<btConcaveShape*>(ShapeBase); }

#ifdef MEZZXML
    void MeshCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->MeshCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->MeshCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

    }

    void MeshCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->MeshCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String MeshCollisionShape::SerializableName()
        {   return String("MeshCollisionShape"); }
#endif

    /////////////////////////////////////////
    // CompoundCollisionShape Functions

    CompoundCollisionShape::CompoundCollisionShape(const String& Name)
    {
        this->Name = Name;
        ShapeBase = new btCompoundShape(false);
    }

    CompoundCollisionShape::CompoundCollisionShape(const String& Name, btCompoundShape* BulletShape)
    {
        this->Name = Name;
        ShapeBase = BulletShape;

        Whole NumChildren = GetBulletCompoundShape()->getNumChildShapes();
        for( Whole X = 0 ; X < NumChildren ; X++ )
        {
            btCollisionShape* CurrChild = GetBulletCompoundShape()->getChildShape(X);
            CollisionShape* CreatedShape = CreateShape(BulletSapeTypeToShapeType(CurrChild->getShapeType()), this->Name+"Child"+ToString(X), CurrChild);
            ChildShapes.push_back(CreatedShape);
            CollisionShapeManager::GetSingletonPtr()->StoreShape(CreatedShape);
        }
    }

    CompoundCollisionShape::~CompoundCollisionShape()
    {
        /*for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
        {
            //GetBulletCompoundShape()->removeChildShapeByIndex(X);
            delete ChildShapes[X];
        }
        ChildShapes.clear();*/
        delete GetBulletCompoundShape();
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation, const Quaternion& ChildRotation)
    {
        btTransform ChildTrans(ChildRotation.GetBulletQuaternion(),ChildLocation.GetBulletVector3());
        GetBulletCompoundShape()->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
        CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Vector3& ChildLocation)
    {
        btTransform ChildTrans;
        ChildTrans.setIdentity();
        ChildTrans.setOrigin(ChildLocation.GetBulletVector3());
        GetBulletCompoundShape()->addChildShape(ChildTrans,Child->GetBulletShape());
        ChildShapes.push_back(Child);
        CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
    }

    void CompoundCollisionShape::AddChildShape(CollisionShape* Child, const Transform& ChildLocation)
    {
        GetBulletCompoundShape()->addChildShape(ChildLocation.GetBulletTransform(),Child->GetBulletShape());
        ChildShapes.push_back(Child);
        CollisionShapeManager::GetSingletonPtr()->StoreShape(Child);
    }

    Whole CompoundCollisionShape::GetNumChildren() const
    {
        return ChildShapes.size();
    }

    CollisionShape* CompoundCollisionShape::GetChild(Whole Index) const
    {
        return ChildShapes.at(Index);
    }

    void CompoundCollisionShape::RemoveChildShape(CollisionShape* Child)
    {
        for( std::vector<CollisionShape*>::iterator CSit = ChildShapes.begin() ; CSit != ChildShapes.end() ; CSit++ )
        {
            if(Child == (*CSit))
            {
                ChildShapes.erase(CSit);
                GetBulletCompoundShape()->removeChildShape(Child->GetBulletShape());
            }
        }
    }

    void CompoundCollisionShape::RemoveChildShape(const Whole& ChildIndex)
    {
        std::vector<CollisionShape*>::iterator CSit = ChildShapes.begin() + ChildIndex;
        ChildShapes.erase(CSit);
        GetBulletCompoundShape()->removeChildShapeByIndex(ChildIndex);
    }

    CollisionShape::ShapeType CompoundCollisionShape::GetType() const
    {
        return CollisionShape::ST_Compound;
    }

    btCompoundShape* CompoundCollisionShape::GetBulletCompoundShape() const
        { return static_cast<btCompoundShape*>(ShapeBase); }

#ifdef MEZZXML
    void CompoundCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CompoundCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CompoundCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->CollisionShape::ProtoSerialize(CollisionNode);

        xml::Node ChildShapesNode = CollisionNode.AppendChild("Shapes");
        if (!ChildShapesNode) { SerializeError("create ChildShapesNode",this->CompoundCollisionShape::SerializableName());}
        for( Whole X = 0 ; X < ChildShapes.size() ; X++ )
        {
            //if() //the shape is in the manager
            //{
                xml::Node OneChildShapeNode = ChildShapesNode.AppendChild("ChildShapeFromManager");
                if(!OneChildShapeNode) { SerializeError("create ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName());}
                xml::Attribute OneName = OneChildShapeNode.AppendAttribute("Name");
                if(!OneName) { SerializeError("create Name Attribute on OneChildShapeNode",this->CompoundCollisionShape::SerializableName());}
                OneName.SetValue(ChildShapes[X]->GetName());
            //}else{
            //    ChildShapes[X].ProtoSerialize(ChildShapesNode);
            //}
        }

    }

    void CompoundCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->CompoundCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild("CollisionShape");
                if(!CollisionNode)
                    { DeSerializeError("locate CollisionShape node",SerializableName()); }
                this->CollisionShape::ProtoDeSerialize(CollisionNode);

                ChildShapes.clear(); // this will leak if any childshapes are not in the CollisionManager.
                xml::Node ChildShapesNode = OneNode.GetChild("Shapes");
                if(!ChildShapesNode) { DeSerializeError("Find Shapes Node",this->CompoundCollisionShape::SerializableName());}

                xml::Node ChildNode = ChildShapesNode.GetFirstChild();
                while(ChildNode)
                {
                    if(ChildNode.Name()=="ChildShapeFromManager")
                    {
                        xml::Attribute OneName = ChildNode.GetAttribute("Name");
                        if(!OneName) { DeSerializeError("find Name Attribute on ChildShapeFromManager Node",this->CompoundCollisionShape::SerializableName()); }
                        CollisionShape* CurrentShape = CollisionShapeManager::GetSingletonPtr()->GetShape(OneName.AsString());
                        if(!CurrentShape) { DeSerializeError("find correct shape in CollisionShape Manager",this->CompoundCollisionShape::SerializableName()); }
                        ChildShapes.push_back(CurrentShape);
                    }else{
                        CollisionShape* CurrentShape = CreateShape(ChildNode);
                        ChildShapes.push_back(CurrentShape);
                    }
                    ChildNode=ChildNode.GetNextSibling();
                }

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CompoundCollisionShape::SerializableName()
        {   return String("CompoundCollisionShape"); }
#endif

    /////////////////////////////////////////
    // BoxCollisionShape Functions

    BoxCollisionShape::BoxCollisionShape(const String& Name, const Vector3& HalfExtents)
    {
        this->Name = Name;
        SetPointers(new btBoxShape(HalfExtents.GetBulletVector3()));
    }

    BoxCollisionShape::BoxCollisionShape(const String& Name, btBoxShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

#ifdef MEZZXML
    BoxCollisionShape::BoxCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            this->Name=OneName.AsString();

            /*xml::Node HalfExtentsNode = OneNode.GetChild("HalfExtents").GetFirstChild();
            if (!HalfExtentsNode) { DeSerializeError("find HalfExtentsNode",BoxCollisionShape::SerializableName()); }
            SetPointers(new btBoxShape(Vector3(HalfExtentsNode).GetBulletVector3()));
            // */
            SetPointers(new btBoxShape(Vector3().GetBulletVector3()));

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    BoxCollisionShape::~BoxCollisionShape()
    {
        delete GetBulletBoxShape();
    }

    Vector3 BoxCollisionShape::GetCleanHalfExtents() const
    {
        return (this->GetHalfExtentsWithMargin()/this->GetScaling());
    }

    Vector3 BoxCollisionShape::GetHalfExtents() const
    {
        return Vector3(GetBulletBoxShape()->getHalfExtentsWithoutMargin());
    }

    Vector3 BoxCollisionShape::GetHalfExtentsWithMargin() const
    {
        return Vector3(GetBulletBoxShape()->getHalfExtentsWithMargin());
    }

    bool BoxCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
    {
        return GetBulletBoxShape()->isInside(Location.GetBulletVector3(),Tolerance);
    }

    CollisionShape::ShapeType BoxCollisionShape::GetType() const
    {
        return CollisionShape::ST_Box;
    }

    btBoxShape* BoxCollisionShape::GetBulletBoxShape() const
        { return static_cast<btBoxShape*>(ShapeBase); }
#ifdef MEZZXML
    void BoxCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->BoxCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->BoxCollisionShape::SerializableName());}

        /*
        xml::Node HalfExtentsNode = CollisionNode.AppendChild("HalfExtents");
        if (!HalfExtentsNode) { SerializeError("create HalfExtentsNode",this->BoxCollisionShape::SerializableName());}
        //this->GetHalfExtents().ProtoSerialize(HalfExtentsNode);
        this->GetCleanHalfExtents().ProtoSerialize(HalfExtentsNode);
        */

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void BoxCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->BoxCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String BoxCollisionShape::SerializableName()
        {   return String("BoxCollisionShape"); }
#endif // /MEZZXML
    /////////////////////////////////////////
    // CapsuleCollisionShape Functions

    void CapsuleCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
    {
        this->Name = Name;
        btCapsuleShape* CapsuleShape = 0;
        if(Axis_Y == UpAxis) CapsuleShape = new btCapsuleShape(Radius,Height);
        else if(Axis_X == UpAxis) CapsuleShape = new btCapsuleShapeX(Radius,Height);
        else if(Axis_Z == UpAxis) CapsuleShape = new btCapsuleShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow("Non-supported up Axis passed into CapsuleCollisionShape constructor.");
        SetPointers(CapsuleShape);
    }

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
        { this->Construct(Name,Radius,Height,UpAxis.IsStandardUnitAxis()); }

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        { this->Construct(Name,Radius,Height,UpAxis); }

#ifdef MEZZXML
    CapsuleCollisionShape::CapsuleCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            String Name_(OneName.AsString());

            xml::Attribute Axis = OneNode.GetAttribute("Axis");
            if (!Axis) { DeSerializeError("find Axis Attribute",CapsuleCollisionShape::SerializableName()); }
            /*
            xml::Attribute Radius = OneNode.GetAttribute("Radius");
            if (!Radius) { DeSerializeError("find Radius Attribute",CapsuleCollisionShape::SerializableName()); }
            xml::Attribute Height = OneNode.GetAttribute("Height");
            if (!Height) { DeSerializeError("find Height Attribute",CapsuleCollisionShape::SerializableName()); }
            //SetPointers(new CapsuleCollisionShape(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());  // make and deserialize the shape
            this->Construct(Name_,Radius.AsReal(),Height.AsReal(),(StandardAxis)Axis.AsInteger());
            */
            this->Construct(Name_,0,0,(StandardAxis)Axis.AsInteger());

            this->ProtoDeSerialize(OneNode);

        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

    CapsuleCollisionShape::~CapsuleCollisionShape()
    {
        delete GetBulletCapsuleShape();
    }

    Real CapsuleCollisionShape::GetRadius() const
        { return GetBulletCapsuleShape()->getRadius(); }
    Real CapsuleCollisionShape::GetHeight() const
        { return (GetBulletCapsuleShape()->getHalfHeight() * 2.0); }

    Real CapsuleCollisionShape::GetCleanRadius() const
        { return (this->GetRadius()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
        //{ return GetRadiusWithMargin() / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
    Real CapsuleCollisionShape::GetCleanHeight() const
        //{ return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[this->GetUpStandardAxis()] - GetBulletCapsuleShape()->getMargin()); }
        { return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()) / GetScaling()[this->GetUpStandardAxis()]); }
    /* Real CapsuleCollisionShape::GetRadiusWithMargin() const
        { return this->GetRadius()+GetBulletCapsuleShape()->getMargin(); }*/

    Vector3 CapsuleCollisionShape::GetUpAxis() const
        { return Vector3::UnitOnAxis( (StandardAxis)GetBulletCapsuleShape()->getUpAxis() ); }

    StandardAxis CapsuleCollisionShape::GetUpStandardAxis() const
        { return (StandardAxis)GetBulletCapsuleShape()->getUpAxis(); }

    CollisionShape::ShapeType CapsuleCollisionShape::GetType() const
    {
        return CollisionShape::ST_Capsule;
    }

    btCapsuleShape* CapsuleCollisionShape::GetBulletCapsuleShape() const
        { return static_cast<btCapsuleShape*>(ShapeBase); }

#ifdef MEZZXML
    void CapsuleCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CapsuleCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CapsuleCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }
/*
        xml::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
        if (RadiusAttr)
            { RadiusAttr.SetValue(this->GetCleanRadius()); }
        else
            { SerializeError("Create RadiusAttr Attribute", SerializableName()); }

        xml::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
        if (HeightAttr)
            { HeightAttr.SetValue(this->GetCleanHeight()); }
        else
            { SerializeError("Create HeightAttr Attribute", SerializableName()); }
*/
        xml::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
        if (AxisAttr)
            { AxisAttr.SetValue(this->GetUpStandardAxis()); }
        else
            { SerializeError("Create AxisAttr Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void CapsuleCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->CapsuleCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CapsuleCollisionShape::SerializableName()
        {   return String("CapsuleCollisionShape"); }
#endif

    /////////////////////////////////////////
    // ConeCollisionShape Functions

    void ConeCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
    {
        this->Name = Name;
        btConeShape* ConeShape;
        if(Axis_Y == UpAxis) ConeShape = new btConeShape(Radius,Height);
        else if(Axis_X == UpAxis) ConeShape = new btConeShapeX(Radius,Height);
        else if(Axis_Z == UpAxis) ConeShape = new btConeShapeZ(Radius,Height);
        else World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up StandardAxis passed into ConeCollisionShape constructor."));
        SetPointers(ConeShape);
        this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

    Vector3 ConeCollisionShape::GetAxisMathBS() const
    {
        Vector3 Results;
        switch (GetUpStandardAxis())
        {
            case 0:
                    Results[0] = 1;
                    Results[1] = 0;
                    Results[2] = 2;
                break;
            case 1:
                    Results[0] = 0;
                    Results[1] = 1;
                    Results[2] = 2;
                break;
            case 2:
                    Results[0] = 0;
                    Results[1] = 2;
                    Results[2] = 1;
                break;
            default:
                World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up StandardAxis passed into ConeCollisionShape::GetAxisMathBS()."));
        }

        return Results;
    }

    ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
        { Construct(Name, Radius, Height, UpAxis.IsStandardUnitAxis()); }

    ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        { Construct(Name, Radius, Height, UpAxis); }

#ifdef MEZZXML
    ConeCollisionShape::ConeCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }
            String Name_(OneName.AsString());

            xml::Attribute Radius = OneNode.GetAttribute("Radius");                                                                             // Find Attributes
            if (!Radius) { DeSerializeError("find Radius Attribute",ConeCollisionShape::SerializableName()); }
            xml::Attribute Axis = OneNode.GetAttribute("Axis");
            if (!Axis) { DeSerializeError("find Axis Attribute",ConeCollisionShape::SerializableName()); }
            xml::Attribute Height = OneNode.GetAttribute("Height");
            if (!Height) { DeSerializeError("find Height Attribute",ConeCollisionShape::SerializableName()); }

            this->Construct(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());        // make and deserialize the shape
            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    ConeCollisionShape::ConeCollisionShape(const String& Name, btConeShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

    ConeCollisionShape::~ConeCollisionShape()
    {
        delete GetBulletConeShape();
    }

    Real ConeCollisionShape::GetRadius() const
    {
        return GetBulletConeShape()->getRadius();
    }

    Real ConeCollisionShape::GetHeight() const
    {
        return GetBulletConeShape()->getHeight();
    }

    Real ConeCollisionShape::GetCleanRadius() const
    {
        return (GetRadius()) / GetRadiusScaling();
    }

    Real ConeCollisionShape::GetCleanHeight() const
    {
        return GetHeight() / GetHeightScaling();
    }

    Real ConeCollisionShape::GetRadiusScaling() const
        { return (GetScaling()[GetAxisMathBS()[0]]+GetScaling()[GetAxisMathBS()[2]])/2.0; }

    Real ConeCollisionShape::GetHeightScaling() const
        { return GetScaling()[GetUpStandardAxis()]; }

    Vector3 ConeCollisionShape::GetUpAxis() const
        { return Vector3::UnitOnAxis( (StandardAxis)GetBulletConeShape()->getConeUpIndex() ); }

    StandardAxis ConeCollisionShape::GetUpStandardAxis() const
        { return (StandardAxis)GetBulletConeShape()->getConeUpIndex(); }


    CollisionShape::ShapeType ConeCollisionShape::GetType() const
    {
        return CollisionShape::ST_Cone;
    }

    btConeShape* ConeCollisionShape::GetBulletConeShape() const
        { return static_cast<btConeShape*>(ShapeBase); }

#ifdef MEZZXML
    void ConeCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->ConeCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->ConeCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
        if (RadiusAttr)
            { RadiusAttr.SetValue(this->GetCleanRadius()); }
        else
            { SerializeError("Create RadiusAttr Attribute", SerializableName()); }

        /*xml::Attribute DirtyRadius = CollisionNode.AppendAttribute("DirtyRadius");
        if (DirtyRadius)
            { DirtyRadius.SetValue(this->GetRadius()); }
        else
            { SerializeError("Create DirtyRadius Attribute", SerializableName()); }*/

        xml::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
        if (HeightAttr)
            { HeightAttr.SetValue(this->GetCleanHeight()); }
        else
            { SerializeError("Create HeightAttr Attribute", SerializableName()); }

        xml::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
        if (AxisAttr)
            { AxisAttr.SetValue(this->GetUpStandardAxis()); }
        else
            { SerializeError("Create AxisAttr Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void ConeCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ConeCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ConeCollisionShape::SerializableName()
        {   return String("ConeCollisionShape"); }
#endif

    /////////////////////////////////////////
    // ConvexHullCollisionShape Functions

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, const std::vector<Vector3>& Points)
    {
        btScalar* BulletPoints = new btScalar[Points.size() * 3];
        for( Whole X = 0 ; X < Points.size() ; ++X )
        {
            BulletPoints[X*3]     = Points[X][0];
            BulletPoints[X*3+1]   = Points[X][1];
            BulletPoints[X*3+2]   = Points[X][2];
        }

        this->Name = Name;
        SetPointers(new btConvexHullShape(BulletPoints,Points.size(),3*sizeof(btScalar)));
        this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        delete[] BulletPoints;
    }

    ConvexHullCollisionShape::ConvexHullCollisionShape(const String& Name, btConvexHullShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        this->GetBulletHullShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

#ifdef MEZZXML
    ConvexHullCollisionShape::ConvexHullCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            this->Name = OneName.AsString();

            SetPointers(new btConvexHullShape());

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version", ConvexHullCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    ConvexHullCollisionShape::~ConvexHullCollisionShape()
    {
        delete GetBulletHullShape();
    }

    void ConvexHullCollisionShape::AddPoint(const Vector3& Point)
    {
        GetBulletHullShape()->addPoint(Point.GetBulletVector3());
    }

    Vector3 ConvexHullCollisionShape::GetUnscaledPoint(const Whole& Index) const
    {

        return Vector3 ( *(GetBulletHullShape()->getUnscaledPoints()+Index) );
    }

    Vector3 ConvexHullCollisionShape::GetPoint(const Whole& Index) const
    {
        return Vector3(GetBulletHullShape()->getScaledPoint(Index));
    }

    Whole ConvexHullCollisionShape::GetNumPoints() const
    {
        return GetBulletHullShape()->getNumPoints();
    }

    bool ConvexHullCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
    {
        return GetBulletHullShape()->isInside(Location.GetBulletVector3(),Tolerance);
    }

    CollisionShape::ShapeType ConvexHullCollisionShape::GetType() const
    {
        return CollisionShape::ST_ConvexHull;
    }

    btConvexHullShape* ConvexHullCollisionShape::GetBulletHullShape() const
        { return static_cast<btConvexHullShape*>(ShapeBase); }

#ifdef MEZZXML
    void ConvexHullCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->ConvexHullCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->ConvexHullCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Node PointsNode = CollisionNode.AppendChild("UnscaledPoints");
        if (!PointsNode) { SerializeError("create UnscaledPoints",this->ConvexHullCollisionShape::SerializableName());}

        for(Whole c=0; c<this->GetNumPoints(); ++c)
        {
            this->GetUnscaledPoint(c).ProtoSerialize(PointsNode);
        }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void ConvexHullCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ConvexHullCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

                xml::Node UnscaledPoints = OneNode.GetChild("UnscaledPoints");
                if(!UnscaledPoints)
                    { DeSerializeError("locate UnscaledPoints node",SerializableName()); }

                xml::Node OnePoint = UnscaledPoints.GetFirstChild();
                while (OnePoint)
                {
                    this->AddPoint(Vector3(OnePoint));
                    OnePoint = OnePoint.GetNextSibling();
                }

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String ConvexHullCollisionShape::SerializableName()
        {   return String("ConvexHullCollisionShape"); }
#endif


    /////////////////////////////////////////
    // CylinderCollisionShape Functions

    void CylinderCollisionShape::Construct(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis)
    {
        btCylinderShape* CylinderShape;

        switch (UpAxis)
        {
            case Axis_Y:
                CylinderShape = new btCylinderShape(HalfExtents.GetBulletVector3());
                break;
            case Axis_X:
                CylinderShape = new btCylinderShapeX(HalfExtents.GetBulletVector3());
                break;
            case Axis_Z:
                CylinderShape = new btCylinderShapeZ(HalfExtents.GetBulletVector3());
                break;
            default: World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into CylinderCollisionShape constructor."));
        }
        Construct(Name, CylinderShape);
    }

    void CylinderCollisionShape::Construct(const String& Name, btCylinderShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
        { Construct(Name, CreateHalfExtents(Radius,Height,UpAxis.IsStandardUnitAxis()), UpAxis.IsStandardUnitAxis()); }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        { Construct(Name, CreateHalfExtents(Radius,Height,UpAxis), UpAxis); }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, const Vector3& UpAxis)
        { Construct(Name, HalfExtents, UpAxis.IsStandardUnitAxis()); }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis)
        { Construct(Name, HalfExtents, UpAxis); }

    CylinderCollisionShape::CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape)
        { Construct(Name,BulletShape); }


#ifdef MEZZXML
    CylinderCollisionShape::CylinderCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            /*xml::Node HalfExtentsNode = OneNode.GetChild("HalfExtents").GetFirstChild();
            if (!HalfExtentsNode) { DeSerializeError("find HalfExtentsNode",CylinderCollisionShape::SerializableName()); }
            SetPointers(new CylinderShape(Vector3(HalfExtentsNode).GetBulletVector3()));
            // */

            xml::Attribute Axis = OneNode.GetAttribute("Axis");
            if (!Axis) { DeSerializeError("find Axis Attribute",CylinderCollisionShape::SerializableName()); }

            Construct(OneName.AsString(), Vector3(), (StandardAxis)Axis.AsInteger());

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version", CylinderCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    CylinderCollisionShape::~CylinderCollisionShape()
    {
        delete GetBulletCylinderShape();
    }

    Vector3 CylinderCollisionShape::CreateHalfExtents(const Real& Radius, const Real& Height, StandardAxis UpAxis)
    {
        switch (UpAxis)
        {
            case Axis_Y: return Vector3(Radius,Height,Radius);
            case Axis_X: return Vector3(Height,Radius,Radius);
            case Axis_Z: return Vector3(Radius,Radius,Height);
            default: World::GetWorldPointer()->LogAndThrow(Exception("Non-supported up Axis passed into CylinderCollisionShape::CreateHalfExtents ."));
        }
    }

    Vector3 CylinderCollisionShape::GetHalfExtents() const
    {
        Vector3 HalfExtents(GetBulletCylinderShape()->getHalfExtentsWithoutMargin());
        return HalfExtents;
    }

    Vector3 CylinderCollisionShape::GetHalfExtentsWithMargin() const
    {
        Vector3 HalfExtents(GetBulletCylinderShape()->getHalfExtentsWithMargin());
        return HalfExtents;
    }

    Real CylinderCollisionShape::GetRadius() const
        { return GetBulletCylinderShape()->getRadius(); }

    Vector3 CylinderCollisionShape::GetUpAxis() const
        { return Vector3::UnitOnAxis((StandardAxis)GetBulletCylinderShape()->getUpAxis()); }

    CollisionShape::ShapeType CylinderCollisionShape::GetType() const
        { return CollisionShape::ST_Cylinder; }

    btCylinderShape* CylinderCollisionShape::GetBulletCylinderShape() const
        { return static_cast<btCylinderShape*>(ShapeBase); }

#ifdef MEZZXML
    void CylinderCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->CylinderCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->CylinderCollisionShape::SerializableName());}

        /*
        xml::Node HalfExtentsNode = CollisionNode.AppendChild("HalfExtents");
        if (!HalfExtentsNode) { SerializeError("create HalfExtentsNode",this->CylinderCollisionShape::SerializableName());}
        //this->GetHalfExtents().ProtoSerialize(HalfExtentsNode);
        this->GetCleanHalfExtents().ProtoSerialize(HalfExtentsNode);
        */

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Attribute Axis = CollisionNode.AppendAttribute("Axis");
        if (Axis)
            { Axis.SetValue((StandardAxis)GetBulletCylinderShape()->getUpAxis()); }
        else
            { SerializeError("Create Axis Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void CylinderCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->CylinderCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String CylinderCollisionShape::SerializableName()
        {   return String("CylinderCollisionShape"); }
#endif

    /////////////////////////////////////////
    // MultiSphereCollisionShape Functions

    void MultiSphereCollisionShape::Construct(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
    {
        if(Locations.size() != Radii.size())
            World::GetWorldPointer()->LogAndThrow(Exception("Vector size mismatch between Locations and Radii in MultiSphereCollisionShape constructor."));
        Whole Spheres = Locations.size();
        btVector3* BulletLocs = new btVector3[Spheres];
        btScalar* BulletRadii = new btScalar[Spheres];
        for( Whole X = 0 ; X < Spheres ; X++ )
        {
            BulletLocs[X] = Locations[X].GetBulletVector3();
            BulletRadii[X] = Radii[X];
        }

        Construct(Name,new btMultiSphereShape(BulletLocs,BulletRadii,Spheres));

        delete[] BulletLocs;
        delete[] BulletRadii;
    }

    void MultiSphereCollisionShape::Construct(const String& Name, btMultiSphereShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        this->GetMultiSphereShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }


    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, const std::vector<Vector3>& Locations, const std::vector<Real>& Radii)
    {
        Construct(Name,Locations,Radii);
    }

    MultiSphereCollisionShape::MultiSphereCollisionShape(const String& Name, btMultiSphereShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
    }

#ifdef MEZZXML
    MultiSphereCollisionShape::MultiSphereCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            xml::Node Spheres = OneNode.GetChild("Spheres");
            if(!Spheres)
                { DeSerializeError("locate Spheres node",SerializableName()); }

            std::vector<Vector3> Locations;
            std::vector<Real> Radii;
            xml::Node OneSphere = Spheres.GetFirstChild();
            while (OneSphere)
            {
                Locations.push_back(Vector3(OneSphere.GetFirstChild()));
                Radii.push_back(OneSphere.GetFirstAttribute().AsReal());

                OneSphere = OneSphere.GetNextSibling();
            }

            Construct(OneName.AsString(),Locations,Radii);

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",MultiSphereCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    MultiSphereCollisionShape::~MultiSphereCollisionShape()
    {
        delete GetMultiSphereShape();
    }

    Vector3 MultiSphereCollisionShape::GetSphereLocation(const Whole& Index) const
    {
        Vector3 Loc(GetMultiSphereShape()->getSpherePosition(Index));
        return Loc;
    }

    Real MultiSphereCollisionShape::GetSphereRadius(const Whole& Index) const
    {
        return GetMultiSphereShape()->getSphereRadius(Index);
    }

    Whole MultiSphereCollisionShape::GetNumSpheres() const
    {
        return GetMultiSphereShape()->getSphereCount();
    }

    CollisionShape::ShapeType MultiSphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_MultiSphere;
    }

    btMultiSphereShape* MultiSphereCollisionShape::GetMultiSphereShape() const
        { return static_cast<btMultiSphereShape*>(ShapeBase); }

#ifdef MEZZXML
    void MultiSphereCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->MultiSphereCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->MultiSphereCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        xml::Node PointsNode = CollisionNode.AppendChild("Spheres");
        if (!PointsNode) { SerializeError("create Spheres",this->MultiSphereCollisionShape::SerializableName());}

        for(Whole c=0; c<this->GetNumSpheres(); ++c)
        {
            xml::Node Sphere = PointsNode.AppendChild("Sphere");
            if (!Sphere) { SerializeError(String("create Sphere ")+ToString(c),this->MultiSphereCollisionShape::SerializableName());}

            xml::Attribute Radius = Sphere.AppendAttribute("Radius");
            if (!Radius) { SerializeError(String("Append readius to Sphere ")+ToString(c),this->MultiSphereCollisionShape::SerializableName());}
            Radius.SetValue( this->GetSphereRadius(c) );

            this->GetSphereLocation(c).ProtoSerialize(Sphere);
        }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void MultiSphereCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->MultiSphereCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String MultiSphereCollisionShape::SerializableName()
        {   return String("MultiSphereCollisionShape"); }
#endif

    /////////////////////////////////////////
    // SphereCollisionShape Functions

    SphereCollisionShape::SphereCollisionShape(const String& Name, const Real& Radius)
    {
        this->Name = Name;
        SetPointers(new btSphereShape(Radius));
        this->GetSphereShape()->setImplicitShapeDimensions(Vector3(Radius,0,0).GetBulletVector3());
    }

    SphereCollisionShape::SphereCollisionShape(const String& Name, btSphereShape* BulletShape)
    {
        this->Name = Name;
        SetPointers(BulletShape);
        //this->GetSphereShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
    }

#ifdef MEZZXML
    SphereCollisionShape::SphereCollisionShape(xml::Node OneNode)
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            xml::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
            if(!OneName) { throw( Exception("Could not find Name Attribute on CollsionShape Node during preparation for deserialization")); }

            this->Name = Name;
            SetPointers(new btSphereShape(0));
            this->GetSphereShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());

            this->ProtoDeSerialize(OneNode);
        }else{
            DeSerializeError("find usable serialization version",SphereCollisionShape::SerializableName());
        }
    }
#endif // /MEZZXML

    SphereCollisionShape::~SphereCollisionShape()
    {
        delete GetSphereShape();
    }

    Real SphereCollisionShape::GetRadius() const
    {
        return GetSphereShape()->getRadius();
    }

    CollisionShape::ShapeType SphereCollisionShape::GetType() const
    {
        return CollisionShape::ST_Sphere;
    }

    btSphereShape* SphereCollisionShape::GetSphereShape() const
        { return static_cast<btSphereShape*>(ShapeBase); }

#ifdef MEZZXML
    void SphereCollisionShape::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node CollisionNode = CurrentRoot.AppendChild(this->SphereCollisionShape::SerializableName());
        if (!CollisionNode) { SerializeError("create CollisionNode",this->SphereCollisionShape::SerializableName());}

        xml::Attribute Version = CollisionNode.AppendAttribute("Version");
        if (Version)
            { Version.SetValue(1); }
        else
            { SerializeError("Create Version Attribute", SerializableName()); }

        this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
    }

    void SphereCollisionShape::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->SphereCollisionShape::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                xml::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                if(!CollisionNode)
                    { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);

            }else{
                DeSerializeError("find usable serialization version",SerializableName());
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
        }
    }

    String SphereCollisionShape::SerializableName()
        {   return String("SphereCollisionShape"); }
#endif

    /////////////////////////////////////////
    // DynamicMeshCollisionShape Functions

    DynamicMeshCollisionShape::DynamicMeshCollisionShape(const String& Name, btGImpactMeshShape* BulletShape)
    {
        this->Name = Name;
        GImpactShape = BulletShape;
        SetPointers(GImpactShape);
    }

    DynamicMeshCollisionShape::~DynamicMeshCollisionShape()
    {
        delete GImpactShape;
    }

    CollisionShape::ShapeType DynamicMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_DynamicTriMesh;
    }

    /////////////////////////////////////////
    // HeightfieldCollisionShape Functions

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name)
    {

    }

    HeightfieldCollisionShape::HeightfieldCollisionShape(const String& Name, btHeightfieldTerrainShape* BulletShape)
    {
        this->Name = Name;
        HeightfieldShape = BulletShape;
        SetPointers(HeightfieldShape);
    }

    HeightfieldCollisionShape::~HeightfieldCollisionShape()
    {
        delete HeightfieldShape;
    }

    CollisionShape::ShapeType HeightfieldCollisionShape::GetType() const
    {
        return CollisionShape::ST_Heightfield;
    }

    /////////////////////////////////////////
    // PlaneCollisionShape Functions

    PlaneCollisionShape::PlaneCollisionShape(const String& Name)
    {

    }

    PlaneCollisionShape::PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape)
    {
        this->Name = Name;
        PlaneShape = BulletShape;
        SetPointers(PlaneShape);
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        delete PlaneShape;
    }

    CollisionShape::ShapeType PlaneCollisionShape::GetType() const
    {
        return CollisionShape::ST_Plane;
    }

    /////////////////////////////////////////
    // ActorSoftCollisionShape Functions

    ActorSoftCollisionShape::ActorSoftCollisionShape(const String& Name, btSoftBodyCollisionShape* BulletShape)
    {
        this->Name = Name;
        ActorSoftShape = BulletShape;
        SetPointers(ActorSoftShape);
    }

    ActorSoftCollisionShape::~ActorSoftCollisionShape()
    {
        delete ActorSoftShape;
    }

    CollisionShape::ShapeType ActorSoftCollisionShape::GetType() const
    {
        return CollisionShape::ST_ActorSoft;
    }

    /////////////////////////////////////////
    // StaticMeshCollisionShape Functions

    StaticMeshCollisionShape::StaticMeshCollisionShape(const String& Name, btBvhTriangleMeshShape* BulletShape)
    {
        /// @todo May need to create a btScaledBvhTriangleMeshShape for scaling this shape appropriately.
        this->Name = Name;
        BvhShape = BulletShape;
        SetPointers(BvhShape);
    }

    StaticMeshCollisionShape::~StaticMeshCollisionShape()
    {
        delete BvhShape;
    }

    CollisionShape::ShapeType StaticMeshCollisionShape::GetType() const
    {
        return CollisionShape::ST_StaticTriMesh;
    }

    /////////////////////////////////////////
    // Utility Functions
    CollisionShape* CreateShape(CollisionShape::ShapeType ShapeToCreate, const String& Name_, btCollisionShape* ShapeToModel)
    {
        if (ShapeToModel)
        {
            switch(ShapeToCreate)
            {
                case CollisionShape::ST_Box:
                    return new BoxCollisionShape(Name_,(btBoxShape*)ShapeToModel);
                case CollisionShape::ST_Capsule:
                    return new CapsuleCollisionShape(Name_,(btCapsuleShape*)ShapeToModel);
                case CollisionShape::ST_Cone:
                    return new ConeCollisionShape(Name_,(btConeShape*)ShapeToModel);
                case CollisionShape::ST_ConvexHull:
                    return new ConvexHullCollisionShape(Name_,(btConvexHullShape*)ShapeToModel);
                case CollisionShape::ST_Cylinder:
                    return new CylinderCollisionShape(Name_,(btCylinderShape*)ShapeToModel);
                case CollisionShape::ST_MultiSphere:
                    return new MultiSphereCollisionShape(Name_,(btMultiSphereShape*)ShapeToModel);
                case CollisionShape::ST_Sphere:
                    return new SphereCollisionShape(Name_,(btSphereShape*)ShapeToModel);
                case CollisionShape::ST_DynamicTriMesh:
                    return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                case CollisionShape::ST_Heightfield:
                    return new HeightfieldCollisionShape(Name_,(btHeightfieldTerrainShape*)ShapeToModel);
                case CollisionShape::ST_Plane:
                    return new PlaneCollisionShape(Name_,(btStaticPlaneShape*)ShapeToModel);
                case CollisionShape::ST_ActorSoft:
                    return new ActorSoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                case CollisionShape::ST_StaticTriMesh:
                    return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                case CollisionShape::ST_Compound: // holy recursive batman
                    return new CompoundCollisionShape(Name_,(btCompoundShape*)ShapeToModel);
                default:
                    World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance.");
            }
        }else{
            World::GetWorldPointer()->LogAndThrow("Attempting to convert an empty Bullet Collision Shape type into a CollisionShape instance.");
        }
    }

    CollisionShape* CreateShape(xml::Node OneNode)
    {
        CollisionShape::ShapeType ShapeToCreate = StringToShapeType(OneNode.Name());

        String Name_("Defunct");

        switch(ShapeToCreate)
        {
            case CollisionShape::ST_Box:
                return new BoxCollisionShape(OneNode);
            case CollisionShape::ST_Capsule:
                return new CapsuleCollisionShape(OneNode);
            case CollisionShape::ST_Cone:
                return new ConeCollisionShape(OneNode);
            case CollisionShape::ST_ConvexHull:
                return new ConvexHullCollisionShape(OneNode);
            case CollisionShape::ST_Cylinder:
                return new CylinderCollisionShape(OneNode);
            case CollisionShape::ST_MultiSphere:
                return new MultiSphereCollisionShape(OneNode);
            case CollisionShape::ST_Sphere:
                return new SphereCollisionShape(OneNode);
            case CollisionShape::ST_DynamicTriMesh:     /// @todo Complete CreateShape function once DynamicMeshCollisionShape can be deserialized
                //return new DynamicMeshCollisionShape(Name_,(btGImpactMeshShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a DynamicMeshCollisionShape.");
            case CollisionShape::ST_Heightfield:
                return new HeightfieldCollisionShape(Name_);
            case CollisionShape::ST_Plane:
                return new PlaneCollisionShape(Name_);
            case CollisionShape::ST_ActorSoft:          /// @todo Complete CreateShape function once ActorSoftCollisionShape can be deserialized
                //return new ActorSoftCollisionShape(Name_,(btSoftBodyCollisionShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a ActorSoftCollisionShape.");
            case CollisionShape::ST_StaticTriMesh:      /// @todo Complete CreateShape function once StaticMeshCollisionShape can be deserialized
                //return new StaticMeshCollisionShape(Name_,(btBvhTriangleMeshShape*)ShapeToModel);
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance, specifically a ActorSoftCollisionShape.");
            case CollisionShape::ST_Compound: // holy recursive batman
                return new CompoundCollisionShape(Name_);
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a CollisionShape instance.");
        }
    }

    CollisionShape::ShapeType BulletSapeTypeToShapeType(int BulletShapeType)
    {
        switch(BulletShapeType)
        {
            case BOX_SHAPE_PROXYTYPE:           return CollisionShape::ST_Box;
            case CAPSULE_SHAPE_PROXYTYPE:       return CollisionShape::ST_Capsule;
            case CONE_SHAPE_PROXYTYPE:          return CollisionShape::ST_Cone;
            case CONVEX_HULL_SHAPE_PROXYTYPE:   return CollisionShape::ST_ConvexHull;
            case CYLINDER_SHAPE_PROXYTYPE:      return CollisionShape::ST_Cylinder;
            case MULTI_SPHERE_SHAPE_PROXYTYPE:  return CollisionShape::ST_MultiSphere;
            case SPHERE_SHAPE_PROXYTYPE:        return CollisionShape::ST_Sphere;
            case GIMPACT_SHAPE_PROXYTYPE:       return CollisionShape::ST_DynamicTriMesh;
            case TERRAIN_SHAPE_PROXYTYPE:       return CollisionShape::ST_Heightfield;
            case STATIC_PLANE_PROXYTYPE:        return CollisionShape::ST_Plane;
            case SOFTBODY_SHAPE_PROXYTYPE:      return CollisionShape::ST_ActorSoft;
            case TRIANGLE_MESH_SHAPE_PROXYTYPE: return CollisionShape::ST_StaticTriMesh;
            case COMPOUND_SHAPE_PROXYTYPE:      return CollisionShape::ST_Compound;
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Bullet Collision Shape type into a Mezzanine::CollisionShapeShapeType.");
        }
    }

    String ShapeTypeToString(CollisionShape::ShapeType ShapeToConvert)
    {
        switch(ShapeToConvert)
        {
            case CollisionShape::ST_Box:            return String("BoxCollisionShape");
            case CollisionShape::ST_Capsule:        return String("CapsuleCollisionShape");
            case CollisionShape::ST_Cone:           return String("ConeCollisionShape");
            case CollisionShape::ST_ConvexHull:     return String("ConvexHullCollisionShape");
            case CollisionShape::ST_Cylinder:       return String("CylinderCollisionShape");
            case CollisionShape::ST_MultiSphere:    return String("MultiSphereCollisionShape");
            case CollisionShape::ST_Sphere:         return String("SphereCollisionShape");
            case CollisionShape::ST_DynamicTriMesh: return String("DynamicMeshCollisionShape");
            case CollisionShape::ST_Heightfield:    return String("HeightfieldCollisionShapeString(");
            case CollisionShape::ST_Plane:          return String("PlaneCollisionShape");
            case CollisionShape::ST_ActorSoft:      return String("ActorSoftCollisionShape");
            case CollisionShape::ST_StaticTriMesh:  return String("StaticMeshCollisionShape");
            case CollisionShape::ST_Compound:       return String("CompoundCollisionShape");
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an unsupported/unwrapped Collision Shape type into a String.");
        }
    }

    CollisionShape::ShapeType StringToShapeType(const String& TypeName)
    {
        if(TypeName.size()<5)
            { World::GetWorldPointer()->LogAndThrow("Attempting to convert a CollisionShape::ShapeType String into a CollisionShape::ShapeType which is too short to be valid."); }
        switch(TypeName.at(3))
        {
            case 'C':
                if (String("BoxCollisionShape")==TypeName)
                    { return CollisionShape::ST_Box; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'C'."); }
            case 's':
                if (String("CapsuleCollisionShape")==TypeName)
                    { return CollisionShape::ST_Capsule; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 's'."); }
            case 'e':
                if (String("SphereCollisionShape")==TypeName)
                    { return CollisionShape::ST_Sphere; }
                else if (String("ConeCollisionShape")==TypeName)
                    { return CollisionShape::ST_Cone; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'e'."); }
            case 'v':
                if (String("ConvexHullCollisionShape")==TypeName)
                    { return CollisionShape::ST_ConvexHull; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'v'."); }
            case 'i':
                if (String("CylinderCollisionShape")==TypeName)
                    { return CollisionShape::ST_Cylinder; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'i'."); }
            case 't':
                if (String("StaticMeshCollisionShape")==TypeName)
                    { return CollisionShape::ST_StaticTriMesh; }
                else if (String("MultiSphereCollisionShape")==TypeName)
                    { return CollisionShape::ST_MultiSphere; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 't'."); }
            case 'a':
                if (String("DynamicMeshCollisionShape")==TypeName)
                    { return CollisionShape::ST_DynamicTriMesh; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'a'."); }
            case 'g':
                if (String("HeightfieldCollisionShapeString")==TypeName)
                    { return CollisionShape::ST_Heightfield; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'g'."); }
            case 'n':
                if (String("PlaneCollisionShape")==TypeName)
                    { return CollisionShape::ST_Plane; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'n'."); }
            case 'o':
                if (String("ActorSoftCollisionShape")==TypeName)
                    { return CollisionShape::ST_ActorSoft; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'o'."); }
            case 'p':
                if (String("CompoundCollisionShape")==TypeName)
                    { return CollisionShape::ST_Compound; }
                else
                    { World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType 'p'."); }
            default:
                World::GetWorldPointer()->LogAndThrow("Attempting to convert an invalid CollisionShape::ShapeType String into a CollisionShape::ShapeType.");
        }
    }

    ///////////////////////////////////////
    // CollisionShapeDeSerializer
    CollisionShape* CollisionShapeDeSerializer::PerformDeSerialization(std::istream& Stream)
    {
        xml::Document* Doc = new xml::Document();
        String OneTag( Mezzanine::xml::GetOneTag(Stream) );
        if(!Doc->Load(OneTag.c_str()))
        {
            delete Doc;
            World::GetWorldPointer()->LogAndThrow(StringTool::StringCat("Could not Deserialize XML Stream which should contain a Collision Shape, XML looked Like: ", OneTag) );
        }

        CollisionShape* Results = ProtoDeSerialize(Doc->GetFirstChild());
        delete Doc;
        return Results;
    }

    CollisionShape* CollisionShapeDeSerializer::ProtoDeSerialize(const xml::Node& OneNode)
        { return CreateShape(OneNode); }

    std::istream& CollisionShapeDeSerializer::DeSerialize(std::istream& Stream)
    {
        PerformDeSerialization(Stream);
        return Stream;
    }

    CollisionShape* CollisionShapeDeSerializer::DeSerializeAndRetrieve(std::istream& Stream)
        { return PerformDeSerialization(Stream); }

    String CollisionShapeDeSerializer::ContainerName() const
        { return String("Shapes"); }
}

#ifdef MEZZXML
/*
CollisionShape
PrimitiveCollisionShape
FieldCollisionShape
MeshCollisionShape
BoxCollisionShape
CapsuleCollisionShape
CompoundCollisionShape
ConeCollisionShape
ConvexHullCollisionShape
CylinderCollisionShape
MultiSphereCollisionShape
SphereCollisionShape
DynamicMeshCollisionShape
HeightfieldCollisionShape
PlaneCollisionShape
ActorSoftCollisionShape
StaticMeshCollisionShape
 */
    std::ostream& operator << (std::ostream& stream, const Mezzanine::CollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::CollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::CollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::PrimitiveCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::PrimitiveCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::PrimitiveCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::FieldCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::FieldCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::FieldCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::MeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::MeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::MeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::BoxCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::BoxCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::BoxCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::CapsuleCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::CapsuleCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::CapsuleCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::CompoundCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::CompoundCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::CompoundCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::ConeCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::ConeCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ConeCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::ConvexHullCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::ConvexHullCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ConvexHullCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::CylinderCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::CylinderCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::CylinderCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::MultiSphereCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::MultiSphereCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::MultiSphereCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::SphereCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::SphereCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::SphereCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }
     std::ostream& operator << (std::ostream& stream, const Mezzanine::DynamicMeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::DynamicMeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::DynamicMeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::HeightfieldCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::HeightfieldCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::HeightfieldCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::PlaneCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::PlaneCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::PlaneCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::ActorSoftCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::ActorSoftCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ActorSoftCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

    std::ostream& operator << (std::ostream& stream, const Mezzanine::StaticMeshCollisionShape& ShapeToSerialize)
        { Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::StaticMeshCollisionShape& x)
        { return DeSerialize(stream, x); }

    void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::StaticMeshCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

#endif  // \meaaxml


#endif
