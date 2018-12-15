#include "physics/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

namespace _462 {

SphereBody::SphereBody( Sphere* geom )
{
    sphere = geom;
    position = sphere->position;
    radius = sphere->radius;
    orientation = sphere->orientation;
    mass = 0.0;
    velocity = Vector3::Zero;
    angular_velocity = Vector3::Zero;
    force = Vector3::Zero;
    torque = Vector3::Zero;
}

Vector3 SphereBody::step_position( real_t dt, real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in position dt in the future
    // I have to ignore the change in spring force (with x). 
    // Here we see force as a const, and there's damping relating to the velocity, i.e., m v' = Force - cv; v = x'
    Vector3 a0 = this->force / this->mass;
    real_t r = motion_damping / this->mass;
    Vector3 K1 = this->velocity, L1 = a0;
    Vector3 K2 = this->velocity + L1 * dt/2;
    Vector3 L2 = a0 - r * K2;
    Vector3 K3 = this->velocity + L2 * dt/2;
    Vector3 L3 = a0 - r * K3;
    Vector3 K4 = this->velocity + L3 * dt;
    Vector3 L4 = a0 - r * K4;
    this->velocity += (K1 + 2 * K2 + 2 * K3 + K4) * dt /6;
    this->position += (L1 + 2 * K2 + 2 * K3 + K4) * dt /6;

    return position;
}

Vector3 SphereBody::step_orientation( real_t dt, real_t motion_damping )
{
    // Note: This function is here as a hint for an approach to take towards
    // programming RK4, you should add more functions to help you or change the
    // scheme
    // TODO return the delta in orientation dt in the future
    // vec.x = rotation along x axis
    // vec.y = rotation along y axis
    // vec.z = rotation along z axis

    return Vector3::Zero;
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    // TODO apply force/torque to sphere
    // Update every time but not add up. 
    // Cause the spring force should update all the time and derive the change of the force is painful
    this->force = f;
    this->torque = cross(f, offset);
}

void SphereBody::add_force(const Vector3& f, const Vector3& offset){
    this->force += f;
    this->torque += cross(f, offset);
}

// Vector3 SphereBody::RK4_position(real_t dt){
//     // Below are the sub-process of RK4, where dt can be h or h/2.
//     this->position += dt * this->velocity;
//     this->velocity += dt * this->force / this->mass;
//     return this->velocity;
// }

// Vector3 SphereBody::RK4_velocity(real_t dt){
//     return this->force / this->mass;
// }

}
