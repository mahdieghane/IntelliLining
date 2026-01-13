#include <Arduino.h>
#include <math.h>
#include "model.h"

#define WINDOW_SIZE 256
#define SAMPLE_RATE 100  // Hz

static int16_t sensor_buffer[NUM_SENSORS][WINDOW_SIZE];
static int write_idx = 0;

int adc_pins[NUM_SENSORS] = {32, 33, 34, 35, 36};
void read_sensors(int16_t samples[NUM_SENSORS]) {
    for (int i = 0; i < NUM_SENSORS; i++) {
        samples[i] = analogRead(adc_pins[i]);
    }
}
void push_samples(int16_t samples[NUM_SENSORS]) {
    for (int s = 0; s < NUM_SENSORS; s++) {
        sensor_buffer[s][write_idx] = samples[s];
    }
    write_idx = (write_idx + 1) % WINDOW_SIZE;
}
void extract_features(float *features) {
    int idx = 0;

    for (int s = 0; s < NUM_SENSORS; s++) {
        int64_t sum_sq = 0;

        for (int i = 0; i < WINDOW_SIZE; i++) {
            int32_t v = sensor_buffer[s][i];
            sum_sq += (int64_t)v * v;
        }

        float rms = sqrtf((float)sum_sq / WINDOW_SIZE);
        float energy = (float)sum_sq / WINDOW_SIZE;

        features[idx++] = rms;
        features[idx++] = energy;
    }
}
void setup() {
    Serial.begin(115200);
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(adc_pins[i], INPUT);
    }
}
void loop() {
    static int sample_count = 0;
    int16_t samples[NUM_SENSORS];

    read_sensors(samples);
    push_samples(samples);
    sample_count++;

    if (sample_count >= WINDOW_SIZE) {
        float feats_f[FEATURES];
        extract_features(feats_f);

        int8_t feats_q[FEATURES];
        for (int i = 0; i < FEATURES; i++) {
            feats_q[i] = (int8_t)(feats_f[i] / 50.0f);
        }

        int activity = classify_activity(feats_q);

        Serial.print("Activity: ");
        Serial.println(activity);

        sample_count = 0;
    }

    delay(1000 / SAMPLE_RATE);
}
