#include "image.h"
#include <fstream>
#include <sstream>

Image image::load(const std::string& filename, int width, int height)
{
    std::ifstream file(filename, std::ios::in);
    std::vector<uint8_t> buffer { std::istreambuf_iterator<char>(file), {} };

    Image image;

    image.width = width;
    image.height = height;

    for (size_t i = 0; i < buffer.size(); i += 4) {
        unsigned char r = buffer[i];
        unsigned char g = buffer[i + 1];
        unsigned char b = buffer[i + 2];
        unsigned char a = buffer[i + 3];
        Pixel c { r, g, b, a };
        image.pixels.push_back(c);
    }

    return image;
}
