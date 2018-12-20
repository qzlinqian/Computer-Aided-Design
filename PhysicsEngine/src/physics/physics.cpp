#include "physics/physics.hpp"

namespace _462 {

Physics::Physics()
{
    reset();
}

Physics::~Physics()
{
    reset();
}

void Physics::step( real_t dt )
{
    // TODO step the world forward by dt. Need to detect collisions, apply
    // forces, and integrate positions and orientations.
    //
    // Note: put RK4 here, not in any of the physics bodies
    //
    // Must use the functions that you implemented
    //
    // Note, when you change the position/orientation of a physics object,
    // change the position/orientation of the graphical object that represents
    // it
    // TODO collision check & velocity update
    // dt /= 10;  // To slow down the speed of the frame, or I couldnot see it :)

    for ( PlaneList::iterator i = planes.begin(); i != planes.end(); i++ ){
        for ( SphereList::iterator j = spheres.begin(); j != spheres.end(); j++ ){
            collides(*(*j),*(*i),collision_damping);
        }
    }

    for ( TriangleList::iterator i = triangles.begin(); i != triangles.end(); i++ ){
        for ( SphereList::iterator j = spheres.begin(); j != spheres.end(); j++ ){
            collides(*(*j),*(*i),collision_damping);
        }
    }

    for ( SphereList::iterator i = spheres.begin(); i != spheres.end(); i++ ){
        for ( SphereList::iterator j = spheres.begin(); j != spheres.end(); j++ ){
            if (j<=i) continue;
            collides(*(*j),*(*i),collision_damping);
        }
    }


    for (SpringList::iterator i = springs.begin(); i != springs.end(); i++){
        (*i)->step(dt);
    }
    
    for (SphereList::iterator i = spheres.begin(); i != spheres.end(); i++){
        (*i)->step_position(dt, 0);
        (*i)->step_orientation(dt, 0);
    }
}

void Physics::add_sphere( SphereBody* b )
{
    spheres.push_back( b );
    b->apply_force(gravity * b->mass, Vector3::Zero);
}

size_t Physics::num_spheres() const
{
    return spheres.size();
}

void Physics::add_plane( PlaneBody* p )
{
    planes.push_back( p );
}

size_t Physics::num_planes() const
{
    return planes.size();
}

void Physics::add_triangle( TriangleBody* t )
{
    triangles.push_back( t );
}

size_t Physics::num_triangles() const
{
    return triangles.size();
}

void Physics::add_spring( Spring* s )
{
    springs.push_back( s );
    s->prev_body1_offset = s->body1_offset;
    s->prev_body2_offset = s->body2_offset;
}

size_t Physics::num_springs() const
{
    return springs.size();
}

void Physics::reset()
{
    for ( SphereList::iterator i = spheres.begin(); i != spheres.end(); i++ ) {
        delete *i;
    }
    for ( PlaneList::iterator i = planes.begin(); i != planes.end(); i++ ) {
        delete *i;
    }
    for ( TriangleList::iterator i = triangles.begin(); i != triangles.end(); i++ ) {
        delete *i;
    }
    for ( SpringList::iterator i = springs.begin(); i != springs.end(); i++ ) {
        delete *i;
    }

    spheres.clear();
    planes.clear();
    triangles.clear();
    springs.clear();
    
    gravity = Vector3::Zero;
	collision_damping = 0.0;
}

}
