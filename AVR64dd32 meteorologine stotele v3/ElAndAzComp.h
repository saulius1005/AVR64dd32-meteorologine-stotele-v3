/*
 * ElAndAzComp.h
 *
 * Created: 2024-12-10 18:44:47
 * Author: Saulius
 *
 * This header file contains the declaration of the `SunAngles` structure and the external
 * `SUN` variable used to store and manipulate solar angle data and sun level information.
 */

#ifndef ELANDAZCOMP_H_
#define ELANDAZCOMP_H_

/**
 * @brief Structure to hold solar angle data.
 *
 * This structure stores the solar elevation and azimuth angles, along with their adjusted
 * values considering the refraction effect, and the sun level measured from an ADC.
 */
typedef struct {
    float elevation;       ///< The solar elevation angle in degrees.
    float azimuth;         ///< The solar azimuth angle in degrees.
    float adjelevation;    ///< The adjusted solar elevation considering refraction (in degrees).
    float adjazimuth;      ///< The adjusted solar azimuth (in degrees).
    uint16_t sunlevel;     ///< The measured sun level, typically obtained from an ADC (scaled value).
} SunAngles;

extern SunAngles SUN;  ///< External instance of the SunAngles structure to store solar data.

#endif /* ELANDAZCOMP_H_ */
