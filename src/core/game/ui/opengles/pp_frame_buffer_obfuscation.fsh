precision mediump float;

uniform sampler2D u_TextureUnit;

varying vec2 v_TextureCoordinates;

float SCurve (float x)
{
    x = x * 2.0 - 1.0;
    
    return -x * abs(x) * 0.5 + x + 0.5;
}

vec4 BlurH (sampler2D source, vec2 uv, float radius)
{
    if (radius >= 1.0)
    {
        vec4 A = vec4(0.0);
        vec4 C = vec4(0.0);
        
        float width = 1.0 / 512.0;
        
        float divisor = 0.0;
        float weight = 0.0;
        
        float radiusMultiplier = 1.0 / radius;
        
        // Hardcoded for radius 20 (normally we input the radius
        // in there), needs to be literal here
        
        for (float x = -20.0; x <= 20.0; x++)
        {
            A = texture2D(source, uv + vec2(x * width, 0.0));
            
            weight = SCurve(1.0 - (abs(x) * radiusMultiplier));
            
            C += A * weight;
            
            divisor += weight;
        }
        
        return vec4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
    }
    
    return texture2D(source, uv);
}

void main()
{
    // Apply horizontal blur to final output
    gl_FragColor = BlurH(u_TextureUnit, v_TextureCoordinates, 20.0);
    gl_FragColor.a = 0.5;
}
