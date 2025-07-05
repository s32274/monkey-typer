#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include "WordObject.h"

auto main() -> int {
    auto window = sf::RenderWindow(
            sf::VideoMode(1280, 720), "Monke Typer", sf::Style::Default);

    window.setFramerateLimit(60);
    auto gameOver = false;

    // Minecraft.otf -> https://github.com/IdreesInc/Minecraft-Font/releases
    // Comic Sans MS.ttf -> https://github.com/antimatter15/doge/blob/master/Comic%20Sans%20MS.ttf
    // Courier New.ttf -> https://github.com/justrajdeep/fonts/blob/master/Courier%20New.ttf
    auto minecraftFont = sf::Font();
    if (!minecraftFont.loadFromFile("../MonkeTyper-assets/fonts/Minecraft.otf")) {
        fmt::println("{}", "couldn't load Minecraft font!");
        return -1;
    }
    auto comicSansFont = sf::Font();
    if (!comicSansFont.loadFromFile("../MonkeTyper-assets/fonts/Comic Sans MS.ttf")) {
        fmt::println("{}", "couldn't load Comic Sans font!");
        return -1;
    }
    auto courierNewFont = sf::Font();
    if (!courierNewFont.loadFromFile("../MonkeTyper-assets/fonts/Courier New.ttf")) {
        fmt::println("{}", "couldn't load Courier New font!");
        return -1;
    }
    //----------------------------------------------------------------------------------------------------------------//
    // Licznik czasu
    auto globalClock = sf::Clock();
    auto timePassed = int(0);
    auto timePassedWordObject = WordObject(
            std::string("Time passed: " + std::to_string(timePassed / 1000)
                    + "," + (std::to_string(timePassed % 1000))+ "s "),
            minecraftFont,
            sf::Vector2f(900.f, 650.f),
            0.0f
    );

    // Licznik punktow za wpisane slowa
    auto points = int(0);
    auto pointsString = sf::String("Points: " + std::to_string(points));
    auto pointCounterObject = WordObject(
            pointsString, minecraftFont, sf::Vector2f(50, 650), 0.0f
    );

    auto livesRemaining = int(42);
    auto livesRemainingString = sf::String("Lives: " + std::to_string(livesRemaining));
    auto livesRemainingObject = WordObject(
            livesRemainingString, comicSansFont, sf::Vector2f(250.f, 650.f), 0.0f
    );

    auto monkeSrcVec = std::vector<std::string>{"Malpo", "mlynarzu", "co", "robisz", "Monke"};

    auto rdDisplacementY = std::random_device();
    auto divergenceY = std::uniform_real_distribution<float>(10.0f, 600.f);
    auto rdMovementSpeed = std::random_device();
    auto speedDistribution = std::uniform_real_distribution<float>(0.2f, 0.95f);

    // Tutaj gracz wpisuje slowa
    auto userWordObject = WordObject(
            std::string("press ENTER to start typing"),
            courierNewFont,
            sf::Vector2f(450,645),
            0.0f
    );

    // Trafione znaki na minute, zeby jeszcze bardziej sie porownywac
    auto charactersPerMinute = float(0.0f);
    auto charactersPerMinuteString = sf::String(
            "Characters per minute: " + std::to_string(charactersPerMinute)
    );
    auto charactersPerMinuteObject = WordObject(
            charactersPerMinuteString,
            courierNewFont,
            sf::Vector2f(425, 680),
            0.0f
    );
    charactersPerMinuteObject.getWordText().setFillColor(sf::Color::White);

    //------------------Pliki tekstowe----------------------------------------------------------------------------------
    auto uniquePtrShrekWordObjects = std::vector<std::unique_ptr <WordObject>>();
    auto shrekFile = std::fstream("../MonkeTyper-assets/texts/Shrek.txt");
    auto word = std::string();
    auto wordsOfPoetry = std::vector<std::string>();

    while (shrekFile >> word) {
        wordsOfPoetry.push_back(word);
    }

    // Dla odliczania czasu przy dodawaniu slowek na ekran
    auto timeLastAdded = int(0);
    auto addingInterval = int(875);

    // Zmiana kolorkow :D
    // jesli slowko podleci zbyt blisko krawedzi, to na zolty, a jesli dwa razy blizej, to na czerwony
    auto yellowFontMargin = 500.f;
    auto changeWordObjectColor =
        [&window] (WordObject& wo, float yellowFontMargin) -> void {

        auto redFontMargin = yellowFontMargin / 2;
        if (window.getSize().x - (wo.getWordText().getGlobalBounds().getPosition().x
                                  + wo.getWordText().getGlobalBounds().getSize().x) < yellowFontMargin) {
            wo.getWordText().setFillColor(sf::Color::Yellow);

            if (window.getSize().x - (wo.getWordText().getGlobalBounds().getPosition().x
                                      + wo.getWordText().getGlobalBounds().getSize().x) < redFontMargin) {
                wo.getWordText().setFillColor(sf::Color::Red);
            }
        }
    };
    //--Glowna petla----------------------------------------------------------------------------------------------------
    while (window.isOpen()) {
        auto deltaTime = globalClock.restart().asMilliseconds();

        if (!gameOver) {
            timePassed += deltaTime;
        }

        // Aktualizuje napis z czasem, ktory juz minal
        auto sec = timePassed / 1000;   // bo milisekundy
        auto ms = timePassed % 1000;
        auto timeString = std::string("Time passed: " + std::to_string(sec) + "," + std::to_string(ms) + "s");
        timePassedWordObject.getWordText().setString(timeString);

        // Rowniez ilosc dobrze trafionych znakow na minute
        charactersPerMinute = static_cast<float>(points / (0.001f * timePassed / 60.f));
        charactersPerMinuteString = sf::String(
                "Characters per minute: " + std::to_string(charactersPerMinute)
        );
        charactersPerMinuteObject.getWordText().setString(charactersPerMinuteString);


        auto event = sf::Event();
        while (window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::TextEntered: {
                    if (event.text.unicode < 128 && event.text.unicode != 13) {
                        // TEN DRUGI WARUNEK URATOWAL MOJE ZDROWIE PSYCHICZNE oraz potencjalnie ITN
                        // Uzywalem Entera jako zatwierdzenie wpisywanego slowa i porownanie go do tych na ekranie
                        // Przez ten cholerny enter, ktory wpisywal sie jako slowo (unicode 13),
                        // program dzialal tylko raz
                        // a pozniej dawal jakies chore rezultaty
                        // Musialem nad tym siedziec kilka godzin
                        // literalnie kilkadziesiat procent czasu spedzonego nad tym projektem
                        // prawdziwie C++ moment. Nie no jk
                        // japiernicze dziala
                        // co za szczescie!

                        // Przechwytuje wcisniecie Backspace, bo on tez sie wstawia (ma kod ASCII 8)
                        if (event.text.unicode == 8) {
                            auto currentString = userWordObject.getWordText().getString();
                            if (userWordObject.getWordText().getString().getSize() > 0) {
                                userWordObject.getWordText().setString(
                                        userWordObject.getWordText().getString()
                                                .substring(
                                                        0,
                                                        userWordObject.getWordText().getString().getSize() - 1));
                            }
                        } else {    // Jesli to nie Backspace ANI ENTER AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA:
                            // dodaje wpisywane literki do userWordObject na dole ekranu
                            userWordObject.getWordText().setString(
                                    userWordObject.getWordText().getString() + static_cast<char>(event.text.unicode)
                            );
                        }
                    }
                    break;
                }

                case sf::Event::KeyPressed: {
                    // PO WCISNIECIU ENTERA:
                    if (event.key.code == sf::Keyboard::Enter) {

                        if (!gameOver) {
                            // Szuka pasujacych slow i usuwa je, jesli gracz wpisze identyczne i zatwierdzi Enterem
                            for (auto iterator = uniquePtrShrekWordObjects.begin();
                                 iterator < uniquePtrShrekWordObjects.end();) {
                                if (userWordObject == *(*iterator)) {
                                    points += (*iterator)->getWordText().getString().getSize();
                                    iterator = uniquePtrShrekWordObjects.erase(iterator);


                                    break;
                                } else {
                                    ++iterator;
                                }
                            }
                        }
                        pointsString = std::string("Points: " + std::to_string(points));
                        pointCounterObject.getWordText().setString(pointsString);

                        userWordObject.getWordText().setString(std::string(""));
                    }
                }
            }
        }
        //--------------------------------Koniec petli eventow----------------------------------------------------------

        // Stopniowo w czasie dodaje slowka na do wektora unique pointerow do WordObject'ow
        auto currentTime = timePassed;
        if (currentTime - timeLastAdded >= addingInterval && wordsOfPoetry.size() > 0) {
            auto randomY = divergenceY(rdDisplacementY);
            auto randomY2f = sf::Vector2f(-21.0f, randomY);
            auto spdIncrease = speedDistribution(rdMovementSpeed);

            // All Star od konca :DD
            auto nextWord = wordsOfPoetry.back();
            wordsOfPoetry.pop_back();
            uniquePtrShrekWordObjects.push_back(
                    std::make_unique<WordObject>(WordObject(nextWord, minecraftFont,
                                                            randomY2f, spdIncrease))
            );
            timeLastAdded = currentTime;
        }

        // Przesuwa slowka w prawo
        if (!gameOver) {
            for (auto iterator = uniquePtrShrekWordObjects.begin();
                 iterator < uniquePtrShrekWordObjects.end();) {
                (*iterator)->getWordText().move((*iterator)->getMovementSpd(), 0.f);
                changeWordObjectColor(*(*iterator), yellowFontMargin);

                // Odejmie zycia, jesli slowko cale wyleci poza ekran i UNICESTWI to slowko
                if ((*iterator)->getWordText().getGlobalBounds().left > window.getSize().x) {
                    livesRemaining -= (*iterator)->getWordText().getString().getSize();

                    livesRemainingString = std::string("Lives: " + std::to_string(livesRemaining));
                    livesRemainingObject.getWordText().setString(livesRemainingString);

                    if (livesRemaining <= 0) {
                        gameOver = true;
                        fmt::println("GAME OVER\nPOINTS: {}\n{}", points, timeString);
                        break;
                    }

                    iterator = uniquePtrShrekWordObjects.erase(iterator);
                } else {
                    ++iterator;
                }
            }
        }

        window.clear(sf::Color::Black);
            for (auto &uniquePointer: uniquePtrShrekWordObjects) {
                window.draw(uniquePointer->getWordText());
            }
        window.draw(pointCounterObject.getWordText());
        window.draw(timePassedWordObject.getWordText());
        window.draw(livesRemainingObject.getWordText());
        window.draw(charactersPerMinuteObject.getWordText());
        window.draw(userWordObject.getWordText());
        window.display();
    }
}