#include <iostream>
#include <memory_resource>   // pmr core types
#include <vector>            // pmr::vector

int main() {
    char buffer[64] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');
    std::cout << buffer << '\n';

    std::pmr::monotonic_buffer_resource pool{ std::data(buffer), std::size(buffer) };

    std::pmr::vector<char> vec{ &pool };
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        vec.push_back(ch);
    }

    std::cout << buffer << '\n';

    return 0;
}
