#include "TKGeom.h"
#include <glm/fwd.hpp>

namespace Geom {
	Triangle::Triangle(const P3D &A, const P3D &B, const P3D &C){
		coords[0] = A;
		coords[1] = B;
		coords[2] = C;
	}
	
	Triangle::~Triangle(){}

	int Triangle::direction(){
		P3D V20 = coords[2] - coords[0];
		P3D V21 = coords[2] - coords[1];
		glm::c
	}

	Polygon::Polygon(){}

	Polygon::~Polygon(){}

	
	
}
