## OBJ

OBJ（Wavefront .obj）是一种常见的三维模型文件格式，被广泛应用于计算机图形学、游戏制作、虚拟现实等领域。以下是 OBJ 模型文件的基本格式和内容：

1. 文件格式

OBJ 模型文件通常包含两个文件：一个 .obj 文件和一个 .mtl 文件。其中，.obj 文件包含模型的几何数据，而 .mtl 文件包含材质和纹理等信息。

2. 几何数据

.obj 文件中几何数据的主要内容包括顶点、法向量、纹理坐标和面数据等。具体格式如下：

- 顶点数据（Vertex data）：使用 v 关键字表示，其格式为 v x y z，其中 x、y、z 分别表示顶点的坐标值。
- 法向量数据（Normal data）：使用 vn 关键字表示，其格式为 vn x y z，其中 x、y、z 分别表示法向量的坐标值。
- 纹理坐标数据（Texture coordinate data）：使用 vt 关键字表示，其格式为 vt u v [w]，其中 u、v、w 表示纹理坐标值。
- 面数据（Face data）：使用 f 关键字表示，其格式为 f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3]，其中 v1、v2、v3 表示面对应的顶点序号，vt1、vt2、vt3 表示纹理坐标序号，vn1、vn2、vn3 表示法向量序号。

需要注意的是，在 OBJ 格式中，索引从 1 开始，而不是从 0 开始。

3. 材质数据

.mtl 文件中的材质数据包括材质名称、漫反射系数、镜面反射系数、折射率、纹理图像等信息。具体格式如下：

- 新材质定义（New material definition）：使用 newmtl 关键字表示，其后紧跟着材质名称。
- 漫反射系数（Diffuse reflectivity）：使用 Kd 关键字表示，其格式为 Kd r g b，其中 r、g、b 分别表示红、绿、蓝三色分量的系数。
- 镜面反射系数（Specular reflectivity）：使用 Ks 关键字表示，其格式同上。
- 折射率（Refractive index）：使用 Ni 关键字表示，其格式为 Ni n，其中 n 表示折射率。
- 纹理图像（Texture image）：使用 map_Kd 关键字表示，其格式为 map_Kd filename，其中 filename 表示纹理图像的文件名。

以上是 OBJ 模型文件的基本格式和内容，通过解析 OBJ 文件，我们可以读取模型的几何数据、材质信息等，进而进行渲染、编辑、导出等操作。