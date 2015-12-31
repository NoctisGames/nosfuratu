precision mediump float;

uniform sampler2D u_TextureUnit;
uniform float u_TimeElapsed;

varying vec4 v_Color;
varying vec2 v_TextureCoordinates;
varying vec2 v_Center;

void main()
{
    vec2 uv = v_TextureCoordinates;
    vec2 center = vec2(v_Center.x, v_Center.y);
    vec2 texCoord = uv;
    float distance = distance(uv, center);
    float time = u_TimeElapsed;
    if ((distance <= (time + 0.1)) && (distance >= (time - 0.1)))
    {
        float diff = (distance - time);
        float powDiff = 1.0 - pow(abs(diff * 10.0), 0.8);
        float diffTime = diff  * powDiff;
        vec2 diffUV = normalize(uv - center);
        texCoord = uv + (diffUV * diffTime);
    }
    
    gl_FragColor = texture2D(u_TextureUnit, texCoord);
}