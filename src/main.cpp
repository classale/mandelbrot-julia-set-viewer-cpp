#include <SFML/Graphics.hpp>
#include <complex.h>
#include <iostream>

// Define pre-processor constants


#define X_START -2.5
#define X_END 1
#define Y_START -1
#define Y_END 1

using namespace std;

// Define the mandelbrot function that takes the complex number c as an argument whose real value is the x value of the set and whose imaginary value is its y value on the set

constexpr float mandelbrot(const complex<float> c, int& MAX_ITERS, complex<float>& EXPONANT) {
    complex<float> z(0, 0);

    for(int i = 0; i < MAX_ITERS; i++) {
        z = pow(z, EXPONANT);
        z += c;
        if(norm(z) > 16) {
            float log_nz = log2(norm(z));
            float nu = log2(log_nz / 2.0f);
            return max(0.f, i + 1 - nu);
        }
    }
    return MAX_ITERS;
}

// Define the julia set function that takes the complex number c as an argument whose real value is the x value of the set and whose imaginary value is its y value on the set
// This function also has a z argument that mandelbrot doesn't have, z represents a point in the mandelbrot set, the way its value works is the same as c's

constexpr float juliaset(complex<float> z, complex<float> c, int& MAX_ITERS, complex<float>& EXPONANT) {
    for(int i = 0; i < MAX_ITERS; i++) {
        z = pow(z, EXPONANT);
        z += c;
        if(norm(z) > 16) {
            float log_nz = log2(norm(z));
            float nu = log2(log_nz / 2.0f);
            return max(0.f, i + 1 - nu);
        };
    }
    return MAX_ITERS;
}

// Define the function to draw the mandelbrot set to the window

void DrawMandelbrotToWindow(const int width, const int height, sf::RenderWindow& window, int& MAX_ITERS, complex<float>& EXPONANT) {
    // Initialize a new image
    sf::Image image;
    image.create(width, height);


    // Compute for each pixel of the image
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            // Normalize the X and Y values to turn them from pixel positions on the image to a location on the mandelbrot set
            float normalizedx = ((static_cast<float>(x) / width) * (X_END - X_START)) + X_START;
            float normalizedy = ((static_cast<float>(y) / height) * (Y_END - Y_START)) + Y_START;

            // Compute the result of the mandelbrot function
            float mandelbrot_result = mandelbrot(complex<float>(normalizedx, normalizedy), MAX_ITERS, EXPONANT);

            if(mandelbrot_result != MAX_ITERS) {
                // If the point didn't escape, calculate its opacity by dividing its number of iterations by the maximum number of iterations and draw the pixel to the screen
                unsigned int result = (mandelbrot_result / MAX_ITERS) * 255;
                sf::Color color((result * 0x1010100u) + 0xFF);
                image.setPixel(x, y, color);
            } else {
                // Otherwise, draw a white pixel
                image.setPixel(x, y, sf::Color::White);
            }
        }
    }

    // Load the image in a texture
    sf::Texture texture;
    texture.loadFromImage(image);

    // Apply the texture to a sprite, then draw the sprite to the left half of the window
    sf::Sprite sprite(texture);
    window.draw(sprite);
}

void checkKeys(int& x, int& y, int width, int height) {
    // Move the position of the red dot corresponding to which arrow key is pressed
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x++;

    // Prevent the red dot from escaping the mandelbrot set
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x > round(width / 2)) x = round(width / 2);
    if(y > height) y = height;
}

void DrawJuliaSetToWindow(const int width, const int height, sf::RenderWindow& window, int& mandelbrotX, int& mandelbrotY, int& MAX_ITERS, complex<float>& EXPONANT) {
    // Initialize a new image
    sf::Image image;
    image.create(width, height);

    // Compute for each pixel of the image
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            // Normalize the X and Y values to turn them from pixel positions on the image to a location on the mandelbrot set
            float normalizedX = ((static_cast<float>(x) / width) * (X_END - X_START)) + X_START;
            float normalizedY = ((static_cast<float>(y) / height) * (Y_END - Y_START)) + Y_START;

            // Normalize the X and Y values of the red dot from pixel coordinates to a location on the mandelbrot set
            float normalizedMandelbrotX = ((static_cast<float>(mandelbrotX) / width) * (X_END - X_START)) + X_START;
            float normalizedMandelbrotY = ((static_cast<float>(mandelbrotY) / height) * (Y_END - Y_START)) + Y_START;

            // Compute the result of the julia set function
            float juliaset_result = juliaset(complex<float>(normalizedX, normalizedY), complex<float>(normalizedMandelbrotX, normalizedMandelbrotY), MAX_ITERS, EXPONANT);

            if(juliaset_result != MAX_ITERS) {
                // If the point didn't escape, calculate its opacity by dividing its number of iterations by the maximum number of iterations and draw the pixel to the screen
                unsigned int result = (juliaset_result / MAX_ITERS) * 255;
                sf::Color color((result * 0x1010100u) + 0xFF);
                if(result > 255) {
                    cout << result << "\n" << juliaset_result << "\n";
                }
                image.setPixel(x, y, color);
            } else {
                // Otherwise, draw a white pixel.
                image.setPixel(x, y, sf::Color::White);
            }
        }
    }

    // Load the image in a texture
    sf::Texture texture;
    texture.loadFromImage(image);

    // Apply the texture to a sprite, then draw the sprite to the right half of the window
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f(width, 0));
    window.draw(sprite);
}

int main(int argc, char* argv[]) {
    int MAX_ITERS = 20;
    auto EXPONANT = complex<float>(2, 0);

    // Implementing arguments; The first one is the real value of the exponant, the second one its imaginary value and the third one is the value of MAX_ITERS
    // The program will print a warning if there are more than 3 arguments, but the program will still run
    if(argc > 1) EXPONANT = complex<float>(stof(argv[1]), 0);
    if(argc > 2) EXPONANT = complex<float>(stof(argv[1]), stof(argv[2]));
    if(argc > 3) MAX_ITERS = stoi(argv[3]);
    if(argc > 4) cout << "Too many arguments, only accepting the first 3\n";

    // Define the window with a dimension of 600x200
    auto window = sf::RenderWindow({600u, 200u}, "Mandelbrot and Julia Set viewer");

    // Define the x and y coordinates of the red dot
    int x = round(window.getSize().x / 4);
    int y = round(window.getSize().y / 2);

    // Set the maximum framerate
    window.setFramerateLimit(144);

    while (window.isOpen()) {
        // Loop trough every event of the window
        for (auto event = sf::Event(); window.pollEvent(event);) {
            // If the event is closing the window, close it
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // If the event is clicking the window, move the red dot to the position where the button was clicked
            if(event.type == sf::Event::MouseButtonPressed)  {
                x = sf::Mouse::getPosition(window).x;
                y = sf::Mouse::getPosition(window).y;
            }
        }

        // Check the keys in order to detect a movement
        checkKeys(x, y, window.getSize().x, window.getSize().y);

        // Clear the window
        window.clear();

        // Change the size of the window if the window has been resized
        sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
        window.setView(view);

        // Draw the mandelbrot set and its associated julia set to the screen
        DrawMandelbrotToWindow(floor(window.getSize().x / 2), window.getSize().y, window, MAX_ITERS, EXPONANT);
        DrawJuliaSetToWindow(floor(window.getSize().x / 2), window.getSize().y, window, x, y, MAX_ITERS, EXPONANT);

        // Draw the circle
        sf::CircleShape point(2);
        point.setPosition(x - 2, y - 2);
        point.setFillColor(sf::Color::Red);
        window.draw(point);

        // Display everything that has been drawn to the screen
        window.display();
    }
}