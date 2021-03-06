#ifndef CUBEWITHTEXTURE_HPP
#define CUBEWITHTEXTURE_HPP

#include "../../code/glr.hpp"

class CubeWithTexture :
public GLProject {

	public:

		CubeWithTexture( CStr const & p_name = "CubeWithTexture" );

	public:

		glm::mat4
		model,
		view,
		projection;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // TEXTURETEST_HPP
