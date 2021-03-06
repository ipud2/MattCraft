#include "Chunk_Mesh.h"

#include "Chunk.h"

#include "../../Texture/Texture_Atlas.h"

#include "../Block/Block_Location.h"


void Chunk_Mesh::Chunk_Mesh_Part::addVerticies(const std::vector<GLfloat>& verticies )
{
    vertexCoords.insert(vertexCoords.end(), verticies.begin(), verticies.end());
}

void Chunk_Mesh::Chunk_Mesh_Part::addUvCoords (const std::vector<GLfloat>& coords)
{
    textureCoords.insert(textureCoords.end(), coords.begin(), coords.end());
}

void Chunk_Mesh::Chunk_Mesh_Part::buffer()
{
    model.addData(vertexCoords, textureCoords);
    vertexCoords.clear();
    textureCoords.clear();
}


Chunk_Mesh::Chunk_Mesh(const Chunk& chunk)
:   m_p_chunk (&chunk)
{}

void Chunk_Mesh::bufferMesh()
{
    m_solidPart.buffer();
    m_waterPart.buffer();
    m_floraPart.buffer();
}

const Chunk_Mesh::Chunk_Mesh_Part& Chunk_Mesh::getSolidPart() const
{
    return m_solidPart;
}

const Chunk_Mesh::Chunk_Mesh_Part& Chunk_Mesh::getWaterPart() const
{
    return m_waterPart;
}

const Chunk_Mesh::Chunk_Mesh_Part& Chunk_Mesh::getFloraPart() const
{
    return m_floraPart;
}


void Chunk_Mesh::generateMesh(int height)
{
    for (int y = 0 ; y < height ; y++)
    {
        for (char z = 0 ; z < Chunk::SIZE ; z++)
        {
            for (char x = 0 ; x < Chunk::SIZE ; x++)
            {
                if (m_p_chunk->getBlocks().getBlock({x, y, z}).getPhysicalState() == Block::Physical_State::Gas)
                {
                    continue;
                }
                addBlockMesh (x, y, z, m_p_chunk->getBlocks().getBlock({x, y, z}));
            }
        }
    }
}
//This is for the block vertex array generator.
//It basically just determines which vertex array, water or ground, to add the verticies into.
Chunk_Mesh::Chunk_Mesh_Part& Chunk_Mesh::getPart(const Block_t& block)
{
    switch(block.getPhysicalState())
    {
        case Block::Physical_State::Solid:
            return m_solidPart;

        case Block::Physical_State::Liquid:
            return m_waterPart;

        case Block::Physical_State::Flora:
            return m_floraPart;

        default:
            break;
    }
    return m_solidPart;
}

//Adds blocks to mesh if there is a adjacent air block to a block
void Chunk_Mesh::addBlockMesh (float x, float y, float z, const Block_t& block)
{
    if (block.getMeshType() == Block::Mesh_Type::X_Style)
    {
        addPlantToMesh(x, y, z, block);
        return;
    }

    if (shouldMakeMesh(x, y + 1, z, block))
    {
        addBlockTopToMesh   (x, y, z, block);
    }
    if (shouldMakeMesh(x, y -1, z, block) && (int)y != 0)
    {
        addBlockBottomToMesh(x, y, z, block);
    }

    if (shouldMakeMesh(x - 1, y, z, block))
    {
        addBlockLeftToMesh  (x, y, z, block);
    }
    if (shouldMakeMesh(x + 1, y, z, block))
    {
        addBlockRightToMesh (x, y, z, block);
    }

    if (shouldMakeMesh(x, y, z + 1, block))
    {
        addBlockFrontToMesh (x, y, z, block);
    }
    if (shouldMakeMesh(x, y, z - 1, block))
    {
        addBlockBackToMesh  (x, y, z, block);
    }
}

bool Chunk_Mesh::shouldMakeMesh(int x, int y, int z, const Block_t& block)
{
    Block_Location location(x, y, z); //This is so it does not construct this object 3 times, but rather just once.

    return      ( m_p_chunk->getBlocks().getBlock(location).getID() == Block::ID::Air) ||
                (!m_p_chunk->getBlocks().getBlock(location).isOpaque() &&
                  m_p_chunk->getBlocks().getBlock(location).getID() != block.getID());
}


//Some people are confused by this part so here we go:
/*
    So the x, y or z variables refer to the block position, of which is passed into the function.
    The +1 refers to where the vertex is in respect to the block vertex array "origin", of which is the front bottom
    left of a block.
*/
void Chunk_Mesh::addBlockTopToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x,      y + 1, z + 1,
        x + 1,  y + 1, z + 1,
        x + 1,  y + 1, z,
        x + 1,  y + 1, z,
        x,      y + 1, z,
        x,      y + 1, z + 1,
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureTop()));
}

void Chunk_Mesh::addBlockBottomToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x,      y, z,
        x + 1,  y, z,
        x + 1,  y, z + 1,
        x + 1,  y, z + 1,
        x,      y, z + 1,
        x,      y, z
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureBottom()));
}

void Chunk_Mesh::addBlockLeftToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x, y,       z,
        x, y,       z + 1,
        x, y + 1,   z + 1,
        x, y + 1,   z + 1,
        x, y + 1,   z,
        x, y,       z,
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockRightToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x + 1, y,       z + 1,
        x + 1, y,       z,
        x + 1, y + 1,   z,
        x + 1, y + 1,   z,
        x + 1, y + 1,   z + 1,
        x + 1, y,       z + 1,
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockFrontToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x,      y,      z + 1,
        x + 1,  y,      z + 1,
        x + 1,  y + 1,  z + 1,
        x + 1,  y + 1,  z + 1,
        x,      y + 1,  z + 1,
        x,      y,      z + 1,
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

void Chunk_Mesh::addBlockBackToMesh(float x, float y, float z, const Block_t& block)
{
    getPart(block).addVerticies
    (
    {
        x + 1,  y,      z,
        x,      y,      z,
        x,      y + 1,  z,
        x,      y + 1,  z,
        x + 1,  y + 1,  z,
        x + 1,  y,      z,
    });
    getPart(block).addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}

//This makes a sorta X shape
void Chunk_Mesh::addPlantToMesh(float x, float y, float z, const Block_t& block)
{
    m_floraPart.addVerticies
    (
    {
        x,      y,      z,
        x + 1,  y,      z + 1,
        x + 1,  y + 1,  z + 1,
        x + 1,  y + 1,  z + 1,
        x,      y + 1,  z,
        x,      y,      z,

        x,      y,      z + 1,
        x + 1,  y,      z,
        x + 1,  y + 1,  z,
        x + 1,  y + 1,  z,
        x,      y + 1,  z + 1,
        x,      y,      z + 1
    });
    m_floraPart.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
    m_floraPart.addUvCoords(m_p_chunk->getAtlas().getTextureCoords(block.getTextureSide()));
}
