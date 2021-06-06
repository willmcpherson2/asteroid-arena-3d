#include "obj.h"
#include <OpenGL/gl.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

static Image load_image(const std::string& image_filename, int width, int height)
{
    std::ifstream file(image_filename, std::ios::in);
    std::vector<unsigned char> buffer { std::istreambuf_iterator<char>(file), {} };

    Image image;

    image.width = width;
    image.height = height;

    for (size_t i = buffer.size() - 1; i > 4; i -= 4) {
        unsigned char a = buffer[i];
        unsigned char b = buffer[i - 1];
        unsigned char g = buffer[i - 2];
        unsigned char r = buffer[i - 3];
        Pixel c { r, g, b, a };
        image.pixels.push_back(c);
    }

    return image;
}

static Texture load_texture(const std::string& image_filename, int width, int height)
{
    Texture texture;

    texture.image = load_image(image_filename, width, height);

    glPushAttrib(GL_TEXTURE_BIT);
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.image.width, texture.image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.image.pixels.data());
    glPopAttrib();

    return texture;
}

static Colour colour(std::istream_iterator<std::string>& word_it)
{
    auto r = std::stod(*word_it);
    ++word_it;
    auto g = std::stod(*word_it);
    ++word_it;
    auto b = std::stod(*word_it);

    return { r, g, b };
}

static std::unordered_map<std::string, Material> load_mtl(const std::string& mtl_filename, const std::string& image_filename, int width, int height)
{
    enum class Keyword {
        Material,
        Ambient,
        Diffuse,
        Specular,
        Emmissive,
        Texture,
    };

    static std::unordered_map<std::string, Keyword> keywords = {
        { "newmtl", Keyword::Material },
        { "Ka", Keyword::Ambient },
        { "Kd", Keyword::Diffuse },
        { "Ks", Keyword::Specular },
        { "Ke", Keyword::Emmissive },
        { "map_Kd", Keyword::Texture },
    };

    std::ifstream file(mtl_filename, std::ios::in);

    std::unordered_map<std::string, Material> materials;
    Material material;
    std::string name;

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
            continue;
        }
        ++word_it;

        switch (keyword) {
            case Keyword::Material:
                if (name != "") {
                    materials.insert({ name, material });
                }
                name = *word_it;
                material = Material();
                break;
            case Keyword::Ambient:
                material.ambient = colour(word_it);
                break;
            case Keyword::Diffuse:
                material.diffuse = colour(word_it);
                break;
            case Keyword::Specular:
                material.specular = colour(word_it);
                break;
            case Keyword::Emmissive:
                material.emissive = colour(word_it);
                break;
            case Keyword::Texture:
                if (image_filename != "") {
                    material.texture = load_texture(image_filename, width, height);
                    material.display = Display::Texture;
                }
                break;
        }
    }

    if (name != "") {
        materials.insert({ name, material });
    }

    return materials;
}

Model obj::load(const std::string& obj_filename, const std::string& mtl_filename, const std::string& image_filename, int width, int height)
{
    enum class Keyword {
        Comment,
        Vertex,
        Normal,
        Uv,
        Face,
        Smoothing,
        Material,
        MaterialLibrary,
    };

    static std::unordered_map<std::string, Keyword> keywords = {
        { "#", Keyword::Comment },
        { "v", Keyword::Vertex },
        { "vn", Keyword::Normal },
        { "vt", Keyword::Uv },
        { "f", Keyword::Face },
        { "s", Keyword::Smoothing },
        { "usemtl", Keyword::Material },
        { "mtllib", Keyword::MaterialLibrary },
    };

    std::ifstream file(obj_filename, std::ios::in);

    Model model;
    std::vector<Vec> vertex_list;
    std::vector<Vec> normal_list;
    std::vector<Vec> uv_list;
    auto materials = load_mtl(mtl_filename, image_filename, width, height);
    std::string material;

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
            case Keyword::Uv:
            case Keyword::Normal: {
                auto x = std::stod(*word_it);
                ++word_it;
                auto y = std::stod(*word_it);
                ++word_it;
                auto z = word_it != word_it_end ? std::stod(*word_it) : 0;

                Vec v { x, y, z };

                switch (keyword) {
                    case Keyword::Vertex:
                        vertex_list.push_back(v);
                        break;
                    case Keyword::Normal:
                        normal_list.push_back(v);
                        break;
                    case Keyword::Uv:
                        uv_list.push_back(v);
                        break;
                    case Keyword::Comment:
                    case Keyword::Face:
                    case Keyword::Smoothing:
                    case Keyword::Material:
                    case Keyword::MaterialLibrary:
                        assert(false);
                }

                break;
            }
            case Keyword::Face: {
                Polygon polygon;

                auto key_val_it = materials.find(material);
                if (key_val_it != materials.end()) {
                    polygon.material = key_val_it->second;
                }

                for (; word_it != word_it_end; ++word_it) {
                    std::istringstream face_element(*word_it);

                    std::string vertex_index_str;
                    std::getline(face_element, vertex_index_str, '/');
                    auto vertex_index = std::stoull(vertex_index_str) - 1;
                    assert(vertex_index < vertex_list.size());
                    auto vertex = vertex_list[vertex_index];

                    std::string uv_index_str;
                    std::getline(face_element, uv_index_str, '/');
                    auto uv = [&]() {
                        if (uv_index_str != "") {
                            auto uv_index = std::stoull(uv_index_str) - 1;
                            assert(uv_index < uv_list.size());
                            return uv_list[uv_index];
                        } else {
                            return Vec {};
                        }
                    }();

                    std::string normal_index_str;
                    std::getline(face_element, normal_index_str, '/');
                    auto normal_index = std::stoull(normal_index_str) - 1;
                    assert(normal_index < normal_list.size());
                    auto normal = normal_list[normal_index];

                    polygon.add({ vertex, normal, uv });
                }

                model.add(polygon);

                break;
            }
            case Keyword::Material: {
                material = *word_it;
                break;
            }
            case Keyword::MaterialLibrary:
            case Keyword::Comment:
            case Keyword::Smoothing:
                break;
        }
    }

    return model;
}
