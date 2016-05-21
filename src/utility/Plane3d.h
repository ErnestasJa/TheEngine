#ifndef PLANE3D_H_INCLUDED
#define PLANE3D_H_INCLUDED

class Plane3d
{
public:
	///  Points are going to be passed in counter-clockwise
	///  pC--------pD
	///  |       /  |
	///  |     /    |
	///  |   /      |
	///  | /        |
	///  pB--------pA

	Plane3d()
	{
		n = glm::normalize(glm::vec3(1));
		d = 0.f;
	}

	~Plane3d()
	{
	}

	void SetNormalsAndD(float x, float y, float z, float d)
	{
		this->n.x = x;
		this->n.y = y;
		this->n.z = z;
		this->d = d;

		Normalize();
	}

	void Normalize()
	{
		float scale = 1.f / glm::length(n);
		n.x *= scale;
		n.y *= scale;
		n.z *= scale;
		d *= scale;
	}

	const glm::vec3* GetPoints() const
	{
		return points;
	}

	float Distance(const glm::vec3 &p, float radius = 0.f)
	{
		auto distance = d + radius + glm::dot(n, p);
		return distance;
	}
private:
	glm::vec3 points[4];
	glm::vec3 n;
	float d;
protected:
};

#endif
