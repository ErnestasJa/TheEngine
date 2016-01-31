#pragma once

#include "opengl/Mesh.h"

class Quad : public Mesh
{
private:
    float m_size;

public:
    Quad(float size = 1.0f);

    virtual ~Quad(){

    };

    void SetUV(vector<glm::vec2> uvs);
};

typedef std::shared_ptr<Quad> QuadPtr;