#include <SFML/Graphics.hpp>
#include "globals.hpp"
// tu wszystkie funkcje i klasy
static void updateViewViewport(const sf::RenderWindow& window, sf::View& view) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float targetW = mainWin.x;
    float targetH = mainWin.y;

    if (winW <= 0.f || winH <= 0.f) return;

    float scale = std::min(winW / targetW, winH / targetH);
    float viewportW = (targetW * scale) / winW;
    float viewportH = (targetH * scale) / winH;
    float viewportLeft = (1.f - viewportW) / 2.f;
    float viewportTop = (1.f - viewportH) / 2.f;

    view.setViewport(sf::FloatRect(sf::Vector2(viewportLeft, viewportTop), sf::Vector2(viewportW, viewportH)));
}

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const std::string& textString, const sf::Font& font, unsigned int charSize)
        : shape(size), text(font, textString, charSize) {
        shape.setPosition(position);
        shape.setFillColor(color);
        text.setPosition(sf::Vector2f(position.x + size.x / 2.0f - text.getLocalBounds().size.x / 2.0f, position.y + size.y / 2.0f - text.getLocalBounds().size.y / 2.0f - 5.0f));
    }
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    bool isMouseOver(const sf::Vector2f& mousePos) {
        return shape.getGlobalBounds().contains(mousePos);
    }
    void setFillColor(const sf::Color& color) {
        shape.setFillColor(color);
    }
private:
    sf::RectangleShape shape;
    sf::Text text;
};