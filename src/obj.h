#ifndef OBJ_H
#define OBJ_H

#include "math.h"
#include "object.h"
#include <string>
#include <vector>

namespace obj {

Model load(const std::string& filename);

}

#endif
