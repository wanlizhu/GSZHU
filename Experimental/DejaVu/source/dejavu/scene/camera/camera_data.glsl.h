struct CameraData
{
    mat4 viewMat;
    mat4 projMat;
    mat4 viewProjMat;
    mat4 viewProjMatInv;          // Camera inverse view-projection matrix
    mat4 viewProjMatNoJitter;     // Camera view-projection matrix. No jittering is applied!
    mat4 viewProjMatNoJitterPrev; // Camera view-projection matrix associated to previous frame. No jittering is applied!

    vec3  posWorld = vec3(0, 0, 0); // Camera world-space position
    float focalLength = 21.0f;        // Camera focal length in mm. Default is 59 degree vertical, 90 horizontal FOV at 16:9 aspect ratio
    vec3  up = vec3(0, 1, 0);       // Camera world-space up vector
    float aspectRatio = 1.7777f;      // 16:9 aspect-ratio
    vec3  target = vec3(0, 0, -1);  // Camera target point in world-space
    float nearZ = 0.1f;               // Camera near plane
    vec3  cameraU = vec3(0, 0, 1);  // Camera base vector U. Normalized it indicates the right image plane vector. The length is dependent on the FOV
    float farZ = 1000.0f;             // Camera far plane
    vec3  cameraV = vec3(0, 1, 0);  // Camera base vector V. Normalized it indicates the up image plane vector. The length is dependent on the FOV
    float jitterX = 0.0f;             // Eventual camera jitter along the x axis expressed as a subpixel offset divided by screen width (positive value shifts the image right)
    vec3  cameraW = vec3(1, 0, 0);  // Camera base vector W. Normalized it indicates the forward direction. The length is the camera focal distance
    float jitterY = 0.0f;             // Eventual camera jitter along the y axis expressed as a subpixel offset divided by screen height (positive value shifts the image up)

    float  frameHeight = 24.0f;        // Camera film plane height in mm. 24 is the height of a 35mm film
    float  focalDistance = 10000.0f;   // Camera focal distance in scene units
    float  apertureRadius = 0.0f;      // Camera aperture radius in scene units
    float  shutterSpeed = 0.004f;      // Camera shutter speed in seconds
    float  isoSpeed = 100.0f;          // Camera film speed based on ISO standards
    
    vec3 __unused_padding;
};