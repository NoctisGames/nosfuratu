precision mediump float;

uniform sampler2D u_TextureUnit;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;

void main()
{
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
    
    vec4 color = v_Color;
    
    float k = color.r - 1.0;
    
    k = clamp(k, 0.0, 2.0);
    
    color.r = 1.0;
    color.g = 1.0;
    color.b = 1.0;
    
    if (k > 0.0)
    {
        if (gl_FragColor.g > gl_FragColor.r)
        {
            float originalR = gl_FragColor.r;
            gl_FragColor.r = gl_FragColor.g * k;
            gl_FragColor.g = originalR;
            
            gl_FragColor.r = clamp(gl_FragColor.r, 0.0, 1.0);
        }
    }
    
    gl_FragColor *= color;
}
