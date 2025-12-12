#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 mvp;
uniform mat4 matModel;

void main() {
    fragTexCoord = vertexTexCoord;

    fragWorldPos = vec3(matModel * vec4(vertexPosition, 1.0));

    fragNormal = normalize(mat3(matModel) * vertexNormal);

    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
