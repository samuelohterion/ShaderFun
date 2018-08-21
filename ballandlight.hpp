#ifndef BAllANDLIGHT_HPP
#define BAllANDLIGHT_HPP
#include "code/glrenderer.hpp"

class BallAndLight :
public GLProject {

	public:

        BallAndLight( CStr const & p_name, ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mv;

        glm::vec3
        light,
        vel,
        acc;

        glm::mat3
		nrm;

        float
        lastT,
        g;

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
#endif // SPHERE_HPP