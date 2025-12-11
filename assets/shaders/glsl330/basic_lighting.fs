#version 330

// From Vertex Shader
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec3      ambientColor;
uniform float     ambientStrength;

out vec4 FragColor;

void main() {
    vec3 ambient = ambientColor * ambientStrength;
    vec4 texColor = texture(texture0, fragTexCoord) * fragColor;
    FragColor = vec4(ambient,1) * texColor;
}
