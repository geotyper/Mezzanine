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
#ifndef _cameracontroller_h
#define _cameracontroller_h

#include "Graphics/renderablerayquery.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class CameraProxy;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Boundaries for rotation on one axis.
    ///////////////////////////////////////
    class MEZZ_LIB AngleData
    {
    public:
        /// @brief The current rotation on the represented axis in radians.
        Real CurrentAngle;
        /// @brief The upper limit for rotation in radians.
        Real UpperLimit;
        /// @brief The lower limit for rotation in radians.
        Real LowerLimit;

        /// @brief Class constructor.
        AngleData();

        /// @brief Sets the upper and lower limits on this axis.
        /// @note This class will perform a rollover of the angle when it exceeds
        /// positive or negative Pi.  This is to prevent eventual precision loss if
        /// the angle is rotated over an extended period of time.  So setting the
        /// limits to be beyond Pi or -Pi may give expected results as limits are
        /// checked and enforced first, and then rollover is calculated.
        /// @param Upper The upper bound limit of the angle on this axis.
        /// @param Lower The lower bound limit of the angle on this axis.
        void SetLimits(const Real Upper, const Real Lower);
        /// @brief Removes the current limits imposed on this axis.
        void UnsetLimits();
        /// @brief Checks to see if this has valid limits to be enforced.
        /// @return Returns true if this has valid limits, false otherwise.
        Boole HasEnforcableLimit() const;
        /// @brief Performs limits checks and sets the current angle.
        /// @param Angle The angle to be set.
        void SetAngle(Real Angle);
        /// @brief Increases or decreases the angle of rotation by a relative amount.
        /// @param Angle The amount of rotation to apply to the current axis value.
        void Rotate(const Real Angle);
    };//AngleLimits

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a simplified controller class for use with cameras.
    /// @details This class is useful for manipulating cameras to move around in simple ways,
    /// such as flying through a scene.
    ///////////////////////////////////////
    class MEZZ_LIB CameraController
    {
    public:
        /// @brief Possible options for determining how the camera should move relative to the world.
        enum MovementMode
        {
            CCM_Fly,        ///< CCM_Fly: This is the default option for every Camera Controller.  Allows the camera unrestrained movement throughout the scene.
            CCM_Walk        ///< CCM_Walk: This forces the camera to be only a certain distance above the terrain.
        };
    protected:
        /// @brief A ray casting tool used to determine surrounding geometry to limit movement.
        Graphics::RenderableRayQuery RayCaster;
        /// @brief A pointer to the Camera being controlled.
        Graphics::CameraProxy* Controlled;
        /// @brief A pointer to the angle limits for rotations on the Y axis.
        AngleData YawData;
        /// @brief A pointer to the angle limits for rotations on the X axis.
        AngleData PitchData;
        /// @brief A pointer to the angle limits for rotations on the Z axis.
        AngleData RollData;
        /// @brief The height at which the camera is to remain above the terrain.
        Real HoverHeight;
        /// @brief The mode with which the camera should move around the scene.
        MovementMode CurrentMMode;

        /// @brief Ensures the camera position is at the desired height above any world terrain.
        void CheckHeight();
        /// @brief Performs a raycast to get the distance to any terrain beneath the camera.
        /// @return Returns the distance from the camera to terrain in world units.
        Real FindDistanceToGround();
    public:
        /// @brief Blank constructor.
        CameraController();
        /// @brief Camera constructor.
        /// @param ToBeControlled The camera this controller is controlling.
        CameraController(Graphics::CameraProxy* ToBeControlled);
        /// @brief Class destructor.
        ~CameraController();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the camera being controlled by this controller.
        /// @param ToBeControlled The camera this controller is controlling.
        void SetControlledCamera(Graphics::CameraProxy* ToBeControlled);
        /// @brief Gets the camera this controller is controlling.
        /// @return Returns a camera pointer for the camera this controller is applied to.
        Graphics::CameraProxy* GetControlledCamera() const;

        ///////////////////////////////////////////////////////////////////////////////
        // CameraController Properties

        /// @brief Sets the movement mode for this camera/controller.
        /// @param MoveMode The MovementMode value for which mode you want applied.  See MovementMode enum for more info.
        void SetMovementMode(const MovementMode& MoveMode);
        /// @brief Gets the currently set movement mode.
        /// @return Returns an enum value representing the current movement mode.  See MovementMode enum for more info.
        MovementMode GetMovementMode() const;
        /// @brief Sets the hover distance for the camera while it's moving.
        /// @details Hover is only applied in CCM_Walk mode.  Default: 1.0.
        /// @param Hover The distance above the ground to hover, in world units.
        void SetHoverHeight(const Real& Hover);
        /// @brief Gets the distance the camera hovers over terrain while in CCM_Walk mode.
        /// @return Returns a Real represening the distance above terrain the camera is to hover, in world units.
        Real GetHoverHeight() const;
        /// @brief Sets rotational limits on the Y axis.
        /// @details The rotation range is from -Pi to Pi.
        /// @param UpperLimit The allowed upper rotation limit in radians.
        /// @param LowerLimit The allowed lower rotation limit in radians.
        void SetYawLimits(const Real& UpperLimit, const Real& LowerLimit);
        /// @brief Clears any set limits on yaw(Y axis) rotation.
        void RemoveYawLimits();
        /// @brief Sets rotational limits on the X axis.
        /// @details The rotation range is from -Pi to Pi.
        /// @param UpperLimit The allowed upper rotation limit in radians.
        /// @param LowerLimit The allowed upper rotation limit in radians.
        void SetPitchLimits(const Real& UpperLimit, const Real& LowerLimit);
        /// @brief Clears any set limits on pitch(X axis) rotation.
        void RemovePitchLimits();
        /// @brief Sets rotational limits on the Z axis.
        /// @details The rotation range is from -Pi to Pi.
        /// @param UpperLimit The allowed upper rotation limit in radians.
        /// @param LowerLimit The allowed upper rotation limit in radians.
        void SetRollLimits(const Real& UpperLimit, const Real& LowerLimit);
        /// @brief Clears any set limits on roll(Z axis) rotation.
        void RemoveRollLimits();

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        /// @brief Moves the camera forward.
        /// @param Units The distance to be moved in world units.
        void MoveForward(Real Units);
        /// @brief Moves the camera backward.
        /// @param Units The distance to be moved in world units.
        void MoveBackward(Real Units);
        /// @brief Moves the camera to the left.
        /// @param Units The distance to be moved in world units.
        void StrafeLeft(Real Units);
        /// @brief Moves the camera to the right.
        /// @param Units The distance to be moved in world units.
        void StrafeRight(Real Units);
        /// @brief Rotates the camera.
        /// @details This is a safer rotation method that applies all the checks and can lock behaviors
        /// such as roll if configured to do so.
        /// @param Yaw The amount to rotate the camera on it's local Y axis in Radians.
        /// @param Pitch The amount to rotate the camera on it's local X axis in Radians.
        /// @param Roll The amount to rotate the camera on it's local Z axis in Radians.
        void Rotate(Real Yaw, Real Pitch, Real Roll);
        /// @brief Rotates the camera.
        /// @details This is a freeform rotation method that will apply the rotation desired to the
        /// camera without any checks.  This is ideal for spacecraft style controls.
        /// @param Yaw The amount to rotate the camera on it's local Y axis in Radians.
        /// @param Pitch The amount to rotate the camera on it's local X axis in Radians.
        /// @param Roll The amount to rotate the camera on it's local Z axis in Radians.
        void Rotate6DOF(Real Yaw, Real Pitch, Real Roll);
    };//CameraController
}//Mezzanine

#endif
