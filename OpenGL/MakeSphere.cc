#include <vector>
#include <cmath>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void CalculateVertex(vector<float> &vertex,
        vector<float> &tex_coord, int latNumber, int longNumber,
        int precision, float radius);

void CalculateVertex(vector<float> &vertex,
        vector<float> &tex_coord, int latNumber, int longNumber,
        int precision, float radius) {
    float a = latNumber * M_PI / precision;
    float b = 2 * longNumber * M_PI / precision;
    float x = sin(a) * sin(b) * radius;
    float y = cos(a) * radius;
    float z = sin(a) * cos(b) * radius;
    float v = 1 - 1.0f * longNumber / precision;
    float u = 1 - 1.0f * latNumber / precision;

    vertex.push_back(x);
    vertex.push_back(y);
    vertex.push_back(z);

    FILE *v_fd;
    FILE *tex_fd;
    v_fd = fopen("vertex.c", "a+");
    // fprintf(v_fd, "float kSphereVertexs[] = {\n");
    
    tex_fd = fopen("texcoord.c", "a+");
    // fprintf(tex_fd, "float kSphereTexcoord[] = {\n");
    
    if (v_fd != NULL) {
    fprintf(v_fd, "%f, %f, %f, \n", x, y, z);
    } else {
        printf("Error!");
    }
    fclose(v_fd);


    tex_coord.push_back(v);
    tex_coord.push_back(u);

    fprintf(tex_fd,"%f, %f,\n", v, u);
    fclose(tex_fd);
}


int main() {
    vector<float> vertexs;
    vector<float> tex_coord;
    int precision = 40;
    float r = 0.5;

    int vertex_f;

    for (int latNumber = 0; latNumber < precision; ++latNumber) {
        for (int longNumber = 0; longNumber < precision; ++longNumber) {
            CalculateVertex(vertexs, tex_coord, latNumber, longNumber, precision, r);
            CalculateVertex(vertexs, tex_coord, latNumber, longNumber + 1, precision, r);
            CalculateVertex(vertexs, tex_coord, latNumber + 1, longNumber + 1, precision, r);

            CalculateVertex(vertexs, tex_coord, latNumber, longNumber, precision, r);
            CalculateVertex(vertexs, tex_coord, latNumber + 1, longNumber + 1, precision, r);
            CalculateVertex(vertexs, tex_coord, latNumber + 1, longNumber, precision, r);
        }
    }
    return 0;
}


