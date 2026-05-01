#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	// Initalises the camera within the scene
	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->orientation = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	// Recives the current position of the camera
	glm::vec3 getPos()
	{
		return this->pos;
	}

	// Recives the current view projection of the camera
	inline glm::mat4 getViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + orientation, up);
	}

	
	void MoveLeft(float amt)
	{
		pos += glm::cross(up, orientation) * amt;
	}

	
	void MoveRight(float amt)
	{
		pos -= glm::cross(up, orientation) * amt;
	}

protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 orientation;
	glm::vec3 up;
};


