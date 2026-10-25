#ifndef __TOPT_ELEMENT_HPP
#define __TOPT_ELEMENT_HPP 0

#include<topt/matrix.hpp>
#include<topt/shape.hpp>

namespace topt {

    class Element {
    public:
        double  diameter;
        Vec<3>  position;
        Glass   glass;
        Shape*  shape;

        Element(double diam, const Vec<3>& pos, const Glass& g, Shape* s) {
            diameter = diam;
            position = pos;
            glass = g;
            shape = s;
        }
        Element(double diam, double zPos, const Glass& g, Shape* s) {
            diameter = diam;
            position = { 0, 0, zPos },
            glass = g;
            shape = s;
        }
        ~Element() { delete shape; }

        virtual const Ray   trace(Ray&, const Glass&) const = 0;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const = 0;
    };

    class Refractor : public Element {
    public:

        Refractor(double diam, const Vec<3>& pos, const Glass& g, double r) : Element(diam, pos, g, new SphericalSurface(r)) {}
        Refractor(double diam, double zPos, const Glass& g, double r) : Element(diam, zPos, g, new SphericalSurface(r)) {}
        ~Refractor() {}

        virtual const Ray   trace(Ray&, const Glass&) const override;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const override;
    };

    class Aperture : public Element {
    public:

        Aperture(double diam, const Vec<3>& pos) : Element(diam, pos, air, new Plane()) {}
        Aperture(double diam, double zPos) : Element(diam, zPos, air, new Plane()) {}
        ~Aperture() {}

        virtual const Ray   trace(Ray&, const Glass&) const override;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const override;
    };

}

#endif