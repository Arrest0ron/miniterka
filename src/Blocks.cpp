
#include "Blocks.h"
#include <map>
#include <string>


BlocksData BlocksINF[16]
{
    {0,0,0},
    {1,0,2},
    {2,0,2},
    {3,0,2},
    {4,0,1},
    {5,0,1},
    {6,1000,2},
    {7,3000,2},
    {8,2000,2},
    {9,0,1}

};


 std::map<std::string,BlocksData> BlocksMap {
    {"Air", BlocksINF[0]},
    // {"-", 1},
    // {"--", 2},
    // {"---", 3},
    {"WaterOver", BlocksINF[4]},
    {"WaterUnder", BlocksINF[5]},
    {"Stone", BlocksINF[6]},
    {"Diamond", BlocksINF[7]},
    {"Redstone", BlocksINF[8]},
    {"Lava", BlocksINF[9]},
    // {"----", 10},
    // {"-----", 11}
};