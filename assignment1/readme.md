## 用 OpenGL 进行三维模型的显示

#### 作业要求

* 读入一个三维网格模型（见附件.obj文件），并在屏幕上显示。
* 支持显示四种效果：网格效果，仅显示顶点效果，仅显示三角面效果及同时显示面和边的效果，通过按键进行效果的切换。三角面片效果下模型各个面片颜色不同。

* 通过键盘控制模型转动，平移

* 通过键盘控制模型网格效果和顶点效果下的颜色。

####  键盘功能介绍 

（相对于最初的坐标系）

* O-W：控制模型前后平动
* A-S：控制模型左右平动
* Z-X：控制模型上下平动
* E-R：改变俯仰角
* D-F：改变滚转角
* C-V：改变偏航角
* P-L-M：改变网格和顶点效果下的RGB值
* 1-2-3-4：分别对应网格、顶点、三角面、同时显示边和面的效果

#### 具体实现

* 创建meshRotate类，用于更改模型的中心位置和欧拉角信息。后来将改变颜色的操作也集成进此类，但并未改变类名。
* 顶点坐标由obj文件读取，再创建三维向量以表示各顶点的颜色，利用着色器进行绘制。在模式4下需要将网格和三角面分别绘制一次。
* 颜色的显示：在3、4模式下的颜色由随机数生成（注意每三个一个面上的顶点要同色），绑定在VAO中；1、2模式下的颜色由键盘控制改变。我在片段着色器中增加了一个名为displayMode的int变量，将当前的显示模式传入，由此判断是选择VAO中的数组还是统一的meshRotate.displayColor中的值。

#### 说明

* glsl文件与obj文件在config目录下，cmake时自动拷贝到目标目录下
* 目标（build） 目录下的assignment1为最终生成的可执行文件
* include/mesh.h与src/main.cpp为主要源代码，其余为下载的依赖文件