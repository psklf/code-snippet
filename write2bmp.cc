void write2File(GLuint width, GLuint height) {
    FILE *example;
    FILE *stichpic;
    GLubyte bmpHeader[BMP_Header_Length];
    GLubyte *readPixelData;
    GLubyte *pixelDataBGR;
    GLint bmpSize = width * height * 4;

    // Open example file
    example = fopen("/sdcard/example.bmp", "rb");
    if (example == 0) {
        exit(0);
    }

    stichpic = fopen("/sdcard/stich.bmp", "wb");
    if (stichpic == 0) {
        exit(0);
    }

    // Read pixels
    readPixelData = (GLubyte *) malloc(bmpSize);
    pixelDataBGR = (GLubyte *) malloc(bmpSize * 3 / 4);

    glReadPixels(0, 0, width, height,
                 GL_RGBA, GL_UNSIGNED_BYTE, readPixelData);

    checkGLError("read pixels");

    // Adjust rgb -> bgr
    for (int i = 0; i < bmpSize; i += 4) {
        int j = 3 * i / 4;
        pixelDataBGR[j] = readPixelData[i + 2];
        pixelDataBGR[j + 2] = readPixelData[i];
        pixelDataBGR[j + 1] = readPixelData[i + 1];
    }
    free(readPixelData);

    // Create new bmp file
    fread(bmpHeader, BMP_Header_Length, 1, example);
    fwrite(bmpHeader, BMP_Header_Length, 1, stichpic);
    fseek(stichpic, 0x0012, SEEK_SET);
    GLuint h = height;
    GLint w = width;
    fwrite(&w, sizeof(w), 1, stichpic);
    fwrite(&h, sizeof(h), 1, stichpic);

    fseek(stichpic, 0, SEEK_END);
    fwrite(pixelDataBGR, bmpSize * 3 / 4, 1, stichpic);

    fclose(example);
    fclose(stichpic);
    free(pixelDataBGR);
}
