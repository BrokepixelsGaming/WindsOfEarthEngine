#include "TilePalatte.h"
#include "../tools/BleachNew.h"
#include <iostream>
#include "TileTypes.h"



TilePalatte::TilePalatte()
	:m_tileWidth(0)
	,m_tileHeight(0)
{
}

TilePalatte::~TilePalatte()
{
	for (size_t i = 0; i < m_pTileDefs.size(); ++i)
	{
		BLEACH_DELETE(m_pTileDefs[i]);
	}

	m_pTileDefs.clear();
}

void TilePalatte::Load(const StringId& pPalatteResource)
{
//------------------------------------------------------------------------------------------------------------------
// Part 1 Getting all the parameters we want from the tileset
//------------------------------------------------------------------------------------------------------------------

	/**/
	// Params to build
	int m_tileCount = 0;
	int m_columns = 0;
	const char* m_imageSource = "";
	int m_imageWidth = 0;
	int m_imageHeight = 0;

	// Parse the data
	// Get the Tilemamp file name
	const char* fileName = pPalatteResource.Get().c_str();

	// Hard coded file directory
	std::string path = "data/TileMaps/"; 
	path.append(fileName);

	tinyxml2::XMLDocument document;
	document.LoadFile(path.c_str());

	//----------------------------------------------------------------------------------------
	// Get the tileset element
	const tinyxml2::XMLElement* pTileset = document.FirstChildElement("tileset");
	if (!pTileset)
		std::cout << "Couldn't find tileset element!\n";

	// Get the data we want

	// Tile width and height attributes
	const tinyxml2::XMLAttribute* pTileWidth = pTileset->FindAttribute("tilewidth");
	if (!pTileWidth)
		std::cout << "Couldn't find tilewidth atrribute!\n";
	if (pTileWidth->QueryIntValue(&m_tileWidth) != tinyxml2::XML_SUCCESS)
		std::cout << "Couldn't query int value from tilewidth!\n";

	const tinyxml2::XMLAttribute* pTileHeight = pTileset->FindAttribute("tileheight");
	if (!pTileHeight)
		std::cout << "Couldn't find tileheight atrribute!\n";
	if (pTileHeight->QueryIntValue(&m_tileHeight) != tinyxml2::XML_SUCCESS)
		std::cout << "Couldn't query int value from tileheight!\n";

	// Tile count
	const tinyxml2::XMLAttribute* pTileCount = pTileset->FindAttribute("tilecount");
	if (!pTileCount)
		std::cout << "Couldn't find tilecount atrribute!\n";
	if (pTileCount->QueryIntValue(&m_tileCount) != tinyxml2::XML_SUCCESS)
		std::cout << "Couldn't query int value from tilecount!\n";

	// Columns
	const tinyxml2::XMLAttribute* pColumns = pTileset->FindAttribute("columns");
	if (!pColumns)
		std::cout << ("Couldn't find columns atrribute!\n");
	if (pColumns->QueryIntValue(&m_columns) != tinyxml2::XML_SUCCESS)
		std::cout << ("Couldn't query int value from columns!\n");

	//----------------------------------------------------------------------------------------
	// Get the next element (image)
	const tinyxml2::XMLElement* pImage = pTileset->FirstChildElement("image");
	if (!pImage)
		std::cout << ("Couldn't find image element!\n");

	// Get the image source
	const tinyxml2::XMLAttribute* pSource = pImage->FindAttribute("source");
	if (!pSource)
		std::cout << ("Couldn't find source atrribute!\n");
	m_imageSource = pSource->Value();

	// Get the image width
	const tinyxml2::XMLAttribute* pImageWidth = pImage->FindAttribute("width");
	if (!pImageWidth)
		std::cout << ("Couldn't find width attribute\n");
	if (pImageWidth->QueryIntValue(&m_imageWidth) != tinyxml2::XML_SUCCESS)
		std::cout << ("Couldn't find width atrribute!\n");

	// Get the image height
	const tinyxml2::XMLAttribute* pImageHeight = pImage->FindAttribute("height");
	if (!pImageHeight)
		std::cout << ("Couldn't find height attribute\n");
	if (pImageHeight->QueryIntValue(&m_imageHeight) != tinyxml2::XML_SUCCESS)
		std::cout << ("Couldn't find height atrribute!\n");

	// Set the start of the tile element
	const tinyxml2::XMLElement* m_pTileElement = pTileset->FirstChildElement("tile");

//------------------------------------------------------------------------------------------------------------------
// Part 2 Instantiating TileDefs from those params.
//------------------------------------------------------------------------------------------------------------------

	// Get the variables
	const char* imageSource = m_imageSource;

	// Hard coded file directory
	std::string imgPath = "data/TileMaps/"; // TODO(Josh): hardcoded for now. Need to figure out better way to do this.
	imgPath.append(imageSource);
	int tileID = -1;
	int tileWidth = m_tileWidth;
	int tileHeight = m_tileHeight;

	StringId type = "kInvalidStringId"; // TODO(Josh): Should type be left invalid or must all tiles have a type?
	float weight = 1.0f;

	// The position of the palatte transform
	int x = 0;
	int y = 0;
	int numColoums = m_imageWidth / tileWidth;

	// Loop through the tile elements and instantiate tile defs properties
	for (const tinyxml2::XMLElement* pTileElement = m_pTileElement; pTileElement != nullptr; pTileElement = pTileElement->NextSiblingElement())
	{
		
		const tinyxml2::XMLAttribute* pId = pTileElement->FindAttribute("id");
		
		pId->QueryIntValue(&tileID);
		
		// Get the attribute of the tile
		const tinyxml2::XMLAttribute* pType = pTileElement->FindAttribute("class");
		if (!pType)
			std::cout << "Can't find tile type!\n";
		else
			type = pType->Value();

		// For this tile element,

		// Get the properties element
		const tinyxml2::XMLElement* pProperties = pTileElement->FirstChildElement("properties");
		if (!pProperties)
		{
			std::cout << "Proprties are null!\n";
			return;
		}

		// Get the property
		const tinyxml2::XMLElement* pProperty = pProperties->FirstChildElement("property");
		if (!pProperty)
		{
			std::cout << ("Property is null!\n");
			return;
		}

		// TODO(Josh): If there will be more than one properties, this needs to be changed!
		// There is only one property, so we just get the weight value
		const tinyxml2::XMLAttribute* pWeight = pProperty->FindAttribute("value");
		if (!pWeight)
			std::cout << ("weight attribute is null!");
		if (pWeight->QueryFloatValue(&weight) != tinyxml2::XML_SUCCESS)
			std::cout << ("Couldn't query float value from weight!\n");

		// Find the correct source transform
		x = (tileID % numColoums) * tileWidth;
		y = (tileID / numColoums) * tileHeight;

		// Instantiate the tileDef and add it to the array
		TileDef* pTileDef = BLEACH_NEW(TileDef(imgPath.c_str(), x , y, tileWidth, tileHeight, type, weight));
		m_pTileDefs.push_back(pTileDef);
	}
}

TileDef* TilePalatte::GetTileDef(int index)
{
	if (index < m_pTileDefs.size())
		return m_pTileDefs.at(index);
	else
		return nullptr;
}

TileDef* TilePalatte::GetTileDefByType(TileType type)
{
	switch (type)
	{
	case TileType::kEmptyPath:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "emptyPath") == 0)
			{
				return pDef;
			}
		}
	case TileType::kWater:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "water") == 0)
			{
				return pDef;
			}
		}
	case TileType::kIceSheet:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "icesheet") == 0)
			{
				return pDef;
			}
		}
	case TileType::kDesert:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "desert") == 0)
			{
				return pDef;
			}
		}
	case TileType::kGrassland:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "grassland") == 0)
			{
				return pDef;
			}
		}
	case TileType::kAlive:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "alive") == 0)
			{
				return pDef;
			}
		}
	case TileType::kDead:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "dead") == 0)
			{
				return pDef;
			}
		}
	case TileType::kDying:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "dying") == 0)
			{
				return pDef;
			}
		}
	case TileType::kEmpty:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "empty") == 0)
			{
				return pDef;
			}
		}
	case TileType::kBeach:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "beach") == 0)
			{
				return pDef;
			}
		}
	case TileType::kTundra:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "tundra") == 0)
			{
				return pDef;
			}
		}
	case TileType::kTree:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "tree") == 0)
			{
				return pDef;
			}
		}
	case TileType::kDeadTree:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "deadtree") == 0)
			{
				return pDef;
			}
		}
	case TileType::kSnowTree:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "snowtree") == 0)
			{
				return pDef;
			}
		}
	case TileType::kDesertStones:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "desertstones") == 0)
			{
				return pDef;
			}
		}
	case TileType::kMountain:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "mountain") == 0)
			{
				return pDef;
			}
		}
	case TileType::kCave:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "cave") == 0)
			{
				return pDef;
			}
		}
	case TileType::kForest:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "forest") == 0)
			{
				return pDef;
			}
		}
	case TileType::kJungle:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "jungle") == 0)
			{
				return pDef;
			}
		}
	case TileType::kJungleTree:
		for (auto& pDef : m_pTileDefs)
		{
			const char* cmpType = pDef->GetType();
			if (std::strcmp(cmpType, "jungletree") == 0)
			{
				return pDef;
			}
		}
	}
	return nullptr;
}
