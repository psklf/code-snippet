/**
 * This function is just for remember something about render depth and read back
 *    and can't run now.
 */
int ReadDepth() { 
    BaseRender render;
    render.InitProgram(kSphereDepthVerStr, kSphereDepthFragStr);
    GLuint program = render.get_program_id();
    glUseProgram(program);

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * kVertexNums, kSphereVertexs, GL_STATIC_DRAW);
    evo::CheckGlError("2");
    GLint pos_index = glGetAttribLocation(program, kPositionName);
    glVertexAttribPointer(pos_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint matrix_loc = glGetUniformLocation(program, kMvpMat4);
    glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, glm::value_ptr(m_p_mat4 * m_v_mat4));

    GLuint framebuffer_obj;
    glGenFramebuffers(1, &framebuffer_obj);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj);

    GLuint tex_for_fbo;
    glGenTextures(1, &tex_for_fbo);
    glBindTexture(GL_TEXTURE_2D, tex_for_fbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F,
            m_view_width, m_view_height,
            0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_for_fbo, 0);

    glDrawBuffers(0, GL_NONE);
    glReadBuffer(GL_NONE);

    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_obj);

    glViewport(0, 0, m_view_width, m_view_height);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0, 0, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, kVertexNums);
    evo::CheckGlError("6");

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    uint8_t *dest_ptr = new uint8_t[m_view_width * m_view_height * 4];
    glReadPixels(0, 0, m_view_width, m_view_height, GL_RGBA, GL_UNSIGNED_BYTE, dest_ptr);
    evo::CheckGlError("read depth");

    int row = (int) (m_view_height * (1.0 - v) + 0.5);
    int column = (int) (m_view_width * u + 0.5);
    int index = row * m_view_width + column;

    uint8_t depth_data = dest_ptr[index * 4];
    LOGE(TAG, "depth_data %f", depth_data / 255.0f);
    delete[] dest_ptr;
}
