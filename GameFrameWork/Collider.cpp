#include "Collider.h"

bool  learning::Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs)
{
	return (rhs.center - lhs.center).LengthSquared() <= pow(lhs.radius + rhs.radius, 2);
}

bool learning::Intersect(ColliderBox const& lhs, ColliderBox const& rhs)
{
	//x-direction
	if (lhs.center.x - lhs.halfSize.x > rhs.center.x + rhs.halfSize.x) return false;
	if (lhs.center.x + lhs.halfSize.x < rhs.center.x - rhs.halfSize.x) return false;

	//y-direction
	if (lhs.center.y - lhs.halfSize.y > rhs.center.y + rhs.halfSize.y) return false;
	if (lhs.center.y + lhs.halfSize.y < rhs.center.y - rhs.halfSize.y) return false;

	return true;

}