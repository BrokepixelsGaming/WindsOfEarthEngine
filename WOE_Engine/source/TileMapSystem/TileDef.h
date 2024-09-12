#pragma once
#include "..\3rdParty\include\SDL\SDL.h"
#include "../tools/Utilities/StringId.h"

class Tile;
struct SDL_Texture;

//------------------------------------------------------------------------------------------------------------------
// Tile definition. Used as a part of a palatte to create, render tiles, and add functionality.
// Notes:
//		- This class needs to be updated when we want to add new properties for our tiles.
//------------------------------------------------------------------------------------------------------------------
class TileDef
{
private:
	// The texture associated with this tile
	SDL_Texture* m_pTexture;

	// The area of the texture to render (this will be different for different tileDefs)
	SDL_Rect m_sourceTransform;

	// Tile properties
	StringId m_type;
	float m_weight;

public:
	// Takes in a source transform (the area to render in the texture)
	TileDef(const char* imageSource, int x, int y, int w, int h, StringId type, float weight);

	// Creates a tile from this definition
	Tile* CreateTile(int x, int y);

	// Render a tile at this position (Called from different tiles)
	void Render(int x, int y);

	// Getters
	int GetWidth() { return m_sourceTransform.w; }
	int GetHeight() { return m_sourceTransform.h; }
	SDL_Texture* GetTexture() { return m_pTexture; }
	SDL_Rect GetSourceTransform() { return m_sourceTransform; }
	StringId GetType();
	float GetWeight() { return m_weight; }

	// TODO(Josh): add setters for easy initialization
};