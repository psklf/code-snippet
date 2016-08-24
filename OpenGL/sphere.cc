GLuint get_sphere_vertices(GLint size, GLfloat radius,
        GLuint point_len, GLfloat *vertex) {
    int j;
    int index = 0;
    int next_index = 0;
    for (j = 0; j < size; j += 1) {
        index = point_len * 2 * j * (size + 1);
        next_index = point_len * 2 * (j + 1) * (size + 1);
        GLfloat small_angle;

        GLfloat angle = (GLfloat) (M_PI * (j * 1.0) / size);
        GLfloat angle2 = (GLfloat) (M_PI * (j + 1) * 1.0 / size);


        GLfloat small_radius = (GLfloat) (radius * sin(angle));
        GLfloat small_radius2 = (GLfloat) (radius * sin(angle2));

        int i;
        for (i = index; i < next_index; i += point_len) {
            // e.g. inner_index / 100 * 2 * len
            // and inner_index [0~100]
            // small_angle [0~2PI]
            int inner_index = i - index;

            small_angle = (GLfloat) (M_PI * (inner_index * 2.0) / (point_len * 2 * size));

            // XYZ
            vertex[i] = (GLfloat) (small_radius * cos(small_angle));
            vertex[i + point_len] = (GLfloat) (small_radius2 * cos(small_angle));
            ++i;
            vertex[i] = (GLfloat) (radius * cos(angle));
            vertex[i + point_len] = (GLfloat) (radius * cos(angle2));
            ++i;
            vertex[i] = (GLfloat) (small_radius * sin(small_angle));
            vertex[i + point_len] = (GLfloat) (small_radius2 * sin(small_angle));
            ++i;

            // texture coordinate
            vertex[i] = (GLfloat) ((inner_index * 1.0) / (point_len * 2 * size));
            vertex[i + point_len] = (GLfloat) ((inner_index * 1.0) / (point_len * 2 * size));
            ++i;

            // Here if the pic is upside down reverse
            // Change Y axis value
            // vertex[i] = (GLfloat) (1.0f - (j * 1.0) / size);
            // vertex[i + point_len] = (GLfloat) (1.0f - ((j + 1) * 1.0) / size);
            vertex[i] = (GLfloat) (1.0f * (j * 1.0) / size);
            vertex[i + point_len] = (GLfloat) (1.0f * ((j + 1) * 1.0) / size);
            ++i;
        } // for X axis

    } // for Y axis

    return next_index / point_len;
}
