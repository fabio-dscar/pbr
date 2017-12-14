#ifndef __PBR_PERSPECTIVE_H__
#define __PBR_PERSPECTIVE_H__

#include <Camera.h>

namespace pbr {

    class Perspective : public Camera {
    public:
        Perspective();
        Perspective(int32 width, int32 height, const Vec3& eye,
                    const Vec3& at, const Vec3& up, float n, float f, float fov);

        float fov() const;

    private:
        float _fov;
    };

}

#endif