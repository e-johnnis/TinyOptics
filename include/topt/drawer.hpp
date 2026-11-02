#ifndef __TOPT_DRAWER_HPP
#define __TOPT_DRAWER_HPP 0

#include<topt/system.hpp>
#include<cstdio>
#include<vector>

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
        void drawRays(const std::vector<Ray>&, double) const;
    };

}

#endif