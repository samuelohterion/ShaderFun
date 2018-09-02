#include "triangle2d.hpp"
#include "../../code/glmprinter.hpp"

Triangle2D::Triangle2D( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
Triangle2D::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	// frame buffers
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-SIMPLE-TRIANGLE
		{
			glr.vertices( "V-TRIANGLE-2D" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 2 ). attrib( "color", 2, 3 ) <<
				-1.f << -1.f <<           1.f << 0.f << 0.f <<
				+1.f << -1.f <<           1.f << 0.f << 0.f <<
				+0.f << +1.f <<           1.f << 0.f << 0.f <<
				GLR::VertexArray::Object( 0, 3, GL_TRIANGLES );
		}
	}

	// shaders
	{
		//S-TRIANGLE-2D"
		{
			glr.shader(
				"S-TRIANGLE-2D",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec2 vertex;\n"
				"void main( void ) {\n"
					"gl_Position = vec4( vertex, 0., 1. );"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = vec4( .8, .7, .3, 1. );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE );
		}
	}

	// container
	{
		// C-TRIANGLE-2D
		{
			glr.container( "C-TRIANGLE-2D" ).
				setVertexArray( "V-TRIANGLE-2D" ).
				setShader( "S-TRIANGLE-2D" ).
				build( );
			}
		}
	}

void
Triangle2D::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "C-TRIANGLE-2D" } );
}

void
Triangle2D::resize( int p_width, int p_height ) {

}
