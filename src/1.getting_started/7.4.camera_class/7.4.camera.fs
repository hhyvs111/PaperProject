#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture1, TexCoord);
//暂时不用上面的纹理，直接划线
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}