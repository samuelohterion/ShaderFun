#include "multisampling.hpp"

MultiSampling::MultiSampling( CStr & p_name ) :
GLProject ( p_name ) {

}

void
MultiSampling::init( ) {

	glClearColor( .12f, .11f, .10f, 1. );

	projection = view = model = glm::mat4( 1. );

	maxSamples = 2;

	sampleIndex = -1;


	// frame buffer
	{
		// FRAMEBUFFER-MULTISAMPLING
		{
			glr.frameBuffer( "FRAMEBUFFER-MULTISAMPLING" );
		}
	}

	// vertex arrays
	{
		// VERTICES-LIGHTED-CHECKERBOARD-BACKGROUND
		{
			glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-BACKGROUND" ).
				attrib( "vertex", 3, 0 ) <<
				-1.f << -1.f << +0.f <<
				+1.f << -1.f << +0.f <<
				+1.f << +1.f << +0.f <<
				-1.f << +1.f << +0.f <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// VERTICES-2D-QUAD
		{
			glr.vertices( "VERTICES-2D-QUAD" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 2, 0 ) <<
				-1.f << -1.f <<
				+1.f << -1.f <<
				+1.f << +1.f <<
				-1.f << +1.f <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// VERTICES-LIGHTED-CHECKERBOARD-LIGHT
		{
			glr.vertices( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "color", 3, 0 ) <<
				0.f << 0.f << 0.f <<
				GLR::VertexArray::Object( 0, 1, GL_POINTS );
		}
	}

	// shaders
	{
		// SHADER-LIGHTED-CHECKERBOARD-BACKGROUND
		{
			glr.shader(
				"SHADER-LIGHTED-CHECKERBOARD-BACKGROUND",
				// vertex shader
				"#version 330 core\n"
				//	all matrices one needs
				"in vec3 vertex;\n"
				"out VS2FS {\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"
				"void main( ) {\n"
				"	vs2fs.texCoord = .5 + .5 * vertex.xy;\n"
				"	gl_Position = vec4( vertex, 1 );\n"
				"}\n",
				// fragment shader
				"#version 330 core\n"
				"uniform sampler2D txIn0;\n"
				"in VS2FS {\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = vec4( .3 - .2 * texture( txIn0, vs2fs.texCoord ).rgb, 1 );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE );
		}
		// SHADER-LIGHTED-CHECKERBOARD-CHECKERBOARD
		{
			glr.shader(
				"SHADER-LIGHTED-CHECKERBOARD-CHECKERBOARD",

				// VERTEX SHADER

				//	version 3.3 core
				"#version 330 core\n"

				//	all matrices one needs
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"

				//	light in every space that makes sense to show their behavior
				"uniform vec3 light1InModelSpacePosition;\n"
				"uniform vec3 light2InCameraSpacePosition;\n"
				"uniform vec3 light1InModelSpaceColor;\n"
				"uniform vec3 light2InCameraSpaceColor;\n"

				//	only 2d-vertices come in
				"in vec2 vertex;\n"

				//	Vertex To Fragment
				"out VS2FS {\n"

				//	squareCoordsInInitialSpace (0,0) - (0,7)
				"	vec2 squareCoordsInWorldSpace;\n"

				//	tranformed vectors
				"	vec4\n"
				"		vertexInModelSpace,\n"
				"		vertexInCameraSpace,\n"
				"		vertexInProjectionSpace,\n"
				"		normalVectorInModelSpace,"
				"		normalVectorInCameraSpace;\n"
				"} vs2fs;\n"

				//	now the main part (O;
				"void main( ) {\n"

				//	save the xy - coords transformed from (-1,+1) to (0,7)
				"	vs2fs.squareCoordsInWorldSpace = 4. + 4. * vertex.xy;\n"

				// create a 4d vector out of the 2d input
				"	vec4 v4 = vec4( vertex.x, 0, - vertex.y, 1 );\n"

				//	make some trafos
				"	vs2fs.vertexInModelSpace          = model * v4;\n"
				"	vs2fs.vertexInCameraSpace         = view  * model * v4;\n"
				"	vs2fs.vertexInProjectionSpace     = projection * view * model * v4;\n"
				"	vs2fs.normalVectorInModelSpace    = vec4( 0, 1, 0, 0 ) * inverse( model );\n"
				"	vs2fs.normalVectorInCameraSpace   = vec4( 0, 1, 0, 0 ) * inverse( view * model );\n"

				// position of course has to be transformed into projection space
				"	gl_Position = projection * view * model * v4;\n"
				"}\n",

				// FRAGMENT SHADER

				// version 3.3 core
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform vec3 light1InModelSpacePosition;\n"
				"uniform vec3 light2InCameraSpacePosition;\n"
				"uniform vec3 light1InModelSpaceColor;\n"
				"uniform vec3 light2InCameraSpaceColor;\n"
				"uniform int  faceIndex;\n"
				"uniform sampler2D txIn1;\n"

				// Vertex To Fragment
				"in VS2FS {\n"

				//	squareCoordsInInitialSpace (0,0) - (7,7)
				"	vec2\n"
				"		squareCoordsInWorldSpace;\n"

				//	tranformed vectors
				"	vec4\n"
				"		vertexInModelSpace,\n"
				"		vertexInCameraSpace,\n"
				"		vertexInProjectionSpace,\n"
				"		normalVectorInModelSpace,\n"
				"		normalVectorInCameraSpace;\n"
				"} vs2fs;\n"

				// output color
				"out vec4 fColor;\n"

				// 2d random
				"float random ( in vec2 st ) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"

				// main again
				"void main( ) {\n"
				"	vec3\n"
				"	d1 = light1InModelSpacePosition  - vs2fs.vertexInModelSpace.xyz,\n"
				"	d2 = light2InCameraSpacePosition - vs2fs.vertexInCameraSpace.xyz;\n"

				// compute 1st step of simple lighting
				"	float\n"

				// 1st: in model space
				"	a1 = max( 0, .5 * dot( d1, vs2fs.normalVectorInModelSpace.xyz )  / dot( d1, d1 ) ),\n"

				// 2nd: now in camera (model-view) space
				"	a2 = max( 0, .5 * dot( d2, vs2fs.normalVectorInCameraSpace.xyz ) / dot( d2, d2 ) ),\n"

				// 3rd: global light
				"	a3 = max( 0, .5 * dot( vec3( 0, 0, 1 ), vs2fs.normalVectorInCameraSpace.xyz ) );\n"

				"	fColor = vec4( "
				"		( faceIndex < 4 )\n"
				"			? ( faceIndex < 2 )\n"
				"				? ( faceIndex < 1 )\n"
				"					? vec3( 1,0,0 )\n"
				"					: vec3( 0,1,0 )\n"
				"				: ( faceIndex < 3 )"
				"					? vec3( 1,1,0 )\n"
				"					: vec3( 0,0,1 )\n"
				"			: ( faceIndex < 6 )\n"
				"				? vec3( 1,0,1 )\n"
				"				: vec3( 1,1,0 ), 1 );\n"

				// checkerboard color
				"	fColor += ( ( int( vs2fs.squareCoordsInWorldSpace.x ) + int( vs2fs.squareCoordsInWorldSpace.y ) ) % 2 == 0 )\n"
				"		? vec4( .5, .5, .5, 0. )\n"
				"		: vec4( .0, .0, .0, 0. );\n"

				//
				"	fColor.xyz = clamp( fColor.xyz + .5 * random( vs2fs.squareCoordsInWorldSpace ), vec3( 0 ), vec3( 1 ) ) *\n"
				"		( ( faceIndex % 2 == 0 )"
				"			? ( texture( txIn1, .125 * vs2fs.squareCoordsInWorldSpace ) ).xyz"
				"			: ( 1. - ( texture( txIn1, .125 * vs2fs.squareCoordsInWorldSpace ) ).xyz ) );\n"
				"	fColor.xyz = max( vec3( 0 ), fColor.xyz );\n"

				// add some noise

				// and now compute 2nd step of simple lighting
				"	fColor.xyz = fColor.xyz * ( pow( a1, 2. ) * light1InModelSpaceColor + pow( a2, 2. ) * light2InCameraSpaceColor + a3 * vec3( 1 ) );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "light1InModelSpacePosition",  GLR::Shader::VEC3, GLR::Shader::SCALAR, & light1InModelSpacePosition ).
				addUniform( "light2InCameraSpacePosition", GLR::Shader::VEC3, GLR::Shader::SCALAR, & light2InCameraSpacePosition ).
				addUniform( "light1InModelSpaceColor",  GLR::Shader::VEC3, GLR::Shader::SCALAR, & light1InModelSpaceColor ).
				addUniform( "light2InCameraSpaceColor", GLR::Shader::VEC3, GLR::Shader::SCALAR, & light2InCameraSpaceColor ).
				addUniform( "faceIndex", GLR::Shader::INT, GLR::Shader::SCALAR, & faceIndex );
		}
		// SHADER-LIGHTED-CHECKERBOARD-NORMALS
		{
			glr.shader(
				"SHADER-LIGHTED-CHECKERBOARD-NORMALS",

				"#version 330 core\n"
				"precision highp float;\n"
				"precision highp int;\n"
				"in vec2 vertex;\n"
				"out VS2GS {\n"
				"	vec4 vertex;\n"
				"} vs2gs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				"void main( ) {\n"
				"	vec4 v = view * model * vec4( vertex.y, 0, vertex.x, 1 );\n"
				"	vs2gs.vertex = v;\n"
				"	gl_Position  = projection * v;\n"
				"}\n",

				"#version 330 core\n"
				"precision highp float;\n"
				"precision highp int;\n"
				"layout ( triangles ) in;\n"
				"layout ( line_strip, max_vertices = 6 ) out;\n"
				"uniform mat4 projection;\n"
				"in VS2GS {\n"
				"	vec4 vertex;\n"
				"} vs2gs[ ];\n"
				"out GS2FS {\n"
					"vec3 color;\n"
				"} gs2fs;\n"
				"const float MAGNITUDE = 4.;\n"
				"void generateLine( int index, vec3 n ) {\n"
				"	gl_Position = gl_in[ index ].gl_Position;\n"
				"	gs2fs.color = n;\n"
				"	EmitVertex( );\n"
				"	gl_Position = gl_in[ index ].gl_Position + projection * vec4( n, 0. ) * MAGNITUDE;\n"
				"	gs2fs.color = vec3( .12f, .11f, .10f );\n"
				"	EmitVertex( );\n"
				"	EndPrimitive( );\n"
				"}\n"
				"void main( ) {\n"
				"	vec3"
				"	n = normalize( cross( vs2gs[ 1 ].vertex.xyz - vs2gs[ 0 ].vertex.xyz, vs2gs[ 2 ].vertex.xyz - vs2gs[ 0 ].vertex.xyz ) );\n"
				"	generateLine( 0, n ); // first vertex normal\n"
				"	generateLine( 1, n ); // second vertex normal\n"
				"	generateLine( 2, n ); // third vertex normal\n"
				"}\n",

				"#version 330 core\n"
				"precision highp float;\n"
				"precision highp int;\n"
				"in GS2FS {\n"
					"vec3 color;\n"
				"} gs2fsIn;\n"
				"out vec4 fCol;\n"
				"void main( void ) {\n"
					"fCol = vec4( gs2fsIn.color, 1.f );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
					addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
					addUniform( "view",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
					addUniform( "projection",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
		// SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE
		{
			glr.shader(
				"SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE",

				// VERTEX SHADER

				//	version 3.3 core
				"#version 330 core\n"

				//	matrices in every space to show their behavior
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				//	light in model space to show their behavior
				"uniform vec3 light1InModelSpacePosition;\n"
				"uniform vec3 light1InModelSpaceColor;\n"

				"in vec3 color;\n"

				//	Vertex To Fragment
				"out VS2FS {\n"

				//	tranformed vectors
				"vec4\n"
				"	color;\n"
				"} vs2fs;\n"

				//	now the main part (O;
				"void main( ) {\n"

				"	vec4\n"
				"		v = projection * view * vec4( light1InModelSpacePosition, 1 );"

				"	gl_PointSize = 150 - 30. * v.z;\n"

				//	make some trafos
				"	vs2fs.color  = vec4( light1InModelSpaceColor, 1 );\n"

				// position of course has to be transformed into projection space
				"	gl_Position = projection * view * vec4( light1InModelSpacePosition, 1 );\n"
				"}\n",

				// FRAGMENT SHADER

				// version 3.3 core
				"#version 330 core\n"

				// Vertex To Fragment
				"in VS2FS {\n"

				//	tranformed vectors
				"vec4\n"
				"		color;\n"
				"} vs2fs;\n"

				// output color
				"out vec4 fColor;\n"

				// 2d random
				"float random ( in vec2 st ) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"

				// main again
				"void main( ) {\n"
				"	vec2  v = 2. * gl_PointCoord.xy - 1.;\n"
				"	float s = dot( v, v );\n"
				"	if( s > .999 )\n"
				"		discard;\n"
				"	fColor = vec4( ( 1 - s ) +  vs2fs.color.xyz, 1. );\n"
				"	if( random( gl_PointCoord.xy ) < 1.1 * ( 1. - .1 / ( .1 + sqrt( s ) ) ) )\n"
				"		discard;\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",               GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",                GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection",          GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "light1InModelSpacePosition",  GLR::Shader::VEC3, GLR::Shader::SCALAR, & light1InModelSpacePosition ).
				addUniform( "light1InModelSpaceColor",  GLR::Shader::VEC3, GLR::Shader::SCALAR, & light1InModelSpaceColor );
		}
		// SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE
		{
			glr.shader(
				"SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE",

				// VERTEX SHADER

				//	version 3.3 core
				"#version 330 core\n"

				//	matrices in every space to show their behavior
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				//	light in model space to show their behavior
				"uniform vec3 light2InCameraSpacePosition;\n"
				"uniform vec3 light2InCameraSpaceColor;\n"

				"in vec3 color;\n"

				//	Vertex To Fragment
				"out VS2FS {\n"

				//	tranformed vectors
				"vec4\n"
				"		color;\n"
				"} vs2fs;\n"

				//	now the main part (O;
				"void main( ) {\n"

				"	vec4\n"
				"	v = projection * vec4( light2InCameraSpacePosition, 1 );\n"

				"	gl_PointSize = 250 - v.z;\n"

				//	make some trafos
				"	vs2fs.color  = vec4( light2InCameraSpaceColor, 1 );\n"

				// position of course has to be transformed into projection space
				"	gl_Position = v;\n"
				"}\n",

				// FRAGMENT SHADER

				// version 3.3 core
				"#version 330 core\n"

				// Vertex To Fragment
				"in VS2FS {\n"

				//	tranformed vectors
				"vec4\n"
				"	color;\n"
				"} vs2fs;\n"

				// output color
				"out vec4 fColor;\n"

				// 2d random
				"float random ( in vec2 st ) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"

				// main again
				"void main( ) {\n"
				"	vec2  v = 2. * gl_PointCoord.xy - 1.;\n"
				"	float s = dot( v, v );\n"
				"	if( s > .999 )\n"
				"		discard;\n"
				"	fColor = vec4( ( 1 - s ) +  vs2fs.color.xyz, 1. );\n"
				"	if( random( gl_PointCoord.xy ) < 1.1 * ( 1. - .1 / ( .1 + sqrt( s ) ) ) )\n"
				"		discard;\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",               GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",                GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection",          GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "light2InCameraSpacePosition",  GLR::Shader::VEC3, GLR::Shader::SCALAR, & light2InCameraSpacePosition ).
				addUniform( "light2InCameraSpaceColor", GLR::Shader::VEC3, GLR::Shader::SCALAR, & light2InCameraSpaceColor );
		}
		// SHADER-MULTISAMPLING
		{
			glr.shader(
				"SHADER-MULTISAMPLING",
				// vertex shader
				"#version 330 core\n"
				"in vec2 vertex;\n"
				"out VS2FS {\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"

				"void main( ) {\n"
				"	vs2fs.texCoord  = .5 + .5 * vertex;\n"
				"	gl_Position = vec4( vertex, 0, 1 );\n"
				"}\n",
				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform vec2 dPixPos;\n"
				"uniform sampler2D txMS;\n"
				"in VS2FS {\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = texture( txMS, vs2fs.texCoord );\n"
				"	vec3 "
				"		dx =\n"
				"			texture( txMS, vec2( vs2fs.texCoord.x + dPixPos.x, vs2fs.texCoord.y ) ).rgb -\n"
				"			texture( txMS, vec2( vs2fs.texCoord.x - dPixPos.x, vs2fs.texCoord.y ) ).rgb,\n"
				"		dy =\n"
				"			texture( txMS, vec2( vs2fs.texCoord.x, vs2fs.texCoord.y + dPixPos.y ) ).rgb -\n"
				"			texture( txMS, vec2( vs2fs.texCoord.x, vs2fs.texCoord.y - dPixPos.y ) ).rgb;\n"
				"	fColor.xyz = fColor.xyz + ( ( dx + dy ) );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "dPixPos",  GLR::Shader::VEC2, GLR::Shader::SCALAR, & dPixPos );
		}
	}

	// textures
	{
		// TEXTURE-LIGHTED-CHECKERBOARD-BACKGROUND
		{
			glr.texture(
			"TEXTURE-LIGHTED-CHECKERBOARD-BACKGROUND",
			new GLR::Texture( "txIn0", "../pix/Schubler.png" ) );
		}
		// TEXTURE-LIGHTED-CHECKERBOARD-CHECKERBOARD
		{
			glr.texture(
				"TEXTURE-LIGHTED-CHECKERBOARD-CHECKERBOARD",
				new GLR::Texture( "txIn1", "../pix/Schubler.png" ) );
		}
		// TEXTURE-TEXTURE-MULTISAMPLING
		{
			glr.texture(
				"TEXTURE-MULTISAMPLING",
				new GLR::Texture( "txMS",
					GL_TEXTURE_2D, 0, GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT,
					vcd->width, vcd->height ) );
		}
	}

	// programs
	{
		// PROGRAM-LIGHTED-CHECKERBOARD-BACKGROUND
		{
			glr.container( "PROGRAM-LIGHTED-CHECKERBOARD-BACKGROUND" ).
				setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-BACKGROUND" ).
				setShader( "SHADER-LIGHTED-CHECKERBOARD-BACKGROUND" ).
				addInTexture( "TEXTURE-LIGHTED-CHECKERBOARD-BACKGROUND" ).
				setFrameBuffer( "FRAMEBUFFER-MULTISAMPLING" ).
				addOutTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
		// PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD
		{

			glr.container( "PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
				setVertexArray( "VERTICES-2D-QUAD" ).
				setShader( "SHADER-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
				addInTexture( "TEXTURE-LIGHTED-CHECKERBOARD-CHECKERBOARD" ).
				setFrameBuffer( "FRAMEBUFFER-MULTISAMPLING" ).
				addOutTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
		// PROGRAM-LIGHTED-CHECKERBOARD-NORMALS
		{

			glr.container( "PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" ).
				setVertexArray(	"VERTICES-2D-QUAD" ).
				setShader( "SHADER-LIGHTED-CHECKERBOARD-NORMALS" ).
				setFrameBuffer( "FRAMEBUFFER-MULTISAMPLING" ).
				addOutTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
		// PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE
		{
			glr.container( "PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
				setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT" ).
				setShader( "SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE" ).
				setFrameBuffer( "FRAMEBUFFER-MULTISAMPLING" ).
				addOutTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
		// PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE
		{
			glr.container( "PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
				setVertexArray( "VERTICES-LIGHTED-CHECKERBOARD-LIGHT" ).
				setShader( "SHADER-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" ).
				setFrameBuffer( "FRAMEBUFFER-MULTISAMPLING" ).
				addOutTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
		// PROGRAM-MULTISAMPLING
		{
			glr.container( "PROGRAM-MULTISAMPLING" ).
				setVertexArray( "VERTICES-2D-QUAD" ).
				setShader( "SHADER-MULTISAMPLING" ).
				addInTexture( "TEXTURE-MULTISAMPLING" ).
				build( );
		}
	}
}

void
MultiSampling::paint( ) {

//	if( ++sampleIndex < maxSamples ) {

		// do the usual stuff
		{

			glDisable( GL_DEPTH_TEST );
			glDisable( GL_CULL_FACE );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}

		// light colors
		{
			light1InModelSpaceColor  = glm::vec3( 1.0f, .50f, .25f );
			light2InCameraSpaceColor = glm::vec3( .25f, .50f, 1.0f );
		}

		//	look from a certain point
		{
			view =
			glm::lookAt(
				glm::vec3( 0, 0, 3 ), // Camera is at ( 0, 0, 3 ), in World Space
				glm::vec3( 0, 0, 0 ), // and looks at the origin
				glm::vec3( .0, 1., 0. ) ); // Head is up (set to 0,-1,0 to look upside-down)
		}

		glr.run( { "PROGRAM-LIGHTED-CHECKERBOARD-BACKGROUND" } );

		glEnable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );

		// construct model an start in center
		model = glm::mat4( 1.f );

		// let the lights dance a little

		// screw in model space
		light1InModelSpacePosition  = glm::vec3( model * glm::vec4( .9f * cosf( 2.7f * vcd->time ), 1.f + .9f * sinf( .5f *  vcd->time ), .9f * sinf( 2.7f * vcd->time ), 1.f ) );

		// swing in front of the camera
		light2InCameraSpacePosition = glm::vec3( view * model * glm::vec4( 2.f * sinf( .5f * sinf( 3.f * vcd->time ) ), 1.25f - 2.f * cosf( .5f * sinf( 3.f * vcd->time ) ), 0.f, 1.f ) );

		// rotate the whole model in time and by mouse-x
		model = glm::rotate( model, .01f * ( vcd->mousex - 0.f * vcd->time ), glm::vec3( 0., 1., 0. ) );

		// move the model up and down by mouse-y
		model = glm::translate( model, glm::vec3( 0., .01f * vcd->mousey - 5.0f, 0. ) );

		// now set the light in model space
		light1InModelSpacePosition = model * glm::vec4( light1InModelSpacePosition, 1.f );


		// paint there a checkerboard and its normal vectors
		faceIndex = 0;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

		// rotate the checkerboard and move it
		model = glm::translate( model, glm::vec3( +0., +1., -1. ) );
		model = glm::rotate( model, .5f  * 3.1415f, glm::vec3( 1., 0., 0. ) );

		// paint another checkerboard and its normal vectors
		++ faceIndex;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

	//	// and so on
		model = glm::translate( model, glm::vec3( +1., +1., 0. ) );
		model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 0., 0., 1. ) );

		++ faceIndex;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

		model = glm::translate( model, glm::vec3( +1., +1., 0. ) );
		model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 0., 0., 1. ) );

		++ faceIndex;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

		model = glm::translate( model, glm::vec3( +1., +1., +0. ) );
		model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 0., 0., 1. ) );

		++ faceIndex;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

		model = glm::translate( model, glm::vec3( 0., +1., -1. ) );
		model = glm::rotate( model, .5f * 3.1415f, glm::vec3( 1., 0., 0. ) );

		++ faceIndex;
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-CHECKERBOARD",
			"PROGRAM-LIGHTED-CHECKERBOARD-NORMALS" } );

		// enable point sizing
		glEnable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
		glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

		// paint the lights
		glr.run( {
			"PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-MODEL-SPACE",
			"PROGRAM-LIGHTED-CHECKERBOARD-LIGHT-IN-CAMERA-SPACE" } );

		// disable point sizing
		glDisable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
		glDisable( GL_VERTEX_PROGRAM_POINT_SIZE );
//	}
//	else {

		// do the usual stuff
		{

			glDisable( GL_DEPTH_TEST );
			glDisable( GL_CULL_FACE );

			//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}

		glr.run( { "PROGRAM-MULTISAMPLING" } );

//		sampleIndex = 0;
//	}
}

void
MultiSampling::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );

	dPixPos = glm::vec2( 1.f / p_width, 1.f / p_height );
}
