precision mediump float;

uniform sampler2D u_TextureUnit;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;

void main()
{
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
    
    vec4 color = v_Color;
    
    float k = 2.0 - color.a;
    
    if (k >= 1.0)
    {
        k = 1.0;
        color.r = 1.0;
        color.g = 1.0;
        color.b = 1.0;
    }
    
    color.a = clamp(color.a, 0.0, 1.0);
    
    if (gl_FragColor.a > 0.0)
    {
        float m = min(gl_FragColor.r, gl_FragColor.g);
        m = min(m, gl_FragColor.b);
        float additive = (1.0 - m) * k;
        gl_FragColor += additive;
    }
    
    if (gl_FragColor.r > 0.85
        && gl_FragColor.r < 1.0
        && gl_FragColor.g > 0.73
        && gl_FragColor.g < 0.88
        && gl_FragColor.b > 0.0
        && gl_FragColor.b < 0.15)
    {
        // We are sampling the Snake's Eye, make sure it stays Vampire Yellow!
        gl_FragColor.a *= color.a;
    }
    else if (gl_FragColor.r > 0.94
             && gl_FragColor.g > 0.94
             && gl_FragColor.b > 0.94)
    {
        // We are sampling the Snake's Fangs, make sure they stay White!
        gl_FragColor.a *= color.a;
    }
    else
    {
        gl_FragColor *= color;
    }
}