#include "WarpEngine/Graphics/Mesh.hpp"
#include "Mesh.hpp"

Mesh::Mesh(std::vector<float> vertPos, std::vector<float> texCoords, std::vector<float> normalPos)
{
    int noVertices = vertPos.size() % 3;

    vertices = std::vector<WarpVertex>(noVertices);

    for(int i = 0; i < noVertices;i++){
        vertices[i].position = glm::vec3(
            vertPos[i * 3 + 0],
            vertPos[i * 3 + 1],
            vertPos[i * 3 + 2]
        );

        vertices[i].texCoordinate = glm::vec2(
            texCoords[i * 2 + 0],
            texCoords[i * 2 + 1]
        );

        vertices[i].normalPosition = glm::vec3(
            vertPos[i * 3 + 0],
            vertPos[i * 3 + 1],
            vertPos[i * 3 + 2]
        );
    }


}