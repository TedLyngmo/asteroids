#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

template<class T>
T open(std::string_view prog, std::string_view file) {
    using namespace std::string_literals;
    auto stream = T(std::string(file));

    if(!stream) throw std::runtime_error(std::string(prog) + ": ERROR: \""s + std::string(file) + "\", "s + std::strerror(errno));
    return stream;
}

int cppmain(std::string_view prog, std::vector<std::string> args) {
    if(args.size() != 2) {
        std::cerr << "USAGE: " << prog << " infile outfile\n";
        return 1;
    }
    try {
        auto is = open<std::ifstream>(prog, args[0]);
        auto os = open<std::ofstream>(prog, args[1]);
        os << std::hex << std::setfill('0');

        os << R"aw(#include "res.h")aw" << "\n\n"
              R"aw(static constexpr unsigned char res[] = {)aw" << '\n';

        auto io = [&](bool c, int i) {
            auto ch = is.get();
            if(ch == EOF) return false;
            if(c) { os << ',';
                if(i%16 == 0) os << '\n';
            }
            os << "0x" << std::setw(2) << static_cast<unsigned>(static_cast<unsigned char>(ch));
            return true;
        };
        if(io(false, 0)) {
            for(int i = 1; io(true, i);) {
                ++i;
            }
        }
        os << "};\n";
    } catch(const std::exception& ex) {
        std::cerr << prog << ": ERROR: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    return cppmain(argv[0], {argv + 1, argv + argc});
}
