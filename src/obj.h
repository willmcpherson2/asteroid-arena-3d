#ifndef OBJ_H
#define OBJ_H

#include "object.h"
#include <string>

namespace obj {

Model load(const std::string& obj_filename, const std::string& mtl_filename);

}

#endif
