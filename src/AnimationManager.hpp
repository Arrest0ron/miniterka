#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

using namespace std;

class AnimationManager {

private:

  static map<string, sf::Texture> m_textures;

  static map<string, sf::Vector2i> m_indicies;


  static map<string, sf::Vector2i> m_startingIndicies;
  static map<string, sf::Vector2i> m_endingIndicies;


  static map<string, sf::Vector2i> m_sheetSizes;
  static map<string, sf::Vector2i> m_spriteSizes;


  static map<string, int> m_frequencies;

  static map<string, int> m_timesUpdated;
public:

  static void update(string animation, sf::Sprite& sprite);


  static void updateAll(map<string, sf::Sprite>& map);


  static void addAnimation(string animation, sf::Texture texture, sf::Vector2i sheetSize,
    sf::Vector2i spriteSize, sf::Vector2i index = sf::Vector2i(0, 0), int frequency = 0,
    sf::Vector2i startingIndex = sf::Vector2i(0, 0));

  static void deleteAnimation(string animation);

  static void setAnimationFrequency(string animation, int frequency);
  static void setAnimationSpriteSize(string animation, sf::Vector2i size);
  static void setAnimationSheetSize(string animation, sf::Vector2i size);
  static void setAnimationIndex(string animation, sf::Vector2i index);
  static void setAnimationTexture(string animation, sf::Texture texture);
  static void setAnimationStartingIndex(string animation, sf::Vector2i index);
  static void setAnimationEndingIndex(string animation, sf::Vector2i index);


  static void resetAnimationIndex(string animation);
};