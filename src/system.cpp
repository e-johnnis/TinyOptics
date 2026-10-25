#include<topt.hpp>

namespace topt {

    int System::add(Element* ep) {
        elements.push_back(ep);
        return elements.size();
    }

    int System::removeAt(int i) {
        delete elements[i];
        elements.erase(elements.begin() + i);
        return elements.size();
    }

    NearAxisResult System::analyze_nearAxis(double waveLength) const {
        double ps = 0;
        Mat<2> tm = eMat<2>();
        Mat<2> gm = elements[0]->getGaussianMatrix(waveLength, air, ps);

        for(int i = 1; i < elements.size(); i++) {
            tm[0][1] = (elements[i-1]->position[2] - elements[i]->position[2]) / getRefractance(elements[i-1]->glass, waveLength);
            gm = elements[i]->getGaussianMatrix(waveLength, elements[i-1]->glass, ps) * tm * gm;
        }

        NearAxisResult result;
        result.focalLength = -1.0 / gm[1][0];
        result.frontToPrincipal = (1.0 - gm[1][1]) * result.focalLength;
        result.rearToPricipal = (1.0 - gm[0][0]) * result.focalLength;
        result.petzvarSum = ps;
        return result;
    }
}