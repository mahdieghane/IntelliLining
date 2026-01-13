#include "model.h"

static int8_t weights[CLASSES][FEATURES] = {
    // These values come from Python training (exported)
    { 12, -8,  5,  3,  7, -2,  6, -1,  4,  9 },
    { -6, 11, -3,  8, -4,  5, -7,  6, -2, 10 },
    {  4,  3,  9, -6,  8, -5, 10, -3,  7, -4 },
    { -8, -4,  6, 10, -7,  9, -2,  5, -6,  3 }
};

static int8_t bias[CLASSES] = { 3, -2, 1, 0 };

int classify_activity(int8_t *x) {
    int best_class = 0;
    int32_t best_score = -1000000;

    for (int c = 0; c < CLASSES; c++) {
        int32_t score = bias[c];
        for (int i = 0; i < FEATURES; i++) {
            score += weights[c][i] * x[i];
        }
        if (score > best_score) {
            best_score = score;
            best_class = c;
        }
    }
    return best_class;
}
