#pragma once

#include <map>
#include <string>
#include "Blocks.h"
#include <iostream>

extern BlocksData BlocksINF[16];
extern std::map<std::string,BlocksData> BlocksMap;
class Tile
{

private: 

    BlocksData& Data;

public:

    Tile () :  Data(BlocksINF[0]) 
    {
        // std::cout << "null ";

        
    }
    Tile(BlocksData& data):     Data(data)    {std::cout << "data ";}


    void operator=(int n)
    {
        this->Data= BlocksINF[n];
        std::cout<< this->Data.ID;
    }
    void operator=(BlocksData& data)
    {
        this->Data = data;
        std::cout<< this->Data.ID;
    }
    Tile(Tile* other) : Data(other->Data)
    {
        this->Data = other->Data;
        std::cout << "other ";
    }

    void SetID(int n)
    {
        this->Data= BlocksINF[n];
        std::cout<< this->Data.ID;
    }
    void SetBlock(BlocksData& data)
    {
        this->Data = BlocksINF[0];
        std::cout<< this->Data.ID;
    }

    int GetTile(){
        return Data.ID;
    }

    int GetType()
    {
        return Data.Type;
    }
    
    int GetDurability()
    {
        return 0;
    }

    bool operator==(int N)
    {
        return Data.ID==N;
    }
    bool operator==(BlocksData& N)
    {
        return Data.ID==N.ID;
    }
    bool operator!=(int N)
    {
        return Data.ID!=N;
    }
    bool operator!=(BlocksData& N)
    {
        return Data.ID!=N.ID;
    }
    friend void SwapTiles(Tile& one, Tile& two)
    {
        std::cout<< one.Data.ID;
        BlocksData& inter = one.Data;
        one.Data = two.Data;
        two.Data = inter;
    }

    ~Tile(){}
};
