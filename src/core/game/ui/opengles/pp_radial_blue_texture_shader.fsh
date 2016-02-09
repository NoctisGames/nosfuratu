precision mediump float;

uniform sampler2D u_TextureUnit;
uniform int u_Direction; // 0 for left, 1 for up-left, 2 for down-left

varying vec2 v_TextureCoordinates;

void main()
{
    const float strength = 0.125;
    const int samples = 8; // multiple of 2
    
    vec2 fragCoord = v_TextureCoordinates;
    
    vec2 uv = fragCoord.xy;
    
    vec2 pos = vec2(0.0, u_Direction == 0 ? 0.5 : u_Direction == 1 ? 1.0 : 0.0);
    vec2 dir = (fragCoord.xy - pos.xy);
    
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    
    for (int i = 0; i < samples; i += 2) // operating at 2 samples for better performance
    {
        color += texture2D(u_TextureUnit, uv + float(i) / float(samples) * dir * strength);
        color += texture2D(u_TextureUnit, uv + float(i + 1) / float(samples) * dir * strength);
    }
    
    gl_FragColor = color / float(samples);
}