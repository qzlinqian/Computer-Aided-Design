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
    // body1_offset = Vector3(body1->orientation * body1_offset);
    // body2_offset = Vector3(body2->orientation * body2_offset);
    Matrix3 rotation1 = Matrix3::Identity;
    body1->orientation.to_matrix(&rotation1);
    Matrix3 rotation2 = Matrix3::Identity;
    body2->orientation.to_matrix(&rotation2);
    Vector3 body1_offset_new = rotation1 * body1_offset;
    Vector3 body2_offset_new = rotation2 * body2_offset;
    // if (std::abs(body1_offset.x) < _EPSILON) body1_offset.x = 0;
    // if (std::abs(body1_offset.y) < _EPSILON) body1_offset.y = 0;
    // if (std::abs(body1_offset.z) < _EPSILON) body1_offset.z = 0;
    // if (std::abs(body2_offset.x) < _EPSILON) body2_offset.x = 0;
    // if (std::abs(body2_offset.y) < _EPSILON) body2_offset.y = 0;
    // if (std::abs(body2_offset.z) < _EPSILON) body2_offset.z = 0;

    // std::cout<<"offset1"<<body1_offset<<std::endl;

    Vector3 vectorSpring = this->body1->position + body1_offset_new - this->body2->position - body2_offset_new;
    // std::cout<<vectorSpring<<std::endl;
    Vector3 normalvectorSpring = normalize(vectorSpring);
    // Vector3 vector2TO1 = normalize(this->body1->position - this->body2->position - this->body2_offset);
    // When they are static, F = -k      * x; x = |p1 - p2|    - l (x can be negative).
    // real_t staticForce = this->constant * (length(vectorSpring) - this->equilibrium);
    // std::cout<<"f"<<staticForce<<std::endl;
    // std::cout<<"x"<<length(vectorSpring)- this->equilibrium<<std::endl;
    // Maybe the unit of constant is N/m, but that of position is cm (cm/s for v, cm/s2 for a). The constant is too large! NO! It IS m!
    // dx/dt (body1) = v//(body1) = (v \cdot p1p2) / |p1p2| = -(v \cdot p2p1) / |p2p1|
    // body1->apply_force(-staticForce * normalvectorSpring - this->damping * dot(vector2TO1, body1->velocity) * vector2TO1, body1_offset);
    // body2->apply_force(staticForce * normalvectorSpring - this->damping * dot(vector2TO1, body2->velocity) * vector2TO1, body2_offset);
    // body1->reset_force();
    // body2->reset_force();

    // body1->apply_force(-staticForce * normalvectorSpring, body1_offset);
    // body2->apply_force(staticForce * normalvectorSpring, body2_offset);
    // body1->apply_force(-staticForce * normalvectorSpring, Vector3::Zero);
    // body2->apply_force(staticForce * normalvectorSpring, Vector3::Zero);
    // body1->damping_adapt()
    Vector3 f = - constant * (length(vectorSpring) - this->equilibrium) * normalvectorSpring 
                         - damping * (dot(body1->velocity - body2->velocity, vectorSpring)) * vectorSpring;
    
    body1->apply_force(-prev_force, prev_body1_offset);
    body2->apply_force(-prev_force, prev_body2_offset);                     

    body1->apply_force(f, body1_offset_new);
    body2->apply_force(f, body2_offset_new);

    prev_force = f;
    prev_body1_offset = body1_offset_new;
    prev_body2_offset = body2_offset_new;
}

}


