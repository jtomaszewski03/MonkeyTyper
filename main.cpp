#include "monkeyutils.h"

auto main() -> int {
    const float rangeX = 1024;
    const float rangeY = 768;
    int defaultCharSize = 35;
    auto window = sf::RenderWindow(
            sf::VideoMode({1024, 768}), "Monkey Typer",
            sf::Style::Default, sf::ContextSettings(0, 0, 8)
    );
    sf::Font font;
    font.loadFromFile("../superbrain.ttf");

    std::vector<std::string> words;

    //main game
    srand(time(nullptr));
    initiateWords(words);
    auto currentInput = sf::Text("Type them to disappear!", font, defaultCharSize + 15);
    std::vector<sf::Text> texts;
    initiateTexts(texts, words, font, rangeY, defaultCharSize);
    std::vector<sf::CircleShape> livesvec;

    int userscore = 0;
    auto score = sf::Text("Score " + std::to_string(userscore), font, defaultCharSize);
    score.setFillColor(sf::Color::Cyan);
    score.setPosition(rangeX - 300, 0);

    auto velocity = sf::Vector2f(0.005, 0);

    currentInput.setPosition(0, rangeY - 60);
    currentInput.setFillColor(sf::Color::Red);
    std::string userinp;

    //end of the game
    auto wordsUnwritten = 0;
    bool finish = false;
    sf::Text finalScore("Score " + std::to_string(userscore), font, defaultCharSize * 2);
    finalScore.setPosition(rangeX / 2 - 200, 300);
    finalScore.setOutlineThickness(2);

    //startscreen
    bool startScreen = true;
    auto monkeyFont = sf::Font();
    monkeyFont.loadFromFile("../atop.ttf");
    sf::Text startScreenText("Monkey\nTyper", monkeyFont, 120);
    sf::Text authorText("by Jakub Tomaszewski", monkeyFont, 20);
    sf::Text shortcutText("Ctrl +    Ctrl -\nto manage font size", font, static_cast<int>(defaultCharSize * 0.6));
    startScreenText.setPosition(rangeX / 2 - 100, rangeY / 2 - 150);
    startScreenText.setOutlineThickness(3);
    authorText.setPosition(rangeX - 250, rangeY - 25);
    authorText.setOutlineThickness(2);
    shortcutText.setPosition(10, 0);
    shortcutText.setOutlineThickness(2);

    //pause
    bool pause = false;
    sf::Text pauseText;
    createButton(pauseText, rangeX / 2 - 250, rangeY / 2 - 100, "Pause", monkeyFont, 150);

    //startButton
    sf::Text startButtonText;
    createButton(startButtonText, 10, rangeY / 2, "Start", font, defaultCharSize + 10);

    //levelButton
    auto level = 0;
    sf::Text levelButtonText;
    createButton(levelButtonText, 10, 100, "Beginner", font, defaultCharSize + 10);

    //fontButton
    auto fontPicked = 0;
    sf::Text fontButtonText;
    createButton(fontButtonText, 10, rangeY - 100, "Super Brain", font, defaultCharSize + 10);

    //load previous game
    sf::Text previousButtonText;
    createButton(previousButtonText, rangeX - 500, 10, "Load previous game", font,
                 defaultCharSize - 10);
    previousButtonText.setOutlineThickness(2);
    sf::Text saveButtonText;
    createButton(saveButtonText, rangeX / 2 - 170, rangeY / 2 + 100, "Save and exit", font,
                 defaultCharSize);
    saveButtonText.setFillColor(sf::Color::Green);

    //highscores
    std::set<int, std::greater<int>> highscores;
    readScores(highscores);
    std::vector<sf::Text> highScoreTexts;


    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (!pause) pause = true;
                else pause = false;

                if (finish) window.close();
            }
            if (event.key.code == sf::Keyboard::BackSpace && !userinp.empty()) {
                userinp.pop_back();
                currentInput.setString(userinp);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
                if (defaultCharSize < 45) {
                    defaultCharSize += 5;
                    updateSize(texts, defaultCharSize, score, currentInput);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen)) {
                if (defaultCharSize > 30) {
                    defaultCharSize -= 5;
                    updateSize(texts, defaultCharSize, score, currentInput);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    if (startScreen) {
                        if (startButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                       static_cast<float>(mousePos.y))) {
                            startScreen = false;
                            window.sf::Window::setMouseCursorVisible(false);
                        }
                        if (levelButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                       static_cast<float>(mousePos.y))) {
                            pickLevel(level, levelButtonText, velocity);
                        }
                        if (fontButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                      static_cast<float>(mousePos.y))) {
                            pickFont(fontPicked, fontButtonText, font, defaultCharSize);
                            levelButtonText.setCharacterSize(defaultCharSize + 10);
                            startButtonText.setCharacterSize(defaultCharSize + 10);
                            fontButtonText.setCharacterSize(defaultCharSize + 10);
                            updateSize(texts, defaultCharSize, score, currentInput);
                        }
                        if (previousButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                          static_cast<float>(mousePos.y))) {
                            getSavedScore(userscore, level, texts, wordsUnwritten);
                            pickLevel(level, levelButtonText, velocity);
                            score.setString("Score " + std::to_string(userscore));
                            startScreen = false;
                            window.sf::Window::setMouseCursorVisible(false);

                        }
                    } else if (pause) {
                        if (saveButtonText.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                      static_cast<float>(mousePos.y))) {
                            saveGame(userscore, level, texts, wordsUnwritten);
                            window.close();
                        }
                    }
                }
            }
            // checking user input
            if (event.type == sf::Event::TextEntered && !pause) {
                if (event.text.unicode < 123 && event.text.unicode > 96 && !startScreen) {
                    userinp += static_cast<char> (event.text.unicode);
                    for (sf::Text &checkedtext: texts) {
                        if (userinp == checkedtext) {
                            resetFrontText(texts, userinp, words);
                            userinp.erase();
                            currentInput.setString("");
                            userscore++;
                            score.setString("Score " + std::to_string(userscore));
                        } else currentInput.setString(userinp);
                    }
                }
            }
        }

        //words hitting rangeX
        if (!finish)
            for (sf::Text &texthitting: texts) {
                if (texthitting.getPosition().x <= rangeX) continue;
                else {
                    if (++wordsUnwritten > 5) {
                        finish = true;
                        finalScore.setString("Score " + std::to_string(userscore));
                        highscores.insert(userscore);
                        saveScores(highscores);
                        initiateScoreTexts(highScoreTexts, highscores, font);
                        userinp.erase();
                        currentInput.setString("Type r to restart");
                        break;
                    } else {
                        texthitting.setPosition(-200, texthitting.getPosition().y);

                        texthitting.setString(words.at(rand() % words.size()));
                    }
                }
            }

        //restart
        if (userinp == "r" && finish) {
            userinp = "";
            currentInput.setString(userinp);
            finish = false;
            wordsUnwritten = 0;
            userscore = 0;
            restartTexts(texts, words);
            score.setString("Score " + std::to_string(userscore));
        }

        window.clear(sf::Color(105, 105, 105));
        if (startScreen) {
            startButtonText.setFillColor(sf::Color::Red);
            window.draw(shortcutText);
            window.draw(authorText);
            window.draw(startScreenText);
            window.draw(startButtonText);
            window.draw(levelButtonText);
            window.draw(fontButtonText);
            window.draw(previousButtonText);
        } else {
            if (!finish) {
                window.draw(score);
                updateLives(6 - wordsUnwritten, livesvec);
                for (sf::CircleShape &live: livesvec) window.draw(live);
            }
            window.draw(currentInput);
        }
        if (!finish && !startScreen && !pause) {
            window.setMouseCursorVisible(false);
            for (sf::Text &texttodraw: texts) {
                colorTexts(texts);
                window.draw(texttodraw);
            }
            for (sf::Text &texttomove: texts) texttomove.move(velocity);
        }
        if (pause && !startScreen && !finish) {
            window.setMouseCursorVisible(true);
            window.draw(pauseText);
            window.draw(saveButtonText);
        }
        if (finish) {
            window.draw(finalScore);
            for (sf::Text &scoreText: highScoreTexts) window.draw(scoreText);
        }
        window.display();
    }
}