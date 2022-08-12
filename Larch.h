#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

struct Color
{
private:
    std::unordered_map<std::string, std::string> color{
        {"black", "\033[0;30m"},
        {"red", "\033[0;31m"},
        {"green", "\033[0;32m"},
        {"yellow", "\033[0;33m"},
        {"blue", "\033[0;34m"},
        {"magenta", "\033[0;35m"},
        {"cyan", "\033[0;36m"},
        {"white", "\033[0;37m"},
        {"reset", "\033[0;39m"},
    };

public:
    auto operator()(std::string key)
    {
        return color[key];
    }
};

class Larch
{

private:
    Color color;

public:

    void print_help();
    void print_version();

    void encodeLZ78(std::istream &, std::ostream &);
    void decodeLZ78(std::istream &, std::ostream &);
    void encodeLZW(std::istream &, std::ostream &);
    void decodeLZW(std::istream &, std::ostream &);

private:
    void print_time_taken(const std::chrono::system_clock::time_point &);
    
};
