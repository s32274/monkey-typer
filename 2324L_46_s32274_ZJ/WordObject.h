#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>

class WordObject {

private:
    sf::Text wordText;
    sf::Font wordFont;
    float movementSpd;

public:
    WordObject(const sf::Text& wordText, const sf::Font& wordFont, const std::string& stdString,
               int size, const sf::Vector2f& pos, const sf::Vector2f& scale, float movementSpd
    );
    WordObject(const std::string& stdString, const sf::Font& font, const sf::Vector2f& pos, float movementSpd);
    WordObject(const sf::String& stdString, const sf::Font& font, const sf::Vector2f& pos, float movementSpd);
    WordObject(const std::string& string, const sf::Font& font);

    auto operator==(WordObject& other) -> bool;

    auto getWordText() -> sf::Text&;
    auto setWordText(const sf::Text& wordText) -> void;

    auto getWordFont() -> sf::Font&;
    auto setWordFont(sf::Font& wordFont) -> void;

    auto getMovementSpd() const -> float;
    auto setMovementSpd(float movementSpd) -> void;
};