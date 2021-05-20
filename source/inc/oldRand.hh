// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2015-03-17
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef RAND_HH
#define RAND_HH 1

#include <chrono>
#include <cmath>
#include <functional>
#include <random>
#include <type_traits>
#include <utility>

#include "Math.hh"
#include "Vector3.hh"

namespace tools
{
// a useful typedef
template<class vector = Vector3>
class Random;
using Rand = Random<Vector3>;

// template machinery to test the presence of methods of G4ThreeVector
namespace detail_rand
{
template<class T>
static auto test_setRThetaPhi(int)  // NOLINT(readability/casting)
    -> decltype(void(std::declval<T>().setRhoPhiTheta(0., 0., 0.)), std::true_type{});
template<class T>
static auto test_setRThetaPhi(float) -> std::false_type;

template<class T>
static auto test_setRhoPhiZ(int)  // NOLINT(readability/casting)
    -> decltype(void(std::declval<T>().setRhoPhiZ(0., 0., 0.)), std::true_type{});
template<class T>
static auto test_setRhoPhiZ(float) -> std::false_type;

template<class T>
static auto test_setX(int)  // NOLINT(readability/casting)
    -> decltype(void(std::declval<T>().setX(0.)), std::true_type{});
template<class T>
static auto test_setX(float) -> std::false_type;
}  // namespace detail_rand

template<class T>
struct has_setRThetaPhi: decltype(detail_rand::test_setRThetaPhi<T>(0)) {};

template<class T>
struct has_setRhoPhiZ: decltype(detail_rand::test_setRhoPhiZ<T>(0)) {};

template<class T>
struct has_setX: decltype(detail_rand::test_setX<T>(0)) {};

// the Random class, almost a namespace (everything is static!)
template<class vector>
class Random {
private:
    Random() = delete;
    Random(Random&) = delete;

    static decltype(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) theSeed;
    static std::default_random_engine theGenerator;

    static std::uniform_real_distribution<double> uniform_real_distro;
    static std::normal_distribution<double> gaussian_distro;
    static std::bernoulli_distribution bernouilli_distro;
    static std::binomial_distribution<int> binomial_distro;
    static std::geometric_distribution<int> geometric_distro;
    static std::poisson_distribution<int> poisson_distro;
    static std::exponential_distribution<double> exponential_distro;
    static std::gamma_distribution<double> gamma_distro;
    static std::chi_squared_distribution<double> chi2_distro;

    static std::function<double(void)> UniformRand;
    static std::function<double(void)> GaussianRand;
    static std::function<bool(void)> BernouilliRand;
    static std::function<int(void)> BinomialRand;
    static std::function<int(void)> GeometricRand;
    static std::function<int(void)> PoissonRand;
    static std::function<double(void)> ExponentialRand;
    static std::function<double(void)> GammaRand;
    static std::function<double(void)> Chi2Rand;

public:
    enum Axis
    {
        X = 0,
        Y = 1,
        Z = 2
    };

    inline static auto GetSeed() -> decltype(theSeed) const { return theSeed; }
    inline static auto GetGenerator() -> decltype(theGenerator) const {
        return theGenerator;
    }

    inline static double Uniform() { return UniformRand(); }
    inline static double Uniform(double max) { return max * UniformRand(); }
    inline static double Uniform(double min, double max) {
        return min + (max - min) * UniformRand();
    }

    inline static double Gauss() { return GaussianRand(); }
    inline static void SetGaussPar(double mean, double stddev) {
        gaussian_distro.param(
            std::normal_distribution<double>::param_type(mean, stddev));
    }
    inline static double Gauss(double mean, double stddev) {
        return mean + stddev * GaussianRand();
    }

    inline static bool Bernouilli() { return BernouilliRand(); }
    inline static void SetBernouilliPar(double p) {
        bernouilli_distro.param(std::bernoulli_distribution::param_type(p));
    }
    inline static bool Bernouilli(double p) {
        SetBernouilliPar(p);
        return BernouilliRand();
    }

    inline static int Binomial() { return BinomialRand(); }
    inline static void SetBinomialPar(int t, double p) {
        binomial_distro.param(std::binomial_distribution<int>::param_type(t, p));
    }
    inline static int Binomial(int t, double p) {
        SetBinomialPar(t, p);
        return BinomialRand();
    }

    inline static int Geometric() { return GeometricRand(); }
    inline static void SetGeometric(double p) {
        geometric_distro.param(std::geometric_distribution<int>::param_type(p));
    }
    inline static int Geometric(double p) {
        SetGeometric(p);
        return GeometricRand();
    }

    inline static int Poisson() { return PoissonRand(); }
    inline static void SetPoissonPar(double mean) {
        poisson_distro.param(std::poisson_distribution<int>::param_type(mean));
    }
    inline static int Poisson(double mean) {
        SetPoissonPar(mean);
        return PoissonRand();
    }

    inline static double Exp() { return ExponentialRand(); }
    inline static void SetExpPar(double lambda) {
        exponential_distro.param(
            std::exponential_distribution<double>::param_type(lambda));
    }
    inline static double Exp(double lambda) {
        SetExpPar(lambda);
        return ExponentialRand();
    }

    inline static double Gamma() { return GammaRand(); }
    inline static void SetGammaPar(double alpha, double beta) {
        gamma_distro.param(std::gamma_distribution<double>::param_type(alpha, beta));
    }
    inline static double Gamma(double alpha, double beta) {
        SetGammaPar(alpha, beta);
        return GammaRand();
    }

    inline static double Chi2() { return Chi2Rand(); }
    inline static void SetChi2Par(double n) {
        chi2_distro.param(std::chi_squared_distribution<double>::param_type(n));
    }
    inline static double Chi2(double n) {
        SetChi2Par(n);
        return Chi2Rand();
    }

    // Shoot a direction, the actual implementation is at the end of the file
private:  // the different implementations
    static const vector Dir_impl(std::true_type const&);
    static const vector Dir_impl(std::false_type const&);

    static const vector Cyl_imp(
        double rho, double hh, Random<vector>::Axis axis, std::true_type const&);
    static const vector Cyl_imp(
        double rho, double hh, Random<vector>::Axis axis, std::false_type const&);

    static const vector Cub_impl(double a, double b, double c, std::true_type const&);
    static const vector Cub_impl(double a, double b, double c, std::false_type const&);

public:  // the wrappers around the implementations
    inline static const vector Direction() {
        return Dir_impl(has_setRThetaPhi<vector>());
    }

    static const vector Sphere(double r);
    static const vector Sphere(double r, const vector& center);

    inline static const vector Cylinder(
        double rho, double hh, Random<vector>::Axis axis = Z) {
        return Cyl_imp(rho, hh, axis, has_setRhoPhiZ<vector>());
    }
    static const vector Cylinder(
        double rho, double hh, const vector& center, Random<vector>::Axis axis = Z);

    inline static const vector Cuboid(double a, double b, double c) {
        return Cub_impl(a, b, c, has_setX<vector>());
    }
    static const vector Cuboid(double a, double b, double c, const vector& center);
};

/// initialisation of members (static but template so in header)
template<class vector>
decltype(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    Random<vector>::theSeed =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
template<class vector>
std::default_random_engine Random<vector>::theGenerator(theSeed);

template<class vector>
std::uniform_real_distribution<double> Random<vector>::uniform_real_distro(0., 1.);
template<class vector>
std::normal_distribution<double> Random<vector>::gaussian_distro(0., 1.);
template<class vector>
std::bernoulli_distribution Random<vector>::bernouilli_distro(0.5);
template<class vector>
std::binomial_distribution<int> Random<vector>::binomial_distro(1l, 0.5);
template<class vector>
std::geometric_distribution<int> Random<vector>::geometric_distro(0.5);
template<class vector>
std::poisson_distribution<int> Random<vector>::poisson_distro(1.);
template<class vector>
std::exponential_distribution<double> Random<vector>::exponential_distro(1.);
template<class vector>
std::gamma_distribution<double> Random<vector>::gamma_distro(1., 1.);
template<class vector>
std::chi_squared_distribution<double> Random<vector>::chi2_distro(1.);

template<class vector>
std::function<double(void)> Random<vector>::UniformRand =
    std::bind(uniform_real_distro, theGenerator);
template<class vector>
std::function<double(void)> Random<vector>::GaussianRand =
    std::bind(gaussian_distro, theGenerator);
template<class vector>
std::function<bool(void)> Random<vector>::BernouilliRand =
    std::bind(bernouilli_distro, theGenerator);
template<class vector>
std::function<int(void)> Random<vector>::BinomialRand =
    std::bind(binomial_distro, theGenerator);
template<class vector>
std::function<int(void)> Random<vector>::GeometricRand =
    std::bind(geometric_distro, theGenerator);
template<class vector>
std::function<int(void)> Random<vector>::PoissonRand =
    std::bind(poisson_distro, theGenerator);
template<class vector>
std::function<double(void)> Random<vector>::ExponentialRand =
    std::bind(exponential_distro, theGenerator);
template<class vector>
std::function<double(void)> Random<vector>::GammaRand =
    std::bind(gamma_distro, theGenerator);
template<class vector>
std::function<double(void)> Random<vector>::Chi2Rand =
    std::bind(chi2_distro, theGenerator);

/// definition of method non depending on the actual type of vector
// Sphere
template<class vector>
inline const vector Random<vector>::Sphere(double r) {
    vector v;
    v = std::pow(UniformRand() * r * r * r, 1. / 3.) * Direction();
    return std::move(v);
}
template<class vector>
inline const vector Random<vector>::Sphere(double r, const vector& center) {
    vector v;
    v = center + Sphere(r);
    return std::move(v);
}

// Cylinder
template<class vector>
inline const vector Random<vector>::Cylinder(
    double rho, double hh, const vector& center, Random<vector>::Axis axis) {
    vector v;
    v = center + Cylinder(rho, hh, axis);
    return std::move(v);
}

// Cuboid
template<class vector>
inline const vector Random<vector>::Cuboid(
    double a, double b, double c, const vector& center) {
    vector v;
    v = center + Cuboid(a, b, c);
    return std::move(v);
}

/// Implementation of method depending on the actual type of vector
// Direction
template<class vector>
inline const vector Random<vector>::Dir_impl(std::true_type const&) {
    vector v;
    v.setRThetaPhi(
        1., std::acos(1. - 2. * UniformRand()), Math::TwoPi() * UniformRand());
    return std::move(v);
}
template<class vector>
inline const vector Random<vector>::Dir_impl(std::false_type const&) {
    vector v;
    v.SetMagThetaPhi(
        1., std::acos(1. - 2. * UniformRand()), Math::TwoPi() * UniformRand());
    return std::move(v);
}

// Cylinder
template<class vector>
inline const vector Random<vector>::Cyl_imp(
    double rho, double hh, Random<vector>::Axis axis, const std::true_type&) {
    vector v;
    v.setRhoPhiZ(
        rho * UniformRand(),
        Math::TwoPi() * UniformRand(),
        hh * (2. * UniformRand() - 1.));
    switch (axis) {
        case X:
            v.rotateY(Math::PiOver2());
            break;
        case Y:
            v.rotateX(Math::PiOver2());
            break;
        default:
            break;
    }
    return std::move(v);
}

template<class vector>
inline const vector Random<vector>::Cyl_imp(
    double rho, double hh, Random<vector>::Axis axis, const std::false_type&) {
    vector v;
    v.SetRhoPhiZ(
        rho * UniformRand(),
        Math::TwoPi() * UniformRand(),
        hh * (2. * UniformRand() - 1.));
    switch (axis) {
        case X:
            v.RotateY(Math::PiOver2());
            break;
        case Y:
            v.RotateX(Math::PiOver2());
            break;
        default:
            break;
    }
    return std::move(v);
}

// Cuboid
template<class vector>
inline const vector Random<vector>::Cub_impl(
    double a, double b, double c, const std::true_type&) {
    vector v;
    v.setX(a * (UniformRand() - 0.5));
    v.setY(b * (UniformRand() - 0.5));
    v.setZ(c * (UniformRand() - 0.5));
    return std::move(v);
}

template<class vector>
inline const vector Random<vector>::Cub_impl(
    double a, double b, double c, const std::false_type&) {
    vector v;
    v.SetX(a * (UniformRand() - 0.5));
    v.SetY(b * (UniformRand() - 0.5));
    v.SetZ(c * (UniformRand() - 0.5));
    return std::move(v);
}

}  // namespace tools

#endif  // RAND_HH
