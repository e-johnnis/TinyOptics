#ifndef __TOPT_ELEMENT_HPP
#define __TOPT_ELEMENT_HPP 0

#include<topt/matrix.hpp>
#include<topt/shape.hpp>
#include<cstdio>

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

        virtual const char* getType() const = 0;

        virtual const Ray   trace(Ray&, const Glass&) const = 0;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const = 0;

        virtual void        drawSvgModel(std::FILE*) const = 0;
    };

    class Refractor : public Element {
    public:

        Refractor(double diam, const Vec<3>& pos, const Glass& g, Shape* s) : Element(diam, pos, g, s) {}
        Refractor(double diam, double zPos, const Glass& g, Shape* s) : Element(diam, zPos, g, s) {}
        Refractor(double diam, const Vec<3>& pos, const Glass& g, double r) : Element(diam, pos, g, new SphericalSurface(r)) {}
        Refractor(double diam, double zPos, const Glass& g, double r) : Element(diam, zPos, g, new SphericalSurface(r)) {}
        ~Refractor() {}

        virtual const char* getType() const override { return "REFRACTOR"; }

        virtual const Ray   trace(Ray&, const Glass&) const override;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const override;

        virtual void        drawSvgModel(std::FILE*) const override;
    };

    class Aperture : public Element {
    public:

        Aperture(double diam, const Vec<3>& pos) : Element(diam, pos, air, new Plane()) {}
        Aperture(double diam, double zPos) : Element(diam, zPos, air, new Plane()) {}
        ~Aperture() {}

        virtual const char* getType() const override { return "APERTURE"; }

        virtual const Ray   trace(Ray&, const Glass&) const override;
        virtual Mat<2>      getGaussianMatrix(double, const Glass&, double&) const override;

        virtual void        drawSvgModel(std::FILE*) const override;
    };

}

#endif