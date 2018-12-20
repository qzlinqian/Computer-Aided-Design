## PhysicsEngine

The final project of the course. It's a course assignment of a Computer Aided Design couse in CMU. The detailed instruction is in *addtionalMaterial.pdf* and *README*. The source edited files are src/physics/collision\*, physics\*, spherebody\*, spring\*.

#### Demo

![](demo/1.gif)

![](demo/2.gif)

![](demo/3.gif)

![](demo/4.gif)

![](demo/5.gif)

![](demo/6.gif)

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
* Collision detection between spheres should not avoid check with two same bodies, e.g., when 1 & 4 is not in collision, but 3 & 4 is, and 4 & 1 may then in collision after velocity update. Or some of the spheres gain would energy and more spheres in newtows_cradle scene would move.