precision mediump float;

uniform sampler2D u_TextureUnit;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;

void main()
{
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
    
    if (gl_FragColor.a > 0.0)
    {
        float m = min(gl_FragColor.r, gl_FragColor.g);
        m = min(m, gl_FragColor.b);
        float additive = (1.0 - m) * v_Color.r;
        gl_FragColor += additive;
    }
    
    vec4 color = v_Color;
    color.r = 1.0;
    
    gl_FragColor *= color;
}