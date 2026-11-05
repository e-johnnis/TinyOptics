#ifndef __TOPT_DRAWER_HPP
#define __TOPT_DRAWER_HPP 0

#include<topt/system.hpp>
#include<cstdio>
#include<vector>

#define TOPT_LINE_BOLD 0.1
#define TOPT_LINE_REGULAR 0.05

namespace topt {

    struct SvgViewBox {
        double x;
        double y;
        double width;
        double height;
    };

    class SvgDrawer {
    public:
        SvgDrawer(std::FILE* fp) { _fp = fp; }
        ~SvgDrawer() {}

        virtual void initialize(const SvgViewBox&) const;
        virtual void finalize() const;

    protected:
        std::FILE*  _fp;
    };

    class ModelDrawer : public SvgDrawer {
    public:
        ModelDrawer(std::FILE* fp) : SvgDrawer(fp) {};
        ~ModelDrawer() {}

        virtual void initialize(const SvgViewBox&) const override;
        void draw(System&) const;
        void draw(System&, const NearAxisResult&, double) const;
        int drawRays(const std::vector<Ray>&, double) const;
    };

    class RayFanDrawer : public SvgDrawer {
    public:
        RayFanDrawer(std::FILE* fp, double);
        ~RayFanDrawer() {}

        void draw(System&, const std::vector<std::vector<Ray>>&, const NearAxisResult&) const;
    private:
        double _yScale;
    };

}

#endif