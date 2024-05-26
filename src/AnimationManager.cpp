#include "AnimationManager.hpp"
using namespace sf;

map<string, Texture> AnimationManager::m_textures;
map<string, Vector2i> AnimationManager::m_indicies;
map<string, Vector2i> AnimationManager::m_startingIndicies;
map<string, Vector2i> AnimationManager::m_endingIndicies;
map<string, Vector2i> AnimationManager::m_sheetSizes;
map<string, Vector2i> AnimationManager::m_spriteSizes;
map<string, int> AnimationManager::m_frequencies;
map<string, int> AnimationManager::m_timesUpdated;



void AnimationManager::update(string animation, Sprite &sprite) {

	if (m_sheetSizes[animation] != Vector2i(0, 0)) {

		IntRect rect(m_indicies[animation].x * m_spriteSizes[animation].x,
			 m_indicies[animation].y * m_spriteSizes[animation].y,
				m_spriteSizes[animation].x, m_spriteSizes[animation].y);


		if (m_indicies[animation].y < m_sheetSizes[animation].y) {
			m_indicies[animation].y++;
		} else {

			m_indicies[animation].y = 0;
			m_indicies[animation].x++;

			if (m_indicies[animation].x >= m_sheetSizes[animation].x)
				m_indicies[animation].x = 0;
		}


		sprite.setTexture(m_textures[animation]);

		sprite.setTextureRect(rect);

	} else {

		cout << "No animation entry found for \"" << animation << "\"!" << endl;
	}
}


void AnimationManager::updateAll(map<string, Sprite> &map) {

	for (auto element: map) {
		update(element.first, element.second);
	}
}


void AnimationManager::addAnimation(string animation, Texture texture,
	 Vector2i sheetSize, Vector2i spriteSize, Vector2i index, int frequency,
	 Vector2i startingIndex) {
	
	m_textures[animation] = texture;
	
	m_sheetSizes[animation].x = sheetSize.x;
	m_sheetSizes[animation].y = sheetSize.y;

	
	m_spriteSizes[animation].x = spriteSize.x;
	m_spriteSizes[animation].y = spriteSize.y;

	
	m_indicies[animation].x = index.x;
	m_indicies[animation].y = index.y;

	
	m_startingIndicies[animation].x = startingIndex.x;
	m_startingIndicies[animation].y = startingIndex.y;

	
	m_endingIndicies[animation].x = sheetSize.x;
	m_endingIndicies[animation].y = sheetSize.y;

	
	m_frequencies[animation] = frequency;
 
}

void AnimationManager::deleteAnimation(string animation) {
	
	m_textures.erase(animation);
	m_indicies.erase(animation);
	m_startingIndicies.erase(animation);
	m_sheetSizes.erase(animation);
	m_spriteSizes.erase(animation);
	m_frequencies.erase(animation);
	m_timesUpdated.erase(animation);
	m_endingIndicies.erase(animation);
	
}


void AnimationManager::setAnimationFrequency(string animation, int frequency) {
	m_frequencies[animation] = frequency;
}

void AnimationManager::setAnimationIndex(string animation, Vector2i index) {
	m_indicies[animation].x = index.x;
	m_indicies[animation].y = index.y;
}

void AnimationManager::setAnimationSheetSize(string animation, Vector2i size) {
	m_sheetSizes[animation].x = size.x;
	m_sheetSizes[animation].y = size.y;
}

void AnimationManager::setAnimationSpriteSize(string animation, Vector2i size) {
	m_spriteSizes[animation].x = size.x;
	m_spriteSizes[animation].y = size.y;
}

void AnimationManager::setAnimationTexture(string animation, Texture texture) {
	m_textures[animation] = texture;
}

void AnimationManager::resetAnimationIndex(string animation) {
	m_indicies[animation].x = m_startingIndicies[animation].x;
	m_indicies[animation].y = m_startingIndicies[animation].y;
}

void AnimationManager::setAnimationStartingIndex(string animation, Vector2i index) {
	m_startingIndicies[animation].x = index.x;
	m_startingIndicies[animation].y = index.y;
}

void AnimationManager::setAnimationEndingIndex(string animation, Vector2i index) {
	m_endingIndicies[animation].x = index.x;
	m_endingIndicies[animation].y = index.y;
}