#version 330 core

precision highp float;
precision highp int;

uniform float
time;

in vec4
vPos,
vVerts,
vNorms,
vColors;

out vec4
fCol;

void
main( void ) {

	vec3
	light = vec3( +3. * sin( 2.5 * time ), +3. * sin( .15 * time ), +3. + 3. * cos( 2.5 * time ) ),
	diff  = light - vVerts.xyz;

	float
	a = pow( clamp( dot( vNorms.xyz, diff ), 0., 1e5f ) / dot( diff, diff ), 2. );

	vec3
	v3 = 1. - vPos.xyz * vPos.xyz;

	vec3
	b = vec3(
		abs( v3.x ) < .1 ||
		abs( v3.y ) < .1 ||
		abs( v3.z ) < .1
			? abs( 1. - v3 ) : vec3( .5 ) );

	fCol = vec4( clamp( b * vColors.xyz * ( .1 + .25 * a ), 0., 1. ), 1. );
	//fCol = vec4( v3, 1. );
	//fCol = vec4( vColors.xyz, 1. );
//	fCol = vec4( ( .5 + .5 * vNorms.xyz ) * vVerts.xyz, 1. );
}
