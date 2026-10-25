#include<topt.hpp>

namespace topt {

    SvgDrawer::SvgDrawer(std::FILE* fp, const SvgViewBox& viewBox, int isProcess = 0) {
        _fp = fp;
        _isProcess = isProcess;

        std::fprintf(
            _fp,
            "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"%5.3lf %5.3lf %5.3lf %5.3lf\">\n",
            viewBox.x, viewBox.y, viewBox.width, viewBox.height
        );
    }

    void SvgDrawer::close() {
        if(!_fp) return;
        
        std::fprintf(_fp, "</svg>\n");

        if(_isProcess) pclose(_fp);
        else
    }
}