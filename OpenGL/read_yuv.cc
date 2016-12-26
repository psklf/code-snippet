unsigned char *read_yuv(char *path, int width, int height) {
    FILE *fp;
    unsigned char *buffer;
    long size = width * height * 3 / 2;

    if ((fp = fopen(path, "rb")) == NULL) {
        return nullptr;
    }

    buffer = new unsigned char[size];
    memset(buffer, '\0', size);
    fread(buffer, size, 1, fp);
    fclose(fp);
    return buffer;
}
