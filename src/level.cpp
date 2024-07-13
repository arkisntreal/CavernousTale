#include "level.h"
#include "globals.h"
#include "rectangle.h"
#include "tinyxml2.h"
#include "utils.h"

#include <cmath>
#include <sstream>
#include <string>

Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics)
    :   _mapName(mapName),
        _size(Vector2i(0, 0))
{
    this->loadMap(mapName, graphics);
}

void Level::loadMap(std::string mapName, Graphics& graphics) {
    //  Parse the .tmx file
    tinyxml2::XMLDocument doc;
    std::stringstream tmxStream;
    tmxStream << "/home/arc/Projects/CavernousTale/content/maps/" << mapName << ".tmx";
    doc.LoadFile(tmxStream.str().c_str());

    tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");

    int width, height;

    mapNode->QueryIntAttribute("width", &width);
    mapNode->QueryIntAttribute("height", &height);

    this->_size.x = width;
    this->_size.y = height;

    int tileWidth, tileHeight;

    mapNode->QueryIntAttribute("tilewidth", &tileWidth);
    mapNode->QueryIntAttribute("tileheight", &tileHeight);

    this->_tileSize.x = tileWidth;
    this->_tileSize.y = tileHeight;

    tinyxml2::XMLElement* pTileset = mapNode->FirstChildElement("tileset");
    if (pTileset != NULL) {
        while (pTileset) {    
            const char* source = pTileset->Attribute("source");
            int firstgid = pTileset->IntAttribute("firstgid");

            std::stringstream tilesetStream;
            tilesetStream << "/home/arc/Projects/CavernousTale/content/tilesets/" << source;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(tilesetStream.str()));
            this->_tilesets.push_back(Tileset(texture, firstgid));
            
            pTileset = pTileset->NextSiblingElement("tileset");
        }
    }

    //  Parse texture layers
    tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
    if (pLayer != NULL) {
        while (pLayer) {
            tinyxml2::XMLElement* pData = pLayer->FirstChildElement("data");
            if (pData != NULL) {
                while (pData) {
                    const char* data = pData->GetText();
                    int val = 0;
                    std::vector<int> values;
                    std::stringstream dataStream;
                    //  Pull values from data element into vector holding tile id's
                    for (int i = 0; i < strlen(data); i++) {
                        if (data[i] >= 48 && data[i] <= 57) {
                            dataStream << data[i];
                        } else if (data[i] != '\n' || data[i + 1] == '\0') {
                            dataStream >> val;
                            dataStream.clear();
                            values.push_back(val);
                            val = 0;
                        }
                    }

                    int tileCounter = 0;
                    //  Iterating over every tile
                    for (int i = 0; i < values.size(); i++) {
                        if (values[i] == 0) {
                            tileCounter++;
                            if (i == values.size() - 1)
                                break;
                            else
                                continue;
                        }

                        Tileset tls;
                        //  Getting the tileset for current tile
                        for (int j = 0; j < this->_tilesets.size(); j++) {
                            //  If the firstGid on a tileset (lowest gid of the tileset) is smaller than our own,
                            //  that tileset holds the tile needed to draw this tile
                            if (this->_tilesets[j].firstGid <= values[i]) {
                                tls = this->_tilesets[j];
                                break;
                            }
                        }

                        if (values[i] == -1) {
                            tileCounter++;
                            if (i == values.size() - 1)
                                break;
                            else
                                continue;
                        }

                        //  Getting tile position in level
                        int x = 0;
                        int y = 0;
                        x = (tileCounter % width) * tileWidth;
                        y = (tileCounter / width) * tileHeight;
                        Vector2i position = Vector2i(x, y);

                        //  Getting tile position in tileset
                        int tilesetWidth = 0, tilesetHeight = 0;
                        SDL_QueryTexture(tls.texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
                        int tx = values[i] % (tilesetWidth / tileWidth) - 1;
                        int ty = values[i] / (tilesetWidth / tileWidth);
                        Vector2i tilesetPosition = Vector2i(tx * tileWidth, ty * tileHeight);

                        //  Building tile and adding it to levels _tileList
                        this->_tileList.push_back(Tile(tls.texture, Vector2i(tileWidth, tileHeight), tilesetPosition, position));

                        tileCounter++;
                    }

                    pData = pData->NextSiblingElement("data");
                }
            }

            pLayer = pLayer->NextSiblingElement("layer");
        }
    }

    tinyxml2::XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
    if (pObjectGroup != NULL) {
        while (pObjectGroup) {
            const char* name = pObjectGroup->Attribute("name");
            std::stringstream groupStream;
            groupStream << name;

            if (groupStream.str() == "Collisions") {
                //  Parse collison rectangles
                tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        float x, y, width, height;
                        x = std::ceil(pObject->FloatAttribute("x")) * globals::SPRITE_SCALE;
                        y = std::ceil(pObject->FloatAttribute("y")) * globals::SPRITE_SCALE;
                        width = std::ceil(pObject->FloatAttribute("width")) * globals::SPRITE_SCALE;
                        height = std::ceil(pObject->FloatAttribute("height")) * globals::SPRITE_SCALE;
                        this->_collisionRects.push_back(Rectangle(x, y, width, height));
                        // SDL_Texture* debug_texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(globals::PROGRAM_PATH + "content/sprites/debug_tile.png")); 
                        // this->_tileList.push_back(Tile(debug_texture, Vector2i(width / globals::SPRITE_SCALE, height / globals::SPRITE_SCALE), Vector2i(0, 0), Vector2i(x / globals::SPRITE_SCALE, y / globals::SPRITE_SCALE)));

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            } else if (groupStream.str() == "Polygons") {
                //  Parse collision polygons
                tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while(pObject) {
                        std::vector<Vector2i> points;
                        Vector2i pos;
                        pos = Vector2i(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

                        tinyxml2::XMLElement* pPolygon = pObject->FirstChildElement("polygon");
                        if (pPolygon != NULL) {
                            std::vector<std::string> pairs;
                            const char* pointString = pPolygon->Attribute("points");
                            std::stringstream pointStream;
                            
                            pointStream << pointString;
                            Utils::split(pointStream.str(), pairs, ' ');

                            for (int i = 0; i < pairs.size(); i++) {
                                std::vector<std::string> coordinate;
                                Utils::split(pairs[i], coordinate, ',');
                                points.push_back(Vector2i(std::stoi(coordinate[0]), std::stoi(coordinate[1])));
                            }
                        }

                        for (int i = 0; i < points.size(); i += 2) {
                            int next = (i != points.size() - 1) ? (i + 1) : 0;
                            if (points[i].x != points[next].x &&
                                points[i].y != points[next].y) {
                                this->_collisionSlopes.push_back(Slope(
                                            Vector2i((pos.x + points[i].x) * globals::SPRITE_SCALE,
                                                (pos.y + points[i].y) * globals::SPRITE_SCALE),
                                            Vector2i((pos.x + points[next].x) * globals::SPRITE_SCALE,
                                                (pos.y + points[next].y) * globals::SPRITE_SCALE)
                                            ));
                            }
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            } else if (groupStream.str() == "SpawnPoints") {
                //  Parse spawn points
                tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        const char* spawnName = pObject->Attribute("name");
                        std::stringstream spawnStream;
                        spawnStream << spawnName;

                        float x, y;
                        x = std::ceil(pObject->FloatAttribute("x")) * globals::SPRITE_SCALE;
                        y = std::ceil(pObject->FloatAttribute("y")) * globals::SPRITE_SCALE;
                        if (spawnStream.str() == "Player") {
                            this->_spawnPoint = Vector2i(x, y);
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            
            pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
        }
    }
}

void Level::draw(Graphics& graphics) {
    for (int i = 0; i < this->_tileList.size(); i++) {
        this->_tileList[i].draw(graphics);
    }
}

void Level::update(float elapsedTime) {

}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle& collider) {
    std::vector<Rectangle> others;
    for (int i = 0; i < this->_collisionRects.size(); i++) {
        if (this->_collisionRects[i].collidesWith(collider)) {
            others.push_back(this->_collisionRects[i]);
        }
    }
    return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle& collider) {
    std::vector<Slope> others;
    for (int i = 0; i < this->_collisionSlopes.size(); i++) {
        if (this->_collisionSlopes[i].collidesWith(collider)) {
            others.push_back(this->_collisionSlopes[i]);
        }
    }
    return others;
}

const Vector2i Level::getSpawnPoint() const {
    return this->_spawnPoint;
}
