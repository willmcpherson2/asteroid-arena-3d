#ifndef OBJ_H
#define OBJ_H

#include "math.h"
#include <string>
#include <vector>

namespace obj {

std::vector<Vec> load(const std::string& filename);

}

#endif
