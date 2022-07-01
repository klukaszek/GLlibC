#include "GLshaders.h"

//primary handler of rendering
void gl_render(GLuint program_id, GLuint vao_id)
{
    glUseProgram(program_id);
    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

//upload vertex data to gpu
void gl_upload_data(GLuint vao_id, GLuint vbo_id, GLuint ebo_id, void *data)
{

}

// compile vertex shader from .glsl file
GLuint gl_compile_shader(const char *filepath)
{
    //get glsl source code as string
    char *content = parse_shader_contents(filepath);
    if(content == NULL)
    {
        log_error("Could not parse shader: %s", filepath);
        return -1;
    }

    GLenum type = parse_shader_type(filepath);
    if(type == -1)
    {
        return -1;
    }

    // initialize shader id/handle
    GLuint shader_id = glCreateShader(type);

    //create const char* for glShaderSource()
    const char *glsl = (const char *)content;

    //send glsl code to GL then GPU
    glShaderSource(shader_id, 1, &glsl, 0);

    //compile shader
    glCompileShader(shader_id);

    if(!gl_verify_shader_compilation(shader_id))
    {
        return -1;
    }

    return shader_id;
}

// checks to see if a shader was compiled correctly, returns true when successful, false when unsuccesful
GLboolean gl_verify_shader_compilation(GLuint shader_id)
{
    GLint is_compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if(is_compiled == GL_FALSE)
    {
        //if compilation fails, get info log length
        GLint len;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);

        //get info log into GLchar string
        GLchar *info = malloc(sizeof(char)*len);
        glGetShaderInfoLog(shader_id, len, &len, info);

        //delete shader upon compilation failure
        glDeleteShader(shader_id);
        
        //log error and free error info
        log_error("Compilation of shader id %d failed:\n%s", (uint32_t)shader_id, info);
        free(info);

        return GL_FALSE;
    }

    log_debug("Compilation of shader id %d succesful.", (uint32_t)shader_id);
    return GL_TRUE;

}

// links all shaders in shader_ids array to program_id/handle
GLboolean gl_link_shaders(GLuint program_id, ShaderArray *shaders)
{   
    GLboolean linked = GL_TRUE;
    for(int i = 0; i < shaders->num_shaders; i++)
    {
        glAttachShader(program_id, shaders->shader_ids[i]);

        log_debug("Attached shader id %d to program id %d", shaders->shader_ids[i], program_id);
    }

    glLinkProgram(program_id);

    if (!gl_verify_shader_linking(program_id))
    {
        linked = GL_FALSE;
        for(int i = 0; i < shaders->num_shaders; i++)
        {
            glDetachShader(program_id, shaders->shader_ids[i]);
            glDeleteShader(shaders->shader_ids[i]);

            log_error("Shader id %d has been deleted due to linking failure with program id %d, it must be recompiled.", (uint32_t)shaders->shader_ids[i], (uint32_t)program_id);
        }   
    }

    return linked;
}

// checks to see if a shader was succesfully linked to shader program, returns true when successful, false when unsuccesful
GLboolean gl_verify_shader_linking(GLuint program_id)
{

    GLint is_linked = GL_FALSE;
    //this gets specified status of program handle 
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);

    if(is_linked == GL_FALSE)
    {
        //get info log length
        GLint len = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);

        //get info log into GLchar string
        GLchar *info = malloc(len);
        glGetProgramInfoLog(program_id, len, &len, info);

        log_error("Linking of shader program id %d failed:\n%s", (uint32_t)program_id, &info[0]);

        free(info);

        return GL_FALSE;
    }

    log_debug("Linking of program id %d succesful.", (uint32_t)program_id);
    return GL_TRUE;
}

// creates OpenGL program using gl_compile_shader, gl_link_shaders, and gl_use_program
GLuint gl_create_program(char **shader_filepaths, size_t num_files)
{
    if(num_files == 0) return -1;

    printf("\n");
    //create program so that we can use and link shaders
    GLuint program_id = glCreateProgram();
    if(program_id == 0)
    {
        gl_free_string_array(shader_filepaths, num_files);
        log_error("Failed to initialize program object id %d", (uint32_t)program_id);
        return -1;
    }

    GLuint *shader_ids = malloc(sizeof(GLuint)*num_files);

    //create individual shaders to pass to ShaderArray
    for(int i = 0; i < num_files; i++)
    {
        shader_ids[i] = gl_compile_shader(shader_filepaths[i]);
    }

    //create ShaderArray
    ShaderArray *shaders = gl_shader_array(shader_ids, num_files);
    if(shaders == NULL)
    {   
        log_error("Failed to create shader array.");
        gl_free_string_array(shader_filepaths, num_files);
        free(shader_ids);
        return -1;
    }

    //link shaders and use program
    GLboolean linked = gl_link_shaders(program_id, shaders);

    if(linked)
    {
        // delete shaders once they have been linked
        gl_delete_shaders(program_id, shaders);
    }
    else
    {
        gl_free_shader_array(shaders);
        glDeleteProgram(program_id);
        program_id = -1;
    }

    //free filenames from memory
    gl_free_string_array(shader_filepaths, num_files);

    printf("\n");

    return program_id;
}

//deletes shader objects in a ShaderArray
void gl_delete_shaders(GLuint program_id, ShaderArray *shaders)
{
    for (int i = 0; i < shaders->num_shaders; i++)
    {
        glDetachShader(program_id, shaders->shader_ids[i]);
        glDeleteShader(shaders->shader_ids[i]);

        log_debug("Shader id %d has been deleted.", (uint32_t)shaders->shader_ids[i]);
    }

    gl_free_shader_array(shaders);
}

// parse glsl code into a string to pass to GL in gl_compile_shader (STRING DOES NOT NEED TO BE FREED)
char *parse_shader_contents(const char *filepath)
{

    FILE *fp = fopen(filepath, "r");
    
    if(fp == NULL)
    {
        log_error("Could not parse shader: %s", filepath);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp) - 8;
    rewind(fp);

    char *content = malloc(len);
    fread(content, len, 1, fp);
    fclose(fp);
    content[len] = '\0';

    return content;
}

//get shader type from filename
GLenum parse_shader_type(const char *filepath)
{
    char *tmp = malloc(strlen(filepath));

    strcpy(tmp, (char *)filepath);
    char *filename = strtok(tmp, "/");
    filename = strtok(NULL, "/");
    filename = strtok(NULL, "/");
    filename = strtok(NULL, "/");
    log_debug("Shader being loaded: %s", filename);

    char *type = strtok(filename, "_");
    GLenum val;

    if (strcmp(type, "vertex") == 0)
    {
        val = GL_VERTEX_SHADER;
    }
    else if (strcmp(type, "fragment") == 0)
    {
        val = GL_FRAGMENT_SHADER;
    }
    else if (strcmp(type, "compute") == 0)
    {
        val = GL_COMPUTE_SHADER;
    }
    else if (strcmp(type, "tesscontrol") == 0)
    {
        val = GL_TESS_CONTROL_SHADER;
    }
    else if (strcmp(type, "tesseval") == 0)
    {
        val = GL_TESS_EVALUATION_SHADER;
    }
    else if (strcmp(type, "geometry") == 0)
    {
        val = GL_GEOMETRY_SHADER;
    }
    else
    {
        log_error("Shader %s has no identifiable type. Check shader naming conventions for this library.", filename);
        return -1;
    }

    log_debug("Shader type: %s shader", type);
    return val;
    
}

/*--------------------- Buffer Functions ----------------------------*/
// initializes and returns a vertex buffer object
GLuint gl_create_vbo(void *data, GLenum draw_type)
{
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, draw_type);
    return vbo_id;
}

//initializes and returns a vertex array object
GLuint gl_create_vao()
{
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    return vao_id;
}

/*---------------------- Struct Functions ----------------------------*/

// takes an array of shader ids and the number of shaders in the array
ShaderArray *gl_shader_array(GLuint *shader_ids, size_t n)
{

    if(shader_ids == NULL) return NULL;
    if(n == 0) return NULL;

    ShaderArray *shaders = malloc(sizeof(ShaderArray));

    shaders->shader_ids = shader_ids;
    shaders->num_shaders = n;

    return shaders;
}

// frees shader array
void gl_free_shader_array(ShaderArray *shaders)
{
    if(shaders == NULL) return;

    free(shaders->shader_ids);
    free(shaders);
}

// initializes vertex struct
Vertex *gl_vertex(float *pos, float *color)
{
    Vertex *vertex = malloc(sizeof(Vertex));

    vertex->pos = pos;
    vertex->color = color;

    return vertex;
}

// initializes array of vertices
Vertex **gl_vertex_array(float **pos, float **color, size_t n)
{
    if(n == 0) return NULL;

    Vertex **vertex_array = malloc(sizeof(Vertex) * n);

    for(int i = 0; i < n; i++)
    {
        Vertex *vertex = gl_vertex(pos[i], color[i]);
        vertex_array[i] = vertex;
    }

    return vertex_array;
}

//frees vertex array memory
void gl_free_vertex_array(Vertex **vertex_array, size_t n)
{
    for(int i = 0; i < n; i++)
    {
        free(vertex_array[i]);
    }

    free(vertex_array);
}

//frees array of strings
void gl_free_string_array(char **array, size_t n)
{
    if (array == NULL) return;

    for (int i = 0; i < n; i++)
    {
        free(array[i]);
    }

    free(array);
}