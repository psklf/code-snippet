//
// Created by psklf on 10/03/2017.
//

#include "ProcessCpu.h"

#include <cstring>
#include <math.h>

const float kRadiusOfSphere = 0.5;

int ProcessCpu(void *bmp_data, int width, int height, int dest_width, int dest_height,
               uint8_t *dest) {
    int dest_size = dest_width * dest_height * 4;
    int src_size = width * height * 4;

    float ratio = 0.1;
    double outer_radius = kRadiusOfSphere * sin(ratio * M_PI);

    for (int row = 0; row < dest_height; ++row) {
        for (int col = 0; col < dest_width; ++col) {
            int dest_index = (row * dest_width + col) * 4;

            if (row < (1.0f - ratio) * dest_height) {
                // empty, alpha is 0
                uint8_t empty_data[4] = {0, 0, 0, 0};
                memcpy(dest + dest_index, empty_data, 4);
            } else {
                // cy the src data
                double theta = (1.0f - (row * 1.0f / dest_height)) * M_PI;
                double varphi = col * 1.0f / dest_width * M_PI * 2;

                double x = kRadiusOfSphere * sin(theta) * cos(varphi);
                double y = kRadiusOfSphere * sin(theta) * sin(varphi);
                double x_ratio = x / outer_radius;
                double y_ratio = y / outer_radius;

                int logo_row = (int) ((y_ratio + 1.0f) * 0.5f * height);
                int logo_column = (int) ((x_ratio + 1.0f) * 0.5f * width);
                int logo_index = logo_row * width + logo_column;

                uint8_t *src_logo_data = static_cast<uint8_t *>(bmp_data) + logo_index * 4;
                uint8_t *dest_tex_data = dest + dest_index;

                memcpy(dest_tex_data, src_logo_data, 4);
            }
        }
    }

    return 0;
}
