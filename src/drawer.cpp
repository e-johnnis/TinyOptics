#include<topt.hpp>
#include<topt/color.hpp>
#include<cstdio>
#include<vector>

namespace topt {

    void SvgDrawer::initialize(const SvgViewBox& viewBox) const {
        std::fprintf(
            _fp,
            "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%5.3lfmm\" height=\"%5.3lfmm\" viewBox=\"%5.3lf %5.3lf %5.3lf %5.3lf\">\n",
            viewBox.width, viewBox.height, viewBox.x, viewBox.y, viewBox.width, viewBox.height
        );
    }

    void SvgDrawer::finalize() const {
        if(!_fp) return;
        
        std::fprintf(_fp, "</svg>\n");
    }

    void ModelDrawer::initialize(const SvgViewBox& viewBox) const {
        SvgDrawer::initialize(viewBox);
        std::fprintf(
            _fp,
            "  <line x1=\"%5.3lf\" y1=\"0\" x2=\"%5.3lf\" y2=\"0\" stroke=\"#000000\" stroke-width=\"0.15\" />\n",
            viewBox.x, viewBox.width + viewBox.x
        );
    }

    void ModelDrawer::draw(System& sys) const {
        for(int i = 0; i < sys.size(); i++) {
            sys[i]->drawSvgModel(_fp);
        }
    }

    void ModelDrawer::drawRays(const std::vector<Ray>& rays, double zFocus) const {
        int il = rays.size()-1;
        int col = wl2color(rays[0].waveLength);
        for(int i = 0; i < il; i++) {
            if(!rays[i].valid) return;
            std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2 = \"%5.3lf\" y2=\"%5.3lf\" stroke=\"#%08X\" stroke-width=\"0.15\" />\n",
                rays[i].origin[2], rays[i].origin[1],
                rays[i+1].origin[2], rays[i+1].origin[1],
                col
            );
        }
        if(!rays[il].valid) return;
        double yf = rays[il].origin[1] + (zFocus - rays[il].origin[2]) * rays[il].direction[1] / rays[il].direction[2];
        std::fprintf(
            _fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2 = \"%5.3lf\" y2=\"%5.3lf\" stroke=\"#%08X\" stroke-width=\"0.15\" />\n",
            rays[il].origin[2], rays[il].origin[1],
            zFocus, yf,
            col
        );
    }
}