#include "Larch.h"

void Larch::print_time_taken(const std::chrono::system_clock::time_point &start)
{

    std::cout << "Done! Took: " << color("green");

    auto ms_count = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
    if (ms_count > 1000)
    {
        auto s_count = static_cast<double>(ms_count) / 1000;
        if (s_count > 10)
            std::cout << color("red");
        else if (s_count > 3)
            std::cout << color("yellow");

        std::cout << s_count << "s\n";
    }
    else
    {
        std::cout << ms_count << "ms\n";
    }
    std::cout << color("reset");
}

void Larch::print_help()
{
    print_version();
    std::cout
        << "Ledatu Corporation\n"
        << "The easy-to-use console archiver.\n"
        << "\n"
        << color("magenta") << "USAGE:\n"
        << color("reset")
        << "\tlarch <args>\n"
        << "\n"
        << color("magenta") << "OPTIONS:\n"
        << color("cyan")
        << "\t-h, --help\t\tPrint help information.\n"
        << "\t-v, --version\t\tPrint version information.\n";
}

void Larch::print_version()
{
    std::cout << color("cyan") << "larch " << color("reset") << "0.0.1\n";
}

void Larch::encodeLZ78(std::istream &is, std::ostream &os)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int16_t> dict;

    auto code = 0;
    dict[""] = code++;

    std::string s;
    char ch;
    while (is.peek() != EOF)
    {
        is.read(&ch, 1);

        if (dict.find(s + ch) != dict.end())
        {
            s += ch;
        }
        else
        {
            os.write((char *)&dict[s], 2);
            os.write((char *)&ch, 1);

            // std::cout << "( " << dict[s] << " " << ch << " ) " << s <<"\n";

            s += ch;
            dict[s] = code++;
            s.clear();
        }
    }

    if (!s.empty())
    {

        auto last_ch = s.back();
        s.pop_back();

        os.write((char *)&dict[s], 2);
        os.write((char *)&last_ch, 1);

        // std::cout << "( " << dict[s] << " " << ch << " ) " << s <<"\n";
    }

    print_time_taken(start);
}

void Larch::decodeLZ78(std::istream &is, std::ostream &os)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_map<int16_t, std::string> dict;

    auto code = 0;
    dict[code++] = "";

    int16_t enc_code;
    char ch;
    while (is.peek() != EOF)
    {

        is.read((char *)&enc_code, 2);
        is.read(&ch, 1);

        // std::cout << "( " << enc_code << " " << ch << " ) " << dict[enc_code] <<"\n";

        std::string s;
        s = dict[enc_code] + ch;

        dict[code++] = s;

        os << s;

        enc_code = 0;
        ch = 0;
    }

    print_time_taken(start);
}
