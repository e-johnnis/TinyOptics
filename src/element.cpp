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

        Vec<3> vs = -rb.direction / rb.direction.size();
        double sn = vs * vn;
        double D = 1.0 - ne * ne * (1.0 - sn * sn);
        int valid = (D >= 0);

        Vec<3> vr = valid ? -ne * (vs - vn * sn) - vn * std::sqrt(D) : rb.direction;

        return { rb.waveLength, vi, vr, valid };
    }

    Mat<2> Refractor::getGaussianMatrix(double waveLength, const Glass& gb, double& petzvarSum) const {
        double n1 = getRefractance(gb, waveLength);
        double n2 = getRefractance(glass, waveLength);
        petzvarSum += (1.0 / n1 - 1.0 / n2) / shape->radius;
        return {
            1.0,  0.0,
            (n1 - n2) / (shape->radius * n2), n1 / n2
        };
    }

    void Refractor::drawSvgModel(std::FILE* fp) const {
        shape->drawSvgModel(fp, position, diameter);
    }

    const Ray Aperture::trace(Ray& rb, const Glass& gb) const {
        if(!rb.valid) return rb;

        Vec<3> vi, vn;
        
        if(shape->getIntersection(rb.origin, rb.direction, position, vi, vn) <= diameter*0.5) {
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
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"0.3\" />\n",
            position[2], position[1] - d2, position[2], position[1] - i2
        );
        std::fprintf(
            fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"0.3\" />\n",
            position[2], position[1] + d2, position[2], position[1] + i2
        );
    }
}