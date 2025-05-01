#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include "Primitives/primitives.h"
#include "world.h"

class Tonemapper {
    public:
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const = 0;

        static float GetLuminance(const float& red, const float& green, const float& blue);
        static float CalculateLogAverage(const std::vector<std::vector<Primitives::Color>>& irradiances);
};

class WardTonemapper : Tonemapper {
    private:
        float ld_max;
        float adaptation_luminance;

    public:
        WardTonemapper(float _ld_max, float _adaptation_luminance);
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const;
};

class ReinhardTonemapper : Tonemapper {
    private:
        float ld_max;
        float key_value;

    public:
        ReinhardTonemapper(float _ld_max, float _key_value);
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const;
};

class AdaptiveLogTonemapper : Tonemapper {
    private:
        float ld_max;
        float bias;
        float adaptation_luminance;

    public:
        AdaptiveLogTonemapper(float _ld_max, float _bias, float _adaptation_luminance);
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const;

    private:
        float BiasFunction(const float& t) const;
};


#endif