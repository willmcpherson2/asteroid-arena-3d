#include "obj.h"
#include <fstream>
#include <regex>

Model obj::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in);
    std::string s(std::istreambuf_iterator<char> { file }, {});

    std::string number("[-|+]?\\d+\\.\\d+");
    std::string integer("\\d+");
    std::string vertex_element(number + "\\s");
    std::string face_element(integer + "//" + integer + "\\s");
    std::string vertex("v (" + vertex_element + ")*");
    std::string face("f (" + face_element + ")*");

    std::regex number_re(number);
    std::regex integer_re(integer);
    std::regex vertex_re(vertex);
    std::regex face_element_re(face_element);
    std::regex face_re(face);

    std::vector<Vec> vertex_list;

    auto regex_it = [](const auto& s, const auto& re) {
        return std::sregex_iterator(s.begin(), s.end(), re);
    };

    auto regex_it_end = [](const auto& it) {
        return it == std::sregex_iterator();
    };

    for (auto vertex_it = regex_it(s, vertex_re); !regex_it_end(vertex_it); ++vertex_it) {
        auto vertex_match = vertex_it->str();

        auto number_it = regex_it(vertex_match, number_re);

        auto x = std::stod(number_it->str());
        ++number_it;
        auto y = std::stod(number_it->str());
        ++number_it;
        auto z = std::stod(number_it->str());

        Vec v { x, y, z };
        vertex_list.push_back(v);
    }

    Model model;

    for (auto face_it = regex_it(s, face_re); !regex_it_end(face_it); ++face_it) {
        auto face_match = face_it->str();

        Polygon p;

        for (auto face_element_it = regex_it(face_match, face_element_re); !regex_it_end(face_element_it); ++face_element_it) {
            auto face_element_match = face_element_it->str();

            auto integer_it = regex_it(face_element_match, integer_re);

            size_t vertex_index = std::stoull(integer_it->str()) - 1;
            Vec vertex = vertex_list[vertex_index];
            ++integer_it;

            size_t normal_index = std::stoull(integer_it->str()) - 1;
            Vec normal = vertex_list[normal_index];

            p.add(vertex, normal);
        }

        model.add(p);
    }

    return model;
}
