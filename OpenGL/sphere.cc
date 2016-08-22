int getBallPoints(int times, GLfloat radius) {
    int j;
    int index = 0;
    int nextIndex = 0;
    for (j = 0; j < times; j += 1) {
        index = kPointLen * 2 * j * (times + 1);
        nextIndex = kPointLen * 2 * (j + 1) * (times + 1);
        GLfloat smallAngle;

        GLfloat angle = (GLfloat) (M_PI * (j * 1.0) / times);
        GLfloat angle2 = (GLfloat) (M_PI * (j + 1) * 1.0 / times);


        GLfloat smallRadius = (GLfloat) (radius * sin(angle));
        GLfloat smallRadius2 = (GLfloat) (radius * sin(angle2));

        int i;
        for (i = index; i < nextIndex; i += kPointLen) {
            // e.g. mIndex / 100 * 2 * len
            // and mIndex [0~100]
            // smallAngle [0~2PI]
            int mIndex = i - index;

            smallAngle = (GLfloat) (M_PI * (mIndex * 2.0) / (kPointLen * 2 * times));

            // XYZ
            gVertexAttr[i] = (GLfloat) (smallRadius * cos(smallAngle));
            gVertexAttr[i + kPointLen] = (GLfloat) (smallRadius2 * cos(smallAngle));
            ++i;
            gVertexAttr[i] = (GLfloat) (radius * cos(angle));
            gVertexAttr[i + kPointLen] = (GLfloat) (radius * cos(angle2));
            ++i;
            gVertexAttr[i] = (GLfloat) (smallRadius * sin(smallAngle));
            gVertexAttr[i + kPointLen] = (GLfloat) (smallRadius2 * sin(smallAngle));
            ++i;

            // texture coordinate
            gVertexAttr[i] = (GLfloat) ((mIndex * 1.0) / (kPointLen * 2 * times));
            gVertexAttr[i + kPointLen] = (GLfloat) ((mIndex * 1.0) / (kPointLen * 2 * times));
            ++i;
            gVertexAttr[i] = (GLfloat) (1.0f - (j * 1.0) / times);
            gVertexAttr[i + kPointLen] = (GLfloat) (1.0f - ((j + 1) * 1.0) / times);
            ++i;
        } // for X axis

    } // for Y axis

    return nextIndex / kPointLen;
}