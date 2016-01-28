precision mediump float;

uniform sampler2D u_TextureUnit;

varying vec2 v_TextureCoordinates;

void main()
{
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);
    
    gl_FragColor.r *= 1.2;
    gl_FragColor.b *= 1.1;
}