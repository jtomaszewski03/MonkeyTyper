#include "monkeyutils.h"
#include <fstream>


bool operator==(const std::string &left, const sf::Text &right) {
    return left == right.getString();
}

auto initiateWords(std::vector<std::string> &words) -> void {
    auto file = std::fstream("../words.txt");
    for (std::string word; file >> word; words.push_back(word));
}

auto getSavedScore(int &score, int &level, std::vector<sf::Text> &texts, int &lives) -> void {
    auto file = std::fstream("../previous.txt");
    file >> score;
    file >> level;
    file >> lives;
    auto pos = int();
    for (sf::Text &text: texts) {
        file >> pos;
        text.setPosition(static_cast<float>(pos), text.getPosition().y);
    }
}

auto saveGame(int const &score, int const &level, std::vector<sf::Text> const &texts, int const &lives) -> void {
    auto file = std::fstream("../previous.txt", std::ios::out | std::ios::trunc);
    file << score << "\n";
    file << level - 1 << "\n";
    file << lives << "\n";
    for (sf::Text const &text: texts) {
        file << static_cast<int>(text.getPosition().x) << "\n";
    }
}

auto initiateTexts(std::vector<sf::Text> &texts, std::vector<std::string> const &words,
                   sf::Font const &font, float const &rangeY, int const &charSize) -> void {

    float freeY = 50;
    srand(time(nullptr));
    for (int i = 0; freeY < rangeY - 100; i += 2) {
        auto random = rand() % 1000 - 1100;
        texts.push_back(sf::Text(words.at(rand() % words.size()), font, charSize));
        texts.push_back(sf::Text(words.at(rand() % words.size()), font, charSize));
        texts.at(i).setPosition(static_cast<float>(random), freeY);
        texts.at(i + 1).setPosition(static_cast<float>(random - 700), freeY);
        freeY += 100;
    }
}

auto restartTexts(std::vector<sf::Text> &texts, std::vector<std::string> const &words) -> void {
    srand(time(nullptr));
    for (int i = 0; i < texts.size(); i += 2) {
        auto random = rand() % 1000 - 1100;
        texts.at(i).setPosition(static_cast<float>(random), texts.at(i).getPosition().y);
        texts.at(i).setString(words.at(rand() % words.size()));
        texts.at(i + 1).setPosition(static_cast<float>(random - 700), texts.at(i + 1).getPosition().y);
        texts.at(i + 1).setString(words.at(rand() % words.size()));
    }
}

auto eliminateCollision(sf::Text &text1, sf::Text &text2) -> void {
    if (text1.getGlobalBounds().intersects(text2.getGlobalBounds()))
        text1.setPosition(text1.getPosition().x - 550, text1.getPosition().y);
}

auto updateLives(int const &lives, std::vector<sf::CircleShape> &livesvec) -> void {
    if (lives == livesvec.size()) return;
    if (lives > livesvec.size())
        for (int i = livesvec.size(); i < lives; ++i) {
            livesvec.push_back(sf::CircleShape(20));
            if (i > 0)
                livesvec.at(i).setPosition(livesvec.at(i - 1).getPosition().x + 45, 10);
            else
                livesvec.at(i).setPosition(30, 10);
            livesvec.at(i).setFillColor(sf::Color::Red);
            livesvec.at(i).setOutlineThickness(2);
            livesvec.at(i).setOutlineColor(sf::Color::Black);

        }
    else
        for (int i = livesvec.size(); i > lives; i--) {
            livesvec.pop_back();
        }
}


auto createButton(sf::Text &buttonText, float const &posX, float const &posY,
                  sf::String const &text,
                  sf::Font const &font, int const &size) -> void {

    buttonText = sf::Text(text, font, size);
    buttonText.setPosition(posX + 10, posY);
    buttonText.setOutlineThickness(6);
}

auto pickLevel(int &level, sf::Text &levelButtonText, sf::Vector2f &velocity) -> void {
    if (++level > 3) level = 0;
    switch (level) {
        case 0 :
            velocity = sf::Vector2f(0.008, 0);
            levelButtonText.setString("Beginner");
            break;
        case 1 :
            velocity = sf::Vector2f(0.01, 0);
            levelButtonText.setString("Intermediate");
            break;
        case 2 :
            velocity = sf::Vector2f(0.012, 0);
            levelButtonText.setString("Advanced");
            break;
        case 3 :
            velocity = sf::Vector2f(0.02, 0);
            levelButtonText.setString("Pro");
            break;
    }
}

auto pickFont(int &currentFont, sf::Text &fontButtonText, sf::Font &font, int &defaultSize) -> void {
    if (++currentFont > 3) currentFont = 0;
    switch (currentFont) {
        case 0 :
            font.loadFromFile("../superbrain.ttf");
            fontButtonText.setString("Super Brain");
            defaultSize = 35;
            break;
        case 1 :
            font.loadFromFile("../atop.ttf");
            fontButtonText.setString("Atop");
            defaultSize = 40;
            break;
        case 2 :
            font.loadFromFile("../pixel.ttf");
            fontButtonText.setString("Pixel");
            defaultSize = 40;
            break;
        case 3 :
            font.loadFromFile("../swansea.ttf");
            fontButtonText.setString("Swansea");
            defaultSize = 40;
            break;
    }
}

auto resetFrontText(std::vector<sf::Text> &texts, std::string const &userinp,
                    std::vector<std::string> const &words) -> void {
    srand(time(nullptr));
    auto max = 0;
    auto maxId = int();
    for (int i = 0; i < texts.size(); i++) {
        if (userinp == texts.at(i))
            if (texts.at(i).getPosition().x > static_cast<float>(max)) {
                max = static_cast<int>(texts.at(i).getPosition().x);
                maxId = i;
            }
    }
    texts.at(maxId).setPosition(-200, texts.at(maxId).getPosition().y);
    texts.at(maxId).setString(words.at(rand() % words.size()));
    if (maxId % 2 == 0)
        eliminateCollision(texts.at(maxId), texts.at(maxId + 1));
    else
        eliminateCollision(texts.at(maxId), texts.at(maxId - 1));

}

auto updateSize(std::vector<sf::Text> &texts, int const &charSize, sf::Text &score, sf::Text &input) -> void {
    for (sf::Text &text: texts) {
        text.setCharacterSize(charSize);
    }
    score.setCharacterSize(charSize);
    input.setCharacterSize(charSize + 10);
}

auto colorTexts(std::vector<sf::Text> &texts) -> void {
    for (sf::Text &text: texts) {
        int x = static_cast<int>(text.getPosition().x);
        if (x > 600 && x < 750) text.setFillColor(sf::Color(255, 160, 0));
        else if (x > 750) text.setFillColor(sf::Color::Red);
        else text.setFillColor(sf::Color::Green);
    }
}

auto readScores(std::set<int, std::greater<int>> &highscores) -> void {
    auto file = std::fstream("../highscores.txt");
    for (int score; file >> score; highscores.insert(score));
}

auto saveScores(std::set<int, std::greater<int>> const &highscores) -> void {
    auto file = std::fstream("../highscores.txt", std::ios::out | std::ios::trunc);
    for (int x: highscores) file << x << "\n";
}

auto initiateScoreTexts(std::vector<sf::Text> &texts, std::set<int, std::greater<int>> const &values,
                        sf::Font const &font) -> void {
    if (texts.empty()) texts.clear();
    int freeY = 10;
    int index = 0;
    for (int const &x: values) {
        if (index < 6) {
            if (index == 0) {
                texts.push_back(sf::Text("Highscores", font, 25));
            } else
                texts.push_back(sf::Text(std::to_string(index) + ". " + std::to_string(x), font, 25));
            texts.at(index).setPosition(10, static_cast<float>(freeY));
            texts.at(index).setFillColor(sf::Color::White);
            texts.at(index).setOutlineThickness(2);
            freeY += 30;
            index++;
        }
    }
}





