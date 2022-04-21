#include <iostream>
#include <vector>

int pos = 0;
int passes = 0;

void reset() {
    pos = 0;
    passes = 0;
}

bool bubble_sort(std::vector<int> &values) {
    if(values[pos] > values[pos + 1]) {
        int tmp = values[pos];
        values[pos] = values[pos + 1];
        values[pos + 1] = tmp;
    } 

    if (pos == values.size() - 1 - passes) {
        passes++;
        pos = 0;
    } else {
        pos++;
    }

    return passes == values.size() - 1;
}

bool do_algorithm_pass(std::string algorithm_name, std::vector<int> &values) {
    if (algorithm_name == "BubbleSort") {
        return bubble_sort(values);
    }
    std::cerr << "undefinded sorting algorithm name";
    exit(-1);
}

