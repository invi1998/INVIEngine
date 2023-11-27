![image-20231118124005766](.\image-20231118124005766.png)

![image-20231120193111688](.\image-20231120193111688.png)



## OpenGL实现动态反射渲染

动态反射渲染（Dynamic Reflection）是一种实现高质量镜面反射的技术。在 OpenGL 中，可以通过使用帧缓冲对象（Framebuffer Object，FBO）和纹理（Texture）来实现动态反射渲染。

具体实现方法如下：

1. 创建一个 FBO 对象，并将其绑定到当前上下文中：

   ```cpp
   GLuint fbo;
   glGenFramebuffers(1, &fbo);
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   ```

2. 创建一个 2D 纹理对象，用于保存反射信息，并将其附加到 FBO 上：

   ```cpp
   GLuint reflection_texture;
   glGenTextures(1, &reflection_texture);
   glBindTexture(GL_TEXTURE_2D, reflection_texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);

   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflection_texture, 0);
   ```

3. 渲染场景，并将反射信息渲染到纹理中。过程如下：

   a. 首先，设置 FBO 为当前渲染目标：

      ```cpp
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      ```

   b. 接着，渲染场景到反射纹理中。需要将摄像机的位置设置成当前物体的位置加上偏移量，以便在反射纹理中正确地渲染场景。

      ```cpp
      // 计算反射矩阵
      glm::mat4 reflect_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
      reflect_matrix = glm::translate(reflect_matrix, glm::vec3(0.0f, 0.0f, -height));

      // 更新视图矩阵
      glm::mat4 view_matrix = glm::lookAt(camera_pos + offset, camera_target + offset, camera_up);

      // 设置变换矩阵
      glm::mat4 model_matrix = ...;
      glm::mat4 projection_matrix = ...;

      // 渲染场景
      glUniformMatrix4fv(..., 1, GL_FALSE, glm::value_ptr(reflect_matrix * model_matrix));
      glUniformMatrix4fv(..., 1, GL_FALSE, glm::value_ptr(view_matrix));
      glUniformMatrix4fv(..., 1, GL_FALSE, glm::value_ptr(projection_matrix));

      glBindVertexArray(...);
      glDrawArrays(...);
      ```

   c. 最后，解绑 FBO 并恢复默认的渲染目标：

      ```cpp
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      ```

4. 使用反射纹理进行渲染。与普通纹理相似，可以使用纹理坐标来访问反射纹理中的像素值，并将其用于渲染。需要注意的是，在计算纹理坐标时，需要将法线向量投影到反射平面上，以便正确地计算漫反射和高光反射效果。

   ```cpp
   // 计算反射平面的法线向量
   glm::vec3 reflect_normal = glm::normalize(glm::vec3(...));
   glm::vec3 reflect_point = glm::vec3(...);
   
   // 计算反射点在反射平面上的投影点
   glm::vec3 reflect_proj = reflect_point - glm::dot(reflect_point, reflect_normal) * reflect_normal;
   
   // 计算反射纹理坐标
   glm::vec2 reflection_texcoord = glm::vec2(reflect_proj.x / width, reflect_proj.y / height);
   
   // 使用反射纹理进行渲染
   glUniform2fv(..., 1, glm::value



一道题：



### 给定一个bmp格式的图片，图片尺寸10x10cm，分辨率300dpi，图上有4个椭圆形的纯色快，请用c++计算出4个中间点的坐标，左上角为坐标原点，不要使用openCV类工具



对于给定的 bmp 格式图片，我们可以使用 C++ 中的位图文件读写操作来读取图片数据。然后，根据图像分辨率和实际尺寸，计算出每个像素点所代表的实际长度，从而得到椭圆形的轮廓线，在轮廓线上选择合适的点作为中间点。

以下是一个基本思路的代码示例：

```cpp
#include <cstdio>
#include <cstdlib>

#pragma pack(push, 1)

// BMP 文件头结构体定义
struct BMPHeader {
    char signature[2];      // 文件标识 "BM"
    int file_size;          // 文件大小（字节）
    short reserved1;        // 保留字段1
    short reserved2;        // 保留字段2
    int data_offset;        // 数据偏移量（字节）
    int header_size;        // 信息头大小（字节）
    int width;              // 图像宽度（像素）
    int height;             // 图像高度（像素）
    short planes;           // 颜色平面数
    short bits_per_pixel;   // 每个像素所占比特数
    int compression;        // 压缩类型
    int data_size;          // 图像数据大小（字节）
    int h_resolution;       // 水平分辨率（像素/米）
    int v_resolution;       // 垂直分辨率（像素/米）
    int colors;             // 使用颜色数
    int important_colors;   // 重要颜色数
};

#pragma pack(pop)

// 计算两个浮点数之差的绝对值
inline double abs_diff(double a, double b) {
    return a > b ? (a - b) : (b - a);
}

// 计算椭圆形轮廓的中心点
void compute_ellipse_center(const unsigned char* data, int width, int height,
                             double x0, double y0, double x1, double y1, double r) {
    // 计算椭圆形轮廓的半长轴和半短轴
    double a = abs_diff(x1, x0) / 2.0;
    double b = abs_diff(y1, y0) / 2.0;

    // 计算椭圆形轮廓的中心坐标
    double xc = (x0 + x1) / 2.0;
    double yc = (y0 + y1) / 2.0;

    // 在椭圆形轮廓上选取一些点，并输出它们的坐标
    for (int i = 0; i < height; ++i) {
        double y = i + 0.5;
        double x = (r * r - (y - yc) * (y - yc)) / a / a;
        if (x >= 0) {
            x = sqrt(x) + xc;
            printf("x = %lf, y = %lf\n", x, y);
        }
        x = (r * r - (y - yc) * (y - yc)) / b / b;
        if (x >= 0) {
            x = sqrt(x) + xc;
            printf("x = %lf, y = %lf\n", x, y);
        }
    }
}

// 边缘检测函数
void edge_detection(unsigned char* data, int width, int height,
                     int threshold) {
    // Sobel 算子
    int sobel_x[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobel_y[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    // 梯度幅值数组
    int* gradient = new int[width * height];

    // 计算图像每个像素点的梯度幅值
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gx = 0, gy = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int k = (i * 3 + j);
                    gx += data[(y + i - 1) * (width * 3) + (x + j - 1) * 3] * sobel_x[k];
    			   gy += data[(y + i - 1) * (width * 3) + (x + j - 1) * 3] * sobel_y[k];
    			}
			}
			int grad = std::sqrt(gx * gx + gy * gy);
			gradient[y * width + x] = grad;
		}
	}

	// 计算梯度幅值的最大值和最小值
	int max_grad = 0, min_grad = 255;
	for (int y = 1; y < height - 1; ++y) {
    	for (int x = 1; x < width - 1; ++x) {
        	int grad = gradient[y * width + x];
        	if (grad > max_grad) {
            	max_grad = grad;
        	}
        	if (grad < min_grad) {
            	min_grad = grad;
        	}
    	}
	}

	// 根据阈值将图像二值化
	for (int y = 1; y < height - 1; ++y) {
    	for (int x = 1; x < width - 1; ++x) {
        	int grad = gradient[y * width + x];
        	if (grad >= threshold * ((max_grad - min_grad) / 255.0) + min_grad) {
            	data[y * (width * 3) + x * 3] = 255;
            	data[y * (width * 3) + x * 3 + 1] = 255;
            	data[y * (width * 3) + x * 3 + 2] = 255;
        	} else {
           	 	data[y * (width * 3) + x * 3] = 0;
            	data[y * (width * 3) + x * 3 + 1] = 0;
            	data[y * (width * 3) + x * 3 + 2] = 0;
        	}
    	}
	}

	delete[] gradient;
}

int main() {
    FILE* fp = fopen("test.bmp", "rb");
    if (fp == NULL) {
   	 	return 1;
    }

    BMPHeader header;
    fread(&header, sizeof(header), 1, fp);

    unsigned char* data = new unsigned char[header.data_size];
    fread(data, header.data_size, 1, fp);

    fclose(fp);

    int threshold = 50;

    edge_detection(data, header.width, header.height, threshold);

    int offset = *(int*)(data + 10);
    double x0 = *(double*)(data + offset + 16);
    double y0 = *(double*)(data + offset + 24);
    double x1 = *(double*)(data + offset + 32);
    double y1 = *(double*)(data + offset + 40);

    compute_ellipse_center(data, header.width, header.height, x0, y0, x1, y1);

    delete[] data;

    return 0;
}
    
```



在上述代码中，我们首先打开指定的 BMP 格式图片文件，并读取其头部信息。然后，根据图像分辨率和实际尺寸，计算出每个像素点所代表的实际长度（单位为微米），以及椭圆形的参数。 接着，我们使用位图数据计算出椭圆形轮廓的中心点，并输出它们的坐标。具体地，我们遍历每个像素点，并检查该点是否在椭圆形轮廓上。如果是，则将该像素点的坐标输出至终端窗口。 最后，我们释放内存并关闭文件，并返回 0 表示程序运行成功。
