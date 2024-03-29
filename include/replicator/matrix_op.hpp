#ifndef _REPLICATOR_MATRIX_HPP_
#define _REPLICATOR_MATRIX_HPP_

#include "glm/mat4x4.hpp"

#include <string>

// This header defines common matrix operations
namespace matrix_op {

    // matrix scale operation
    glm::mat4 scale( float sx, float sy, float sz );
    glm::mat4 scale( float s );

    // matrix translation operation
    glm::mat4 translation( float tx, float ty, float tz );
    
    // matrix rotation operation
    glm::mat4 rotate_y( float angle );
    glm::mat4 rotate_z( float angle );
    glm::mat4 rotate_x( float angle );
    glm::mat4 rotate( float angle, glm::vec4 axis );



    // orthographic projection matrix
    glm::mat4 orthographic( 
            float left, 
            float right, 
            float bottom, 
            float top, 
            float far, 
            float near 
    );

    // perspective projection matrix
    glm::mat4 perspective( float fov, float aspect_ratio, float near, float far ); 

    // print matrix to string
    std::string print( const glm::mat4& matrix );


}


#endif // _REPLICATOR_MATRIX_HPP_
