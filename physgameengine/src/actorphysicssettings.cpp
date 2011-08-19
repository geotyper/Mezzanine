//� Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _actorphysicssettings_cpp
#define _actorphysicssettings_cpp

#include "actorphysicssettings.h"
#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "actorterrain.h"
#include "world.h"
#include "collisionshape.h"
#include "collisionshapemanager.h"

#ifdef PHYSXML
#include <memory>
#endif

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

namespace phys
{
    ActorBasePhysicsSettings::ActorBasePhysicsSettings(ActorBase* Actor, btCollisionObject* PhysicsObject)
        : Parent(Actor),
          ActorShape(NULL),
          ActorCO(PhysicsObject),
          CollisionGroup(0),
          CollisionMask(0)
    {
    }

    ActorBasePhysicsSettings::~ActorBasePhysicsSettings()
    {
    }

    void ActorBasePhysicsSettings::AssignShape(CollisionShape* Shape)
    {
        this->ActorShape = Shape;
        Parent->Shape = Shape->GetBulletShape(); // I got second crash here
    }

    void ActorBasePhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
    }

    CollisionShape* ActorBasePhysicsSettings::GetCollisionShape() const
    {
        return ActorShape;
    }

    void ActorBasePhysicsSettings::SetCollisionGroupAndMask(const Whole& Group, const Whole& Mask)
    {
        CollisionGroup = Group;
        CollisionMask = Mask;
        if(Parent->IsInWorld())
        {
            Parent->RemoveObjectFromWorld(World::GetWorldPointer());
            Parent->AddObjectToWorld(World::GetWorldPointer());
        }
    }

    Whole ActorBasePhysicsSettings::GetCollisionGroup() const
    {
        return CollisionGroup;
    }

    Whole ActorBasePhysicsSettings::GetCollisionMask() const
    {
        return CollisionMask;
    }

    void ActorBasePhysicsSettings::SetFriction(const Real& Friction)
    {
        ActorCO->setFriction(Friction);
    }

    Real ActorBasePhysicsSettings::GetFriction() const
    {
        return ActorCO->getFriction();
    }

    void ActorBasePhysicsSettings::SetRestitution(const Real& Restitution)
        { ActorCO->setRestitution(Restitution); }

    Real ActorBasePhysicsSettings::GetRestitution() const
        { return ActorCO->getRestitution(); }

    void ActorBasePhysicsSettings::SetKinematic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsKinematic() const
    {
        return ActorCO->isKinematicObject();
    }

    void ActorBasePhysicsSettings::SetStatic()
    {
        ActorCO->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    }

    bool ActorBasePhysicsSettings::IsStatic() const
    {
        return ActorCO->isStaticObject();
    }

    bool ActorBasePhysicsSettings::IsStaticOrKinematic() const
    {
        return ActorCO->isStaticOrKinematicObject();
    }

    void ActorBasePhysicsSettings::EnableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                /*if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{*/
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() - btCollisionObject::CF_NO_CONTACT_RESPONSE);
                //}
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    void ActorBasePhysicsSettings::DisableCollisionResponse()
    {
        World* GameWorld = World::GetWorldPointer();
        switch (Parent->GetType())
        {
            case ActorBase::Actorrigid:
            case ActorBase::Actorsoft:
            case ActorBase::Actorterrain:
            {
                /*if(Parent->IsInWorld())
                {
                    Parent->RemoveObjectFromWorld(GameWorld);
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                    Parent->AddObjectToWorld(GameWorld);
                }else{*/
                    ActorCO->setCollisionFlags(ActorCO->getCollisionFlags() + btCollisionObject::CF_NO_CONTACT_RESPONSE);
                //}
                break;
            }
            case ActorBase::Actorcharacter:
            {
                return;
                break;
            }
            default:
                return;
        }
    }

    bool ActorBasePhysicsSettings::GetCollisionResponse() const
    {
        return !(ActorCO->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    bool ActorBasePhysicsSettings::IsActive() const
    {
        int Activation = ActorCO->getActivationState();
        if( ACTIVE_TAG == Activation )
        {
            return true;
        }else{
            return false;
        }
    }

    void ActorBasePhysicsSettings::SetActivationState(phys::ActorActivationState State, bool Force)
    {
        if(Force)
            ActorCO->forceActivationState(State);
        else
            ActorCO->setActivationState(State);
    }

    ActorBasePhysicsSettings* ActorBasePhysicsSettings::GetBasePointer()
        { return dynamic_cast<ActorBasePhysicsSettings*>(this); }


    phys::ActorActivationState ActorBasePhysicsSettings::GetActivationState() const
        { return (phys::ActorActivationState)ActorCO->getActivationState(); }


    ActorRigidPhysicsSettings::ActorRigidPhysicsSettings(ActorRigid* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          RigidParent(Actor),
          ActorRB(PhysicsObject)
    {
        bool Dynamic = !IsStaticOrKinematic();
        CollisionGroup = Dynamic ? phys::CF_GenericFilter : phys::CF_StaticFilter;
        CollisionMask = Dynamic ? phys::CF_AllFilter : phys::CF_AllFilter ^ phys::CF_StaticFilter;
    }

    ActorRigidPhysicsSettings::~ActorRigidPhysicsSettings()
    {
    }

    void ActorRigidPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        if(CollisionShape::ST_TriMesh != Shape->GetType())
        {
            btScalar mass = this->ActorRB->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->GetBulletShape()->calculateLocalInertia(mass, inertia);
            this->ActorRB->setMassProps(mass,inertia);
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
            this->ActorRB->updateInertiaTensor();
        }else{
            this->ActorRB->setCollisionShape(Shape->GetBulletShape());
        }
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Shape);
    }

    void ActorRigidPhysicsSettings::SetDamping(const Real& LinDamping, const Real& AngDamping)
        { ActorRB->setDamping(LinDamping, AngDamping); }

    Real ActorRigidPhysicsSettings::GetLinearDamping() const
        { return ActorRB->getLinearDamping(); }

    Real ActorRigidPhysicsSettings::GetAngularDamping() const
        { return ActorRB->getAngularDamping(); }

    void ActorRigidPhysicsSettings::SetLinearVelocity(const Vector3& LinVel)
    {
        ActorRB->setLinearVelocity(LinVel.GetBulletVector3()); }

    Vector3 ActorRigidPhysicsSettings::GetLinearVelocity() const
    {
        Vector3 LinVel(ActorRB->getLinearVelocity());
        return LinVel;
    }

    void ActorRigidPhysicsSettings::SetAngularVelocity(const Vector3& AngVel)
    {
        ActorRB->setAngularVelocity(AngVel.GetBulletVector3());
    }

    Vector3 ActorRigidPhysicsSettings::GetAngularVelocity() const
    {
        Vector3 AngVel(ActorRB->getAngularVelocity());
        return AngVel;
    }

    void ActorRigidPhysicsSettings::SetIndividualGravity(const Vector3& Gravity)
    {
        ActorRB->setGravity(Gravity.GetBulletVector3());
    }

    Vector3 ActorRigidPhysicsSettings::GetIndividualGravity() const
    {
        Vector3 Grav(ActorRB->getGravity());
        return Grav;
    }

    Vector3 ActorRigidPhysicsSettings::GetForce() const
        { return Vector3(ActorRB->getTotalForce()); }

    Vector3 ActorRigidPhysicsSettings::GetTorque() const
        { return Vector3(ActorRB->getTotalTorque()); }

    void ActorRigidPhysicsSettings::ApplyForce(const Vector3& Force)
        { ActorRB->applyCentralForce(Force.GetBulletVector3()); }

    void ActorRigidPhysicsSettings::ApplyTorque(const Vector3& Torque)
        { ActorRB->applyTorque(Torque.GetBulletVector3()); }




    ActorSoftPhysicsSettings::ActorSoftPhysicsSettings(ActorSoft* Actor, btSoftBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          SoftParent(Actor),
          ActorSB(PhysicsObject)
    {
        CollisionGroup = phys::CF_GenericFilter;
        CollisionMask = phys::CF_AllFilter;
    }

    ActorSoftPhysicsSettings::~ActorSoftPhysicsSettings()
    {
    }

    void ActorSoftPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        // do nothing, cause soft bodies get unique custom shapes.
    }

    ActorTerrainPhysicsSettings::ActorTerrainPhysicsSettings(ActorTerrain* Actor, btRigidBody* PhysicsObject)
        : ActorBasePhysicsSettings(Actor,PhysicsObject),
          TerrainParent(Actor),
          ActorRB(PhysicsObject)
    {
        CollisionGroup = phys::CF_StaticFilter;
        CollisionMask = phys::CF_AllFilter ^ phys::CF_StaticFilter;
    }

    ActorTerrainPhysicsSettings::~ActorTerrainPhysicsSettings()
    {
    }

    void ActorTerrainPhysicsSettings::SetCollisionShape(CollisionShape* Shape)
    {
        AssignShape(Shape);
        if(CollisionShape::ST_TriMesh != Shape->GetType())
        {
            btScalar mass = this->ActorRB->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            Shape->GetBulletShape()->calculateLocalInertia(mass, inertia);
            this->ActorRB->setMassProps(mass,inertia);
            this->ActorRB->updateInertiaTensor();
        }
        this->ActorRB->setCollisionShape(Shape->GetBulletShape());
        World::GetWorldPointer()->GetCollisionShapeManager()->StoreShape(Shape);
    }
}

#ifdef PHYSXML
std::ostream& operator<< (std::ostream& stream, const phys::ActorBasePhysicsSettings& Ev)
{
    stream      << "<ActorBasePhysicsSettings Version=\"1\" "
                << "CollisionShape=\"" << Ev.GetCollisionShape()->GetName() << "\" "
                << "Friction=\"" << Ev.GetFriction() << "\" "
                << "Restitution=\"" << Ev.GetRestitution() << "\" "
                << "Kinematic=\"" << Ev.IsKinematic() << "\" "
                << "Static=\"" << Ev.IsStatic() << "\" "
                << "CollisionResponse=\"" << Ev.GetCollisionResponse() << "\" "
                << "CollisionGroup=\"" << Ev.GetCollisionGroup() << "\" "
                << "CollisionMask=\"" << Ev.GetCollisionMask() << "\" "
                << "ActivationState=\"" << Ev.GetActivationState() << "\" />";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorBasePhysicsSettings& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorBasePhysicsSettings", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}


phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorBasePhysicsSettings& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ActorBasePhysicsSettings") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetCollisionShape( phys::World::GetWorldPointer()->GetCollisionShapeManager()->GetShape(  OneNode.GetAttribute("CollisionShape").AsString()  ) );
            Ev.SetFriction(OneNode.GetAttribute("Friction").AsReal());
            Ev.SetRestitution(OneNode.GetAttribute("Restitution").AsReal());
            if (OneNode.GetAttribute("Kinematic").AsBool())
                { Ev.SetKinematic(); }
            if (OneNode.GetAttribute("Static").AsBool())
                { Ev.SetStatic(); }
            Ev.SetCollisionGroupAndMask(OneNode.GetAttribute("CollisionGroup").AsWhole(),OneNode.GetAttribute("CollisionMask").AsWhole());
            Ev.SetActivationState((phys::ActorActivationState)OneNode.GetAttribute("ActivationState").AsInt());

        }else{
            throw( phys::Exception("Incompatible XML Version for ActorBasePhysicsSettings: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorBasePhysicsSettings, found a ", OneNode.Name())));
    }
}

std::ostream& operator << (std::ostream& stream, const phys::ActorRigidPhysicsSettings& Ev)
{
    stream      << "<ActorRigidPhysicsSettings Version=\"1\" "
                << "AngularDamping=\"" << Ev.GetAngularDamping() << "\" "
                << "LinearDamping=\"" << Ev.GetLinearDamping() << "\">"
                << "<AngularVelocity>" << Ev.GetAngularVelocity() << "</AngularVelocity>"
                << "<LinearVelocity>" << Ev.GetLinearVelocity() << "</LinearVelocity>"
                << "<IndividualGravity>" << Ev.GetIndividualGravity() << "</IndividualGravity>"
                << "<TotalTorque>" << Ev.GetTorque() << "</TotalTorque>"
                << "<TotalForce>" << Ev.GetForce() << "</TotalForce>";

        operator<<(stream, static_cast<const phys::ActorBasePhysicsSettings>(Ev));

    stream      << "</ActorRigidPhysicsSettings>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ActorRigidPhysicsSettings& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ActorRigidPhysicsSettings", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ActorRigidPhysicsSettings& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ActorRigidPhysicsSettings") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetDamping(OneNode.GetAttribute("LinearDamping").AsReal(),OneNode.GetAttribute("AngularDamping").AsReal());

            phys::Vector3 TempVec;

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[5])
                {
                    case 'B':   //ActorBasePhysicsSettings
                        if(Name==phys::String("ActorBasePhysicsSettings"))
                        {
                            Child >> *(Ev.GetBasePointer());
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element B-\"",Name,"\"")) );
                        }
                        break;
                    case 'a':   //AngularVelocity
                        if(Name==phys::String("AngularVelocity"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetAngularVelocity(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element a-\"",Name,"\"")) );
                        }
                        break;
                    case 'r':   //LinearVelocity
                        if(Name==phys::String("LinearVelocity"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLinearVelocity(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element r-\"",Name,"\"")) );
                        }
                        break;
                    case 'i':   //IndividualGravity
                        if(Name==phys::String("IndividualGravity"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetIndividualGravity(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element i-\"",Name,"\"")) );
                        }
                        break;
                    case 'T':   //TotalTorque
                        if(Name==phys::String("TotalTorque"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.ApplyTorque(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element T-\"",Name,"\"")) );
                        }
                        break;
                    case 'F':   //TotalForce
                        if(Name==phys::String("TotalForce"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.ApplyForce(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ActorRigidPhysicsSettings: Includes unknown Element F-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for Light: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }


        }else{
            throw( phys::Exception("Incompatible XML Version for ActorRigidPhysicsSettings: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ActorRigidPhysicsSettings, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML


#endif
