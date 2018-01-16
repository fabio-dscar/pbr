#ifndef __PBR_MATERIAL_H__
#define __PBR_MATERIAL_H__

#include <PBR.h>

#include <Skybox.h>

namespace pbr {

    class PBR_SHARED Material {
    public:
        Material() { }

        void use() const;
        RRID program() const;

        virtual void update(const Skybox& skybox) = 0;
        virtual void uploadData() const = 0;

    protected:
        RRID _prog;
    };

}

#endif