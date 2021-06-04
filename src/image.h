#ifndef IMAGE_H
#define IMAGE_H

#include "object.h"
#include <string>

namespace image {

Image load(const std::string& filename, int width, int height);

}

#endif
