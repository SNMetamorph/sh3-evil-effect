#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;

void main()
{
	normal = aNormal;
	fragPos = vec3(matModel * vec4(aPosition, 1.0f));
	texCoords = aTexCoords;
	gl_Position = matProjection * matView * matModel * vec4(aPosition, 1.0f);
}
