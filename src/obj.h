#ifndef OBJ_H
#define OBJ_H

#include "object.h"
#include <string>

namespace obj {

Model load(const std::string& obj_filename = "", const std::string& mtl_filename = "", const std::string& image_filename = "", int width = 0, int height = 0);

}

#endif
