#include "matrix_op.hpp"

#include <sstream>

namespace matrix_op {

    // matrix scale operation
    glm::mat4 scale( float sx, float sy, float sz ) {
        return glm::mat4{
            sx, 0.0, 0.0, 0.0,
            0.0, sy, 0.0, 0.0,
            0.0, 0.0, sz, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
    }
    glm::mat4 scale( float s ) { return scale( s, s, s ); }

    // matrix translation operation
    glm::mat4 translation( float tx, float ty, float tz ) {
        glm::mat4 translation{1.0};
        translation[3][0] = tx;
        translation[3][1] = ty;
        translation[3][2] = tz;
        return translation;
    }

    glm::mat4 rotate_y( float angle ) {
        auto s = std::sin( angle );
        auto c = std::cos( angle );
        return glm::mat4{
              c, 0.f,  -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
              s, 0.f,   c, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }
    glm::mat4 rotate_z( float angle ) {
        auto s = std::sin( angle );
        auto c = std::cos( angle );
        return glm::mat4{
              c,  -s, 0.f, 0.f,
              s,   c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }
    glm::mat4 rotate_x( float angle ) {
        auto s = std::sin( angle );
        auto c = std::cos( angle );
        return glm::mat4{
            1.f, 0.f, 0.f, 0.f,
            0.f,   c,  -s, 0.f,
            0.f,   s,   c, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
    }

    glm::mat4 rotate( float angle, glm::vec4 axis ) {
        float c = cos(-angle);
        float s = sin(-angle);
        axis = glm::normalize(axis);
        return glm::mat4{
               axis.x*axis.x*(1-c)+c,   axis.x*axis.y*(1-c)-axis.z*s,    axis.x*axis.z*(1-c)+axis.y*s,   0.0,
        axis.x*axis.y*(1-c)+axis.z*s,          axis.y*axis.y*(1-c)+c,    axis.y*axis.z*(1-c)-axis.x*s,   0.0,
        axis.x*axis.z*(1-c)-axis.y*s,   axis.y*axis.z*(1-c)+axis.x*s,           axis.z*axis.z*(1-c)+c,   0.0,
                                 0.0,                            0.0,                             0.0,   1.0
    
        };
    }

    // orthographic projection matrix
    glm::mat4 orthographic( 
            float left, 
            float right, 
            float bottom, 
            float top, 
            float far, 
            float near 
            ) {
        return glm::mat4{
            2.f/(right-left),              0.f,            0.f, 0.f,
                           0.f, 2.f/(top-bottom),            0.f, 0.f, 
                           0.f,              0.f, 2.f/(far-near), 0.f,
            -(right+left)/(right-left),
            -(top+bottom)/(top-bottom),
            -(far+near)/(far-near),
            1.f
        };
    }

    // perspective projection matrix
    glm::mat4 perspective( float fov, float aspect_ratio, float near, float far ) {
        float top = fabs(near) * tan( fov / 2.f );
        float bottom = -top;
        float right = top * aspect_ratio;
        float left = -right;
        
        glm::mat4 P{
            near,  0.f,       0.f, 0.f,
             0.f, near,       0.f, 0.f,
             0.f,  0.f,  near+far, 1.f,
             0.f,  0.f, -far*near, 0.f
        };

        auto M = orthographic(
                left, right,
                bottom, top,
                far, near
        );

        return -M*P;
    }

    std::string print( const glm::mat4& matrix ) {
        std::stringstream ss;
        for( int i=0; i<4; i++ ) {
            ss << "[";
            for( int j=0; j<4; j++ ) {
                ss << " " << matrix[j][i];
            }
            ss << " ]\n";
        }
        return ss.str();
    }

}

