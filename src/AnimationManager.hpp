#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class AnimationManager {

private:

  static map<string, Texture> m_textures;

  static map<string, Vector2i> m_indicies;


  static map<string, Vector2i> m_startingIndicies;
  static map<string, Vector2i> m_endingIndicies;


  static map<string, Vector2i> m_sheetSizes;
  static map<string, Vector2i> m_spriteSizes;


  static map<string, int> m_frequencies;

  static map<string, int> m_timesUpdated;
public:

  static void update(string animation, Sprite& sprite);


  static void updateAll(map<string, Sprite>& map);


  static void addAnimation(string animation, Texture texture, Vector2i sheetSize,
     Vector2i spriteSize, Vector2i index = Vector2i(0, 0), int frequency = 0,
    Vector2i startingIndex = Vector2i(0, 0));

  static void deleteAnimation(string animation);

  static void setAnimationFrequency(string animation, int frequency);
  static void setAnimationSpriteSize(string animation, Vector2i size);
  static void setAnimationSheetSize(string animation, Vector2i size);
  static void setAnimationIndex(string animation, Vector2i index);
  static void setAnimationTexture(string animation, Texture texture);
  static void setAnimationStartingIndex(string animation, Vector2i index);
  static void setAnimationEndingIndex(string animation, Vector2i index);


  static void resetAnimationIndex(string animation);
};