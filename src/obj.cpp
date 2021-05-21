#include "obj.h"
#include <fstream>
#include <regex>

Model obj::load(const std::string& filename)
{
    static std::string number("-?\\d+\\.\\d+");
    static std::string integer("\\d+");
    static std::string vertex_element(number + "\\s");
    static std::string face_element(integer + "//" + integer + "\\s");
    static std::string vertex("v (" + vertex_element + ")*");
    static std::string normal("vn (" + vertex_element + ")*");
    static std::string face("f (" + face_element + ")*");

    static std::regex number_re(number);
    static std::regex integer_re(integer);
    static std::regex vertex_re(vertex);
    static std::regex normal_re(normal);
    static std::regex face_element_re(face_element);
    static std::regex face_re(face);

    static auto regex_it = [](const auto& text, const auto& re) {
        return std::sregex_iterator(text.begin(), text.end(), re);
    };

    static auto regex_it_end = [](const auto& it) {
        return it == std::sregex_iterator();
    };

    std::ifstream file(filename, std::ios::in);
    std::string text(std::istreambuf_iterator<char> { file }, {});

    std::vector<Vec> vertex_list;

    for (auto vertex_it = regex_it(text, vertex_re); !regex_it_end(vertex_it); ++vertex_it) {
        auto vertex_match = vertex_it->str();

        auto number_it = regex_it(vertex_match, number_re);

        auto x = std::stod(number_it->str());
        ++number_it;
        auto y = std::stod(number_it->str());
        ++number_it;
        auto z = std::stod(number_it->str());

        assert(regex_it_end(++number_it));

        Vec v { x, y, z };
        vertex_list.push_back(v);
    }

    std::vector<Vec> normal_list;

    for (auto normal_it = regex_it(text, normal_re); !regex_it_end(normal_it); ++normal_it) {
        auto normal_match = normal_it->str();

        auto number_it = regex_it(normal_match, number_re);

        auto x = std::stod(number_it->str());
        ++number_it;
        auto y = std::stod(number_it->str());
        ++number_it;
        auto z = std::stod(number_it->str());

        assert(regex_it_end(++number_it));

        Vec v { x, y, z };
        normal_list.push_back(v);
    }

    Model model;

    for (auto face_it = regex_it(text, face_re); !regex_it_end(face_it); ++face_it) {
        auto face_match = face_it->str();

        Polygon p;

        for (auto face_element_it = regex_it(face_match, face_element_re); !regex_it_end(face_element_it); ++face_element_it) {
            auto face_element_match = face_element_it->str();

            auto integer_it = regex_it(face_element_match, integer_re);

            size_t vertex_index = std::stoull(integer_it->str()) - 1;
            assert(vertex_index < vertex_list.size());
            Vec vertex = vertex_list[vertex_index];
            ++integer_it;

            size_t normal_index = std::stoull(integer_it->str()) - 1;
            assert(normal_index < normal_list.size());
            Vec normal = normal_list[normal_index];

            assert(regex_it_end(++integer_it));

            p.add({ vertex, normal });
        }

        model.add(p);
    }

    return model;
}
