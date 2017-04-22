#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D u_from;
uniform sampler2D u_to;
uniform float u_progress;

varying vec2 v_TextureCoordinates;

void main()
{
    vec3 color = vec3(0, 0, 0);
    float colorPhase = 0.4;
    
    vec2 p = v_TextureCoordinates;
    
    if (u_progress > 0.4999 && u_progress < 0.5001)
    {
        vec4 black = vec4(0, 0, 0, 1);
        gl_FragColor = black;
    }
    else
    {
        gl_FragColor = mix(
                           mix(vec4(color, 1.0), texture2D(u_from, p), smoothstep(1.0 - colorPhase, 0.0, u_progress)),
                           mix(vec4(color, 1.0), texture2D(u_to,   p), smoothstep(      colorPhase, 1.0, u_progress)),
                           u_progress);
    }
}
