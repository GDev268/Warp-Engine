#ifndef WARP_MESH
#define WARP_MESH

#include "./Common.hpp"

struct WarpVertex{
    glm::vec3 position;
    glm::vec2 texCoordinate;
    glm::vec3 normalPosition;
};

class Mesh{
    public:
    Mesh(std::vector<float> vertPos,std::vector<float> texCoords,std::vector<float> normalPos);
    ~Mesh();


    std::vector<WarpVertex> vertices;

};

#endif