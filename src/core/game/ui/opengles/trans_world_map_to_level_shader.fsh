#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D u_from;
uniform sampler2D u_to;
uniform float u_progress;

varying vec2 v_TextureCoordinates;

vec2 zoom(vec2 uv, float amount)
{
    return 0.5 + ((uv - 0.5) * amount);
}

void main() {
    vec2 uv = v_TextureCoordinates;
    vec2 r =  2.0 * vec2(uv.xy - 0.5 * 1.0) / 1.0;
    float pro = u_progress / 0.8;
    float z = (pro) * 0.2;
    float t = 0.0;
    
    if (pro > 1.0)
    {
        z = 0.2 + (pro - 1.0) * 5.;
        t = clamp((u_progress - 0.8) / 0.07,0.0,1.0);
    }
    
    if (length(r) < 0.5+z)
    {
        //uv = zoom(uv, 0.9 - 0.1 * pro);
    }
    else if (length(r) < 0.8+z*1.5)
    {
        uv = zoom(uv, 1.0 - 0.15 * pro);
        t = t * 0.5;
    }
    else if (length(r) < 1.2+z*2.5)
    {
        uv = zoom(uv, 1.0 - 0.2 * pro);
        t = t * 0.2;
    }
    else
    {
        uv = zoom(uv, 1.0 - 0.25 * pro);
    }
    
    gl_FragColor = mix(texture2D(u_from, uv), texture2D(u_to, uv), t);
}