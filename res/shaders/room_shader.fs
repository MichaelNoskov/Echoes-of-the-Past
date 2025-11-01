#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float lightsOn;
uniform vec2 flashlightPos;
uniform vec2 screenSize;

out vec4 finalColor;

void main() {
    vec4 texelColor = texture(texture0, fragTexCoord);

    vec2 aspectRatio = vec2(screenSize.x / screenSize.y, 1.0);
    vec2 normalizedCoords = fragTexCoord * aspectRatio;
    vec2 normalizedFlashlightPos = flashlightPos * aspectRatio;

    float radius = 0.1;
    float distanceToFlashlight = distance(normalizedCoords, normalizedFlashlightPos);
    
    bool inFlashlightRange = distanceToFlashlight < radius;
    
    if (lightsOn > 0.5 || inFlashlightRange) {
        finalColor = texelColor;
    } else {
        if (texelColor.r < 0.1 && texelColor.g < 0.1 && texelColor.b < 0.1) {
            finalColor = vec4(1.0, 1.0, 1.0, texelColor.a);
        } else {
            finalColor = vec4(0.0, 0.0, 0.0, texelColor.a);
        }
    }
}