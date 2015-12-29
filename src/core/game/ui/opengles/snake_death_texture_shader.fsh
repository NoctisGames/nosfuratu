precision mediump float;

uniform sampler2D u_TextureUnit;
uniform float u_ColorAdditive;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;

void main()
{
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
    
    if (gl_FragColor.w > 0.0)
    {
        float m = min(gl_FragColor.x, gl_FragColor.y);
        m = min(m, gl_FragColor.z);
        float additive = (1.0 - m) * u_ColorAdditive;
        gl_FragColor += additive;
    }
    
    gl_FragColor *= v_Color;
}