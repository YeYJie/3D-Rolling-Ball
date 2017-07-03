#include "include.h"

extern const int WIDTH;
extern const int HEIGHT;

void getCornersOfCameraFrustum(float fovy, float aspect, float near, float far,
								const glm::vec3 & cameraPosition,
								const glm::vec3 & cameraLookAt,
								glm::vec3 * result)
{
	float Hnear = 2 * tan(fovy / 2.0) * near;
	float Wnear = Hnear * aspect;

	float Hfar = 2 * tan(fovy / 2.0) * far;
	float Wfar = Hfar * aspect;

	glm::vec3 v = glm::normalize(cameraLookAt - cameraPosition);

	glm::vec3 Cnear = cameraPosition + v * near;
	glm::vec3 Cfar = cameraPosition + v * far;

	static glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 rightVector = glm::cross(v, upVector);

	glm::vec3 nearTopLeft = Cnear + (upVector * (Hnear / 2)) - (rightVector * (Wnear / 2));
	glm::vec3 nearTopRight = Cnear + (upVector * (Hnear / 2)) + (rightVector * (Wnear / 2));
	glm::vec3 nearBottomLeft = Cnear - (upVector * (Hnear / 2)) - (rightVector * (Wnear /2));
	glm::vec3 nearBottomRight = Cnear + (upVector * (Hnear / 2)) + (rightVector * (Wnear / 2));

	glm::vec3 farTopLeft = Cfar + (upVector * (Hfar / 2)) - (rightVector * (Wfar / 2));
	glm::vec3 farTopRight = Cfar + (upVector * (Hfar / 2)) + (rightVector * (Wfar / 2));
	glm::vec3 farBottomLeft = Cfar - (upVector * (Hfar / 2)) - (rightVector * (Wfar / 2));
	glm::vec3 farBottomRight = Cfar - (upVector * (Hfar / 2)) + (rightVector * (Wfar / 2));

	result[0] = nearTopLeft;
	result[1] = nearTopRight;
	result[2] = nearBottomLeft;
	result[3] = nearBottomRight;
	result[4] = farTopLeft;
	result[5] = farTopRight;
	result[6] = farBottomLeft;
	result[7] = farBottomRight;
}

glm::vec3 getfrustumCentroid(const glm::vec3 * frustumCorner)
{
	glm::vec3 res(0.0f);
	for(int i = 0; i < 8; ++i)
		res += frustumCorner[i];
	res /= 8.0f;
	return res;
}

void translateToLightSpace(glm::vec3 * worldSpace, const glm::mat4 & view,
								glm::vec3 * lightSpace)
{
	for(int i = 0; i < 8; ++i)
		lightSpace[i] = view * glm::vec4(worldSpace[i].x,
											worldSpace[i].y,
											worldSpace[i].z, 1.0f);
}

void calculateOrtho(const glm::vec3 & cameraPosition,
						const glm::vec3 & cameraLookAt,
						const glm::vec3 & lightDirection,
						glm::mat4 & view,
						glm::mat4 & projection)
{
	glm::vec3 frustumCornerWorldSpace[8];
	getCornersOfCameraFrustum(45.0f, float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f,
						cameraPosition, cameraLookAt, frustumCornerWorldSpace);

	glm::vec3 frustumCentroid = getfrustumCentroid(frustumCornerWorldSpace);

	view = glm::lookAt(frustumCentroid - (lightDirection * 100.0f),
									frustumCentroid,
									glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 frustumCornerLightSpace[8];
	translateToLightSpace(frustumCornerWorldSpace, view, frustumCornerLightSpace);


	glm::vec3 mins = frustumCornerLightSpace[0];

	glm::vec3 maxes = frustumCornerLightSpace[0];

	for (int i = 0; i < 8; i++)
	{
		if (frustumCornerLightSpace[i].x > maxes.x)
			maxes.x = frustumCornerLightSpace[i].x;
		else if (frustumCornerLightSpace[i].x < mins.x)
			mins.x = frustumCornerLightSpace[i].x;

		if (frustumCornerLightSpace[i].y > maxes.y)
			maxes.y = frustumCornerLightSpace[i].y;
		else if (frustumCornerLightSpace[i].y < mins.y)
			mins.y = frustumCornerLightSpace[i].y;

		if (frustumCornerLightSpace[i].z > maxes.z)
			maxes.z = frustumCornerLightSpace[i].z;
		else if (frustumCornerLightSpace[i].z < mins.z)
			mins.z = frustumCornerLightSpace[i].z;
	}

	projection = glm::ortho(mins.x, maxes.x, mins.y, maxes.y, -maxes.z - 10.0f, -mins.z);
}