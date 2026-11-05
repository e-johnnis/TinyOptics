#include<topt.hpp>
#include<cstdio>

const double yScale = 0.2;

int main(int argc, char** argv) {
    if(argc < 2) {
        std::fprintf(stderr, "[!] no catalog file.\n");
        return -1;
    }

    topt::GlassCatalog catalog = topt::GlassCatalog::ohara(argv[1]);
    if(!catalog.getCount()) {
        std::fprintf(stderr, "[!] failed to load catalog \"%s\".\n", argv[1]);
        return -2;
    }

    topt::Glass g1, g2;
    if(!catalog.find("S-BSL 7", &g1) || !catalog.find("S-TIM 2", &g2)) {
        std::fprintf(stderr, "[!] glass not found.\n");
        return -3;
    }

    topt::System sys;
    double zPos = 0;

    sys.add(new topt::Refractor(102.0, zPos, g1, 479.9824));
    zPos += 11.25;

    sys.add(new topt::Refractor(102.0, zPos, topt::air, -421.7763));
    zPos += 0.10;

    sys.add(new topt::Refractor(102.0, zPos, g2, -411.7371));
    zPos += 6.25;

    sys.add(new topt::Refractor(102.0, zPos, topt::air, -3013.7675));

    topt::NearAxisResult nar = sys.analyze_nearAxis(topt::ray_d);

    std::printf("fl_d = %lf mm\n", nar.focalLength);
    std::printf("df_d = %lf mm\n", nar.frontToPrincipal);
    std::printf("dr_d = %lf mm\n", nar.rearToPricipal);
    std::printf("pr_d = %lf mm\n", 1.0 / nar.petzvarSum);

    std::FILE* fp = std::fopen("svg/seq/model.svg", "w");
    topt::SvgViewBox vb = { -10, -60, 1100, 120 };

    topt::ModelDrawer md(fp);
    md.initialize(vb);

    for(int i = 0; i <= 4; i++) {
        double ih = i * 3.0;
        char fnrf[64];
        std::sprintf(fnrf, "svg/seq/rayFan-%3.1lf.svg", ih);

        topt::Vec<3> dir = { 0, ih, nar.focalLength };
        std::vector<std::vector<topt::Ray>> rays = sys.trace2d(dir, topt::ray_d, nar, 100, 101, &md);
        std::vector<std::vector<topt::Ray>> rays_C = sys.trace2d(dir, topt::ray_C, nar, 100, 101, NULL);
        std::vector<std::vector<topt::Ray>> rays_F = sys.trace2d(dir, topt::ray_F, nar, 100, 101, NULL);
        std::vector<std::vector<topt::Ray>> rays_g = sys.trace2d(dir, topt::ray_g, nar, 100, 101, NULL);

        std::FILE* frf = std::fopen(fnrf, "w");
        topt::RayFanDrawer rfd(frf, yScale);
        rfd.draw(sys, rays_C, nar);
        rfd.draw(sys, rays_F, nar);
        rfd.draw(sys, rays_g, nar);
        rfd.draw(sys, rays, nar);
        rfd.finalize();
        std::fclose(frf);
    }

    md.draw(sys, nar, 12);

    md.finalize();
    std::fclose(fp);

    return 0;
}