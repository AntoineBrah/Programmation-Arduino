#include "parkings.h"
#include <iostream>
#include <cstdio>
#include <sstream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

#define PARKING_NUMBER 41
#define PARKING_NUMBER_WITH_DATA 23

// Données de géolocalisation : https://data.montpellier3m.fr/dataset/parkings-en-ouvrage-de-montpellier
// Identifants : https://data.montpellier3m.fr/dataset/disponibilite-des-places-dans-les-parkings-de-montpellier-mediterranee-metropole
parking_t parkings[] = {
                             /* ID,            name,                      shortname,          longitude,         latitude,           parking_data    parking_dist  parking_score */
                             { "FR_MTP_TRIA",  "Triangle",                "Triangle",         3.881844180000000, 43.609233840000002, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_ANTI",  "Antigone",                "ANTI",             3.888818930000000, 43.608716059999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_COME",  "Comédie",                 "COME",             3.879761960000000, 43.608560920000002, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_EURO",  "Europa",                  "EURO",             3.892530740000000, 43.607849710000004, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_CORU",  "Corum",                   "CORU",             3.882257730000000, 43.613888209999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_FOCH",  "Foch Préfecture",         "FOCH",             3.876570840000000, 43.610749120000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_GAMB",  "Gambetta",                "GAMB",             3.871374360000000, 43.606951379999998, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_GARE",  "Saint Roch",              "GARE",             3.878550720000000, 43.603291489999997, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_ARCT",  "Arc de Triomphe",         "ARCT",             3.873200750000000, 43.611002669999998, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_PITO",  "Pitot",                   "Pitot",            3.870191170000000, 43.612244939999997, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_CIRC",  "Circé Odysseum",          "CIRC",             3.917849500000000, 43.604953770000002, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_SABI",  "Sabines",                 "SABI",             3.860224600000000, 43.583832630000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_GARC",  "Garcia Lorca",            "GARD",             3.890715800000000, 43.590985089999997, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_SABL",  "Notre Dame de Sablassou", "SABL",             3.922295360000000, 43.634191940000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_MOSS",  "Mosson",                  "MOSS",             3.819665540000000, 43.616237159999997, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_STJ_SJLC",  "Saint-Jean-le-Sec",       "SJLC",             3.837931200000000, 43.570822249999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_MEDC",  "Euromédecine",            "MEDC",             3.827723650000000, 43.638953590000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_OCCI",  "Occitanie",               "OCCI",             3.848597960000000, 43.634562320000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_CAS_CDGA",  "Charles de Gaulle",       "CDGA",             3.897762100000000, 43.628542119999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_ARCE",  "Arceaux",                 "ARCE",             3.867490670000000, 43.611716469999998, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_POLY",  "Polygone",                "POLY",             3.884765390000000, 43.608370960000002, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_GA109", "Multiplexe (est)",        "GAUMONT-EST",      3.918980000000000, 43.605060000000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { "FR_MTP_GA250", "Multiplexe (ouest)",      "GAUMONT-OUEST",    3.914030000000000, 43.604000000000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             // Les parkings ci-après n'ont pas de données temps réel sur le site de Montpellier 3M
                             // Je les mets en commentaire étant donné qu'on ne s'en sert pas (ils prennent de la mémoire pour rien et on est pas mal limité avec l'esp...)
                             /*{ 0,              "Peyrou",                      0,              3.870383780000000, 43.611297000000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Hôtel de ville",              0,              3.895853270000000, 43.599231000000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Jacou",                       0,              3.912884750000000, 43.654598700000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Georges Pompidou",            0,              3.921084190000000, 43.649339200000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Via Domitia",                 0,              3.929538080000000, 43.646658010000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Juvignac",                    0,              3.809621860000000, 43.617403740000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Saint-Jean-de-Védas Centre",  0,              3.830585520000000, 43.574962790000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Lattes",                      0,              3.904817620000000, 43.570809879999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Parc expo",                   0,              3.945678520000000, 43.572910210000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Pérols centre",               0,              3.957355560000000, 43.565378570000000, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Décathlon",                   0,              3.923800380000000, 43.606185590000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Ikéa",                        0,              3.925582560000000, 43.604609619999998, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Géant Casino",                0,              3.922104130000000, 43.603155600000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Mare Nostrum",                0,              3.919015140000000, 43.602370800000003, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Végapolis",                   0,              3.914773710000000, 43.602896510000001, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "Multiplexe",                  0,              3.914110760000000, 43.604152429999999, { 0, 0, 0, 0 }, { 0, 0 }, 0 },
                             { 0,              "La Mantilla",                 0,              3.902399940000000, 43.598772959999998, { 0, 0, 0, 0 }, { 0, 0 }, 0 },*/
                             { 0, 0, 0, 0, 0, { 0, 0, 0, 0 }, { 0, 0 }, 0 }
};


#define MONTPELLIER3M_BASE_URL "https://data.montpellier3m.fr/"
#define MONTPELLIER3M_API_PATH_PREFIX "sites/default/files/ressources/"
#define MONTPELLIER3M_API_PATH_SUFFIX ".xml"

const char* buildURL(const char *id) {
  static char res[100];
  memset(res, 0, sizeof(res));
  strcat(res, MONTPELLIER3M_BASE_URL);
  strcat(res, MONTPELLIER3M_API_PATH_PREFIX);
  strcat(res, id);
  strcat(res, MONTPELLIER3M_API_PATH_SUFFIX);
  
  //Serial.println(res); // affichage de l'url
  
  return res;
}

#define MAP_API_BASE_URL "https://router.project-osrm.org/route/v1/car/"
#define DECIMAL_NUMBER 15

const char* distanceBetweenTwoPointsURL(const float lat1, const float long1, const float lat2, const float long2) {
  String latitude_str1 = String(lat1, DECIMAL_NUMBER);
  String longitude_str1 = String(long1, DECIMAL_NUMBER);
  String latitude_str2 = String(lat2, DECIMAL_NUMBER);
  String longitude_str2 = String(long2, DECIMAL_NUMBER);

  static char res[100];
  memset(res, 0, sizeof(res));
  strcat(res, MAP_API_BASE_URL);
  strcat(res, longitude_str1.c_str());
  strcat(res, ",");
  strcat(res, latitude_str1.c_str());
  strcat(res, ";");
  strcat(res, longitude_str2.c_str());
  strcat(res, ",");
  strcat(res, latitude_str2.c_str());

  //Serial.println(res); // affichage de l'url
  
  return res;
}

void parseXML(const char* xml, parking_data_t* parkingData) {

  XMLDocument xmldocument;

  if(xmldocument.Parse(xml) != XML_SUCCESS) {
    Serial.println("Error parsing");
    return;
  }

  XMLNode* root = xmldocument.FirstChild()->NextSibling();
   
  XMLElement* dateTime_xml = root->FirstChildElement("DateTime");
  XMLElement* shortName_xml = root->FirstChildElement("Name");
  XMLElement* status_xml = root->FirstChildElement("Status");
  XMLElement* free_xml = root->FirstChildElement("Free");
  XMLElement* total_xml = root->FirstChildElement("Total");

  const char* dateTime = dateTime_xml->GetText();
  const char* shortName = shortName_xml->GetText();
  const char* status = status_xml->GetText();
  
  unsigned int free = 0;
  unsigned int total = 0;

  stringstream freeValue(free_xml->GetText());
  freeValue >> free;

  stringstream totalValue(total_xml->GetText());
  totalValue >> total;
 
  sprintf(parkingData->dateTime, dateTime);
  parkingData->open = (strcmp(status, "Open") == 0 ? true : false);
  parkingData->free = free;
  parkingData->total = total;
}

float getFloat(const char *str){
    /* First skip non-digit characters */
    /* Special case to handle negative numbers */
    while (*str && !(isdigit(*str) || ((*str == '-' || *str == '+') && isdigit(*(str + 1)))))
        str++;
 
    /* The parse to a double */
    return strtof(str, NULL);
}

void extractDataFromJson(const char* json, parking_dist_t* parkingDist) {
  String myJson = String(json);

  int dist_index = myJson.indexOf("distance");
  int dura_index = myJson.indexOf("duration");

  String distance_str = myJson.substring(dist_index, dura_index);
  String duration_str = myJson.substring(dura_index);

  float distance = getFloat(distance_str.c_str());
  float duration = getFloat(duration_str.c_str());

  parkingDist->distance = distance;
  parkingDist->duration = duration;
}

void printParking(parking_t* parking) {
  Serial.println("========================");
  Serial.println("PARKING INFOS :");
  Serial.print("\t> id : ");
  Serial.println(parking->id);
  Serial.print("\t> name : ");
  Serial.println(parking->name);
  Serial.print("\t> shortName : ");
  Serial.println(parking->shortName);
  Serial.print("\t> longitude : ");
  Serial.println(parking->longitude);
  Serial.print("\t> latitude : ");
  Serial.println(parking->latitude);
  Serial.println("PARKING DATA :");
  Serial.print("\t> Datetime : ");
  Serial.println(parking->parkingData.dateTime);
  Serial.print("\t> Open : ");
  Serial.println(parking->parkingData.open);
  Serial.print("\t> Free : ");
  Serial.println(parking->parkingData.free);
  Serial.print("\t> Total : ");
  Serial.println(parking->parkingData.total);
  Serial.println("PARKING DIST :");
  Serial.print("\t> Distance : ");
  Serial.println(parking->parkingDist.distance);
  Serial.print("\t> Duration : ");
  Serial.println(parking->parkingDist.duration);
  Serial.println("PARKING SCORE :");
  Serial.print("\t> Score : ");
  Serial.println(parking->parkingScore);
  Serial.println("========================");
}

// The lower the score, the better
void scoreAssignment(parking_t* parking) {
  if(!parking->parkingData.open){
    parking->parkingScore = 0;
    return;
  }

  if(parking->parkingData.free)
    parking->parkingScore = (((float)parking->parkingData.total/(float)parking->parkingData.free)*parking->parkingDist.distance);
  else
    parking->parkingScore = (((float)parking->parkingData.total)*parking->parkingDist.distance);
}

// The best parking is the parking with the lowest parking score
void printBestParking() {

  parking_t* bestParking = parkings;
  int min = parkings[0].parkingScore;

  for(int i=0; i<PARKING_NUMBER_WITH_DATA; i++) {
    if(parkings[i].parkingScore <= min) {
      min = parkings[i].parkingScore;
      bestParking = &parkings[i];
    }
  }

  Serial.println("\nEn tenant compte des données de tous les parkings et en tenant compte de la distance qui sépare l'utilisateur des parkings, le meilleur parking pour se garer est : \n");
  printParking(bestParking);
}
