#ifndef _462_PHYSICS_SPHEREBODY_HPP_
#define _462_PHYSICS_SPHEREBODY_HPP_

#include "scene/sphere.hpp"
#include "physics/body.hpp"

namespace _462 {

class Sphere;

class SphereBody : public Body
{
public:
    static real_t stopVelocity;
    Sphere* sphere;
    real_t radius;
    real_t mass;
    Vector3 force;
    Vector3 torque;

    SphereBody( Sphere* geom );
    virtual ~SphereBody() { }
    virtual Vector3 step_position( real_t dt, real_t motion_damping );
    virtual Vector3 step_orientation( real_t dt, real_t motion_damping );
    virtual void apply_force( const Vector3& f, const Vector3& offset );
    virtual void reset_force();

    bool velocity_check();
    // void apply_another_force(const Vector3& f, const Vector3& offset);
    // void RK4_position(real_t dt);
    // void RK4_velocity(real_t dt);
};

}

#endif
