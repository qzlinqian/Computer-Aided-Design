#include "physics/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

namespace _462 {

real_t SphereBody::stopVelocity = 0.01f;

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
    // damping = 0;
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
    std::cout<<"a"<<a0<<std::endl;
    // real_t r = motion_damping / this->mass;
    // Vector3 K1 = this->velocity, L1 = a0;
    // Vector3 K2 = this->velocity + L1 * dt/2;
    // Vector3 L2 = a0 - r * K2;
    // Vector3 K3 = this->velocity + L2 * dt/2;
    // Vector3 L3 = a0 - r * K3;
    // Vector3 K4 = this->velocity + L3 * dt;
    // Vector3 L4 = a0 - r * K4;
    // this->velocity += (K1 + 2 * K2 + 2 * K3 + K4) * dt /6;
    // this->position += (L1 + 2 * K2 + 2 * K3 + K4) * dt /6;
    this->position += this->velocity * dt + 0.5 * a0 * dt * dt;
    if (this->mass > 0.01f) this->velocity += a0 * dt;
    // std::cout<<this->position<<std::endl; 
    std::cout<<"v"<<this->velocity<<std::endl;
    // while (this->position.y < -1.0f) this->position.y += 2.0f;
    this->sphere->position = this->position;

    return this->velocity * dt;
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
    real_t J = 0.4 * this->mass * this->radius * this->radius;
    Vector3 alpha = this->torque / J;
    Vector3 step_angle = this->angular_velocity * dt + 0.5 * alpha * dt * dt;
    Quaternion r(step_angle, length(step_angle));
    this->angular_velocity += alpha * dt;
    std::cout<<"torque"<<this->torque<<std::endl;
    std::cout<<"alpha"<<angular_velocity<<std::endl;
    // When torque = 0, r is invalid!
    if (length(step_angle) < 0.0000001f) r = Quaternion::Identity;
    // std::cout<<r<<std::endl;
    // std::cout<<this->orientation<<std::endl;
    this->orientation = r * this->orientation;
    this->sphere->orientation = this->orientation;

    return this->angular_velocity * dt;
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    // TODO apply force/torque to sphere
    // Update every time but not add up. 
    // Cause the spring force should update all the time and derive the change of the force is painful
    this->force += f;
    this->torque = cross(offset,f);
    // offset = Vector3(this->orientation * offset);
}

void SphereBody::reset_force(){
    this->force = Vector3::Zero;
    this->torque = Vector3::Zero;
}

bool SphereBody::velocity_check(){
    if (length(this->velocity) < stopVelocity){
        this->velocity = Vector3::Zero;
        return false;
    }

    if (length(this->angular_velocity) < stopVelocity){
        this->angular_velocity = Vector3::Zero;
        return false;
    }
    return true;
}

// void SphereBody::damping_adapt(real_t damping_this){
//     this->damping = damping_this;
// }

// void SphereBody::apply_another_force(const Vector3& f, const Vector3& offset){
//     this->force += f;
//     this->torque += cross(f, offset);
// }

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
