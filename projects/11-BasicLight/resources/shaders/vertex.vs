#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// viewPos is here needed only because we compute the lighting in world space and not viewspace
// We can actually compute in view space and not need viewPos here because viewPos is always (0,0,0) in that case.
uniform vec3 viewPos;

out vec3 FragPos;
out vec3 Normal;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   // unsure why but apparently transpose of inverse becomes appropriate for normal. http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
   // Is this really efficient? Computing inverse is slow I believe.
   // Do this on cpu and then use uniform
   Normal = transpose(inverse(mat3(model))) * aNormal;
}
