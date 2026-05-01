#version 330 core

// Inputs
layout (location = 0) in vec3 VertexPosition; // The vertex position from the VAO
layout (location = 1) in vec2 texCoord; // The texture coordinates from the VAO
layout (location = 2) in vec3 aNormal; // The normal from the VAO

// Uniforms
uniform mat4 model; // Stores the model matrix
uniform mat4 transform; // Stores the transform matrix

// Outputs
out vec3 FragPos;
out vec2 textureCoords;
out vec3 Normal;   
out mat3 TBN;

void main()
{
    // Transform the vertex position to world space
    FragPos = vec3(model * vec4(VertexPosition, 1.0));
    
    // Pass the normal to the fragment shader after transforming it
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass the texture coordinates to the fragment shader
    textureCoords = texCoord;

    // Project the vertex position to clip space
    gl_Position = transform * vec4(VertexPosition, 1.0);
}
