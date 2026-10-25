#ifndef __TOPT__DRAWER_HPP
#define __TOPT__DRAWER_HPP 0

#include<topt/system.hpp>
#include<cstdio>

namespace topt {

    struct SvgViewBox {
        double x;
        double y;
        double width;
        double height;
    };

    class SvgDrawer {
    public:
        SvgDrawer(std::FILE*, const SvgViewBox&, int);
        ~SvgDrawer() { close(); }

        virtual void close();

        virtual void drawModel(const System&) const;
        virtual void drawRays(const Ray&, double, int) const;

    private:
        std::FILE*  _fp;
        int         _isProcess;
    };

    class ModelDrawer : public SvgDrawer {
    public:
        ModelDrawer(std::FILE* fp, const SvgViewBox& viewBox, int isProcess) : SvgDrawer(fp, viewBox, isProcess) {}
        ~ModelDrawer() {}
    }

}

#endif