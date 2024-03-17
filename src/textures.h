#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, int** tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with two quads per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i][j];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the quads' vertices of the current tile
                sf::Vertex* quads = &m_vertices[(i + j * width) * 4];

                // define the 6 corners of the two quads
                quads[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quads[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quads[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quads[3].position = sf::Vector2f(i * tileSize.x, (j+1) * tileSize.y);
                

                // define the 6 matching texture coordinates
                quads[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quads[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quads[2].texCoords = sf::Vector2f((tu+1) * tileSize.x, (tv+1) * tileSize.y);
                quads[3].texCoords = sf::Vector2f((tu ) * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};