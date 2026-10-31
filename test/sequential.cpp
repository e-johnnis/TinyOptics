#include<topt.hpp>
#include<cstdio>

int main() {
    topt::Glass sBsl7 = {
        {   1.15150190e+0,  1.18583692e-1,  1.26301359e+0   },
        {   1.05984130e-2, -1.18225190e-2,  1.29617662e+2   },
        "S-BSL 7"
    };
    topt::Glass sTim2 = {
        {   1.42193846e+0,  1.33827968e-1,  1.45060574e+0   },
        {   1.07291511e-2,  5.72587546e-2,  1.45381805e+2   },
        "S-TIM 2"
    };

    topt::System sys;
    double zPos = 0;

    sys.add(new topt::Refractor(102.0, zPos, sBsl7, 479.9824));
    zPos += 11.25;

    sys.add(new topt::Refractor(102.0, zPos, topt::air, -421.7763));
    zPos += 0.10;

    sys.add(new topt::Refractor(102.0, zPos, sTim2, -411.7371));
    zPos += 6.25;

    sys.add(new topt::Refractor(102.0, zPos, topt::air, -3013.7675));

    topt::NearAxisResult nar = sys.analyze_nearAxis(topt::ray_d);

    std::printf("fl_d = %lf mm\n", nar.focalLength);
    std::printf("df_d = %lf mm\n", nar.frontToPrincipal);
    std::printf("dr_d = %lf mm\n", nar.rearToPricipal);
    std::printf("pr_d = %lf mm\n", 1.0 / nar.petzvarSum);

    std::FILE* fp = std::fopen("svg/model-seq.svg", "w");
    topt::SvgViewBox vb = { -10, -60, 1100, 120 };

    topt::ModelDrawer md(fp);
    md.initialize(vb);

    for(int i = 0; i < 4; i++) {
        double ih = i * 6.0;
        topt::Vec<3> dir = { 0, ih, nar.focalLength };
        std::vector<std::vector<topt::Ray>> rays = sys.trace2d(dir, topt::ray_d, nar, 100, 101, &md);
    }

    md.draw(sys);

    md.finalize();
    std::fclose(fp);

    return 0;
}