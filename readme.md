## Computer Aided Design

The source code of the assignments of Fundamentals of Computer Aided Design.

#### Demo

* **Assignment 1** -- load a 3D model from obj file; use keyboard to change visual angle and color.

  <img src="http://pic.qianl.in/uploads/big/df081efaf0a692b8fbe284473f4edcd6.gif" width="50%" height="50%">

* **Assignment 2** -- rotating stars. The stars rotate as logarithmic spiral.

  <img src="http://pic.qianl.in/uploads/big/b3c620bf4b440b3da8f77672b1dce9a4.gif" width="50%" height="50%">

* **Assignment 3** -- snow falling. The snowflakes have different sizes and their number increases gradually.

  <img src="http://pic.qianl.in/uploads/big/66f421a5a29e61c968c3b1794d01d158.gif" width="50%" height="50%">

* **Assignment 4** -- solar system.

  <img src="http://pic.qianl.in/uploads/big/dba8fa19ed5d9a1763a8365d2c01704c.gif" width="50%" height="50%">

* **Assignment 5** -- Bezier surface.

  <img src="http://pic.qianl.in/uploads/big/01ce4ec550f667ff31d5ba5bfcb80fc3.gif" width="50%" height="50%">

* **Assignment 7** -- illumination effect. Add lights to the model in assignment 1.

  <img src="http://pic.qianl.in/uploads/big/f949121fbe084c41b934eba632746e08.gif" width="50%" height="50%">

* Assignment 6 is a design work, so there's no code :-D.
* **Final Project** -- [Physics Engine](https://github.com/qzlinqian/Computer-Aided-Design/tree/master/PhysicsEngine).

#### Environment
* MacOS Mojave + XCode 10
* The other dependencies are listed in each sub-dirs.

#### Note
* The *include* directory contains the public header of all the *test\** and *assignment\** directories.
* In the *test\** directories, all of the source code was given by the TA except the *CMakeList.txt*, which is written by me to compile them on my local machine for further use. The given code was also changed a little to fit my local environment.
* In the *assignment\** directories are the assignments. The readme files were written in Chinese (to hand in) and translating is a tiring work... so there is only implementation detail in Chinese.
* The *PhysicsEngine* directory is the final project and the detailed illustration is in the **readme.md** file in it. There is also a Chinese version *readme-ch.md*.

#### Compile
I didn't group all the programs with one compile file. They should be compiled separately.
* The *test\** and *assignment\** programs should be compiled with the *include* directory and other dependencies mentioned in the readme file in the sub-dir. Just use **cmake** to compile.
* The *PhysicsEngine* was built on the given framework. Compile it with **make**.


