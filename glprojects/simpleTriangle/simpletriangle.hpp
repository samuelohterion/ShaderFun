#ifndef SIMPLETRIANGLE_HPP
#define SIMPLETRIANGLE_HPP
#include "code/glrenderer.hpp"

class SimpleTriangle :
public GLProject {

	public:

		SimpleTriangle( CStr const & p_name = "SimpleTriangle", ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mv;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};

void
print( glm::vec4 const & p_vec );

void
print( glm::mat4 const & p_mat );
#endif // SIMPLETRIANGLE_HPP
