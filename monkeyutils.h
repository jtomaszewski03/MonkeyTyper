#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <cstdlib>

bool operator==(const std::string &left, const sf::Text &right);

void initiateWords(std::vector<std::string> &words);

void initiateTexts(std::vector<sf::Text> &texts, std::vector<std::string> const &words,
                   sf::Font const &font, float const &rangeY, int const &charSize);

void getSavedScore(int &score, int &level, std::vector<sf::Text> &texts, int &lives);

void saveGame(int const &score, int const& level, std::vector<sf::Text> const &texts, int const &lives);

void restartTexts(std::vector<sf::Text> &texts, std::vector<std::string> const &words);

void updateLives(int const& lives, std::vector<sf::CircleShape> &livesvec);

void createButton(sf::Text &buttonText, float const &posX, float const &posY,
                  sf::String const &text, sf::Font const &font, int const &size);

void pickLevel(int &level, sf::Text &levelButtonText, sf::Vector2f &velocity);

void pickFont(int &currentFont, sf::Text &fontButtonText, sf::Font &font, int &defeaultSize);

void resetFrontText(std::vector<sf::Text> &texts, std::string const &userinp, std::vector<std::string> const &words);

void updateSize(std::vector<sf::Text> &texts, int const &charSize, sf::Text &score, sf::Text &input);

void colorTexts(std::vector<sf::Text> &texts);

void readScores(std::set<int, std::greater<int>> &highscores);

void saveScores(std::set<int, std::greater<int>> const &highscores);

void initiateScoreTexts(std::vector<sf::Text> &texts, std::set<int, std::greater<int>> const &values, sf::Font const &font);
