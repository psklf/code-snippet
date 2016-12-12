GLubyte *reverse_data;
reverse_data = (GLubyte *) malloc((size_t) (g_width * g_height * 4));
glReadPixels(0, 0, g_width, g_height, GL_RGBA, GL_UNSIGNED_BYTE, reverse_data);

// up down turn
for (int h = 0; h < g_height; ++h) {
    int new_h = g_height - h - 1;
    memcpy((pixel_data + new_h * g_width * 4), (reverse_data + h * g_width * 4),
            (size_t) (g_width * 4));
}
free(reverse_data);
