#include <iostream>
#include <memory_resource>   // pmr core types
#include <vector>            // pmr::vector
#include <string>            // pmr::string
#include <locale>            // std::isprint


class debug_resource : public std::pmr::memory_resource {
public:
    explicit debug_resource(std::string name, 
                            std::pmr::memory_resource * up = std::pmr::get_default_resource())
    : _name{ std::move(name) }, _upstream{ up }
    {}

    void* do_allocate(size_t bytes, size_t alignment) override {
        std::cout << _name << " do_allocate(): " << bytes << '\n';
        void* ret = _upstream->allocate(bytes, alignment);
        return ret;
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        std::cout << _name << " do_deallocate(): " << bytes << '\n';
        _upstream->deallocate(ptr, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource & other) const noexcept override {
        return this == &other;
    }

private:
    std::string _name;
    std::pmr::memory_resource * _upstream;
};

int main() {
    std::cout << "sizeof(std::string): " << sizeof(std::string) << '\n';
    std::cout << "sizeof(std::pmr::string): " << sizeof(std::pmr::string) << '\n';

    constexpr size_t BUF_SIZE = 128;
    char buffer[BUF_SIZE] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');

    const auto BufferPrinter = [](std::string_view buf, std::string_view title) {
        std::cout << title << ":\n";
        for (auto& ch : buf) {
            std::cout << (ch >= -1 && ch < 255 && std::isprint(static_cast<char>(ch)) ? ch : '#');
        }
        std::cout << '\n' << std::endl;
    };

    BufferPrinter(buffer, "zeroed buffer");

    debug_resource default_dbg_res{ "default" };
    std::pmr::monotonic_buffer_resource pool{ std::data(buffer), std::size(buffer), &default_dbg_res };
    
    debug_resource dbg_res{ "pool", &pool };
    std::pmr::vector<std::string> strings{ &dbg_res };
    strings.emplace_back("Hello Short String");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "First string");

    strings.emplace_back("Hello Short String 2");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "Second String");

    return 0;
}
