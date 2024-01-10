#include <cstdint>
#include "fastexp.h"

template<>
double fastexp_1<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;

    const double coef[2] = {
        -0.05288671,
        0.99232129
    };

    double k = \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_2<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;

    const double coef[3] = {
        0.00365539,
        0.64960693,
        0.34271434
    };

    double k = \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_3<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;

    const double coef[4] = {
        -1.77187919e-04,
        6.96787180e-01,
        2.24169036e-01,
        7.90302044e-02
    };

    double k = \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_4<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;
    double xf4 = xf2 * xf2;

    const double coef[5] = { 
        6.58721338e-06,
        6.92937406e-01,
        2.41696769e-01,
        5.16742848e-02,
        1.36779598e-02
    };

    double k = \
        + coef[4] * xf4 \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_5<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;
    double xf4 = xf2 * xf2;
    double xf5 = xf2 * xf3;

    const double coef[6] = { 
        -1.97880719e-07,
        6.93156327e-01,
        2.40133447e-01,
        5.58740717e-02,
        8.94160147e-03,
        1.89454334e-03
    };

    double k = \
        + coef[5] * xf5 \
        + coef[4] * xf4 \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_6<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;
    double xf4 = xf2 * xf2;
    double xf5 = xf2 * xf3;
    double xf6 = xf3 * xf3;

    const double coef[7] = { 
        4.97074799e-09,
        6.93146861e-01,
        2.40230956e-01,
        5.54792541e-02,
        9.68583180e-03,
        1.23835751e-03,
        2.18728611e-04
    };

    double k = \
        + coef[6] * xf6 \
        + coef[5] * xf5 \
        + coef[4] * xf4 \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_7<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;
    double xf4 = xf2 * xf2;
    double xf5 = xf2 * xf3;
    double xf6 = xf3 * xf3;
    double xf7 = xf3 * xf4;

    const double coef[8] = {
        -1.06974751e-10,
        6.93147190e-01,
        2.40226337e-01,
        5.55053726e-02,
        9.61338873e-03,
        1.34310382e-03,
        1.42959529e-04,
        2.16483090e-05
    };

    double k = \
        + coef[7] * xf7 \
        + coef[6] * xf6 \
        + coef[5] * xf5 \
        + coef[4] * xf4 \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}

template<>
double fastexp_8<double>(double x){
    const double LOG2E = 1.442695040;
    const uint64_t SHIFT = static_cast<uint64_t>(1) << 52;
    x *= LOG2E;
    double xi = floor(x);
    double xf = x - xi;
    double xf2 = xf * xf;
    double xf3 = xf2 * xf;
    double xf4 = xf2 * xf2;
    double xf5 = xf2 * xf3;
    double xf6 = xf3 * xf3;
    double xf7 = xf3 * xf4;
    double xf8 = xf4 * xf4;

    const double coef[9] = {
        2.00811867e-12,
        6.93147180e-01,
        2.40226512e-01,
        5.55040573e-02,
        9.61838113e-03,
        1.33265219e-03,
        1.55193275e-04,
        1.41484217e-05,
        1.87497191e-06
    };

    double k = \
        + coef[8] * xf8 \
        + coef[7] * xf7 \
        + coef[6] * xf6 \
        + coef[5] * xf5 \
        + coef[4] * xf4 \
        + coef[3] * xf3 \
        + coef[2] * xf2 \
        + coef[1] * xf \
        + coef[0] + 1.;

    uint64_t e = reinterpret_cast<const uint64_t &>(k);
    e += SHIFT * static_cast<uint64_t>(xi);
    return reinterpret_cast<double &>(e);
}
