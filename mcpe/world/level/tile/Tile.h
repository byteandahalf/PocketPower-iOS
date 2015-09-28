#pragma once

#include <string>
#include <stdint.h>
#include "../../../world/phys/AABB.h"
#include "../../../client/renderer/texture/TextureUVCoordinateSet.h"
class Material;
class Random;

class Tile {
public:
    Tile(int, TextureUVCoordinateSet, Material*);
    void init();

    static void (*$init)(Tile*);
    static uint8_t* lightBlock;
    static Tile** tiles;
    static bool* solid;
    static bool* shouldTick;
    static void** _vtable;

    void** vtable;
    bool replaceable;
    AABB hitbox;
    TextureUVCoordinateSet tex;
    unsigned char id;
    void* soundType;
    bool canBuildOver;
    int renderPass;
    int renderType;
    int tileType;
    int tileEntityType;
    float thickness;
    bool hasGravity;
    bool ticks;
    float gravity;
    Material* material;
    float friction;
    float destroyTime;
    float explosionResistance;
    int creativeTab;
    bool fancy;
    unsigned int textureIsotropic;
    std::string name;

    static Tile* redstoneDust;
    static Tile* notGate_on;
    static Tile* notGate_off;
    static Tile* buttonStone;
    static Tile* buttonWood;
    static Tile* plateWood;
    static Tile* plateStone;
    static Tile* lever;
    static Tile* lamp_off;
    static Tile* lamp_on;
    static Tile* diode_off;
    static Tile* diode_on;
};