#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D u_from;
uniform sampler2D u_to;
uniform float u_progress;

varying vec2 v_TextureCoordinates;

float rand (vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float prog = 1.0 - u_progress;
    float size = 0.15;
    vec2 p = v_TextureCoordinates;
    float r = rand(vec2(0, p.y));
    float m = smoothstep(0.0, -size, p.x * (1.0-size) + size * r - (prog * (1.0 + size)));
    gl_FragColor = mix(texture2D(u_to, p), texture2D(u_from, p), m);
}