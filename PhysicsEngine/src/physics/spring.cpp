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
    prev_force = Vector3::Zero;
    // prev_body1_offset = body1_offset;
    // prev_body2_offset = body2_offset;
}

void Spring::step( real_t dt )  // Not relevent with dt?
{
    // TODO apply forces to attached bodies
    // body1_offset = Vector3(body1->orientation * body1_offset);  // That's totally wrong! Stuck me a day!
    // body2_offset = Vector3(body2->orientation * body2_offset);
    Matrix3 rotation1 = Matrix3::Identity;
    body1->orientation.to_matrix(&rotation1);
    Matrix3 rotation2 = Matrix3::Identity;
    body2->orientation.to_matrix(&rotation2);
    Vector3 body1_offset_new = rotation1 * body1_offset;
    Vector3 body2_offset_new = rotation2 * body2_offset;

    Vector3 vectorSpring = this->body1->position + body1_offset_new - this->body2->position - body2_offset_new;
    Vector3 normalvectorSpring = normalize(vectorSpring);

    Vector3 f = - constant * (length(vectorSpring) - this->equilibrium) * normalvectorSpring 
                         - damping * (dot(body1->velocity - body2->velocity, vectorSpring)) * vectorSpring;
    
    // The force cannot reset in the main loop, so apply a minus of the previous force first
    body1->apply_force(-prev_force, prev_body1_offset);
    body2->apply_force(-prev_force, prev_body2_offset);                     

    body1->apply_force(f, body1_offset_new);
    body2->apply_force(f, body2_offset_new);

    prev_force = f;
    prev_body1_offset = body1_offset_new;
    prev_body2_offset = body2_offset_new;
}

}


