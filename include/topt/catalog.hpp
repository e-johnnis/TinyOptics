#ifndef __TOPT_CATALOG_HPP
#define __TOPT_CATALOG_HPP 0

#include<topt/element.hpp>
#include<vector>

namespace topt {

    class GlassCatalog {
    public:
        GlassCatalog() {}
        ~GlassCatalog() {
            _data.clear();
        }

        int getCount() { return _data.size(); }
        void add(const Glass&);
        int find(const char*, Glass*) const;
        Glass search(double, double) const;

        static GlassCatalog ohara(const char*);

    private:
        std::vector<Glass> _data;
    };

}

#endif