#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "algorithms.h"

// SETTINGS
const int WIDTH = 1200;
const int HEIGHT = 800;

const int RECT_WIDTH = 5;

// end SETTINGS

const sf::Color HIGHTLIGHT_COLOR = sf::Color(255,71,47);
const sf::Color WHITE = sf::Color(255,255,255);

struct Elements {
    std::vector<int> values;
    std::vector<sf::RectangleShape> rects;

    void gen_elements() {
        srand(time(0));
        int rand_height;
        for(int i = 0; i < WIDTH/RECT_WIDTH; i++) {
            rand_height = 1+rand()%600;
            this->values.push_back(rand_height);

            sf::RectangleShape new_rect(sf::Vector2f(RECT_WIDTH, rand_height));
            new_rect.setPosition(i*RECT_WIDTH,HEIGHT - rand_height);
            this->rects.push_back(new_rect);
        }
    }

    void update_rects() {
        for(int i = 0; i < rects.size(); i++) {
            rects[i].setSize(sf::Vector2f(RECT_WIDTH, values[i]));
            rects[i].setPosition(rects[i].getPosition().x, HEIGHT - values[i]);
        }
    }

    void update_colors() {
        if (pos == 0) {
            rects[rects.size() - passes].setFillColor(WHITE);
        } else {
            rects[pos - 1].setFillColor(WHITE);
        }
        rects[pos].setFillColor(HIGHTLIGHT_COLOR);
    }
};


void handle_keybinds(int keycode, sf::RenderWindow &window) {
    using namespace sf;

    switch(keycode) {
        case Keyboard::Escape:
            window.close();
            break;
    }
    /*
        case Keyboard::H:
            show_help = !show_help;
            break;
        case Keyboard::P:
            do_sort = !do_sort;
            break;
        case Keyboard::R:
            comp = 0;
            passes = 0;
            pos = 0;
            values = gen_rectangles();
            break;
        case Keyboard::Left:
            if(sort_speed > 1) {
                sort_speed--;
            }
            break;
        case Keyboard::Right:
            if(sort_speed < speed_limit) {
                sort_speed++;
            }
            break;
    }
    */
}

int main() { 
    using namespace sf;

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "OpenGL_float", Style::Default);
    window.setFramerateLimit(60);

    Elements elems;
    elems.gen_elements();

    bool do_sort = true;
    int speed = 10;

    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    handle_keybinds(event.key.code, window); 
                    break;
            }
        }
        //Update
        window.clear(Color(25,25,25));
        // Sort
        for(int i = 0; i < speed; i++) {
            if (do_sort) {
                do_sort = !do_algorithm_pass("BubbleSort", elems.values);
                elems.update_rects();
                elems.update_colors();
            }
        }
        // Render
        for(auto rect : elems.rects) {
            window.draw(rect);
        }
        window.display();
    }
}
