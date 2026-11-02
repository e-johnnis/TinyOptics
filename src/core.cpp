#include<topt.hpp>
#include<topt/color.hpp>
#include<cmath>

namespace topt {

    double getRefractance(const Glass& g, double waveLength) {
        double r2 = 1;
        double wl2 = waveLength * waveLength;
        for(int i = 0; i < TOPT_SELLMEIER_N; i++) {
            r2 += g.k[i] * wl2 / (wl2 - g.l[i]);
        }
        return std::sqrt(r2);
    }

    int refract(double n1, double n2, const Vec<3>& vn, const Vec<3>& vi, Vec<3>& vr) {
        double ne = n1 / n2;
        Vec<3> vs = -vi / vi.size();

        double sn = vs * vn;
        double D = 1.0 - ne * ne * (1.0 - sn * sn);
        int valid = (D >= 0);

        vr = valid ? -ne * (vs - vn * sn) - vn * std::sqrt(D) : vi;

        return valid;
    }

    int wl2color(double waveLength) {
        double ip = -1;
        double t = std::modf((waveLength - cie2006.wl0) / cie2006.dwl, &ip);
        int i = (int)ip;
        if(i < 0 || i >= 88) return 0x000000FF;


        Vec<3> xyz = (1-t) * cie2006.values[i] + t * cie2006.values[i+1];
        Mat<3> xyz2rgb = {
            2.36461385,-0.89654057,-0.46807238,
           -0.51516621, 1.42640810, 0.08875810,
            0.00520370,-0.01440816, 1.00920446
        };
        Vec<3> rgb = xyz2rgb * xyz;

        RgbColor col = {
            .a = 255,
            .b = (unsigned char)(std::clamp(rgb[2], 0.0, 1.0) * 255),
            .g = (unsigned char)(std::clamp(rgb[1], 0.0, 1.0) * 255),
            .r = (unsigned char)(std::clamp(rgb[0], 0.0, 1.0) * 255)
        };
        return col.code;
    }

}