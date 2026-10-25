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
        normalVector = { 0, 0, 1 };
        Vec<3> vip = intersection - position;
        return std::sqrt(vip[0] * vip[0] + vip[1] * vip[1]);
    }

    double SphericalSurface::getIntersection (
        const Vec<3>&   origin,
        const Vec<3>&   direction,
        const Vec<3>&   position,
        Vec<3>&         intersection,
        Vec<3>&         normalVector
    ) const {
        // |vp + t*direction| = radius

        Vec<3> vp = origin - position;

        double dd = direction * direction;
        double dp = direction * vp;
        double pp = vp * vp;

        double d4 = dp * dp + dd * (radius * radius - pp);

        if(d4 < 0) return d_inf;

        double t = radius > 0 ? (-dp - std::sqrt(d4)) / dd : (-dp + std::sqrt(d4)) / dd;

        if(t < 0) return d_inf;
        
        intersection = origin + direction * t;
        normalVector = (position - intersection) / radius;
        Vec<3> vip = intersection - position;
        return std::sqrt(vip[0] * vip[0] + vip[1] * vip[1]);
    }

}