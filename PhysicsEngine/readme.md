## PhysicsEngine

The final project of the course. It's a course assignment of a Computer Aided Design couse in CMU. The detailed instruction is in *addtionalMaterial.pdf* and *README*. The source edited files are src/physics/collision\*, physics\*, spherebody\*, spring\*.

#### Demo

<table><tr><td><center><img src="http://pic.qianl.in/uploads/big/ce8708b92442150eabce861137ca6bd2.gif">Collision</center></td><td><center><img src="http://pic.qianl.in/uploads/big/83b25accab5e1d6f86e6f7efedd7b3cd.gif">Collision Stress</center></td><td><center><img src="http://pic.qianl.in/uploads/big/d9d5fe0853380355adad69b45b0203cc.gif">Damping</center></td></tr><tr><td><center><img src="http://pic.qianl.in/uploads/big/02a8842707d1034b3aabf51237450f4f.gif">Spring Rotation</center></td><td><center><img src="http://pic.qianl.in/uploads/big/de1bb04b287d48db31f77504a8706f53.gif">Newtons Cradle</center></td><td><center><img src="http://pic.qianl.in/uploads/big/49991ab19eb29d5a52b871e64fabdad9.gif">Rotation</center></td></tr></table>


#### Implement Details

* **Collision** Detection
  * Sphere & Sphere: A collision would happen when <img src="https://latex.codecogs.com/gif.latex?\overrightarrow{O_1O_2}<r_1&plus;r_2" title="\overrightarrow{O_1O_2}<r_1+r_2" /> and <img src="https://latex.codecogs.com/gif.latex?(\vec{v}_1-\vec{v}_2)\cdot\overrightarrow{O_1O_2}>0" title="(\vec{v}_1-\vec{v}_2)\cdot\overrightarrow{O_1O_2}>0" />. 
  * Sphere & Plane: d<r and sphere moves towards the plane.
  * Sphere & Triangle: When the projection of the sphere center on the triangle's plane falls in the triangle ( i.e., <img src="https://latex.codecogs.com/gif.latex?\overrightarrow{PA_0}&space;\times&space;\overrightarrow{PA_1},&space;\overrightarrow{PA_1}\times&space;\overrightarrow{PA_2},&space;\overrightarrow{PA_2}\times&space;\overrightarrow{PA_0}" title="\overrightarrow{PA_0} \times \overrightarrow{PA_1}, \overrightarrow{PA_1}\times \overrightarrow{PA_2}, \overrightarrow{PA_2}\times \overrightarrow{PA_0}" /> parallel with each other), same as the plane case. Otherwise, consider the projection of P on the three edges' lines (if falls outside, the select the nearest vertex). Calculate the  distance of the three selected points to the sphere center respectively; one of them is the nearest distance of the two bodies, which is also the collision point if collision happens.
* **Spring** Force Apply
  * The step() function applies forces and torques to the attached bodies.
  * The force is applied every frame, so we should clear the force applied in the previous frame.
* **SphereBody** Integration
  * The force varies with velocity and position. So we need to use 2-degree RK4 in **Physics** and call the Spring::step to update force (acceleration). The motion_damping used in **SphereBody**'s step functions also need to be updated by **Spring**, which is a little troublesome. I just see force as a constant in every frame.
  * I was confused by the instructions, so I didn't know where to implement the RK4. The time is limited to complete this task, so I used my friends' method --- <img src="https://latex.codecogs.com/gif.latex?\begin{align*}&space;K1&space;=hf(t),&space;K2&=hf(t&plus;dt/2),&space;K3=hf(t&plus;dt/2),&space;K4=hf(t&plus;dt),&space;\\&space;dx&space;&=(K1&plus;2K2&plus;2K3&plus;K4)/6&space;\end{align*}" title="\begin{align*} K1 =hf(t), K2&=hf(t+dt/2), K3=hf(t+dt/2), K4=hf(t+dt), \\ dx &=(K1+2K2+2K3+K4)/6 \end{align*}" />

* **Physics** Move Forward
  * The position and angular_position of a SphereBody::Sphere has already been updated in **SphereBody**, so in **Physics** the only thing to do is to call them. The gravity is applied in the initialization.
  * Fisrt, check collisions by traverse the sphere list and check them with the other bodies (including sphere).
  * Traverse the spring list, apply forces.
  * Traverse all the sphere and update them.

#### Environment & Dependencies

* MacOS Mojave
* OpenGL and other framework
* SDL1.2 (not 2.0)
* libpng-1.2.57 (this version only)
* GLEW, GLUT

#### Usage

```shell
make # compile with make
./physics scenes/collision_stress.scene # or other scene in scenes dir
```



#### Unsolved Issues

* Integrate with Eurler's method would cause energy gaining in the spring system. But I also didn't realize a RK4.
* The OpenGL version is too old and every time I run a scene, I should minimize the window and reopen it. Just as my issued after updated to Mojave, but in that case resizing or moving window works:). I even had to slow down the process to record some scenes.

#### Reminders

* The force may be added during initializing, so do not reset force during the loop.
* The spring attached points are `body1->position + body1_offset` and `body2->position - body2_offset` . The offset should also rotate with the body. Moreover, I used to rotate the offset vector by `body1_offset = Vector3(body1->orientation * body1_offset);`, which can cause a huge deviation and as a result, the ball flies out of the plane.
* Collision detection between two spheres should avoid check whether they are approaching first, or more spheres in newtows_cradle scene would move.
