#include"GLheader.h"

typedef struct vertex
{
    float *pos;
    float *color;
} Vertex;

typedef struct shaders
{
    GLuint *shader_ids;
    int num_shaders;
} ShaderArray;

//primary handler of rendering
void gl_render();

//upload vertex data to gpu
void gl_upload_data(GLuint vao_id, GLuint vbo_id, GLuint ebo_id, void *data);

//compile shader from .glsl file
GLuint gl_compile_shader(const char *filepath);

//checks to see if a shader was compiled correctly, returns true when successful, false when unsuccesful
GLboolean gl_verify_shader_compilation(GLuint shader_id);

//links all shaders in shader_ids array to program
GLboolean gl_link_shaders(GLuint program_id, ShaderArray *shaders);

//checks to see if a shader was succesfully linked to shader program, returns true when successful, false when unsuccesful
GLboolean gl_verify_shader_linking(GLuint program_id);

//creates OpenGL program using gl_compile_shader, gl_link_shaders, and gl_use_program
GLuint gl_create_program(char **shader_filepaths, size_t num_files);

//deletes shader objects in a ShaderArray
void gl_delete_shaders(GLuint program_id, ShaderArray *shaders);

//parse glsl code into a string to pass to GL in gl_compile_shader
char *parse_shader_contents(const char *filepath);

/*
    Gets type of shader from filename
    SHADER NAMING CONVENTIONS

    GL_VERTEX_SHADER = vertex_filename.glsl
    GL_FRAGMENT_SHADER = fragment_filename.glsl
    GL_COMPUTE_SHADER = compute_filename.glsl
    GL_TESS_CONTROL_SHADER = tesscontrol_filename.glsl
    GL_TESS_EVALUATION_SHADER = tesseval_filename.glsl
    GL_GEOMETRY_SHADER = geometry_filename.glsl
*/
GLenum parse_shader_type(const char *filepath);

/*--------------------- Buffer Functions ----------------------------*/
//initializes and returns a vertex buffer object
GLuint gl_create_vbo(void *data, GLenum draw_type);

//initializes and returns a vertex array object
GLuint gl_create_vao();

/*---------------------- Struct Functions ----------------------------*/

//takes an array of shader ids and the number of shaders in the array
ShaderArray *gl_shader_array(GLuint *shader_ids, size_t n);

//initializes vertex struct
Vertex *gl_vertex(float *pos, float *color);

//initializes array of vertices
Vertex **gl_vertex_array(float **pos, float **color, size_t n);

//frees vertex array
void gl_free_vertex_array(Vertex **vertex_array, size_t n);

//frees shader array
void gl_free_shader_array(ShaderArray *shaders);

//frees array of strings
void gl_free_string_array(char **array, size_t n);