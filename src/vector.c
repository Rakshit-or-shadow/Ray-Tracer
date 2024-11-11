// File: src/assg.c
// Name- Rakshit Singh
// Student ID- 1824303

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"

// adds two vectors
Vec3 add(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; 
}

// subtracts the second vector from the first
Vec3 subtract(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; 
}

// multiplies a vector by a scalar
Vec3 scalarMultiply(float s, Vec3 v) {
    return (Vec3){s * v.x, s * v.y, s * v.z}; 
}

// divides a vector by a scalar
Vec3 scalarDivide(Vec3 v, float d) {
    return (Vec3){v.x / d, v.y / d, v.z / d}; 
}

// computes the dot product of two vectors
float dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

// computes the squared length of a vector
float length2(Vec3 v) {
    return dot(v, v); 
}

// computes the length of a vector
float length(Vec3 v) {
    return sqrt(length2(v)); 
}

// computes the squared distance between two vectors
float distance2(Vec3 v1, Vec3 v2) {
    Vec3 diff = subtract(v1, v2); 
    return length2(diff); 
}

// computes the distance between two vectors
float distance(Vec3 v1, Vec3 v2) {
    return sqrt(distance2(v1, v2)); 
}

// normalizes a vector
Vec3 normalize(Vec3 v) {
    float len = length(v); // compute the length
    if (len == 0.0f) return v; 
    // return the normalized vector
    return scalarDivide(v, len); 
}