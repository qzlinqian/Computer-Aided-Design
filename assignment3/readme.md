## 雪花飘落

![](http://www.qianl.in/2019/01/10/physicsEngine/a3.gif)

#### 作业要求

* 实现下雪的粒子效果
* 粒子的大小不一，模拟雪花的下落过程

* 开始雪花较少，随着时间慢慢增多

#### 实现过程

##### 纹理图片的读取和处理

* 背景图为castle.jpg，没有透明度值，以RGB形式读取；雪花为snow.png，含透明度信息，以RGBA形式读取。为了使雪花更逼真，在片段着色器main.frag.glsl中将A值减半。
* 背景图和雪花纹理均映射到正方形上，正方形顶点分别与castleVAO、snowVAO绑定，并设置相应的VBO和EBO，无需重复定义顶点。

##### 雪花位置和大小的变化

* 雪花的**位置和大小**由include/snow.h中的Snowing类控制，类中含有`std::vector<glm::vec3> Positions`，记录雪花中心点应该移动到的位置；`std::vector<GLfloat> Scales`，记录雪花大小的变化。Positions中元素的x，y方向的初始值以及Scales中的元素均由随机数得到，Positions中元素的z值为0.0f。每一帧都会调用一次`void positionUpdate(GLfloat deltaTime)`，使雪花位置下降一定距离。
* **雪花的增多**：当粒子最上端到达屏幕底部时（判断时增加了一定冗余），这个粒子（i）的生命即可结束，但我没有删除它，而是将它移动到了屏幕的上方，并重新随机获取Position[i].y和Scales[i]，这就保证了雪花总数不减。而在初始化时，我仅生成了最终雪花数量的1/50的雪花，在随后的变化中，每调用`void positionUpdate(GLfloat deltaTime)`300次后，若雪花数量没有达到上限，雪花会增加一片，这就使得雪花的数量逐渐增加。

##### 主要源代码文件

- src/main.cpp（绘图主程序）
- include/snowing.h（SpiralLine类的实现）
- config/main.vert.glsl & main.frag.glsl（着色器）
- 采用cmake编译，macOS Mojave

##### 依赖文件

* src/glad.c
* include/glad/glad.h
* include/KHR/khrplatform.h
* include/glm/*
* include/shader.h

##### 依赖的库

* glfw
* SOIL
* Carbon, IOKit, ForceFeedback, Cocoa, CoreVideo, OpenGL, CoreFoundation (MacOS)

