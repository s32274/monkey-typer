#include "WordObject.h"

WordObject::WordObject(const sf::Text& wordText, const sf::Font& wordFont,
                       const std::string& stdString, int size, const sf::Vector2f& pos,
                       const sf::Vector2f& scale, float movementSpd)
                : wordText(wordText), wordFont(wordFont), movementSpd(movementSpd)
    {
        this->wordText.setString(stdString);
        this->wordText.setFillColor(sf::Color::Green);
        this->wordText.setCharacterSize(size);
        this->wordText.setFont(this->wordFont);
        this->wordText.setPosition(pos);
        this->wordText.setScale(scale);
    }

WordObject::WordObject(const std::string& stdString, const sf::Font& font, const sf::Vector2f& pos, float movementSpd)
    : WordObject(sf::Text(), font, stdString,
      24, pos, sf::Vector2f(1.0f, 1.0f), movementSpd)
{
    this->wordText.setFont(font);
}
WordObject::WordObject(const sf::String& stdString, const sf::Font& font, const sf::Vector2f& pos, float movementSpd)
        : WordObject(sf::Text(), font, stdString,
                     24, pos, sf::Vector2f(1.0f, 1.0f), movementSpd)
{
    this->wordText.setFont(font);
}
WordObject::WordObject(const std::string& string, const sf::Font& font)
    : WordObject(string, font, sf::Vector2f(21.f, 37.f), 0.5f)
{

}


auto WordObject::operator==(WordObject& other) -> bool {
    return this->wordText.getString() == other.getWordText().getString();
}

auto WordObject::getWordText() -> sf::Text& {
    return wordText;
}
auto WordObject::setWordText(const sf::Text& wordText) -> void {
    this->wordText = wordText;
}

auto WordObject::getWordFont() -> sf::Font& {
    return wordFont;
}

auto WordObject::setWordFont(sf::Font& wordFont) -> void {
    this->wordFont = wordFont;
    this->wordText.setFont(wordFont);
}

auto WordObject::getMovementSpd() const -> float {
    return movementSpd;
}

auto WordObject::setMovementSpd(float movementSpd) -> void {
    this->movementSpd = movementSpd;
}