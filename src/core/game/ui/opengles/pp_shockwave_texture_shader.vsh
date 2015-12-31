uniform mat4 u_MvpMatrix;

uniform float u_CenterX;
uniform float u_CenterY;

attribute vec4 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TextureCoordinates;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;
varying vec2 v_Center;

void main()
{
    gl_Position = u_MvpMatrix * a_Position;
    v_Color = a_Color;
    vec2 pos = vec2(gl_Position.x, gl_Position.y);
    v_TextureCoordinates = (pos + 1.0) / 2.0;
    
    vec4 center = vec4(u_CenterX, u_CenterY, 0, 1);
    vec4 screenCenter = u_MvpMatrix * center;
    
    vec2 c = vec2(screenCenter.x, screenCenter.y);
    v_Center = (c + 1.0) / 2.0;
}