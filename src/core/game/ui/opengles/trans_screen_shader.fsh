#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D u_from;
uniform sampler2D u_to;
uniform float u_progress;

varying vec2 v_TextureCoordinates;

const vec2 center = vec2(0.5, 0.5);
const float SQRT_2 = 1.414213562373;

void main() {
    float smoothness = 0.3;
    bool opening = true;
    vec2 p = v_TextureCoordinates;
    float x = opening ? u_progress : 1.-u_progress;
    float m = smoothstep(-smoothness, 0.0, SQRT_2*distance(center, p) - x*(1.+smoothness));
    gl_FragColor = mix(texture2D(u_from, p), texture2D(u_to, p), opening ? 1.-m : m);
} 
