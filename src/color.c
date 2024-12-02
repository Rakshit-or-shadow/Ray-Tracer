#include "color.h"
#include <stdio.h>
#include <math.h>


// Remove the redundant Vec3 definition

// Function to convert HEX to RGB [0, 1]
Vec3 unpackRGB(unsigned int packedRGB) {
    Vec3 color;
    // Extract the RGB values from the packed integer
    color.x = ((packedRGB >> 16) & 0xFF) / 255.0f;
    color.y = ((packedRGB >> 8) & 0xFF) / 255.0f;
    color.z = (packedRGB & 0xFF) / 255.0f;
    return color;
}

// Comparison function for qsort
int compareColor(const void *a, const void *b) {
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++) {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }
    // Compare the RGB values of the two colors and return the result
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}

// Function to write color to file
void writeColour(FILE *ppmFile, Vec3 color) {
    // Convert the color values to [0, 255] range
    int r = (int)(255.999 * fmin(fmax(color.x, 0.0), 1.0));
    int g = (int)(255.999 * fmin(fmax(color.y, 0.0), 1.0));
    int b = (int)(255.999 * fmin(fmax(color.z, 0.0), 1.0));
    fprintf(ppmFile, "%d %d %d ", r, g, b);
}