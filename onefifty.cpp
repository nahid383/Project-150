#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;
using namespace std;

// Player er lives & points store korar jonno structure
struct Player {
    int lives = 3;
    int points = 0;
};

// Life komle ball & paddle ke reset korar function
void resetGame(float& x, float& y, float& dx, float& dy, Sprite& sPaddle, Player& player) {
    x = 300;    // ball er initial X position
    y = 300;    // ball er initial Y position
    dx = 3;     // ball speed X
    dy = 3;     // ball speed Y

    // Paddle ke screen er majhe set kora
    sPaddle.setPosition(520 / 2 - sPaddle.getGlobalBounds().width / 2, 440);

    player.lives--;   // ek life komano
}

int main()
{
    srand(time(0)); // random generate korar jonno

    // Image folder er base directory
    //string BASE = "D:/project150/project/images/";

    RenderWindow app(VideoMode(520, 450), "Arkanoid!");
    app.setFramerateLimit(60); // frame lock 60 FPS

    // Texture load
    Texture t1, t2, t3, t4;
    t1.loadFromFile("Images/block01.png");
    t2.loadFromFile("Images/background.jpg");
    t3.loadFromFile("Images/ball.png");
    t4.loadFromFile("Images/paddle.png");

    // Sprite assign
    Sprite sBackground(t2), sBall(t3), sPaddle(t4);

    // Paddle ke screen er majhe rakha
    sPaddle.setPosition(520 / 2 - sPaddle.getGlobalBounds().width / 2, 440);

    // Blocks create
    Sprite block[1000];
    int n = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);  // grid pattern
            n++;
        }

    Player player;

    // Ball er movement variables
    float dx = 3, dy = 3;
    float x = 300, y = 300;

    // Points Bar (score visualization)
    RectangleShape pointsBar;
    pointsBar.setSize(sf::Vector2f(player.points, 10));
    pointsBar.setFillColor(Color::Green);
    pointsBar.setPosition(10, 10);

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();  // window close
        }

        // ======================
        // Paddle Movement
        // ======================

        float px = sPaddle.getPosition().x;
        float pWidth = sPaddle.getGlobalBounds().width;

        // Right move
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (px + pWidth < 520)   // boundary check
                sPaddle.move(6, 0);
        }

        // Left move
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (px > 0)   // boundary check
                sPaddle.move(-6, 0);
        }

        // ======================
        // Ball Movement
        // ======================

        x += dx; // X direction move
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); // block remove
                dx = -dx;  // bounce
                player.points += 5; // score add
            }

        y += dy; // Y direction move
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0); // block remove
                dy = -dy;  // bounce
                player.points += 5; // score add
            }

        // ======================
        // Wall Collision
        // ======================
        if (x < 0 || x > 520) dx = -dx; // left/right wall bounce
        if (y < 0) dy = -dy;            // top wall bounce

        // ======================
        // Life Lost Check
        // ======================
        if (y > 450) {     // bottom e gele life kombe
            if (player.lives > 1) {
                resetGame(x, y, dx, dy, sPaddle, player);
            }
            else {
                app.close();  // game over
            }
        }

        // ======================
        // Ball hits Paddle
        // ======================
        if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) {
            dy = -(rand() % 5 + 2);  // random bounce
        }

        sBall.setPosition(x, y);

        // Points bar update
        pointsBar.setSize(sf::Vector2f(player.points, 10));

        // ======================
        // Draw Everything
        // ======================
        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        app.draw(pointsBar);

        app.display();
    }

    return 0;
}
