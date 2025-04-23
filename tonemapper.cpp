#include "tonemapper.h"


float Tonemapper::GetLuminance(const float& red, const float& green, const float& blue) {
    return 0.27f * red + 0.67f * green + 0.06f * blue;
}

float Tonemapper::CalculateLogAverage(const std::vector<std::vector<Primitives::Color>>& irradiances) {
    float log_average = 0.0f;
    for (uint32_t y = 0; y < World::GetHeight(); y++) {
        for (uint32_t x = 0; x < World::GetWidth(); x++) {
            const float luminance = Tonemapper::GetLuminance(irradiances[x][y].red, irradiances[x][y].green, irradiances[x][y].blue);
            log_average += std::logf(std::max(luminance, 1.0f));
        }
    }
    log_average = expf(log_average / (World::GetWidth() * World::GetHeight()));

    return log_average;
}

WardTonemapping::WardTonemapping(float _ld_max) :
    ld_max(_ld_max)
{}

void WardTonemapping::Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const {
    float log_average = CalculateLogAverage(irradiances);

    for (uint32_t y = 0; y < World::GetHeight(); y++) {
        for (uint32_t x = 0; x < World::GetWidth(); x++) {

            const float& irradiance_r = irradiances[x][y].red;
            const float& irradiance_g = irradiances[x][y].green;
            const float& irradiance_b = irradiances[x][y].blue;

            const float sf = pow((1.219f + powf(ld_max / 2.0f, 0.4f)) / (1.219f + pow(log_average, 0.4f)), 2.5f);

            irradiances[x][y].red = std::min(1.0f, sf * irradiance_r / ld_max);
            irradiances[x][y].green = std::min(1.0f, sf * irradiance_g / ld_max);
            irradiances[x][y].blue = std::min(1.0f, sf * irradiance_b / ld_max);
        }
    }
}

ReinhardTonemapping::ReinhardTonemapping(float _ld_max, float _key_value) :
    ld_max(_ld_max),
    key_value(_key_value)
{}

void ReinhardTonemapping::Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const {
    for (uint32_t y = 0; y < World::GetHeight(); y++) {
        for (uint32_t x = 0; x < World::GetWidth(); x++) {

            const float r_scaled = (0.18f / key_value) * irradiances[x][y].red;
            const float g_scaled = (0.18f / key_value) * irradiances[x][y].green;
            const float b_scaled = (0.18f / key_value) * irradiances[x][y].blue;


            const float r_reflectance = r_scaled / (1 + r_scaled);
            const float g_reflectance = g_scaled / (1 + g_scaled);
            const float b_reflectance = b_scaled / (1 + b_scaled);


            irradiances[x][y].red = std::min(1.0f, r_reflectance * ld_max / ld_max);
            irradiances[x][y].green = std::min(1.0f, g_reflectance * ld_max / ld_max);
            irradiances[x][y].blue = std::min(1.0f, b_reflectance * ld_max / ld_max);
        }
    }
}