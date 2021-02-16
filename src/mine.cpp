#include "../headers/mine.hpp"
#include "../headers/utils/utils.hpp"
#include "../headers/utils/font_loader.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

//////////////

bool Mine::discovered() const
{
    return state == State::Discovered;
}
bool Mine::flagged() const
{
    return state == State::Flagged;
}
void Mine::reset()
{
    this->bomb = false;
    this->state = State::Default;
    this->neighbors = 0;
}

//////////////

void Mine::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto [top, bottom, size] = Utils::getRectangle(states.transform);

    /**
     * DRAW RECTANGLE
     */

    sf::RectangleShape field{};
    field.setPosition(top);
    field.setSize(size);
    field.setFillColor(sf::Color{155, 155, 155});

    field.setOutlineColor(
        discovered() || flagged() ? 
            sf::Color{200, 200, 200} : 
            sf::Color::Black
    );
    field.setOutlineThickness(
        discovered() ? 
            2 : 
            1
    );

    target.draw(field);

    /**
     * DRAW TEXT ON THE RECTANGLE
     */

    auto fontLoad = FontLoader::load("resources/source-code.ttf");

    if (fontLoad.has_value()) {

        auto [text, color] = getInfo();

        top.y -= size.y * 0.2;

        if (!text.empty()) {
            target.draw(Utils::getText(
                text,
                *fontLoad,
                size.y,
                color,
                top));
        }
    }

}

inline std::pair<std::string, sf::Color> Mine::getInfo() const
{
    std::string text;
        sf::Color color { sf::Color::White };

        if (discovered()) {

            if (bomb) {
                text = "B";
                color = sf::Color::Red;
            }
            else if (neighbors > 0) {
                text = std::to_string(neighbors);
            }
            
        } else if (flagged()) {
            text = "F";
            color = sf::Color::Blue;
        }

        return { text, color };
}

//////////////