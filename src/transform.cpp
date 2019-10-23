#include "transform.hpp"
#include "hierarchy.hpp"

#include "spdlog/spdlog.h"
#include "matrix_op.hpp"

glm::mat4 Transform::local_matrix() const {
    return matrix_op::translation( _translation.x, _translation.y, _translation.z )
         * glm::mat4_cast( _rotation )
         * matrix_op::scale( _scale.x, _scale.y, _scale.z );
}

Transform& Transform::rotate_x( float angle ) {
    _rotation = glm::rotate( _rotation, angle, glm::vec3{1.0, 0.0, 0.0} );
    return *this;
}
Transform& Transform::rotate_y( float angle ) {
    _rotation = glm::rotate( _rotation, angle, glm::vec3{0.0, 1.0, 0.0} );
    return *this;
}
Transform& Transform::rotate_z( float angle ) {
    _rotation = glm::rotate( _rotation, angle, glm::vec3{0.0, 0.0, 1.0} );
    return *this;
}
Transform& Transform::rotate_x_global( float angle ) {
    _rotation = glm::angleAxis( angle, glm::vec3{1.f, 0.f, 0.f} ) * _rotation;
    return *this;
}
Transform& Transform::rotate_y_global( float angle ) {
    _rotation = glm::angleAxis( angle, glm::vec3{0.f, 1.f, 0.f} ) * _rotation;
    return *this;
}
Transform& Transform::rotate_z_global( float angle ) {
    _rotation = glm::angleAxis( angle, glm::vec3{0.f, 0.f, 1.f} ) * _rotation;
    return *this;
}

Transform& Transform::translate( float x, float y, float z ) {
    auto new_base = glm::mat4_cast( _rotation );
    _translation += new_base * glm::vec4{ x, y, z, 0.0 };
    return *this;
} 

void transform_system( entt::registry& registry ) {
    auto group = registry.group<Transform, Hierarchy>();

    group.sort([&registry](const entt::entity lhs, const entt::entity rhs){
            auto& left_h = registry.get<Hierarchy>( lhs );
            auto& right_h = registry.get<Hierarchy>( rhs );
            return lhs == right_h.parent() || left_h.next() == rhs
                || (!(lhs == right_h.parent() || left_h.next() == rhs) 
                        && ( left_h.parent() < right_h.parent() || (left_h.parent() == right_h.parent() && &left_h < &right_h ) ) );
    });

    for( auto entity : group ) {
        auto& transform = registry.get<Transform>(entity);
        auto& hierarchy = registry.get<Hierarchy>(entity);
        if( hierarchy.parent() != entt::null ) {
            auto parent_transform_ptr = registry.try_get<Transform>(hierarchy.parent());
            if( parent_transform_ptr != nullptr ) {
                auto new_transform = transform;
                new_transform._global = parent_transform_ptr->_global * transform.local_matrix();
                registry.replace<Transform>( entity, new_transform );
                continue;
            }
        }
        auto new_transform = transform;
        new_transform._global = transform.local_matrix();
        registry.replace<Transform>( entity, new_transform );
    }

}
