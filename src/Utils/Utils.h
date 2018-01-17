#ifndef __PBR_UTILS_H__
#define __PBR_UTILS_H__

#include <string>
#include <fstream>

#include <PBR.h>
#include <Shape.h>
#include <ParameterMap.h>

namespace pbr {
    namespace Utils {
        bool readFile  (const std::string& filePath, std::ios_base::openmode mode, std::string& str);
        void throwError(const std::string& error);

        sref<Shape> loadSceneObject(const std::string& folder);
        RRID loadTexture(const std::string& path);
        sref<Material> buildMaterial(const std::string& path, const ParameterMap& map);
    }
}

#endif