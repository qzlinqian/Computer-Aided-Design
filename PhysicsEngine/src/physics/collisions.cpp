#include "physics/collisions.hpp"
#include <cmath>

namespace _462 {

bool collides( SphereBody& body1, SphereBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    Vector3 d = body2.position - body1.position;
    real_t dis = length(d);
    if (dis > body1.radius + body2.radius) return false;  // d>r1+r2
    // Then update velocity
    Vector3 v1 = body1.velocity - body2.velocity;
    if (dot(d, v1) <= 0) return false; // apart or the same
    d /= dis;
    Vector3 v22 = dot(v1, d) * 2 * body1.mass / (body1.mass + body2.mass) * d;
    body2.velocity = v22 + body2.velocity;
    body1.velocity -= body2.mass / body1.mass * v22;

    // Collision Damping
    body1.velocity -= collision_damping * body1.velocity;
    body2.velocity -= collision_damping * body2.velocity;
    return true;
}

bool collides( SphereBody& body1, TriangleBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    Vector3 a = body1.position - body2.vertices[0];
    Vector3 normal = normalize(cross(body2.vertices[0]-body2.vertices[1], body2.vertices[0]-body2.vertices[2]));
    real_t d = dot(a, normal);
    if (std::abs(d) > body1.radius) return false;  // d > r
    // Projection near the plane
    Vector3 projection = body1.position - d * normal;
    // real_t r_project = std::sqrt(body1.radius * body1.radius - d * d);  // radius in projection plane
    Vector3 t1 = cross(body2.vertices[0]-projection, body2.vertices[1]-projection);
    Vector3 t2 = cross(body2.vertices[1]-projection, body2.vertices[2]-projection);
    Vector3 t3 = cross(body2.vertices[2]-projection, body2.vertices[0]-projection);
    if (dot(t1, t2) > 0 && dot(t2, t3) > 0){ // point is in the triangle
        body1.velocity -= 2 * dot(body1.velocity, normal) * normal;
        body1.velocity -= collision_damping * body1.velocity;
        return true;
    }

    real_t proj_line[3];
    proj_line[0] = dot(projection-body2.vertices[0], body2.vertices[1]-body2.vertices[0]) 
                   / squared_distance(body2.vertices[1], body2.vertices[0]);  // = v0p0 / v0v1 (vector)
    proj_line[1] = dot(projection-body2.vertices[1], body2.vertices[2]-body2.vertices[1]) 
                   / squared_distance(body2.vertices[2], body2.vertices[1]);  // = v1p1 / v1v2 (vector)
    proj_line[2] = dot(projection-body2.vertices[2], body2.vertices[0]-body2.vertices[2]) 
                   / squared_distance(body2.vertices[0], body2.vertices[2]);  // = v2p2 / v2v0 (vector)

    for (int i=0;i<3;i++){  // If the projection is out of the line, then take the nearest vertex
        if (proj_line[i] < 0) proj_line[i] = 0;
        if (proj_line[i] > 1) proj_line[i] = 1;

        // Assume that there is only one project point of the project center in collision, or it would be detected earlier
        if (distance(body1.position, proj_line[i] * (body2.vertices[(i+1)%3]-body2.vertices[i]) + body2.vertices[i]) < body1.radius){
            // In collision! Then update the velocity
            // Let the possibility of collide with the vertex is 0. Allocate the case into collide with line
            Vector3 normal_line = normalize(cross(normal, body2.vertices[(i+1)%3]-body2.vertices[i]));
            // Velocity update
            body1.velocity -= 2 * dot(body1.velocity, normal_line) * normal_line;
            body1.velocity -= collision_damping * body1.velocity;
            return true;
        }
    }
    
    // Out of range
    return false;
}

bool collides( SphereBody& body1, PlaneBody& body2, real_t collision_damping )
{
    // TODO detect collision. If there is one, update velocity
    Vector3 a = body1.position - body2.position;
    real_t d = dot(a, body2.normal);
    if (std::abs(d) > body1.radius) return false;  // d > r
    // Then update velocity
    body1.velocity -= 2 * dot(body1.velocity, body2.normal) * body2.normal;
    body1.velocity -= collision_damping * body1.velocity;
	return true;
}

// void collideWithPoint(SphereBody& body, Vector3 vertice){
//     // Simplify, see sphere as a point
// } That's too complex! Allocate the point to a line!
// I was about to discuss the cases where sphere collides with a vertice of the triangle. But you konw, that possibility equals to zero and that's too complex.

}
