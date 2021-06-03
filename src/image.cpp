#include "image.h"
#include <fstream>
#include <sstream>

Image image::load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in);
    std::vector<uint8_t> buffer { std::istreambuf_iterator<char>(file), {} };

    Image image;

    for (size_t i = 0; i < buffer.size(); i += 4) {
        double r = static_cast<double>(buffer[i]) / 255;
        double g = static_cast<double>(buffer[i + 1]) / 255;
        double b = static_cast<double>(buffer[i + 2]) / 255;
        double a = static_cast<double>(buffer[i + 3]) / 255;
        Colour c { r, g, b, a };
        image.pixels.push_back(c);
    }

    return image;
}
