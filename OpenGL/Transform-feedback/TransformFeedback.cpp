//
// Created by psklf on 29/08/2017.
//

#include "TransformFeedback.h"

#include <LogUtils.h>
#include <cstdlib>

static const GLchar *kVertexShader =
        "#version 300 es\n"
                "in vec3 a_position;\n"
                "out vec3 vary_position;\n"
                "void main() { \n"
                "    vary_position = a_position * 2.0f;\n"
                "}\n";

static const char *kFragmentShader =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec3 vary_position;\n"
                "out vec4 color_out;\n"
                "void main() {\n"
                "    color_out = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                "}\n";

static const GLchar *kTransformVerying =
        "vary_position";



bool CheckGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("check error", "GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }

    return false;
}


void TransformFeedback::Create() {
    int num = 1;
    GLfloat *vertices = new GLfloat[num * 3];
    vertices[0] = 0;
    vertices[1] = 1;
    vertices[2] = 2;

    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &kVertexShader, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        LOGE("xx", "Compile err");
    }

    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &kFragmentShader, nullptr);
    glCompileShader(f_shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        LOGE("xx", "Compile err");
    }


    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glAttachShader(program, f_shader);

    glTransformFeedbackVaryings(program, 1, &kTransformVerying, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(program);

    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        char *infolog = (char *) malloc(sizeof(GLchar));;
        glGetProgramInfoLog(program, length, NULL, infolog);
        LOGE("stderr", "%s", infolog);
        if (infolog) {
            free(infolog);
        }
    }

    int logLength;
    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *) malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        LOGE("xx", "Program validate log:\n%s \n", log);
        free(log);
    }

    glUseProgram(program);

    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint *vbo_array;
    vbo_array = new GLuint[2];
    glGenBuffers(2, vbo_array);
    CheckGlError("-1");

    /* source */
    glBindBuffer(GL_ARRAY_BUFFER, vbo_array[0]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    GLint pos_location = glGetAttribLocation(program, "a_position");

    glEnableVertexAttribArray(pos_location);
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* dest */
    glBindBuffer(GL_ARRAY_BUFFER, vbo_array[1]);
    glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), nullptr, GL_STATIC_READ);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo_array[1]);

    glBeginTransformFeedback(GL_POINTS);
    CheckGlError("1");

    glDrawArrays(GL_POINTS, 0, 1);
    CheckGlError("15");
    glEndTransformFeedback();
    CheckGlError("2");

    glFlush();

    glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);


    // Get Data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_array[1]);
    GLfloat *mapped_mem = static_cast<GLfloat *>(
            glMapBufferRange(GL_ARRAY_BUFFER, 0, num * 3 * sizeof(GLfloat), GL_MAP_READ_BIT));

    if (mapped_mem != nullptr) {
        LOGE("zzz", "Get Data: %f %f %f", mapped_mem[0], mapped_mem[1], mapped_mem[2]);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}


