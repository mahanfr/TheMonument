#version 330

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 finalColor;

uniform sampler2D texture0;

uniform vec4 ambientColor;
uniform vec4 lightColor;
uniform vec3 lightDirection;

uniform vec3 viewPosition;
uniform float shininess;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-lightDirection);

    float diff = max(dot(N, L), 0.0);

    vec3 V = normalize(viewPosition - fragWorldPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N,H), 0.0), shininess);

    vec4 albedo = texture(texture0, fragTexCoord);

    vec4 lighting = ambientColor +
                    lightColor * diff +
                    lightColor * spec * 0.5;
    finalColor = albedo * lighting;
}
