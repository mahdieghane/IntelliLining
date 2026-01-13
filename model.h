#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

#define NUM_SENSORS 5
#define FEATURES_PER_SENSOR 2
#define FEATURES (NUM_SENSORS * FEATURES_PER_SENSOR)
#define CLASSES 4

int classify_activity(int8_t *features);

#endif
