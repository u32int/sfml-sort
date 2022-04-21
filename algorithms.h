#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

bool do_algorithm_pass(std::string algorithm_name, std::vector<int> &values);
void reset();

extern int pos;
extern int passes;
