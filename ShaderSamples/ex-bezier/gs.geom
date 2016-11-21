/*
    Evaluate a cubic bezier curve on the GPU side using a Geometry Shader.
 */ 

#version 120 
#extension GL_EXT_geometry_shader4 : enable 

uniform int Num;

// Found in nvidia sdk
// more info: http://en.wikipedia.org/wiki/B%C3%A9zier_curve
vec4 evaluateBezierPosition( vec4 v[4], float t )
{
	vec4 p;

	float omt = 1.0 - t;
	float b0 = omt*omt*omt;
	float b1 = 3.0*t*omt*omt;
	float b2 = 3.0*t*t*omt;
	float b3 = t*t*t;
	return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];
}


/**
*	Main
*/
void main() 
{
	vec4 pos[4];
	pos[0] = gl_PositionIn[0];
	pos[1] = gl_PositionIn[1];
	pos[2] = gl_PositionIn[2];
	pos[3] = gl_PositionIn[3];

	float dt = 1.0 / float(Num);
	float t = 0.;
    for( int i=0; i<=Num; i++ )
	{
        vec4 p = evaluateBezierPosition( pos, t );
        gl_Position = vec4( p.xyz, 1.0 );

		EmitVertex();
		t += dt;
    }

	EndPrimitive();
}