#ifndef __TOPT_MATRIX_HPP
#define __TOPT_MATRIX_HPP 0

#include<cmath>

namespace topt {

    template<int N, typename T = double>
    struct Vec {
        T   _data[N];

        T   operator [] (int i) const { return _data[i]; }
        T&  operator [] (int i) { return _data[i]; }

        Vec<N,T>    operator -  () const {
            Vec<N,T> rv;
            for(int i = 0; i < N; i++) rv[i] = -_data[i];
            return rv;
        }

        T   size() const {
            T x = 0;
            for(int i = 0; i < N; i++) x += _data[i] * _data[i];
            return std::sqrt(x);
        }
    };

    template<int N, typename T = double>
    Vec<N,T>    operator +  (const Vec<N,T>& left, const Vec<N,T>& right) {
        Vec<N,T> rv;
        for(int i = 0; i < N; i++) rv[i] = left[i] + right[i];
        return rv;
    }

    template<int N, typename T = double>
    Vec<N,T>    operator -  (const Vec<N,T>& left, const Vec<N,T>& right) {
        Vec<N,T> rv;
        for(int i = 0; i < N; i++) rv[i] = left[i] - right[i];
        return rv;
    }

    template<int N, typename T = double>
    Vec<N,T>    operator *  (const Vec<N,T>& left, T right) {
        Vec<N,T> rv;
        for(int i = 0; i < N; i++) rv[i] = left[i] * right;
        return rv;
    }

    template<int N, typename T = double>
    Vec<N,T>    operator *  (T left, const Vec<N,T>& right) {
        return right * left;
    }

    template<int N, typename T = double>
    Vec<N,T>    operator /  (const Vec<N,T>& left, T right) {
        return left * (1.0/right);
    }

    template<int N, typename T = double>
    T           operator *  (const Vec<N,T>& left, const Vec<N,T>& right) {
        T x = 0;
        for(int i = 0; i < N; i++) x += left[i] * right[i];
        return x;
    }

    template<int R, int C = R, typename T = double>
    struct Mat {
        Vec<C,T>    _data[R];

        Vec<C,T>    operator [] (int r) const { return _data[r]; }
        Vec<C,T>&   operator [] (int r) { return _data[r]; }

        Mat<R,C,T>  operator -  () const {
            Mat<R,C,T> rm;
            for(int i = 0; i < R*C; i++) reinterpret_cast<T*>(&rm)[i] = reinterpret_cast<T*>(this)[i];
            return rm;
        }
    };

    template<int R, int C = R, typename T = double>
    Mat<R,C,T>  operator +  (const Mat<R,C,T>& left, const Mat<R,C,T>& right) {
        Mat<R,C,T> rm;
        for(int i = 0; i < R; i++) rm[i] = left[i] + right[i];
        return rm;
    }

    template<int R, int C = R, typename T = double>
    Mat<R,C,T>  operator -  (const Mat<R,C,T>& left, const Mat<R,C,T>& right) {
        Mat<R,C,T> rm;
        for(int i = 0; i < R; i++) rm[i] = left[i] - right[i];
        return rm;
    }

    template<int R, int C = R, typename T = double>
    Mat<R,C,T>  operator *  (const Mat<R,C,T>& left, T right) {
        Mat<R,C,T> rm;
        for(int i = 0; i < R; i++) rm[i] = left[i] * right;
        return rm;
    }

    template<int R, int C = R, typename T = double>
    Mat<R,C,T>  operator *  (T left, const Mat<R,C,T>& right) {
        return right * left;
    }

    template<int R, int C = R, typename T = double>
    Mat<R,C,T>  operator /  (const Mat<R,C,T>& left, T right) {
        return left * (1.0/right);
    }

    template<int R, int C, int N, typename T = double>
    Mat<R,C,T>  operator *  (const Mat<R,N,T>& left, const Mat<N,C,T>& right) {
        Mat<R,C,T> rm;
        for(int r = 0; r < R; r++) for(int c = 0; c < C; c++) {
            rm[r][c] = 0;
            for(int i = 0; i < N; i++) rm[r][c] += left[r][i] * right[i][c];
        }
        return rm;
    }

    template<int R, int C, typename T = double>
    Vec<R,T>    operator *  (const Mat<R,C,T>& left, const Vec<C,T>& right) {
        Vec<R,T> rv;
        for(int r = 0; r < R; r++) rv[r] = left[r] * right;
        return rv;
    }

    template<int N, typename T = double>
    Mat<N,N,T>  eMat() {
        Mat<N,N,T> rm;
        for(int r = 0; r < N; r++) for(int c = 0; c < N; c++) rm[r][c] = (r == c);
        return rm;
    }

}

#endif