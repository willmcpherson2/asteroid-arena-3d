#include "obj.h"
#include <fstream>
#include <sstream>
#include <unordered_map>

Model obj::load(const std::string& filename)
{
    enum class Keyword {
        Comment,
        Vertex,
        Normal,
        Face,
        Smoothing,
        Material,
        MaterialLibrary,
    };

    static std::unordered_map<std::string, Keyword> keywords = {
        { "#", Keyword::Comment },
        { "v", Keyword::Vertex },
        { "vn", Keyword::Normal },
        { "f", Keyword::Face },
        { "s", Keyword::Smoothing },
        { "usemtl", Keyword::Material },
        { "mtllib", Keyword::MaterialLibrary },
    };

    std::ifstream file(filename, std::ios::in);

    Model model;
    std::vector<Vec> vertex_list;
    std::vector<Vec> normal_list;

    for (std::string line; std::getline(file, line);) {
        std::istringstream line_stream(line);
        std::istream_iterator<std::string> word_it(line_stream);
        std::istream_iterator<std::string> word_it_end;

        Keyword keyword;
        auto first_word = *word_it;
        auto key_val_it = keywords.find(first_word);
        if (key_val_it != keywords.end()) {
            keyword = key_val_it->second;
        } else {
            std::cout << "Unexpected keyword in OBJ file: " << first_word << "\n";
            assert(false);
        }
        ++word_it;

        switch (keyword) {
        case Keyword::Vertex:
        case Keyword::Normal: {
            auto x = std::stod(*word_it);
            ++word_it;
            auto y = std::stod(*word_it);
            ++word_it;
            auto z = std::stod(*word_it);

            Vec v { x, y, z };

            if (keyword == Keyword::Vertex) {
                vertex_list.push_back(v);
            } else {
                normal_list.push_back(v);
            }

            break;
        }
        case Keyword::Face: {
            Polygon polygon;

            for (; word_it != word_it_end; ++word_it) {
                std::istringstream face_element(*word_it);

                std::string vertex_index_str;
                std::getline(face_element, vertex_index_str, '/');
                auto vertex_index = std::stoull(vertex_index_str) - 1;
                assert(vertex_index < vertex_list.size());
                auto vertex = vertex_list[vertex_index];

                std::string texture_index_str;
                std::getline(face_element, texture_index_str, '/');

                std::string normal_index_str;
                std::getline(face_element, normal_index_str, '/');
                auto normal_index = std::stoull(normal_index_str) - 1;
                assert(normal_index < normal_list.size());
                auto normal = normal_list[normal_index];

                polygon.add({ vertex, normal });
            }

            model.add(polygon);

            break;
        }
        case Keyword::Material: {
            break;
        }
        case Keyword::MaterialLibrary: {
            break;
        }
        case Keyword::Comment:
        case Keyword::Smoothing:
            break;
        }
    }

    return model;
}
