#version 330 core

// Inputs 
in vec3 FragPos;  // Interpolated fragment position from the vertex shader
in vec2 textureCoords; // INterpolated texture coordinates
in vec3 Normal;   // Interpolated normal

// Uniforms
uniform vec3 lightPosition;  // Stores the position of the light source
uniform vec3 lightColour; // Stores the colour of the light source
uniform vec3 viewPosition; // Stores the position of the scene camera
uniform sampler2D lightTexture; // Stores the texture used by the objects

uniform vec3 rimColour;

// Outputs
out vec4 FragColour; // Final output color

void main()
{

    // Applies texture
    vec3 textureColour = texture(lightTexture, textureCoords).rgb;

    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColour;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

    // Specular lighting
    float specularStrength = 10.0;

    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDir, norm);
    float shininess = 52.0;
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);

    vec3 specular = specularStrength * spec * lightColour;

    // Rim lighting
    float rim = 1.0 - max(dot(viewDirection, norm), 0.0);
    rim = pow(rim, 2.0); 
    vec3 rimLight = rim * rimColour * 0.8;

    // Combine results
    vec3 result = (ambient + diffuse + rimLight) * textureColour + specular;
    FragColour = vec4(result, 1.0);
}
