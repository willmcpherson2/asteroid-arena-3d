#include "obj.h"
#include <fstream>
#include <regex>

static std::sregex_iterator make_it(const std::string& text, const std::regex& re)
{
    return std::sregex_iterator(text.begin(), text.end(), re);
};

static bool it_end(const std::sregex_iterator& it)
{
    return it == std::sregex_iterator();
};

static void get(const std::string& text, const std::regex& re1, const std::regex& re2, const std::function<void(std::sregex_iterator&)>& action)
{
    for (auto it1 = make_it(text, re1); !it_end(it1); ++it1) {
        auto match = it1->str();
        auto it2 = make_it(match, re2);
        action(it2);
    }
}

Model obj::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in);
    std::string text(std::istreambuf_iterator<char> { file }, {});

    static std::regex number("-?\\d+\\.\\d+");
    static std::regex integer("\\d+");
    static std::regex vertex("v .*");
    static std::regex normal("vn .*");
    static std::regex face("f .*");

    std::vector<Vec> vertex_list;

    get(text, vertex, number, [&](auto& it) {
        auto x = std::stod(it->str());
        ++it;
        auto y = std::stod(it->str());
        ++it;
        auto z = std::stod(it->str());
        ++it;

        assert(it_end(it));

        Vec v { x, y, z };

        vertex_list.push_back(v);
    });

    std::vector<Vec> normal_list;

    get(text, normal, number, [&](auto& it) {
        auto x = std::stod(it->str());
        ++it;
        auto y = std::stod(it->str());
        ++it;
        auto z = std::stod(it->str());
        ++it;

        assert(it_end(it));

        Vec v { x, y, z };

        normal_list.push_back(v);
    });

    Model model;

    get(text, face, integer, [&](auto& it) {
        Polygon p;

        while (!it_end(it)) {
            size_t vertex_index = std::stoull(it->str()) - 1;
            assert(vertex_index < vertex_list.size());
            Vec vertex = vertex_list[vertex_index];
            ++it;

            size_t normal_index = std::stoull(it->str()) - 1;
            assert(normal_index < normal_list.size());
            Vec normal = normal_list[normal_index];
            ++it;

            p.add({ vertex, normal });
        }

        model.add(p);
    });

    return model;
}
