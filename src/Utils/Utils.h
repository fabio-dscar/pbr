#ifndef __PBR_UTILS_H__
#define __PBR_UTILS_H__

#include <string>
#include <fstream>

namespace pbr {
    namespace Utils {
        bool readFile  (const std::string& filePath, std::ios_base::openmode mode, std::string& str);
        void throwError(const std::string& error);
    }
}

#endif