#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_TextureUnit;
uniform sampler2D u_TextureUnitGrayMap;
uniform float u_TimeElapsed;

varying vec2 v_TextureCoordinates;

void main()
{
    vec2 uv = v_TextureCoordinates;
    gl_FragColor = texture2D(u_TextureUnit, uv);
    
    // get gray comp of u_TextureUnitGrayMap
    vec4 secondImageGray = texture2D(u_TextureUnitGrayMap, uv);
    float gray = (secondImageGray.r + secondImageGray.g + secondImageGray.b) / 3.0;
    secondImageGray.r = gray;
    secondImageGray.g = gray;
    secondImageGray.b = gray;
    
    // threshold on sin time
    float thresh = (sin((u_TimeElapsed + 2.6) * 1.6) * 0.5) + 0.5;
    
    if (secondImageGray.r > thresh)
    {
        gl_FragColor.r = 0.0;
        gl_FragColor.g = 0.0;
        gl_FragColor.b = 0.0;
    }
}
