#ifndef LIGHTS_HEADER
#define LIGHTS_HEADER

#include "raylib.h"

// Max dynamic lights supported by shader
#define MAX_LIGHTS  4

typedef struct {   
    int type;
    bool enabled;
    Vector3 position;
    Vector3 target;
    Color color;
    float attenuation;
    float intensity;
    
    // Shader locations
    int enabledLoc;
    int typeLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int attenuationLoc;
    int intensityLoc;
} Light;

// Light type
typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT
} LightType;

Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);   // Create a light and get shader locations
void UpdateLightValues(Shader shader, Light light);         // Send light properties to shader

#endif

