#version 330 core

out vec4 color;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
uniform vec3 cameraPos;
uniform sampler2D texDiffuse;
uniform sampler2D texEffect;

void main()
{
	vec3 colorWhite		= 0.75f * vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDir		= normalize(cameraPos - fragPos);
	float diffuseComp	= max(dot(normal, lightDir), 0.0f);
    vec4 sampleDiffuse  = texture(texDiffuse, texCoords);
	vec4 sampleEffect	= texture(texEffect, texCoords);

	sampleEffect.rgb = sampleEffect.rgb * 0.502f;
	vec3 ambient    = 0.2f * colorWhite;
	vec3 diffuse    = diffuseComp * colorWhite;
	vec3 result     = (ambient + diffuse) * vec3(sampleDiffuse);
	vec3 result2	= (ambient + diffuse) * (sampleEffect.rgb * -vec3(1.f));
	color = vec4(result + result2, sampleEffect.a);
}
