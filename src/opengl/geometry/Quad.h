#pragma once

#include "opengl/Mesh.h"

class Plane3d;

class Quad : public Mesh
{
private:
    float m_size;

public:
	Quad(float size = 1.0f);
	Quad(const Plane3d &plane);

    virtual ~Quad(){

    };

    void SetUV(vector<glm::vec2> uvs);
};

typedef std::shared_ptr<Quad> QuadPtr;