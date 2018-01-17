#include <ParameterMap.h>

using namespace std;
using namespace pbr;
using namespace pbr::math;

ParameterMap::ParameterMap() {

}

//retorna true se o nome tem type float
bool ParameterMap::hasFloat(string name) const {
	/*for (auto& item : _floats){ //prints
	std::cout << item.first << ": " << item.second << std::endl;
	}*/

	auto hasFloat = _floats.find(name); //fica com a posição do iterador
	return hasFloat != _floats.end(); //se a posição é o end() é pk não encontrou caso contrario encontrou

};

//retorna true se o nome tem type RGB
bool ParameterMap::hasRGB(string name) const {
	/*for (auto& item : _rgb){
	std::cout << item.first << ": " << item.second << std::endl;
	}*/
	auto hasRGB = _rgb.find(name); //fica com a posição do iterador
	return hasRGB != _rgb.end(); //se a posição é o end() é pk não encontrou caso contrario encontrou
	
};

bool ParameterMap::hasTexture(string name) const {
	/*for (auto& item : _textures){
	std::cout << item.first << ": " << item.second << std::endl;
	}*/
	auto hasTexture = _textures.find(name); //fica com a posição do iterador
	return hasTexture != _textures.end(); //se a posição é o end() é pk não encontrou caso contrario encontrou
	
}; //retorna true se o nome tem type Texture

bool ParameterMap::hasBlinn() {
	/*for (auto& item : _textures){
	std::cout << item.first << ": " << item.second << std::endl;
	}*/
	auto hasBlinn =_matInfo.find("blinn"); //fica com a posição do iterador
	return hasBlinn != _matInfo.end(); //se a posição é o end() é pk não encontrou caso contrario encontrou

}; //retorna true se o nome tem type Blinn

bool ParameterMap::hasUnreal() {
	/*for (auto& item : _textures){
	std::cout << item.first << ": " << item.second << std::endl;
	}*/
	auto hasUnreal = _matInfo.find("unreal"); //fica com a posição do iterador
	return hasUnreal != _matInfo.end(); //se a posição é o end() é pk não encontrou caso contrario encontrou

}; //retorna true se o nome tem type Blinn

   //popula o parameterMap com o o nome do float e com o seu valor
void ParameterMap::setFloat(string name, float value) {
	std::pair<std::string, float> floats(name, value);
	_floats.insert(floats);
};

//popula o parameterMap com o o nome do rgb e com o seu valor
void ParameterMap::setRGB(string name, Vec3 value) {
	std::pair<std::string, Vec3> rgbs(name, value);
	_rgb.insert(rgbs);
};

//popula o parameterMap com o o nome da textura e com o seu caminho
void ParameterMap::setTexture(string name, string value) {
	std::pair<std::string, std::string> textures(name, value);
	_textures.insert(textures);
};

//popula o parameterMap com o o nome da textura e com o seu valor
void ParameterMap::setMatInfo(string name, string value) {
	std::pair<std::string, std::string> matInfo(name, value);
	_matInfo.insert(matInfo);
};

float ParameterMap::getFloat(string name) const {
	auto _float = _floats.find(name);
	return _float -> second;
}

Vec3 ParameterMap::getRGB(string name) const {
	auto _RGB = _rgb.find(name);
	return _RGB -> second;
}

string ParameterMap::getTexture(string name) const {
	auto _texture = _textures.find(name);
	return _texture -> second;
}

string ParameterMap::getMatInfo(string name) const {
	auto _info = _matInfo.find(name);
	return _info -> second;
}
