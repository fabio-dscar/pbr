#include <LoadXML.h>

#include <PBRMath.h>

#include <sstream>

using namespace pbr;

LoadXML::LoadXML(string filename) {
	//Ler XML
	result = doc.load_file(filename.c_str());

	/*if (result)
	{
		std::cout << "XML [" << filename.c_str() << "] parsed without errors, attr value: [" << doc.child("material").attribute("type").value() << "]\n\n";
	}
	else
	{
		std::cout << "XML [" << filename.c_str() << "] parsed with errors, attr value: [" << doc.child("material").attribute("type").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << (filename.c_str() + result.offset) << "]\n\n";
	}*/

	processXML();
	bool um = map.hasFloat("diffuse");
	bool dois = map.hasRGB("metallic");
	bool tres = map.hasTexture("irradiance");

}

//processa os nodes do xml para popular as listas de floats,rgb e texturas para depois então ser realizado os metodos abaixo
void LoadXML::processXML() {
	pugi::xml_node root = doc.document_element();
	//pugi::xml_node root = doc.child("material");//doc.first_child();
	populateFloatMap(root);
	populateRgbMap(root);
	populateTextureMap(root);
	populateInfoMap(root);
};

//processa a lista dos floats para colocar no parameterMap
void LoadXML::populateFloatMap(const pugi::xml_node root) {
	
	for (pugi::xml_node floatElem : root.children("float")) {
		std::string name = std::string(floatElem.attribute("name").as_string());
		std::string valor = std::string(floatElem.attribute("value").as_string());
		float value = floatElem.attribute("value").as_float();
		map.setFloat(name, value); //adiciona o float e o nome ao mapa
	}
}; 

//processa a lista dos rgb para colocar no parameterMap
void LoadXML::populateRgbMap(const pugi::xml_node root) {
	for (pugi::xml_node rgbElems : root.children("rgb")) {
		std::string name = std::string(rgbElems.attribute("name").as_string());
		std::string valor = std::string(rgbElems.attribute("value").as_string());
		std::string value = std::string(rgbElems.attribute("value").as_string());
		stringstream ss(value);
		Vec3 vec(0);
		ss >> vec;
		//falta por no parameter map
		map.setRGB(name, vec); //adiciona o rgb e o nome ao mapa
	}
}; 

//processa a lista das texturas para colocar no parameterMap
void LoadXML::populateTextureMap(const pugi::xml_node root) {
	for (pugi::xml_node textElems : root.children("texture")) {
		std::string name = std::string(textElems.attribute("name").as_string());
		//std::string valor = std::string(textElems.attribute("value").as_string());
		std::string value = std::string(textElems.attribute("value").as_string());
		map.setTexture(name, value); //adiciona o float e o nome ao mapa
	}
};

//processa a lista das texturas para colocar no parameterMap
void LoadXML::populateInfoMap(const pugi::xml_node root) {	
    std::string name = std::string(root.attribute("id").as_string());
    std::string value = std::string(root.attribute("type").as_string());
    map.setMatInfo(name, value); //adiciona o float e o nome ao mapa
};