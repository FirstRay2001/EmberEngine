// Camera.h  
// 相机类  
// created by FirstRay2001, Nov/6/2025  

#pragma once  

#include <glm/glm.hpp>  
#include <glm/gtc/quaternion.hpp> 

namespace Ember  
{  
	// Note - 这个类设计得不好，应该和Transform分离开来，现在先这样凑合用着吧 :>
	class Camera  
	{  
		friend class Scene;
		friend class Renderer;
	public:  
		Camera(const Camera&) = default;
		Camera(float aspect_ = 16.0f/9.0f, float fov_ = 45.0f, float near_ = 0.1f, float far_ = 100.0f);  

		void SetScreentSize(uint32_t width, uint32_t height);

		const glm::mat4& GetViewMatrix() const				{ return m_ViewMatrix; }  
		const glm::mat4& GetProjectionMatrix() const		{ return m_ProjectionMatrix; }  
		const glm::mat4& GetViewProjectionMatrix() const	{ return m_ViewProjectionMatrix; }  

		float		GetFov() const { return m_Fov; }
		void		SetFov(float fov) { m_Fov = fov; RecalculateProjectionMatrix(); }
		float		GetNearClip() const { return m_Near; }
		void		SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjectionMatrix(); }
		float		GetFarClip() const { return m_Far; }
		void		SetFarClip(float farClip) { m_Far = farClip; RecalculateProjectionMatrix(); }

		const glm::vec3 GetForwardDirection() const
		{
			return glm::normalize(m_Rotation * glm::vec3(0.0f, 0.0f, -1.0f));
		}

		const glm::vec3 GetRightDirection() const
		{
			return glm::normalize(m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f));
		}

		const glm::vec3 GetUpDirection() const
		{
			return glm::normalize(m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f));
		}

	private:  
		// 重新计算视图矩阵  
		void RecalculateViewMatrix();  

		// 重新计算投影矩阵  
		void RecalculateProjectionMatrix();  

		const		glm::vec3& GetPosition() const { return Position; }
		void		SetPosition(const glm::vec3& position) { Position = position; RecalculateViewMatrix(); }
		glm::quat	GetRotation() const { return m_Rotation; }
		void		SetRotation(glm::quat rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		float		GetAspectRatio() const { return m_Aspect; }
		void		SetAspectRatio(float aspect) { m_Aspect = aspect; RecalculateProjectionMatrix(); }

	private:  
		glm::mat4 m_ViewMatrix;  
		glm::mat4 m_ProjectionMatrix;  
		glm::mat4 m_ViewProjectionMatrix;  

		float m_Aspect = 16.0f/9.0f;  
		float m_Fov = 45;  
		float m_Near = 0.1f;  
		float m_Far = 100.0f;  

		bool bInitialized = false;  
		uint32_t m_ScreenWidth;  
		uint32_t m_ScreenHeight;  

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };  
		glm::quat m_Rotation = glm::quat(glm::vec3(0, 0, 0));
	};  
}
