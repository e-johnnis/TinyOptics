#include<topt.hpp>
#include<cmath>
#include<cstdio>

namespace topt {

    const Ray Refractor::trace(Ray& rb, const Glass& gb) const {
        if(!rb.valid) return rb;

        double ne = getRefractance(gb, rb.waveLength) / getRefractance(glass, rb.waveLength);
        Vec<3> vi, vn;
        
        if(shape->getIntersection(rb.origin, rb.direction, position, vi, vn) > diameter*0.5) {
            rb.valid = 0;
            return rb;
        }

        Vec<3> vr;
        int valid = refract(getRefractance(gb, rb.waveLength), getRefractance(glass, rb.waveLength), vn, rb.direction, vr);

        return { rb.waveLength, vi, vr, valid };
    }

    Mat<2> Refractor::getGaussianMatrix(double waveLength, const Glass& gb, double& petzvarSum) const {
        double n1 = getRefractance(gb, waveLength);
        double n2 = getRefractance(glass, waveLength);
        petzvarSum += (1.0 / n1 - 1.0 / n2) / shape->radius;
        return {
            1.0,  0.0,
            (n2 - n1) / shape->radius, 1
        };
    }

    void Refractor::drawSvgModel(std::FILE* fp) const {
        shape->drawSvgModel(fp, position, diameter);
    }

    const Ray Aperture::trace(Ray& rb, const Glass& gb) const {
        if(!rb.valid) return rb;

        Vec<3> vi, vn;
        
        if(shape->getIntersection(rb.origin, rb.direction, position, vi, vn) > diameter*0.5) {
            rb.valid = 0;
            return rb;
        }

        return { rb.waveLength, vi, rb.direction, rb.valid };
    }

    Mat<2> Aperture::getGaussianMatrix(double waveLength, const Glass& gb, double& petzvarSum) const {
        return eMat<2>();
    }

    void Aperture::drawSvgModel(std::FILE* fp) const {
        double d2 = diameter * 0.6;
        double i2 = diameter * 0.5;
        std::fprintf(
            fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
            position[2], -position[1] - d2, position[2], -position[1] - i2,
            TOPT_LINE_BOLD
        );
        std::fprintf(
            fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
            position[2], -position[1] + d2, position[2], -position[1] + i2,
            TOPT_LINE_BOLD
        );
    }
}