## 旋转的星星

![](http://www.qianl.in/2019/01/10/physicsEngine/a2.gif)

#### 作业要求

* 多个星星绕一个中心旋转
* 星星颜色不同
* 类似螺旋线的效果，星星不断向外圈扩散
* 至少实现阿基米德螺线、对数螺线、费马螺线中一种的效果

#### 实现过程

##### 星星位置的产生

* 星星的位置由SpiralLine类产生，使用InitAngle记录当前旋转的角度，而`StartAngleIndex = ceil(InitAngle / angleStep)`，表示螺旋线最前端点的角位置，对应第一个极径。极径用`vector<glm::vec3> Positions`记录。
* 每过一帧，InitAngle增加一点，不过要控制InitAngle在(-2pi, 2pi)之间；为了简化计算，只计算到极径在1以内的范围。
* 同时，注意保持同一方向的星星颜色相同，根据StartAngleIndex更新表示颜色的`vector<int> Colors`，使其与Positions一致。

##### 星星的绘制

* 绘制正方形，与读取的texture文件融合即为星星。
* 创建SpiralLine对象line。在每个周期内，调用SpiralLine类中的PositionUpdate函数后，依次读取line.Positions，和line.Colors，更新着色器中对应的uniform变量，对星星进行绘制。

##### 主要源代码文件

* src/main.cpp（绘图主程序）
* include/sprialLine.h（SpiralLine类的实现）
* config/main.vert.glsl & main.frag.glsl（着色器）
* 采用cmake编译，macOS Mojave