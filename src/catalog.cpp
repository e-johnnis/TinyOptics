#include<topt.hpp>
#include<vector>
#include<cstring>
#include<cmath>
#include<cstdio>

namespace topt {

    void GlassCatalog::add(const Glass& g) {
        _data.push_back(g);
    }

    int GlassCatalog::find(const char* label, Glass* g) const {
        for(int i = 0; i < _data.size(); i++) {
            if(!std::strcmp(label, _data[i].label)) {
                *g = _data[i];
                return 1;
            }
        }
        return 0;
    }

    Glass GlassCatalog::search(double nd, double vd) const {
        Glass gf = air;
        double pf = d_inf;

        for(int i = 0; i < _data.size(); i++) {
            double dnd = (getRefractance(_data[i], ray_d) - nd) / nd;
            double dvd = (getAbbeVd(_data[i]) - vd) / vd;

            double pfi = std::max(std::abs(dnd), std::abs(dvd));

            if(pfi < pf) {
                pf = pfi;
                gf = _data[i];
            }
        }

        return gf;
    }

    GlassCatalog GlassCatalog::ohara(const char* file) {
        GlassCatalog gc;

        std::FILE* fp = std::fopen(file, "r");
        if(!fp) return gc;

        char* line = new char[3200];

        std::fgets(line, 3200, fp);
        std::fgets(line, 3200, fp);

        while(!std::feof(fp)) {
            int pos = 0;
            int nc = 0;
            int col = 1;
            Glass g;

            if(!std::fgets(line, 3200, fp)) {
                if(std::ferror(fp)) break;
                else continue;
            }

            if(!std::sscanf(line, "%*[^,],%[^,],%n", &g.label, &nc)) continue;
            pos += nc;

            while(col < 59) {
                std::sscanf(line + pos, "%*[^,],%n", &nc);
                pos += nc;
                col++;
            }

            for(int i = 0; i < TOPT_SELLMEIER_N; i++) {
                if(i < 3) {
                    if(!std::sscanf(line + pos, "%lf,%n", &(g.k[i]), &nc)) break;
                    pos += nc;
                    col++;
                }else {
                    g.k[i] = 0;
                }
            }

            for(int i = 0; i < TOPT_SELLMEIER_N; i++) {
                if(i < 3) {
                    if(!std::sscanf(line + pos, "%lf,%n", &(g.l[i]), &nc)) break;
                    pos += nc;
                    col++;
                }else {
                    g.k[i] = 0;
                }
            }

            if(col < (58 + TOPT_SELLMEIER_N * 2)) continue;

            gc.add(g);

        }

        delete[] line;

        return gc;
    }

}