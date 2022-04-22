#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>

#include "algorithms.h"

// SETTINGS
const int WIDTH = 1200;
const int HEIGHT = 800;

int RECT_WIDTH = 5;

// -- Globals --

int Framerate = 60;

bool do_sort = true;
bool paused = false;
bool show_help = false;
bool auto_mode = true;

int speed = 1;
int speed_limit = 100;

// used mostly for auto mode
int curr_algo = 2;
const int algorithm_num = 3;
const char* algorithms[algorithm_num] = {"bubble", "selection", "insertion"};
std::string algorithm = algorithms[curr_algo];
// -- Colors --
const sf::Color BG_COLOR = sf::Color(25,25,25);
const sf::Color HIGHTLIGHT_COLOR = sf::Color(255,71,47);
const sf::Color WHITE = sf::Color(255,255,255);

struct Elements {
    std::vector<int> values;
    std::vector<sf::RectangleShape> rects;

    void gen_elements() {
        values.clear();
        rects.clear();
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

void reset(Elements &elems) {
    algo_reset();
    elems.gen_elements();
}

void auto_adjust_sim() {
    if (algorithm == "bubble") {
        RECT_WIDTH = 10;
        speed = 4;
    } else if (algorithm == "selection" || algorithm == "insertion") {
        RECT_WIDTH = 5;
        speed = 1;
    }
}


void handle_keybinds(int keycode, sf::RenderWindow &window, Elements &elems) {
    using namespace sf;

    switch(keycode) {
        case Keyboard::Escape:
            window.close();
            break;
        case Keyboard::H:
            show_help = !show_help;
            break;
        case Keyboard::P:
            paused = !paused;
            do_sort = !do_sort;
            break;
        case Keyboard::A:
            auto_mode = !auto_mode;
            break;
        case Keyboard::R:
            reset(elems); 
            break;
        case Keyboard::Left:
            if(speed > 1) speed--;
            break;
        case Keyboard::Right:
            if(speed < speed_limit) speed++;
            break;
        case Keyboard::I:
            if(RECT_WIDTH < WIDTH/4) {
                RECT_WIDTH++;
            }
            break;
        case Keyboard::O:
            if(RECT_WIDTH > 1) {
                RECT_WIDTH--;
            }
            break;
        case Keyboard::N:
            curr_algo = (curr_algo - 1) % algorithm_num;
            if (curr_algo < 0) curr_algo = algorithm_num - 1;
            algorithm = algorithms[curr_algo];
            reset(elems);
            break; 
        case Keyboard::M:
            curr_algo = (curr_algo + 1) % algorithm_num;
            algorithm = algorithms[curr_algo];
            reset(elems);
            break; 

    }
}

int main() { 
    using namespace sf;
    srand(time(0));

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "OpenGL_float", Style::Default);
    window.setFramerateLimit(Framerate);

    Font font;
    if (!font.loadFromFile("fonts/LiberationMono-Regular.ttf")) std::cerr << "Error: cant load font!";

    Text info_text;
    info_text.setFont(font);
    info_text.setCharacterSize(18);
    std::string info_string = "";
    info_text.setPosition(7,7);

    Text center_text;
    center_text.setFont(font);
    center_text.setCharacterSize(22);
    std::string center_string = "";

    Text mode_text;
    mode_text.setFont(font);
    mode_text.setCharacterSize(18);
    mode_text.setPosition(WIDTH - mode_text.getCharacterSize(), 7);
    std::string mode_string = "";

    std::string help_string = "\n? HELP | p - pause | r - reset | left/right - slow down/speed up | a - toggle automode";
    help_string            += "\n  I/O - Increase/Decrease rect size |";
    

    Elements elems;
    elems.gen_elements();
    auto_adjust_sim();

    Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    handle_keybinds(event.key.code, window, elems); 
                    break;
            }
        }
        window.clear(Color(25,25,25));
        // Sort
        if (!paused)
            for(int i = 0; i < speed; i++) {
                if (do_sort) {
                    do_sort = !do_algorithm_pass(algorithm, elems.values);
                    elems.update_rects();
                    elems.update_colors();
                } else if (auto_mode) {
                    curr_algo = (curr_algo + 1) % algorithm_num;
                    algorithm = algorithms[curr_algo];
                    auto_adjust_sim();
                    reset(elems);
                    do_sort = true;
                    break;
                } else {
                    paused = true;
                    reset(elems);
                }
            }
        // Draw rectangles
        for(auto rect : elems.rects) {
            window.draw(rect);
        }
        // Build && Draw text
        info_string = "speed: " + std::to_string(speed);
        if (!paused) {
            info_string += "\npasses: " + std::to_string(passes) +
                "\ncomparisons: " + std::to_string(comp);
        } else {
            info_string += "\n -- Paused -- ";
        }
        if(show_help) info_string += help_string;
        info_text.setString(info_string);
        window.draw(info_text);

        if (auto_mode) mode_string = "(auto_mode)";
        else mode_string = "(manual_mode)";
        mode_text.setPosition(WIDTH-(mode_text.getGlobalBounds().width) - 7, 7);
        mode_text.setString(mode_string);
        window.draw(mode_text);

        center_string = auto_mode ? algorithm + " sort" : "<[n]" + algorithm + " sort" + "[m]>";
        center_text.setString(center_string);
        center_text.setPosition(WIDTH/2-(center_text.getGlobalBounds().width / 2.f), 7);
        window.draw(center_text);

        // Flush to screen
        window.display();
    }
}
