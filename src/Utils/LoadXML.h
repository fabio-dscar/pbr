#ifndef __PBR_LXML_H__
#define __PBR_LXML_H__

#include <PBR.h>

#include <pugixml.hpp>
#include <ParameterMap.h>

namespace pbr {

    class LoadXML {
    public:
        pugi::xml_document doc;
        pugi::xml_parse_result result;
        ParameterMap map;

        LoadXML(string filename);
        void processXML(); //processa os nodes do xml para popular as listas de floats,rgb e texturas para depois então ser realizado os metodos abaixo
        void populateFloatMap(const pugi::xml_node root); //popula o parameterMap com os nomes(difuso, bump etc) e o seu valor float
        void populateRgbMap(const pugi::xml_node root); //popula o parameterMap com os nomes(difuso, bump etc) e o seu vetor
        void populateTextureMap(const pugi::xml_node root); //popula o parameterMap com os nomes(difuso, bump etc) e caminhos das texturas
        void populateInfoMap(const pugi::xml_node root); //adiciona informação do material extra como o seu id e o seu tipo(unreal, blinn etc)
    };

}

#endif