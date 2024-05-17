#pragma once

class Tile{

private: 
    

    int TextureID;
    int Type;

public:
    Tile(int ID,int type): TextureID(ID), Type(type){}
    Tile(int ID): TextureID(ID), Type(0){}
    Tile(): TextureID(0), Type(0){}

    void operator=(int n)
    {
        TextureID = n;
    }
    void SetID(int n)
    {
        TextureID=n;
    }

    void SetType(int n)
    {
        Type = n;
    }
    int GetTile(){
        return TextureID;
    }
    int GetType()
    {
        return Type;
    }

    bool operator==(int N)
    {
        return TextureID==N;
    }
        bool operator!=(int N)
    {
        return TextureID!=N;
    }

    ~Tile(){}
};