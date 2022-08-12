#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include "Larch.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    auto larch = Larch();
    auto color = Color();

    // TODO: Implement selection of multiple types
    fs::path input_path;
    fs::path output_path;

    auto terminate = [&color](std::string what = "")
    {
        std::cout << "Exiting.\n";
        if (!what.empty())
            std::cout << "Reason: " << color("red") << what << color("reset") << '\n';
        exit(0);
    };
    auto check_path = [&color, terminate](fs::path path)
    {
        // TODO: Make new filename generation based on present files with essentually the same filename

        if (fs::exists(path))
        {
            std::cout << "File " << color("yellow") << path << color("reset") << " already exists.\n";
            std::cout << "Do you wish to replace it? Y/n: ";
            auto ans = ' ';
            std::cin >> ans;
            if (ans != 'Y')
                terminate();

            std::cout << '\n';
        }
    };

    if (argc < 2)
    {
        larch.print_help();
        return 0;
    }

    enum class task_type_enum
    {
        not_selected,
        encoding,
        decodind,
    };
    auto task_type = task_type_enum::not_selected;

    for (auto i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            larch.print_help();
        }
        else if (arg == "-v" || arg == "--version")
        {
            larch.print_version();
        }
        else if (arg == "-c" || arg == "--create")
        {
            if (task_type != task_type_enum::not_selected)
                terminate("Only one task is aviable per command.");

            task_type = task_type_enum::encoding;

            output_path = fs::path(argv[i + 1]);
            ++i;

            if (output_path.extension() == "")
                output_path += ".lar";

            check_path(output_path);
        }
        else if (arg == "-x" || arg == "--extract")
        {
            if (task_type != task_type_enum::not_selected)
                terminate("Only one task is aviable per command.");

            task_type = task_type_enum::decodind;
        }
        else
        {
            input_path = arg;
        }
    }

    if (task_type == task_type_enum::encoding)
    {
        
        if (input_path.empty())
            terminate("No files to encode were provided.");

        auto ifstream = std::ifstream(input_path, std::ios::binary);
        auto ofstream = std::ofstream(output_path, std::ios::binary);

        std::cout << "Encoding..." << '\n';
        larch.encodeLZ78(ifstream, ofstream);
    }
    else if (task_type == task_type_enum::decodind)
    {

        auto ifstream = std::ifstream(input_path, std::ios::binary);
        auto ofstream = std::ofstream({input_path.filename().string() + ".png"}, std::ios::binary);
        
        std::cout << "Decoding...\n";
        larch.decodeLZ78(ifstream, ofstream);
    }
}
