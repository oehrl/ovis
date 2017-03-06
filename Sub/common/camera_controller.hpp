#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "scene_controller.hpp"

enum class ProjectionType : bool
{
    PERSPECTIVE = true,
    ORTHOGRAPHIC = false
};

class CameraController :
    public SceneController
{
public:
    CameraController(Scene* scene);
    
    virtual void Update(Uint32 delta_time) override;
    virtual bool ProcessEvent(const SDL_Event& event) override;
    
    inline glm::vec3 Unproject(const glm::vec3& point)
    {
        const glm::vec4 unprojected =
            m_inverse_projection_matrix * glm::vec4(point, 1.0f);
        return glm::vec3(unprojected) / unprojected.w;
    }
    
    inline glm::vec3 ConvertPointFromCameraSpaceToWorldSpace(const glm::vec3& point)
    {
        return
        {
            m_inverse_view_matrix * glm::vec4(point, 1.0f)
        };
    }
    
    inline void SetProjectionType(ProjectionType projection_type)
    {
        m_projection_type = projection_type;
    }
    
    inline void SetWidth(float width)
    {
        SDL_assert(width > 0.0f);
        m_width = width;
    }
    
    inline void SetHeight(float height)
    {
        SDL_assert(height > 0.0f);
        m_height = height;
    }
    
    inline void SetYaw(float yaw)
    {
        m_yaw = yaw;
    }
    
    inline void SetPitch(float pitch)
    {
        m_pitch = pitch;
    }
    
    inline void SetRoll(float roll)
    {
        m_roll = roll;
    }
    
    inline void Rotate(float yaw, float pitch, float roll)
    {
        m_yaw   += yaw;
        m_pitch += pitch;
        m_roll  += roll;
    }
    
    inline float yaw() const { return m_yaw; }
    inline float pitch() const { return m_pitch; }
    inline float roll() const { return m_roll; }
    
    inline glm::vec3 forward() const { return m_forward; }
    
    inline glm::vec3 position() const
    {
        return m_position;
    }
    
    inline void Move(const glm::vec3& offset)
    {
        m_position += offset;
    }
    
    inline float vertical_fov() const { return m_field_of_view; }
    //inline float horizontal_fov() const { }
    
    inline const glm::mat4& view_matrix() const
    {
        return m_view_matrix;
    }
    
    inline const glm::mat4& projection_matrix() const
    {
        return m_projection_matrix;
    }
    
    inline const glm::mat4& view_projection_matrix() const
    {
        return m_view_projection_matrix;
    }
    
private:
    glm::mat4 m_view_matrix;
    glm::mat4 m_inverse_view_matrix;
    glm::mat4 m_projection_matrix;
    glm::mat4 m_inverse_projection_matrix;
    glm::mat4 m_view_projection_matrix;
    float m_field_of_view;
    float m_near_plane;
    float m_far_plane;
    float m_width;
    float m_height;
    ProjectionType m_projection_type;

    float m_yaw;
    float m_pitch;
    float m_roll;
    glm::vec3 m_forward;
    glm::vec3 m_position;
    float m_moving;
};
