## 物理引擎

编辑的代码为 src/physics/collision\*, physics\*, spherebody\*, spring\*.

#### 实现过程

* **Collision**碰撞检测

  + 球体与球体：计算中心距，小于两球半径之和时碰撞。注意在检测时，在两个球的检测第二次出现时，不应跳过此帧中刚刚检测过的那两个球。因为比如球1和4本来不碰撞，但3和4发生了碰撞并更新了速度，那4和1可能就会碰撞了，所以多次检测是有益的。此时就需要注意球体与自身的碰撞检测（中心距为0时及时返回）
  + 球与平面：距离小于球半径且球运动方向向平面时碰撞。
  + 球与三角形：当球在三角形所在平面上的投影P落在三角形内部时（可用<img src="https://latex.codecogs.com/gif.latex?\overrightarrow{PA_0}&space;\times&space;\overrightarrow{PA_1},&space;\overrightarrow{PA_1}\times&space;\overrightarrow{PA_2},&space;\overrightarrow{PA_2}\times&space;\overrightarrow{PA_0}" title="\overrightarrow{PA_0} \times \overrightarrow{PA_1}, \overrightarrow{PA_1}\times \overrightarrow{PA_2}, \overrightarrow{PA_2}\times \overrightarrow{PA_0}" />同向判断），等同于球与平面碰撞。在外部时，将P再向三边作投影，若落在边外，取离其最近的端点，一次考察上述三个点，若与球心距离小于半径，则发生碰撞。碰撞点即为该点，可由此计算法向量。

* **Spring**弹簧力的施加

  * 弹簧的step函数主要需要对各连接的物块施加力。这里弹簧的两个端点为 `body1->position + body1_offset` 和 `body2->position - body2_offset` ，而且这两个offset需要随物体一起旋转。我曾一度对两个offset进行了这样的更新 `body1_offset = Vector3(body1->orientation * body1_offset);`，就导致每一帧offset都会转很大的角度。
  * 由于每帧都需要调用step函数，需要清除前面的弹簧力并施加新的弹簧力。我曾在每一帧末尾都将各个球体上的力和力矩清零，但有时候力和力矩会有初值，清零会导致错误。所以应该记录前一帧的力和offset（力臂），在施加新的力前，在同一力臂处施加前一帧的力的负值。

* **SpehreBody**球体的积分

  * 在弹簧作用下，球体的加速度为速度与位置的函数，而且不显式含时间t，这需要使用二阶的四阶龙格库塔，实现比较困难。而阻尼力的大小涉及速度矢量的投影，这就需要在Spring类中求值，但Spring对力的更新和球体对速度和位移的更新又需要交替进行，所以都应该在Physics类中调用，这就比较麻烦，而我最终直接将弹簧力在一个dt内视作恒力计算。

  * 计算时若直接使用<img src="https://latex.codecogs.com/gif.latex?\begin{cases}&space;v&space;=&space;v_0&space;&plus;&space;dt&space;\cdot&space;a&space;\\&space;x&space;=&space;dt&space;\cdot&space;v_0&space;&plus;&space;\frac{1}{2}&space;a\cdot&space;dt^2&space;\end{cases}" title="\begin{cases} v = v_0 + dt \cdot a \\ x = dt \cdot v_0 + \frac{1}{2} a\cdot dt^2 \end{cases}" />计算则会导致较大的误差。我使用的是类似RK4的方式<img src="https://latex.codecogs.com/gif.latex?\begin{align*}&space;K1&space;=hf(t),&space;K2&=hf(t&plus;dt/2),&space;K3=hf(t&plus;dt/2),&space;K4=hf(t&plus;dt),&space;\\&space;dx&space;&=(K1&plus;2K2&plus;2K3&plus;K4)/6&space;\end{align*}" title="\begin{align*} K1 =hf(t), K2&=hf(t+dt/2), K3=hf(t+dt/2), K4=hf(t+dt), \\ dx &=(K1+2K2+2K3+K4)/6 \end{align*}" />

    角位置更新也同理。

* **Physics**整体的处理

  * **SphereBody**中已对球体位置和角位置进行了更新，所以在**Physics**内只需要调用即可。
  * 首先进行碰撞检测，依次遍历球体，对每个球体与其余物体进行碰撞检测（包括球体）并更新速度。
  * 遍历Springs，依次对所连接物体施加力。
  * 遍历球体，对每个球体进行更新。

#### 编译环境

* MacOS Mojave
* OpenGL and other framework
* SDL1.2 (not 2.0)
* libpng-1.2.57 (this version only)
* GLEW, GLUT

#### 编译及运行

```shell
make # compile with make
./physics scenes/collision_stress.scene # or other scene in scenes dir
```
