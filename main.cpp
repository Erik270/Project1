#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Paddle {
public:
    Paddle(float x, float y) {
        shape.setSize(sf::Vector2f(25, 100));  // Paddle size
        shape.setPosition(x, y);
    }

    void move(float dy) {
        shape.move(0, dy);
    }

    void update() {
        // Prevent paddle from going out of bounds
        if (shape.getPosition().y < 0)
            shape.setPosition(shape.getPosition().x, 0);
        if (shape.getPosition().y + shape.getSize().y > 600)
            shape.setPosition(shape.getPosition().x, 600 - shape.getSize().y);
    }

    sf::RectangleShape shape;
};

class Ball {
public:
    Ball() {
        shape.setRadius(10);
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(400, 300);
        velocity.x = -0.1f;
        velocity.y = 0.1f;
    }

    void update() {
        shape.move(velocity);

        // Ball bouncing off top and bottom
        if (shape.getPosition().y < 0 ||
            shape.getPosition().y + shape.getRadius() * 2 > 600)
        {
            velocity.y = -velocity.y;
        }
    }

    void reset() {
        shape.setPosition(400, 300);
        velocity.x = -0.1f;
        velocity.y = 0.1f;
    }

    sf::CircleShape shape;
    sf::Vector2f velocity;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ping Pong");

    // Load background texture
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("background.png")) {
        // Error loading background image
        return -1;
    }
    // Create a sprite for the background
    sf::Sprite background(bgTexture);
    // Scale to fit the 800×600 window, if needed
    background.setScale(
        800.f / bgTexture.getSize().x,
        600.f / bgTexture.getSize().y
    );

    // Left Paddle is Red
    Paddle leftPaddle(30, 250);
    leftPaddle.shape.setFillColor(sf::Color::Red);

    // Right Paddle is Blue
    Paddle rightPaddle(745, 250);
    rightPaddle.shape.setFillColor(sf::Color::Blue);

    Ball ball;

    sf::Clock clock;
    float paddleSpeed = 0.5f;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Control left paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            leftPaddle.move(-paddleSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            leftPaddle.move(paddleSpeed);

        // Control right paddle
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            rightPaddle.move(-paddleSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            rightPaddle.move(paddleSpeed);

        leftPaddle.update();
        rightPaddle.update();
        ball.update();

        // Ball-paddle collision detection
        if (ball.shape.getGlobalBounds().intersects(leftPaddle.shape.getGlobalBounds())) {
            ball.velocity.x = -ball.velocity.x;
        }
        if (ball.shape.getGlobalBounds().intersects(rightPaddle.shape.getGlobalBounds())) {
            ball.velocity.x = -ball.velocity.x;
        }

        // Ball reset when scoring
        if (ball.shape.getPosition().x < 0 || ball.shape.getPosition().x > 800) {
            ball.reset();
        }

        // Draw everything
        window.clear();
        window.draw(background);       // draw background first
        window.draw(leftPaddle.shape);
        window.draw(rightPaddle.shape);
        window.draw(ball.shape);
        window.display();
    }

    return 0;
}
