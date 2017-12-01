#ifndef __PBR_MATERIAL_H__
#define __PBR_MATERIAL_H__

#include <PBR.h>

namespace pbr {

    class PBR_SHARED Material {
    public:
        Material() { }

        virtual void uploadData() const = 0;

    protected:

    };

}

#endif