#include "Structure.h"

#include "../Block/Block_Location.h"
#include "../Chunk/Chunk.h"
#include "../Block/D_Blocks.h"

#include "../../Util/Random.h"

namespace Structure
{
    void createOak(Chunk& chunk, const Block_Location& location);
    void createCactus(Chunk& chunk, const Block_Location& location);

    Structure_Function makeOak     (&createOak);
    Structure_Function makeCactus  (&createCactus);

    void createOak(Chunk& chunk, const Block_Location& location)
    {
        auto trunkHeight = Random::integer(5, 8);
        //Make the trunk
        for (auto i = 1 ; i < trunkHeight + 1 ; i++)
        {
            chunk.getBlocks().setBlock({location.x, location.y + i, location.z}, Block::oakWood, false);
        }
        int y = trunkHeight + location.y;

        for (char xLeaf = location.x - 2 ; xLeaf < location.x + 3 ; xLeaf++)
        {
            for (char zLeaf = location.z - 2 ; zLeaf < location.z + 3 ; zLeaf++)
            {
                chunk.getBlocks().setBlock({xLeaf, y, zLeaf}, Block::oakLeaf, false );
                chunk.getBlocks().setBlock({xLeaf, y + 1, zLeaf}, Block::oakLeaf, false );
            }
        }
        y++;

        for (char xLeaf = location.x - 1 ; xLeaf < location.x + 2 ; xLeaf++)
        {
            for (char zLeaf = location.z - 1 ; zLeaf < location.z + 2 ; zLeaf++)
            {
                chunk.getBlocks().setBlock({xLeaf, y, zLeaf}, Block::oakLeaf, false );
            }
        }
        y++;

        chunk.getBlocks().setBlock({location.x - 1, y,   location.z}, Block::oakLeaf, false );
        chunk.getBlocks().setBlock({location.x + 1, y,   location.z}, Block::oakLeaf, false );
        chunk.getBlocks().setBlock({location.x,     y,   location.z}, Block::oakLeaf, false );
        chunk.getBlocks().setBlock({location.x,     y,   location.z + 1}, Block::oakLeaf, false );
        chunk.getBlocks().setBlock({location.x,     y,   location.z - 1}, Block::oakLeaf, false );

    }

    void createCactus(Chunk& chunk, const Block_Location& location)
    {
        auto cactusHeight = Random::integer(5, 7);

        for (auto i = 1 ; i < cactusHeight + 1 ; i++)
        {
            chunk.getBlocks().setBlock({location.x, location.y + i, location.z}, Block::cactus, false);
        }
    }
}

