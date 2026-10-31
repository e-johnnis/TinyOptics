#include<topt.hpp>
#include<cmath>

namespace topt {

    double Plane::getIntersection (
        const Vec<3>&   origin,
        const Vec<3>&   direction,
        const Vec<3>&   position,
        Vec<3>&         intersection,
        Vec<3>&         normalVector
    ) const {
        double t = (position[2] - origin[2]) / direction[2];

        intersection = origin + direction * t;
        normalVector = { 0, 0, -1 };
        Vec<3> vip = intersection - position;
        return std::sqrt(vip[0] * vip[0] + vip[1] * vip[1]);
    }

    void Plane::drawSvgModel(std::FILE* fp, const Vec<3>& position, double diameter) const {
        double d2 = diameter * 0.5;
        std::fprintf(
            fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"0.3\" />\n",
            position[2], position[1] - d2, position[2], position[1] + d2
        );
    }

    double SphericalSurface::getIntersection (
        const Vec<3>&   origin,
        const Vec<3>&   direction,
        const Vec<3>&   position,
        Vec<3>&         intersection,
        Vec<3>&         normalVector
    ) const {
        // |vp + t*direction| = radius

        Vec<3> vc = { 0, 0, radius };
        Vec<3> vq = position + vc;
        Vec<3> vp = origin - vq;

        double dd = direction * direction;
        double dp = direction * vp;
        double pp = vp * vp;

        double d4 = dp * dp + dd * (radius * radius - pp);

        if(d4 < 0) return d_inf;

        double t = (radius > 0) ? ((-dp - std::sqrt(d4)) / dd) : ((-dp + std::sqrt(d4)) / dd);

        if(t < 0) return d_inf;
        
        intersection = origin + direction * t;
        normalVector = (intersection - vq) / radius;
        Vec<3> vip = intersection - position;
        return std::sqrt(vip[0] * vip[0] + vip[1] * vip[1]);
    }

    void SphericalSurface::drawSvgModel(std::FILE* fp, const Vec<3>& position, double diameter) const {
        double d2 = diameter * 0.5;
        double ar = std::abs(radius);
        double dz = -radius * (1.0 - std::cos(std::atan2(d2, ar)));
        std::fprintf(
            fp,
            "  <path d=\"M %5.3lf %5.3lf a %5.3lf %5.3lf 0 0 %d 0 %5.3lf\" stroke=\"#000000\" stroke-width=\"0.3\" fill=\"none\" />\n",
            position[2] - dz, position[1] - d2, ar, ar, radius < 0, diameter
        );
    }

}