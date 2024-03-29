#ifndef _REPLICATOR_SHADERS_HPP_
#define _REPLICATOR_SHADERS_HPP_

#include "material.hpp"
#include "lights.hpp"

#include "glad/glad.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "entt/entt.hpp"

#include <string>
#include <memory>
#include <vector>

class Shader {
    public:
        // Shader types
        enum class Type: GLenum {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER
        };

        // Create a new shader from string
        Shader(Type type, std::string& source);

        ~Shader();

        // Conversion to GLint
        operator GLuint () const { return *_shader_id_shared; }

    private:
        std::shared_ptr<GLuint> _shader_id_shared;
};

class ShaderProgramDeleter;

class ShaderProgram {
    public:
        // Create new program from multiple shaders
        ShaderProgram(std::vector<Shader>);

        // Use shader program
        void use() const;

        // Set uniform value
        void uniform( const std::string name, const glm::mat4& value ) {
            auto location = glGetUniformLocation( _program_id, name.c_str() );
            _uniforms_to_set_m4[ location ] = value;
        }
        void uniform( const std::string name, const GLuint value  ) {
            auto location = glGetUniformLocation( _program_id, name.c_str() );
            _uniforms_to_set_u[ location ] = value;
        }

        void uniform( const std::string name, const Material& value ); 

        void uniform( const std::string& name, const ShaderLight& value ); 

    private:
        GLuint _program_id;

        mutable std::unordered_map<GLint, glm::mat4> _uniforms_to_set_m4;
        mutable std::unordered_map<GLint, glm::vec3> _uniforms_to_set_v3;
        mutable std::unordered_map<GLint, glm::vec4> _uniforms_to_set_v4;
        mutable std::unordered_map<GLint, GLfloat> _uniforms_to_set_f;
        mutable std::unordered_map<GLint, GLuint> _uniforms_to_set_u;
        mutable std::unordered_map<GLint, GLint> _uniforms_to_set_i;
        mutable std::unordered_map<GLenum, GLuint> _textures_to_set;

        friend class ShaderProgramDeleter;
};

class ShaderProgramDeleter {
    public:
        void operator()(ShaderProgram* program);
};

class ShaderException : public std::exception {
    public:
        ShaderException( const std::string& msg ) : _msg{msg} { }
        const char* what() const throw() { return _msg.c_str(); }

    private:
        const std::string _msg;
};

class ShaderProgramLoader final: public entt::resource_loader<ShaderProgramLoader, ShaderProgram> {
    public:
        std::shared_ptr<ShaderProgram> load( const std::vector<std::string>& vs_paths, const std::vector<std::string>& fs_paths ) const;
    private:
        std::string _load_file( const std::string& path ) const;
};

#endif // _REPLICATOR_SHADERS_HPP_


