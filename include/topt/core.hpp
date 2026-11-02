#ifndef __TOPT_OPTCORE_HPP
#define __TOPT_OPTCORE_HPP 0

#include<topt/matrix.hpp>

#define TOPT_SELLMEIER_N 3

namespace topt {

    struct Ray {
        double  waveLength; // [um]
        Vec<3>  origin;
        Vec<3>  direction;
        int     valid = 1;
    };

    struct Glass {
        double  k[TOPT_SELLMEIER_N];
        double  l[TOPT_SELLMEIER_N];
        char    label[8];
    };

    const Glass     air = { { 0, 0, 0 }, { 0, 0, 0 }, "AIR" };

    const double    d_inf = 1.0 / 0.0;

    // Flaunhofer lines [um]
    const double    ray_C = 0.656281;
    const double    ray_d = 0.587562;
    const double    ray_e = 0.546073;
    const double    ray_F = 0.486134;
    const double    ray_g = 0.435835;

    double getRefractance(const Glass&, double);
    int refract(double, double, const Vec<3>&, const Vec<3>&, Vec<3>&);

}

#endif