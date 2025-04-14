uniform sampler2D texture;
uniform vec2 direction;
uniform float radius;
uniform float sigma;
uniform vec2 resolution;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (2.0 * 3.14159265 * sigma * sigma);
}

void main()
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 color = vec4(0.0);
    float total = 0.0;

    for (float i = -radius; i <= radius; i++) {
        float weight = gaussian(i, sigma);
        vec2 offset = direction * i / resolution;
        color += texture2D(texture, texCoord + offset) * weight;
        total += weight;
    }

    gl_FragColor = color / total;
}