#ifndef __TOPT_SYSTEM_HPP
#define __TOPT_SYSTEM_HPP 0

#include<topt/element.hpp>
#include<vector>

namespace topt {

    class System {
    public:
        std::vector<Element*> elements;

        System() {};
        ~System() {
            for(int i = 0; i < elements.size(); i++) {
                if(!elements[i]) delete elements[i];
                elements[i] = NULL;
            }
            elements.clear();
        }

        Element*    operator [] (int i) { return elements[i]; }

        int add(Element*);
        int remove(Element*);
        int removeAt(int);

        NearAxisResult analyze_nearAxis(double) const;

    };

}

#endif