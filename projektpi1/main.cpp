#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include "globals.hpp"
#include "helper.hpp"
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <random>
#include <fstream>

sf::Vector2f mainWin = { 800.0f, 600.0f };
void updateViewViewport(const sf::RenderWindow&, sf::View&);
enum class GameState { Menu, Game, GameMenu };

struct GameStart {
    sf::Vector2f velocity = { 0.f, 0.f };
    float up = 0.0f;
    float left = 0.0f;
    bool isCharging = false;
    bool isFlying = false;
    float maxCharge = 800.0f;
    float bot_ball_x = 100.f;
    float ball_x = 700.f;
    float ball_y = 300.f;
    bool turn = false;
    bool hasHit = false;
    sf::Vector2f initialVelocity;
};

void logic(GameState &currentState, GameStart &game, sf::CircleShape &ball, sf::CircleShape &can, float ramp_up, sf::CircleShape &ball2, float gravity, float dt, sf::Sound &sound);

void odbicie(sf::CircleShape &ball, float pozycja_x, GameStart &game, float dt, sf::CircleShape &can, sf::Sound &sound);

void rzutBot(sf::CircleShape &can, sf::CircleShape &ball2, float gravity, GameStart &game);

void rzutGracz(GameStart &game, float ramp_up);

void bounce(sf::CircleShape &ball, sf::CircleShape &can, GameStart &game, sf::Sound &sound);

void groundReset(sf::CircleShape &ball, GameStart &game, float ball_x);

void drawGame(GameState currentState, Button &playButton, sf::RenderWindow &window, Button &exitButton, sf::CircleShape &ball, sf::CircleShape &can, sf::CircleShape &ball2, GameStart &game);

void drawBars(GameStart &game, sf::CircleShape &ball, sf::RenderWindow &window);

void drawPowerbar(GameStart &game, sf::CircleShape &ball, sf::RenderWindow &window, float direction_up, float direction_left);

void handleMenu(const std::optional<sf::Event> &event, Button &playButton, sf::Vector2f &mousePos, GameState &currentState, Button &exitButton, sf::RenderWindow &window);

void eventLoop(const std::optional<sf::Event> &event, sf::RenderWindow &window, sf::View &view, GameState &currentState, Button &playButton, sf::Vector2f &mousePos, Button &exitButton);

int main()
{
    // SETUP
    GameStart game;

    //  Rozmiar okna, resize
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Flanki");
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect({ 0.f, 0.f }, mainWin));
    view.setCenter(sf::Vector2f(mainWin.x / 2.f, mainWin.y / 2.f));
    updateViewViewport(window, view);
    window.setView(view);

    // stan gry (menu czy gra)
    GameState currentState = GameState::Menu;

    // Assety
    sf::Font font;
    if (!font.openFromFile("assets/fonts/DejaVuSans.ttf")) return -1;

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/music/clank.mp3")) return -1;
    
    sf::Sound sound(buffer);
    
    // Przyciski 
    sf::Clock keyTimer;
    Button playButton({ 200.f, 60.f }, { 300.f, 200.f }, sf::Color(50, 50, 50), "START", font, 30);
    Button exitButton({ 200.f, 60.f }, { 300.f, 300.f }, sf::Color(50, 50, 50), "WYJSCIE", font, 30);

    // Tło
    sf::RectangleShape logicalBackground(mainWin);
    logicalBackground.setPosition({ 0.f, 0.f });
    logicalBackground.setFillColor(sf::Color(0, 0, 255));

    // Obiekty - puszki
    sf::CircleShape can(15.f);
    can.setFillColor(sf::Color::Yellow);
    can.setPosition({ 400.f, 300.f });

    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Black);
    ball.setPosition({ game.ball_x, game.ball_y });

    sf::CircleShape ball2(10.f);
    ball2.setFillColor(sf::Color::White);
    ball2.setPosition({ game.bot_ball_x, game.ball_y });

    // fizyka i czas
    float gravity = 980.f;
    sf::Clock clock;

    // main loop
    while (window.isOpen()) {

        // 0. czas + mysz
        float dt = clock.restart().asSeconds();

        sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePosI, view);
        float ramp_up = 250.f * dt * 2;

        // 1. event loop 
        while (const std::optional event = window.pollEvent()) 
            eventLoop(event, window, view, currentState, playButton, mousePos, exitButton); 
            
        // 2. logika gry
        logic(currentState, game, ball, can, ramp_up, ball2, gravity, dt, sound);

        // 3. rysowanie
        /* RenderWindow window, x
        View view 
        RectangleShape logicalBackground
        GameState currentState x

        CircleShape ball x
        CircleShape ball2 x 
        CircleShape can x
        
        GameStart game 
        RectangleShape powerBar */

        window.clear(sf::Color::Black);
        window.setView(view);
        window.draw(logicalBackground);

        drawGame(currentState, playButton, window, exitButton, ball, can, ball2, game);
        window.display();
        }
}

// 1. event loop 
// =====================================================================================================================

void eventLoop(const std::optional<sf::Event> &event, sf::RenderWindow &window, sf::View &view, GameState &currentState, Button &playButton, sf::Vector2f &mousePos, Button &exitButton){

    // 1.1 zamknięcie
    if (event->is<sf::Event::Closed>())
        window.close();

    // 1.2 resize okna
    else if (const auto *resized = event->getIf<sf::Event::Resized>())
    {
        updateViewViewport(window, view);
        view.setSize(mainWin);
        view.setCenter({mainWin.x / 2.f, mainWin.y / 2.f});
        window.setView(view);
    }

    // 1.3 menu
    else if (currentState == GameState::Menu)
    {
        handleMenu(event, playButton, mousePos, currentState, exitButton, window);
    }
}

void handleMenu(const std::optional<sf::Event> &event, Button &playButton, sf::Vector2f &mousePos, GameState &currentState, Button &exitButton, sf::RenderWindow &window)
{
    if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (playButton.isMouseOver(mousePos))
                currentState = GameState::Game;
            if (exitButton.isMouseOver(mousePos))
                window.close();
        }
    }
}

// 2. logika gry
// =====================================================================================================================

void logic(GameState &currentState, GameStart &game, sf::CircleShape &ball, sf::CircleShape &can, float ramp_up, sf::CircleShape &ball2, float gravity, float dt, sf::Sound &sound){
    if (currentState == GameState::Game)
    {
        // 3.1 Reset rozgrywki
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape)){
            currentState = GameState::Menu;
            game = GameStart();
            ball.setPosition({game.ball_x, game.ball_y});
            can.setFillColor(sf::Color::Yellow);
        }

        // Charge player / bot
        // 3.2 Lot piłki 
        if (!game.isFlying){
            // Ruch p
            if (!game.turn)
                rzutGracz(game, ramp_up);

            else
                rzutBot(can, ball2, gravity, game);
        }
        
        // 3.3 Odbicie piłki
        else{
            game.velocity.y += gravity * dt;
            // Rzut gracza
            if (!game.turn)
                odbicie(ball, game.ball_x, game, dt, can, sound);

            // Rzut bota
            else 
                odbicie(ball2, game.bot_ball_x, game, dt, can, sound);
        }
    }
}

void odbicie(sf::CircleShape &ball, float pozycja_x, GameStart &game, float dt, sf::CircleShape &can, sf::Sound &sound)
{
    ball.move(game.velocity * dt);        // ruch puszki
    bounce(ball, can, game, sound);       // odbicie
    groundReset(ball, game, pozycja_x); // reset po odbiciu
}

void rzutBot(sf::CircleShape &can, sf::CircleShape &ball2, float gravity, GameStart &game)
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(185.0f, 800.0f);
    std::uniform_real_distribution<> dis2(-130.0f, 130.0f);
    float random_x = dis(gen);

    // Flight trajectory math
    //  t = Distance/Vx
    // Height = (Vy​*t)−(1/2​*Gravity*t^2)
    // Vy = ((Gravity*Distance) / (2*Vx)) + (Height * Vx / Distance)

    // Distance ball2 from can

    // x axis
    float distance = std::abs(can.getPosition().x - ball2.getPosition().x);

    // y axis
    float height = can.getPosition().y - ball2.getPosition().y;

    // Velocityx * Velocityy = gravity*distance / 2
    float physicsConstant = (gravity * distance) / 2.0f;

    // Veloicty = gravity * distance /2 / velocityx - ((height * random_x)/ distance) (- because y axis in sfml upside down)
    float random_y = (physicsConstant / random_x) - ((height * random_x) / distance);

    std::cout << random_x << ' ' << random_y << std::endl;

    // error (0 = enemy always hits)
    int error = 300;
    srand(time(NULL));
    float margin = rand() % (2*error) - error;
    game.velocity = sf::Vector2f(random_x, -(margin + random_y));

    // game.velocity = sf::Vector2f(687.352, -175.465);
    game.initialVelocity = game.velocity;
    game.hasHit = false;

    game.isFlying = true;
}

void rzutGracz(GameStart &game, float ramp_up)
{

    bool chargingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);
    bool chargingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left);

    // Ładowanie do rzutu
    if (chargingUp || chargingLeft)
    {
        game.isCharging = true;
        // Charging until max
        if (chargingUp)
            game.up += ramp_up;
        if (chargingLeft)
            game.left += ramp_up;

        // If charged to max:

        // Cap at max

        /*if (game.up > game.maxCharge) game.up = game.maxCharge;
        if (game.left > game.maxCharge) game.left = game.maxCharge;*/

        // Reset bar
        if (game.up > game.maxCharge)
            game.up = 0.0f;
        if (game.left > game.maxCharge)
            game.left = 0.0f;
    }
    // Rzut
    else if (game.isCharging)
    {
        game.isCharging = false;
        game.isFlying = true;
        game.velocity = sf::Vector2f(-game.left, -game.up);
        game.initialVelocity = game.velocity;
        game.hasHit = false;
        game.up = 0.0f;
        game.left = 0.0f;
    }
}

void bounce(sf::CircleShape &ball, sf::CircleShape &can, GameStart &game, sf::Sound &sound)
{
    if (ball.getGlobalBounds().findIntersection(can.getGlobalBounds()))
    {
        if (!game.hasHit)
        {
            /*std::fstream hit_data;
            hit_data.open("../assets/data/hit_x_y.txt", std::ios::out | std::ios::app);
            hit_data << std::abs(game.initialVelocity.x) << ' ' << std::abs(game.initialVelocity.y) << "\n";
            hit_data.close();*/

            game.velocity.x = -game.velocity.x * 1.0f;
            game.velocity.y = -game.velocity.y * 1.0f;
            can.setFillColor(sf::Color::Magenta);
            if (sound.getStatus() != sf::Sound::Status::Playing)
            {
                sound.play();
            }
            game.hasHit = true;
        }
    }
}

void groundReset(sf::CircleShape &ball, GameStart &game, float ball_x)
{
    if (ball.getPosition().y > 600.f)
    {
        game.isFlying = false;
        game.velocity = {0.f, 0.f};
        ball.setPosition({ball_x, game.ball_y});
        game.turn = !game.turn;
    }

}

// 3. rysowanie
// =====================================================================================================================

void drawGame(GameState currentState, Button &playButton, sf::RenderWindow &window, Button &exitButton, sf::CircleShape &ball, sf::CircleShape &can, sf::CircleShape &ball2, GameStart &game)
{
    if (currentState == GameState::Menu)
    {
        playButton.draw(window);
        exitButton.draw(window);
    }
    else if (currentState == GameState::Game)
    {
        window.draw(ball);
        window.draw(can);
        window.draw(ball2);

        drawBars(game, ball, window);
    }
}

void drawBars(GameStart &game, sf::CircleShape &ball, sf::RenderWindow &window)
{
    if (!game.isFlying && (game.up > 0.0f || game.left > 0.0f))
    {
        if (game.up > 0.0f)
        {
            sf::RectangleShape powerBar(sf::Vector2f(10.f, -game.up / 10.f));
            powerBar.setFillColor(sf::Color::Red);
            powerBar.setPosition(ball.getPosition());
            window.draw(powerBar);
        }
        if (game.left > 0.0f)
        {
            sf::RectangleShape powerBar(sf::Vector2f(-game.left / 10.f, 10.f));
            powerBar.setFillColor(sf::Color::Red);
            powerBar.setPosition(ball.getPosition());
            window.draw(powerBar);
        }
    }
}
