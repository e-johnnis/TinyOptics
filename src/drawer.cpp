#include<topt.hpp>
#include<topt/color.hpp>
#include<cstdio>
#include<vector>
#include<algorithm>

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
            "  <line x1=\"%5.3lf\" y1=\"0\" x2=\"%5.3lf\" y2=\"0\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
            viewBox.x, viewBox.width + viewBox.x,
            TOPT_LINE_REGULAR
        );
    }

    void ModelDrawer::draw(System& sys) const {
        for(int i = 0; i < sys.size(); i++) {
            sys[i]->drawSvgModel(_fp);
        }
    }

    void ModelDrawer::draw(System& sys, const NearAxisResult& nar, double maxImageHeight) const {
        draw(sys);
        
        if(sys.size()) {
            double zpf = sys[0]->position[2] + nar.frontToPrincipal;
            double zpr = sys[sys.size()-1]->position[2] + nar.rearToPricipal;
            double zim = zpr + nar.focalLength;

            if(std::isfinite(zpf)) std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"-1\" x2=\"%5.3lf\" y2=\"1\" stroke=\"#BB0000\" stroke-width=\"%f\" />\n",
                zpf, zpf,
                TOPT_LINE_REGULAR
            );
            if(std::isfinite(zpr)) std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"-1\" x2=\"%5.3lf\" y2=\"1\" stroke=\"#0000BB\" stroke-width=\"%f\" />\n",
                zpr, zpr,
                TOPT_LINE_REGULAR
            );
            if(std::isfinite(zim)) std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
                zim, -maxImageHeight, zim, maxImageHeight,
                TOPT_LINE_BOLD
            );
        }
    }

    int ModelDrawer::drawRays(const std::vector<Ray>& rays, double zFocus) const {
        int il = rays.size()-1;
        if(il < 0 || !rays[il].valid) return 0;
        int col = wl2color(rays[0].waveLength);
        for(int i = 0; i < il; i++) {
            std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2 = \"%5.3lf\" y2=\"%5.3lf\" stroke=\"#%08X\" stroke-width=\"%f\" />\n",
                rays[i].origin[2], -rays[i].origin[1],
                rays[i+1].origin[2], -rays[i+1].origin[1],
                col,
                TOPT_LINE_REGULAR
            );
        }
        double yf = rays[il].origin[1] + (zFocus - rays[il].origin[2]) * rays[il].direction[1] / rays[il].direction[2];
        std::fprintf(
            _fp,
            "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2 = \"%5.3lf\" y2=\"%5.3lf\" stroke=\"#%08X\" stroke-width=\"%f\" />\n",
            rays[il].origin[2], -rays[il].origin[1],
            zFocus, -yf,
            col,
            TOPT_LINE_REGULAR
        );
        return 1;
    }

    RayFanDrawer::RayFanDrawer(std::FILE* fp, double yScale) : SvgDrawer::SvgDrawer(fp) {
        _yScale = yScale;

        initialize({ -60, -55, 115, 125 });
        std::fprintf(
            _fp,
            "  <line x1=\"0\" y1=\"-50\" x2=\"0\" y2=\"50\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
            TOPT_LINE_REGULAR
        );
        std::fprintf(
            _fp,
            "  <line x1=\"-50\" y1=\"0\" x2=\"50\" y2=\"0\" stroke=\"#000000\" stroke-width=\"%f\" />\n",
            TOPT_LINE_REGULAR
        );
        std::fprintf(
            _fp,
            "  <rect x=\"-50\" y=\"-50\" width=\"100\" height=\"100\" stroke=\"#000000\" stroke-width=\"%f\" fill=\"none\" />\n",
            TOPT_LINE_BOLD
        );

        std::fprintf(
            _fp,
            "  <text x=\"-51\" y=\"-48\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"end\">%d</text>\n",
            (int)(_yScale * 1000)
        );
        std::fprintf(
            _fp,
            "  <text x=\"-52\" y=\"0\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"middle\" transform=\"rotate(-90, -52, 0)\">Image Height [um]</text>\n"
        );
        std::fprintf(
            _fp,
            "  <text x=\"-51\" y=\"50\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"end\">%d</text>\n",
            -(int)(_yScale * 1000)
        );

        std::fprintf(
            _fp,
            "  <text x=\"-50\" y=\"54\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"begin\">-1</text>\n"
        );
        std::fprintf(
            _fp,
            "  <text x=\"0\" y=\"54\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"middle\">Normalized Entrance Height</text>\n"
        );
        std::fprintf(
            _fp,
            "  <text x=\"50\" y=\"54\" fill=\"#000000\" stroke=\"none\" font-size=\"3\" text-anchor=\"end\">1</text>\n"
        );
    }

    void RayFanDrawer::draw(
        System& sys,
        const std::vector<std::vector<Ray>>& rays,
        const NearAxisResult& nar
    ) const {
        int il = sys.size() - 1;
        if(il < 0) return;

        std::vector<Vec<2>> plots;
        double xs = 0;
        int col = wl2color(rays[0][0].waveLength);

        for(int i = 0; i < rays.size(); i++) {
            int jl = rays[i].size() - 1;
            if(jl < 0 || !rays[i][jl].valid) continue;

            double iy = rays[i][0].origin[1] + (nar.frontToPrincipal - rays[i][0].origin[2]) * rays[i][0].direction[1] / rays[i][0].direction[2];
            double zf = sys[il]->position[2] + nar.rearToPricipal + nar.focalLength;
            double oyi = nar.focalLength * rays[i][0].direction[1] / rays[i][0].direction[2];
            double oyr = rays[i][jl].origin[1] + (zf - rays[i][jl].origin[2]) * rays[i][jl].direction[1] / rays[i][jl].direction[2];

            plots.push_back({ iy, oyr - oyi });
            double aiy = std::abs(iy);
            if(xs < aiy) xs = aiy;
        }

        std::sort(plots.begin(), plots.end(), [](Vec<2> a, Vec<2> b){ return a[0] < b[0]; });

        for(int i = 0; i < plots.size()-1; i++) {
            std::fprintf(
                _fp,
                "  <line x1=\"%5.3lf\" y1=\"%5.3lf\" x2=\"%5.3lf\" y2=\"%5.3lf\" stroke=\"#%08X\" stroke-width=\"%f\" />\n",
                50.0 * plots[i][0] / xs, -50.0 * plots[i][1] / _yScale,
                50.0 * plots[i+1][0] / xs, -50.0 * plots[i+1][1] / _yScale,
                col, TOPT_LINE_BOLD
            );
        }
    }

}