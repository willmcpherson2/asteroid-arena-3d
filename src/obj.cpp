#include "obj.h"
#include <fstream>
#include <regex>

std::vector<Vec> obj::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in);
    std::string s(std::istreambuf_iterator<char> { file }, {});

    std::string number("[-|+]?\\d+\\.\\d+");
    std::string integer("\\d+");
    std::string vec("v " + number + " " + number + " " + number + "\n");
    std::string face("f " + integer + " " + integer + " " + integer + "\n");

    std::regex number_re(number);
    std::regex integer_re(integer);
    std::regex vec_re(vec);
    std::regex face_re(face);

    std::vector<Vec> vertex_list;
    std::vector<Vec> vertices;

    auto regex_it = [](const auto& s, const auto& re) {
        return std::sregex_iterator(s.begin(), s.end(), re);
    };

    auto regex_it_end = [](const auto& it) {
        return it == std::sregex_iterator();
    };

    for (auto vec_it = regex_it(s, vec_re); !regex_it_end(vec_it); ++vec_it) {
        auto vec_match = vec_it->str();

        auto number_it = regex_it(vec_match, number_re);

        auto x = std::stod(number_it->str());
        ++number_it;
        auto y = std::stod(number_it->str());
        ++number_it;
        auto z = std::stod(number_it->str());

        Vec v { x, y, z };
        vertex_list.push_back(v);
    }

    for (auto face_it = regex_it(s, face_re); !regex_it_end(face_it); ++face_it) {
        auto face_match = face_it->str();

        for (auto integer_it = regex_it(face_match, integer_re); !regex_it_end(integer_it); ++integer_it) {
            size_t index = std::stoull(integer_it->str()) - 1;
            Vec v = vertex_list[index];
            vertices.push_back(v);
        }
    }

    return vertices;
}
