#include "physics/spherebody.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "scene/sphere.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

namespace _462 {

real_t SphereBody::stopVelocity = 0.0001;
real_t SphereBody::stopOmega = 0.000005;

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
    // Calculating damping should be done in Spring (!= Spring.daming. 
    // Cause we cannot multiple damping with velocity simply but have to know the two attached bodies' relative position first)
    // So I have to ignore it, or use it as friction later.
    Vector3 a0 = this->force / this->mass;
    // this->position += this->velocity * dt + 0.5 * a0 * dt * dt;
    if (this->mass > 0.01f) this->velocity += a0 * dt;
    velocity_check();

    Vector3 K1, K2, K3, K4;
    K1 = dt * velocity;
    K2 = dt * step_velocity(dt/2.0, 0);
    K3 = dt * step_velocity(dt/2.0, 0);
    K4 = dt * step_velocity(dt, 0);
    Vector3 deltaX = 1/6.0 * K1 + 1/3.0 * K2 + 1/3.0 * K3 + 1/6.0 * K4;
    this->position += deltaX;
    this->sphere->position = this->position;

    return deltaX;
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
    real_t Intertia = 0.4 * this->mass * this->radius * this->radius;
    Vector3 alpha = this->torque / Intertia;
    this->angular_velocity += alpha * dt;
    omega_check();

    Vector3 K1, K2, K3, K4;
    K1 = dt * angular_velocity;
    K2 = dt * step_omega(dt/2.0, 0);
    K3 = dt * step_omega(dt/2.0, 0);
    K4 = dt * step_omega(dt, 0);
    Vector3 deltaTheta = 1/6.0 * K1 + 1/3.0 * K2 + 1/3.0 * K3 + 1/6.0 * K4;
    // if (deltaTheta.x < )

    Quaternion r(deltaTheta, length(deltaTheta));
    // std::cout<<"torque"<<this->torque<<std::endl;
    // std::cout<<"alpha"<<angular_velocity<<std::endl;
    // When torque = 0, r is invalid!
    if (length(deltaTheta) < 0.0000001f) r = Quaternion::Identity;
    // std::cout<<r<<std::endl;
    // std::cout<<this->orientation<<std::endl;
    this->orientation = r * this->orientation;
    this->sphere->orientation = this->orientation;

    return deltaTheta;
}

void SphereBody::apply_force( const Vector3& f, const Vector3& offset )
{
    // TODO apply force/torque to sphere
    // Update every time but not add up. 
    // Cause the spring force should update all the time and derive the change of the force is painful
    force += f;
    torque += cross(offset, f);
}

// void SphereBody::reset_force(){
//     this->force = Vector3::Zero;
//     this->torque = Vector3::Zero;
// }

bool SphereBody::velocity_check(){
    if (length(this->velocity) < stopVelocity){
        this->velocity = Vector3::Zero;
        return false;
    }
    return true;
}

bool SphereBody::omega_check(){  
    if (length(this->angular_velocity) < stopOmega){
        this->angular_velocity = Vector3::Zero;
        return false;
    }
}

Vector3 SphereBody::step_velocity(double dt, double motion_damping){
    Vector3 a0 = this->force / this->mass;
    return this->velocity + a0 * dt;
}

Vector3 SphereBody::step_omega(double dt, double motion_damping){
    Vector3 alpha0 = this->torque / (0.4 * this->mass * this->radius * this->radius);
    return this->angular_velocity + alpha0 * dt;
}

}
