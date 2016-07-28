#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D u_from;
uniform sampler2D u_to;
uniform float u_progress;

varying vec2 v_TextureCoordinates;

void main()
{
    vec2 p = v_TextureCoordinates.xy;
    float x = u_progress;
    float smoothness = 0.1;
    float m = smoothstep(-smoothness, 0.0, SQRT_2*distance(center, p) - x*(1.5+smoothness));
    gl_FragColor = mix(texture2D(u_from, p), texture2D(u_to, p), 1.-m);
}