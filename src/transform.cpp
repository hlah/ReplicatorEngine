#include "transform.hpp"
#include "hierarchy.hpp"
#include "dirty.hpp"

#include "matrix_op.hpp"

#include "spdlog/spdlog.h"

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
Transform& Transform::rotate( const glm::quat& rot ) {
    _rotation = _rotation * rot;
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

Transform& Transform::translate( const glm::vec4& translation ) {
    auto new_base = glm::mat4_cast( _rotation );
    _translation += new_base * translation;
    return *this;
} 
Transform& Transform::rotate_global( const glm::quat& rot ) {
    _rotation = rot * _rotation;
    return *this;
}


void Transform::on_change(entt::entity entity, entt::registry& registry, Transform& transform) {
    Dirty<Transform>::dirty_recurse( registry, entity );
}

void transform_system( entt::registry& registry ) {
    registry.sort<Transform, Hierarchy>();
    registry.sort<Dirty<Transform>, Transform>();
    auto view = registry.view<Transform, Hierarchy, Dirty<Transform>>();


    for( auto entity : view ) {
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

    registry.reset<Dirty<Transform>>();

}


