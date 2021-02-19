#ifndef __TKGeom_h__
#define __TKGeom_h__
#include "../TKPrefix.h"
#include <glm/fwd.hpp>

namespace Geom {
	typedef glm::vec3 P3D;
	class Triangle{
	public:
		Triangle(const P3D &A, const P3D &B, const P3D &C);
		~Triangle();
		int direction();
	private:
		glm::vec3 coords[3];
	};

	class Polygon{
	public:
		Polygon();
		~Polygon();

	private:
		std::list<glm::vec3> nodeList;
	};
};

#endif
