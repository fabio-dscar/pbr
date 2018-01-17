#include <Utils.h>

#include <iostream>

#include <Mesh.h>
#include <LoadXML.h>
#include <PBRMaterial.h>

using namespace pbr;

bool Utils::readFile(const std::string& filePath, std::ios_base::openmode mode, std::string& str) {
    std::ifstream file(filePath, mode);
    if (file.fail()) {
        perror(filePath.c_str());
        return false;
    }

    file.seekg(0, std::ios::end);
    str.reserve((size_t)file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    return true;
}

void Utils::throwError(const std::string& error) {
    std::cerr << error << std::endl;
    std::cin.get();
    exit(EXIT_FAILURE);
}

sref<Shape> Utils::loadSceneObject(const std::string& folder) {
    sref<Shape> obj = make_sref<Mesh>("Objects/" + folder + "/" + folder + ".obj");

    LoadXML loader("Objects/" + folder + "/material.xml");
    sref<Material> mat = buildMaterial("Objects/" + folder, loader.map);

    obj->prepare();
    obj->setMaterial(mat);

    return obj;
}

RRID Utils::loadTexture(const std::string& path) {
    Image image;
    TexSampler texSampler;

    image.loadImage(path);

    RRID rrid = RHI.createTexture(image, texSampler);

    return rrid;
}

sref<Material> Utils::buildMaterial(const std::string& path, const ParameterMap& map) {
    sref<PBRMaterial> mat = make_sref<PBRMaterial>();

    if (map.hasRGB("diffuse"))
        mat->setDiffuse(Color(map.getRGB("diffuse")));
    else if (map.hasTexture("diffuse"))
        mat->setDiffuse(loadTexture(path + "/" + map.getTexture("diffuse")));
    else
        mat->setDiffuse(Color(0.5f, 0.5f, 0.5f));

    if (map.hasTexture("normal"))
        mat->setNormal(loadTexture(path + "/" + map.getTexture("normal")));

    if (map.hasRGB("specular"))
        mat->setSpecular(Color(map.getRGB("specular")));
    else
        mat->setSpecular(Color(0.04f, 0.04f, 0.04f));

    if (map.hasFloat("roughness"))
        mat->setRoughness(map.getFloat("roughness"));
    else if (map.hasTexture("roughness"))
        mat->setRoughness(loadTexture(path + "/" + map.getTexture("roughness")));
    else
        mat->setRoughness(0.2f);

    if (map.hasFloat("metallic"))
        mat->setMetallic(map.getFloat("metallic"));
    else if (map.hasTexture("metallic"))
        mat->setMetallic(loadTexture(path + "/" + map.getTexture("metallic")));
    else
        mat->setMetallic(0.5f);

    return mat;
}