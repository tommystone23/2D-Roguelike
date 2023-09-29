#include "Camera2D.h"

Camera2D::Camera2D() 
: _position_changed(true), _scale(1), _position(0, 0), _camera_matrix(0.0f)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int screen_width, int screen_height)
{
    _screen_width = screen_width;
    _screen_height = screen_height;
    // Pass in each side of the screen. Left, right, bottom, top
    _ortho_matrix = glm::ortho(0.0f, (float)_screen_width, 0.0f, (float)_screen_height);
}

void Camera2D::update()
{
    if(!_position_changed)
        return;
    
    // Translate Camera
    glm::vec3 translate(-_position.x + _screen_width/2.0f, -_position.y + _screen_height/2.0f, 0.0f);
    _camera_matrix = glm::translate(_ortho_matrix, translate);
    // Scale Camera
    glm::vec3 scale(_scale, _scale, 0.0f);
    _camera_matrix = glm::scale(glm::mat4(1.0f), scale) * _camera_matrix;

    _position_changed = false;
}
glm::vec2 Camera2D::get_world_coords(glm::vec2 screen_coords)
{
    // Invert Y coords
    screen_coords.y = _screen_height - screen_coords.y;
    // Make the centre 0,0
    screen_coords -= glm::vec2(_screen_width/2, _screen_height/2);
    // Scale coordinates
    screen_coords /= _scale;
    // Translate with the camera position
    screen_coords += _position;

    return screen_coords;
}

// AABB test to see if an entity is in the camera view
bool Camera2D::is_in_view(const glm::vec2 &position, const glm::vec2 &dimensions)
{
    glm::vec2 scaled_screen_dimensions = glm::vec2(_screen_width, _screen_height) / _scale;

    const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaled_screen_dimensions.x / 2.0f;
    const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaled_screen_dimensions.y / 2.0f;

    glm::vec2 center_position = position + dimensions / 2.0f;
    glm::vec2 center_camera_position = _position;
    glm::vec2 distance_vector = center_position - center_camera_position;

    float x_depth = MIN_DISTANCE_X - abs(distance_vector.x);
    float y_depth = MIN_DISTANCE_Y - abs(distance_vector.y);

    // If this is true we are colliding
    return (x_depth > 0 && y_depth > 0);
}


