#ifndef __TOPT_SHAPE_HPP
#define __TOPT_SHAPE_HPP 0

#include<topt/core.hpp>
#include<cstdio>

namespace topt {

    class Shape {
    public:
        double radius;

        Shape() {
            radius = d_inf;
        };
        ~Shape() {};

        virtual double  getIntersection(const Vec<3>&, const Vec<3>&, const Vec<3>&, Vec<3>&, Vec<3>&) const = 0;
        virtual void    drawSvgModel(std::FILE*, const Vec<3>&, double) const = 0;
    };

    class Plane : public Shape {
    public:
        Plane() {};
        ~Plane() {};

        virtual double  getIntersection(const Vec<3>&, const Vec<3>&, const Vec<3>&, Vec<3>&, Vec<3>&) const override;
        virtual void    drawSvgModel(std::FILE*, const Vec<3>&, double) const override;
    };

    class SphericalSurface : public Shape {
    public:
        SphericalSurface(double r) {
            radius = r;
        }
        ~SphericalSurface() {};

        virtual double  getIntersection(const Vec<3>&, const Vec<3>&, const Vec<3>&, Vec<3>&, Vec<3>&) const override;
        virtual void    drawSvgModel(std::FILE*, const Vec<3>&, double) const override;
    };

}

#endif