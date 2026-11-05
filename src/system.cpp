#include<topt.hpp>
#include<topt/color.hpp>
#include<vector>

namespace topt {

    int System::add(Element* ep) {
        _elements.push_back(ep);
        return _elements.size();
    }

    int System::removeAt(int i) {
        delete _elements[i];
        _elements.erase(_elements.begin() + i);
        return _elements.size();
    }

    NearAxisResult System::analyze_nearAxis(double waveLength) const {
        double ps = 0;
        Mat<2> tm = eMat<2>();
        Mat<2> gm = _elements[0]->getGaussianMatrix(waveLength, air, ps);

        for(int i = 1; i < _elements.size(); i++) {
            tm[0][1] = (_elements[i]->position[2] - _elements[i-1]->position[2]) / getRefractance(_elements[i-1]->glass, waveLength);
            gm = _elements[i]->getGaussianMatrix(waveLength, _elements[i-1]->glass, ps) * tm * gm;
        }

        NearAxisResult result;
        result.focalLength = 1.0 / gm[1][0];
        result.frontToPrincipal = (gm[1][1] - 1.0) * result.focalLength;
        result.rearToPricipal = (1.0 - gm[0][0]) * result.focalLength;
        result.petzvarSum = ps;
        return result;
    }

    std::vector<std::vector<Ray>> System::trace2d(
        const Vec<3>& direction, double waveLength, const NearAxisResult& nar,
        double diameter, int maxRays,
        const ModelDrawer* drawer = NULL
    ) const {
        std::vector<std::vector<Ray>> rays;
        double pitch = diameter / maxRays;
        double zf = nar.focalLength + nar.rearToPricipal + _elements[_elements.size()-1]->position[2];

        for(int i = 0; i < maxRays; i++) {
            double y = (i % 2) ? (-pitch * (1+i/2)) : (pitch * i/2);
            double dydz = direction[1] / direction[2];
            double dz = nar.frontToPrincipal + 1.0;
            Vec<3> orig = { 0, y - dydz * dz, -1.0 };
            Ray ir = { waveLength, orig, direction, 1 };
            
            rays.push_back(_trace(ir));
        }

        if(drawer && rays.size()) {
            drawer->drawRays(rays[0], zf);
            int i = rays.size() - 1;
            while((i >= 0) && !drawer->drawRays(rays[i], zf)) i -= 2;
            i = rays.size() - 2;
            while((i >= 0) && !drawer->drawRays(rays[i], zf)) i -= 2;
        }

        return rays;
    }

    std::vector<Ray> System::_trace(Ray& enter) const {
        std::vector<Ray> rays;
        rays.push_back(enter);
        rays.push_back(_elements[0]->trace(enter, air));

        for(int i = 1; i < _elements.size(); i++) {
            rays.push_back(_elements[i]->trace(rays[i], _elements[i-1]->glass));
        }

        return rays;
    }
}