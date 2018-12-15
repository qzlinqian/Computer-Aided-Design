#include "math/math.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "physics/spring.hpp"
#include "physics/body.hpp"
#include "physics/spherebody.hpp"
#include <iostream>

namespace _462 {

Spring::Spring()
{
    body1_offset = Vector3::Zero;
    body2_offset = Vector3::Zero;
    damping = 0.0;
}

void Spring::step( real_t dt )  // Not relevent with dt?
{
    // TODO apply forces to attached bodies
    Vector3 vector2TO1 = this->body1->position - this->body2->position;
    real_t distance = length(vector2TO1);
    Vector3 normalVector2TO1 = vector2TO1 / distance;
    // When they are static, F = -k      * x; x = |p1 - p2|    - l (x can be negative).
    real_t staticForce = - this->constant * (distance - this->equilibrium);
    // dx/dt (body1) = v//(body1) = (v \cdot p1p2) / |p1p2| = -(v \cdot p2p1) / |p2p1|
    body1->apply_force(-(staticForce + this->damping / distance * dot(vector2TO1, body1->velocity)) * normalVector2TO1, body1_offset);
    body2->apply_force((staticForce - this->damping / distance * dot(vector2TO1, body2->velocity)) * normalVector2TO1, body2_offset);
}

}


