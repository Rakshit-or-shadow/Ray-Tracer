// file: assg.h

#ifndef ASSG_H
#define ASSG_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"

// structures for vectors and rays
typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    Vec3 position;
    float brightness;
} Light;

// global camera and viewport parameters
struct {
    Vec3 position;
    float focal_length;
    float viewport_height;
    float viewport_width;
    float x_value_pixel;  // x between pixel
    float y_value_pixel;  // y between pixel
    Vec3 first_pixel;
} Camera;

// function prototypes
void Initialize_camera(float focal_length, float viewport_height, int image_width, int image_height);
Ray produce_ray(Vec3 pixel_position);
Ray ray_generation(int x, int y, int image_width, int image_height);
Vec3 calculateColor(Ray ray, World *world, Light light_source, Vec3 backgroundcolor);
Vec3 calculateLighting(Vec3 p, Vec3 n, Vec3 sphereColor, Light light);
Vec3 CalculateShadow(Vec3 p, Vec3 n, Vec3 surfaceLightingColor, Vec3 d, Sphere **spheres, int sphereCount, Vec3 lightPosition);
Vec3 calculateLightingAndShadows(Vec3 p, Vec3 n, Vec3 sphereColor, Light light, Sphere **spheres, int sphereCount);
void init_scene(Vec3 light_vector, float light_intensity, Light light_source);
float calculate_light_intensity(Vec3 point, Light light_source);
void readColors(FILE *input_file, unsigned int *colors, int total_colors);
void readSpheres(FILE *input_file, int total_spheres, World *world, unsigned int *colors);
void printVectorOperations(FILE *output_file, Vec3 backgroundcolor, Vec3 globallight_position, float viewport_width);
void printSphereOperations(FILE *output_file, Vec3 globallight_position, World world);
void renderScene(World *world, int image_width, int image_height, FILE *output_file, Light light_source, Vec3 backgroundcolor);
void anti_aliasing(World *world, int image_width, int image_height, FILE *output_file, Light light_source, Vec3 backgroundcolor);
void readAndSortColors(unsigned int *colors, int numColors);
void assignColors(unsigned int *colors, int numColors, int *indices, int numIndices, Vec3 *assignedColors);

#endif // ASSG_H