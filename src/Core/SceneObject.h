#ifndef __PBR_SCENEOBJ_H__
#define __PBR_SCENEOBJ_H__

#include <PBR.h>
#include <PBRMath.h>

using namespace pbr::math;

namespace pbr {

    class SceneObject {
    public:
        SceneObject();
        SceneObject(const Vec3& position);
        SceneObject(const Mat4& objToWorld);

        const Vec3& position()    const;
        const Vec3& scale()       const;
        const Quat& orientation() const;
        const Mat4& objToWorld()  const;

        sref<SceneObject> parent() const;

        void setPosition(const Vec3& position);
        void setScale(float x, float y, float z);
        void setOrientation(const Quat& quat);
        void setObjToWorld(const Matrix4x4& mat);

        virtual void updateMatrix();

    protected:
        Quat _orientation;
        Vec3 _scale;
        Vec3 _position;

        Mat4 _objToWorld;

        sref<SceneObject> _parent;
    };

}

#endif
