#include <iostream>
#include <vector>

int pos = 0;
int passes = 0;
int comp = 0;

void algo_reset() {
    pos = 0;
    passes = 0;
    comp = 0;
}

bool bubble_sort(std::vector<int> &values) {
    comp++;
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

bool selection_sort(std::vector<int> &values) {
    int smallest = 1000;
    int smallest_index;
    for(int i = pos; i < values.size(); i++) {
        if (values[i] < smallest) {
            smallest = values[i];
            smallest_index = i;
        }
    }
    values[smallest_index] = values[pos];
    values[pos] = smallest;

    passes++;
    pos++;
    comp++;
    return pos == values.size();
}

bool insertion_sort(std::vector<int> &values) {
    for (int i = pos + 1; i < values.size(); i++) {
        comp++;
        if (values[pos] > values[i]) {
            int tmp = values[i];
            int endshiftnum = -1;
            for (int l = pos - 1; l >= 0; l--) {
                comp++;
                if (values[l] < tmp) {
                    endshiftnum = l;
                    break;
                }
            }
            for(int j = pos; values[j] > values[endshiftnum]; j--) {
                values[j + 1] = values[j];
            }
            values[endshiftnum + 1] = tmp;
            break;
        } else {
            pos++;
        }
    }
    pos++;
    passes++;
    return pos == values.size();
}

bool do_algorithm_pass(std::string algorithm_name, std::vector<int> &values) {
    if (algorithm_name == "bubble") {
        return bubble_sort(values);
    } else if (algorithm_name == "selection") {
        return selection_sort(values);
    } else if (algorithm_name == "insertion") {
        return insertion_sort(values);
    }

    std::cerr << "undefinded sorting algorithm name";
    exit(-1);
}

