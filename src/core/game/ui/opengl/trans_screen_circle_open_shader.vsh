uniform mat4 u_MvpMatrix;

uniform float u_CenterX;
uniform float u_CenterY;

attribute vec2 a_Position;

varying vec2 v_TextureCoordinates;
varying vec2 v_Center;

void main()
{
    vec4 pos4 = vec4(a_Position, 0, 1);
    gl_Position = u_MvpMatrix * pos4;
    vec2 pos = vec2(gl_Position.x, gl_Position.y);
    v_TextureCoordinates = (pos + 1.0) / 2.0;
    
    vec4 center = vec4(u_CenterX, u_CenterY, 0, 1);
    vec4 screenCenter = u_MvpMatrix * center;
    
    vec2 c = vec2(screenCenter.x, screenCenter.y);
    v_Center = (c + 1.0) / 2.0;
}