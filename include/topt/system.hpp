#ifndef __TOPT_SYSTEM_HPP
#define __TOPT_SYSTEM_HPP 0

#include<topt/core.hpp>
#include<topt/element.hpp>
#include<vector>

namespace topt {

    class ModelDrawer;

    class System {
    public:
        System() {};
        ~System() {
            for(int i = 0; i < _elements.size(); i++) {
                if(!_elements[i]) delete _elements[i];
                _elements[i] = NULL;
            }
            _elements.clear();
        }

        Element*    operator [] (int i) { return _elements[i]; }

        int size() const { return _elements.size(); }

        int add(Element*);
        int remove(Element*);
        int removeAt(int);

        NearAxisResult analyze_nearAxis(double) const;

        std::vector<std::vector<Ray>> trace2d(const Vec<3>&, double, const NearAxisResult&, double, int, const ModelDrawer*) const;

    private:
        std::vector<Element*> _elements;

        std::vector<Ray> _trace(Ray&) const;

    };

}

#endif