#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Добавляем uniform для управления эффектом
uniform float lightsOn;

out vec4 finalColor;

void main() {
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    if (lightsOn > 0.5) {
        // Если свет включен - обычная отрисовка
        finalColor = texelColor;
    } else {
        // Если свет выключен - инвертируем чёрные пиксели
        if (texelColor.r < 0.1 && texelColor.g < 0.1 && texelColor.b < 0.1) {
            finalColor = vec4(1.0, 1.0, 1.0, texelColor.a);
        } else {
            finalColor = vec4(0.0, 0.0, 0.0, texelColor.a);
        }
    }
}