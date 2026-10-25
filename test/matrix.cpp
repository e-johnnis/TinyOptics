#include<topt.hpp>
#include<cstdio>

int main() {
    topt::Mat<2> m2 = topt::eMat<2>();
    topt::Vec<2> v2 = { 2, 3 };

    topt::Vec<2> vo2 = m2 * v2;

    printf("\n*** test matrix.hpp ***\n\n");

    for(int r = 0; r < 2; r++) {
        std::printf("%lf\t", vo2[r]);
    }
    printf("\n\n");

    return 0;
}