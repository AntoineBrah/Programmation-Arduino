#ifndef __PARKINGS_H__
#define __PARKINGS_H__

#include <ctime>
#include <string>
#include <Arduino.h>


/**
 * A parking is identified by a string ID, has a name, a longitude and
 * a latitude.
 *
 * The parking ID is the string used to identify the parking on
 * https://data.montpellier3m.fr/dataset/disponibilite-des-places-dans-les-parkings-de-montpellier-mediterranee-metropole
 * If the parking is not elligible for the service, then the ID is the
 * NULL C string.
 */

/**
 * Structure to handle available informations on parkings available
 * through the Montpellier3M open-data services.
 */ 

typedef struct {
  float distance;
  float duration;
} parking_dist_t;

typedef struct {
  char dateTime[20];
  bool open;
  unsigned int free;
  unsigned int total;
} parking_data_t;


typedef struct {
  const char* id;
  const char* name;
  const char* shortName;
  const float longitude;
  const float latitude;
  parking_data_t parkingData;
  parking_dist_t parkingDist;
  float parkingScore;
} parking_t;

extern const int PARKING_NUMBER;
extern const int PARKING_NUMBER_WITH_DATA;

/**
 * The array containing parking descriptions.
 *
 * It can be iterated using the id attribute to enumerate parkings
 * that can be queried for their free places (by ending the
 * enumeration when the current parking id attribute is NULL) or by
 * the name attribute to include also parkings (by ending the
 * enumeration when the current parking * name attribute is NULL).
 */
extern parking_t parkings[];


/**
 * Convert xml string into a parking_data_t structure
 */
void parseXML(const char* xml, parking_data_t* parkingData);
void extractDataFromJson(const char* json, parking_dist_t* parkingDist);
float getFloat(const char *str);
void printParking(parking_t* parking);
void scoreAssignment(parking_t* parking);
void printBestParking();

const char* buildURL(const char *id);
const char* distanceBetweenTwoPointsURL(const float lat1, const float long1, const float lat2, const float long2);

#endif
