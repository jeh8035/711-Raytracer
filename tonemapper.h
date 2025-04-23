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

class WardTonemapping : Tonemapper {
    private:
        float ld_max;

    public:
        WardTonemapping(float _ld_max);
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const;
};

class ReinhardTonemapping : Tonemapper {
    private:
        float ld_max;
        float key_value;

    public:
        ReinhardTonemapping(float _ld_max, float _key_value);
        virtual void Tonemap(std::vector<std::vector<Primitives::Color>>& irradiances) const;
};

#endif