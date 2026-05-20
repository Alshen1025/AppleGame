#pragma once
#include "Utillity.h"

namespace learning
{
	struct ColliderCircle
	{
		Vector2f center;
		double radius;
	};

	struct ColliderBox
	{
		Vector2f center;
		Vector2f halfSize;
	};

	// Circle
	bool Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs);
	// AABB
	bool Intersect(ColliderBox const& lhs, ColliderBox const& rhs);
}


