#ifndef IBO_CLASS_H
#define IBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class IBO {
public:
    void bind();
    void unbind();
    void deleteIBO();

    GLuint ID;
    IBO(std::vector<GLuint> &indices);
};

#endif