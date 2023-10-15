#include <GL/glew.h>

struct Transform {
    float x;
    float y;
    float w;
    float h;
};

struct Velocity {
    float x;
    float y;
    float speed;
};

struct Render {
    GLuint id;
    // UV offsets into texture atlas
    float x;
    float y;
    float w;
    float h; 
};