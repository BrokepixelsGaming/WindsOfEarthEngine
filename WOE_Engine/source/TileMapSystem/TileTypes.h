// TileTypes.h
#pragma once
#include "tools/Utilities/StringId.h"

enum class TileType
{
    kInvalid,
    // normal tiles
    kGrass,    // easy to move on; low weight
    kGrassland,
    kForest, // hard to move on; high weight
    kJungle,
    kTree,
    kSnowTree,
    kDeadTree,
    kJungleTree,
    kCave,
    kDesertStones,
    kDesert,
    kMountain,
    kBeach,
    kTundra,
    kIceSheet,
    kRiver,    // impassible
    kWater,
    kWall,
    kEmpty,
    kDead,
    kAlive,
    kDying,
    kEmptyPath,
    kSmallOrb,
    kLargeOrb,
    kTreasureRoom,
    kBossRoom,
    kHealingRoom,
    kStartRoom,
    kItemRoom,
    kGuardRoom,

    kNormalTileTypeCount,  // not actually a valid type...

    // special tiles
    kNone,         // no overlay
    kStart,        // the start tile
    kPath,         // the path we found
    kOpenSet,      // the open set of tiles we still need to process
    kClosedSet,    // the closed set of tiles we've already processed
};

// Function to convert TileType to string
inline StringId TileTypeToString(TileType type)
{
    switch (type)
    {
            case TileType::kInvalid: return "kInvalid";
            case TileType::kGrass: return "grass";
            case TileType::kGrassland: return "grassland";
            case TileType::kForest: return "forest";
            case TileType::kJungle: return "jungle";
            case TileType::kTree: return "tree";
            case TileType::kSnowTree: return "snowtree";
            case TileType::kDeadTree: return "deadtree";
            case TileType::kJungleTree: return "jungletree";
            case TileType::kCave: return "cave";
            case TileType::kDesertStones: return "desertstones";
            case TileType::kDesert: return "desert";
            case TileType::kMountain: return "mountain";
            case TileType::kBeach: return "beach";
            case TileType::kTundra: return "tundra";
            case TileType::kIceSheet: return "icesheet";
            case TileType::kRiver: return "river";
            case TileType::kWater: return "water";
            case TileType::kWall: return "wall";
            case TileType::kEmpty: return "empty";
            case TileType::kDead: return "dead";
            case TileType::kAlive: return "alive";
            case TileType::kDying: return "dying";
            case TileType::kEmptyPath: return "emptypath";
            case TileType::kSmallOrb: return "smallorb";
            case TileType::kLargeOrb: return "largeorb";
            case TileType::kTreasureRoom: return "treasureroom";
            case TileType::kBossRoom: return "bossroom";
            case TileType::kHealingRoom: return "healingroom";
            case TileType::kStartRoom: return "startroom";
            case TileType::kItemRoom: return "itemroom";
            case TileType::kGuardRoom: return "guardroom";
    }
    return "Unknown";
}