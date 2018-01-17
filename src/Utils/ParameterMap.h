#ifndef __PBR_PMAP_H__
#define __PBR_PMAP_H__

#include <iostream>
#include <string>
#include <unordered_map>

#include <PBR.h>
#include <PBRMath.h>

using namespace std;
using namespace pbr::math;

namespace pbr {

    class ParameterMap {
    public:
        unordered_map<string, float>  _floats;   // name e o value
        unordered_map<string, string> _textures; // name e o caminho
        unordered_map<string, Vec3>   _rgb;      // name e o vetor do rgb
        unordered_map<string, string> _matInfo;  // name e o vetor do rgb

        ParameterMap();

        //retorna true se o nome tem type float
        bool hasFloat(string name) const;

        //retorna true se o nome tem type RGB
        bool hasRGB(string name) const;

        //retorna true se o nome tem type Texture
        bool hasTexture(string name) const;

        bool hasBlinn();

        bool hasUnreal();

        //popula o parameterMap com o o nome do float e com o seu valor
        void setFloat(string name, float value);

        //popula o parameterMap com o o nome do rgb e com o seu valor
        void setRGB(string name, Vec3 value);

        //popula o parameterMap com o o nome da textura e com o seu caminho
        void setTexture(string name, string value);

        //popula o parameterMap com o id do material e com o seu tipo(unreal, blinn etc.)
        void setMatInfo(string name, string value);

        float getFloat(string name) const;

        Vec3 getRGB(string name) const;

        string getTexture(string name) const;

        string getMatInfo(string name) const;

    };

}

#endif