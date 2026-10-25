#include<topt.hpp>
#include<cmath>

namespace topt {

    const Ray Refractor::trace(Ray& rb, const Glass& gb) const {
        if(!rb.valid) return rb;

        double ne = getRefractance(gb, rb.waveLength) / getRefractance(glass, rb.waveLength);
        Vec<3> vi, vn;
        
        if(shape->getIntersection(rb.origin, rb.direction, position, vi, vn) > diameter*0.5) {
            rb.valid = 0;
            return rb;
        }

        Vec<3> vs = rb.direction / rb.direction.size();
        Vec<3> vsh = vs - vn * (vs * vn);
        double D = 1.0 - ne * ne * (vsh * vsh);
        int valid = D >= 0;

        Vec<3> vr = valid ? vsh * ne + vn * std::sqrt(D) : rb.direction;

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
}