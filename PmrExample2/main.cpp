#include <iostream>
#include <memory_resource>   // pmr core types
#include <vector>            // pmr::vector
#include <string>            // pmr::string
#include <locale>            // std::isprint

int main() {
    std::cout << "sizeof(std::string): " << sizeof(std::string) << '\n';
    std::cout << "sizeof(std::pmr::string): " << sizeof(std::pmr::string) << '\n';

    char buffer[256] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');

    const auto BufferPrinter = [](const std::string_view buf, const std::string_view title) {
        std::cout << title << ":\n";
        for (const auto& ch : buf) {
            std::cout << (ch >= -1 && ch < 255 && std::isprint(ch) ? ch : '#');
        }
        std::cout << '\n' << std::endl;
    };

    BufferPrinter(buffer, "zeroed buffer");

    std::pmr::monotonic_buffer_resource pool{ std::data(buffer), std::size(buffer) };
    std::pmr::vector<std::pmr::string> vec{ &pool };
    vec.reserve(5);

    vec.push_back("Hello World");
    vec.push_back("One Two Three");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after two short strings");

    vec.emplace_back("This is a longer string");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after longer string strings");

    vec.push_back("Four Five Six");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after the last string");
}
