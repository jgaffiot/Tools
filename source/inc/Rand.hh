// --------------------------------------------------------------------------//
/**
 * AUTHOR: J. Gaffiot
 * DATE: 2016-04-04
 * CONTACT: j.gaffiot@laposte.net
 */
// --------------------------------------------------------------------------//

#ifndef RAND_HH
#define RAND_HH 2

#include <Math.hh>
#include <Vector3.hh>

#include <random>
#include <utility>
#include <cmath>
#include <chrono>
#include <functional>
#include <type_traits>

namespace tools {

// a useful typedef
template<class vector=Vector3> class Random;
using Rand = Random<Vector3>;

// template machinery to test the presence of methods of G4ThreeVector
namespace detail_rand {
  // define void_t for all versions of C++ before C++17, which as of April 2017 can only be tested by "after C++14"
#if __cplusplus > 201402L
  // void_t is already defined for C++17
#else
  template< class... > using void_t = void;
#endif

  // test Spherical coordinates method
  template <typename, typename = void>
  struct has_setRThetaPhi : std::false_type {};

  template <typename T>
  struct has_setRThetaPhi<T, void_t<decltype(std::declval<T>().setRThetaPhi(0.,0.,0.))>> : std::true_type {};

  template <typename, typename = void>
  struct has_SetMagThetaPhi : std::false_type {};

  template <typename T>
  struct has_SetMagThetaPhi<T, void_t<decltype(std::declval<T>().SetMagThetaPhi(0.,0.,0.))>> : std::true_type {};

  // test Cylindrical coordinates method
  template <typename, typename = void>
  struct has_setRhoPhiZ : std::false_type {};

  template <typename T>
  struct has_setRhoPhiZ<T, void_t<decltype(std::declval<T>().setRhoPhiZ(0.,0.,0.))>> : std::true_type {};

  template <typename, typename = void>
  struct has_SetRhoPhiZ : std::false_type {};

  template <typename T>
  struct has_SetRhoPhiZ<T, void_t<decltype(std::declval<T>().SetRhoPhiZ(0.,0.,0.))>> : std::true_type {};

  // test Cartesian coordinates method
  template <typename, typename = void>
  struct has_setX : std::false_type {};

  template <typename T>
  struct has_setX<T, void_t<decltype(std::declval<T>().setX(0.))>> : std::true_type {};

  template <typename, typename = void>
  struct has_SetX : std::false_type {};

  template <typename T>
  struct has_SetX<T, void_t<decltype(std::declval<T>().SetX(0.))>> : std::true_type {};

  // test rotation method
  template <typename, typename = void>
  struct has_rotateX: std::false_type {};

  template <typename T>
  struct has_rotateX<T, void_t<decltype(std::declval<T>().rotateX(0.))>> : std::true_type {};

  template <typename, typename = void>
  struct has_RotateX : std::false_type {};

  template <typename T>
  struct has_RotateX<T, void_t<decltype(std::declval<T>().RotateX(0.))>> : std::true_type {};
} // namespace detail_rand


// the Random class, almost a namespace (everything is static!)
template<class vector>
class Random
{
private:
  Random() = delete;
  Random(Random&) = delete;

  using clock_count_ts = decltype(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  static clock_count_ts theSeed;
  static std::default_random_engine theGenerator;

  static std::uniform_real_distribution<double> uniform_real_distro;
  static std::normal_distribution <double> gaussian_distro;
  static std::bernoulli_distribution bernouilli_distro;
  static std::binomial_distribution<int> binomial_distro;
  static std::geometric_distribution<int> geometric_distro;
  static std::poisson_distribution<int> poisson_distro;
  static std::exponential_distribution<double> exponential_distro;
  static std::gamma_distribution<double> gamma_distro;
  static std::chi_squared_distribution<double> chi2_distro;

  static std::function<double(void)> UniformRand;
  static std::function<double(void)> GaussianRand;
  static std::function<bool(void)>   BernouilliRand;
  static std::function<int(void)>    BinomialRand;
  static std::function<int(void)>    GeometricRand;
  static std::function<int(void)>    PoissonRand;
  static std::function<double(void)> ExponentialRand;
  static std::function<double(void)> GammaRand;
  static std::function<double(void)> Chi2Rand;

public:
  enum Axis { X=0, Y=1, Z=2};

  inline static auto GetSeed() { return theSeed; }
  inline static auto GetGenerator() { return theGenerator; }

  inline static double Uniform() { return uniform_real_distro(theGenerator); }
  inline static double Uniform(double max) { return max * uniform_real_distro(theGenerator); }
  inline static double Uniform(double min, double max) { return min + (max-min) * uniform_real_distro(theGenerator); }

  inline static double Gauss() { return gaussian_distro(theGenerator); }
  inline static void   SetGaussPar(double mean, double stddev) { gaussian_distro.param(std::normal_distribution<double>::param_type(mean,stddev)); }
  inline static double Gauss(double mean, double stddev) { return mean + stddev * gaussian_distro(theGenerator); }

  inline static bool   Bernouilli() { return bernouilli_distro(theGenerator); }
  inline static void   SetBernouilliPar(double p) { bernouilli_distro.param(std::bernoulli_distribution::param_type(p)); }
  inline static bool   Bernouilli(double p) { SetBernouilliPar(p); return bernouilli_distro(theGenerator); }

  inline static int    Binomial() { return binomial_distro(theGenerator); }
  inline static void   SetBinomialPar(int t, double p) { binomial_distro.param(std::binomial_distribution<int>::param_type(t,p)); }
  inline static int    Binomial(int t, double p) { SetBinomialPar(t,p); return binomial_distro(theGenerator); }

  inline static int    Geometric() { return geometric_distro(theGenerator); }
  inline static void   SetGeometric(double p) { geometric_distro.param(std::geometric_distribution<int>::param_type(p)); }
  inline static int    Geometric(double p) { SetGeometric(p); return geometric_distro(theGenerator); }

  inline static int    Poisson() { return poisson_distro(theGenerator); }
  inline static void   SetPoissonPar(double mean) { poisson_distro.param(std::poisson_distribution<int>::param_type(mean)); }
  inline static int    Poisson(double mean) { SetPoissonPar(mean); return poisson_distro(theGenerator); }

  inline static double Exp() { return exponential_distro(theGenerator); }
  inline static void   SetExpPar(double lambda) { exponential_distro.param(std::exponential_distribution<double>::param_type(lambda)); }
  inline static double Exp(double lambda) { SetExpPar(lambda); return exponential_distro(theGenerator); }

  inline static double Gamma() { return gamma_distro(theGenerator); }
  inline static void   SetGammaPar(double alpha, double beta) { gamma_distro.param(std::gamma_distribution<double>::param_type(alpha,beta)); }
  inline static double Gamma(double alpha, double beta) { SetGammaPar(alpha, beta); return gamma_distro(theGenerator); }

  inline static double Chi2() { return chi2_distro(theGenerator); }
  inline static void   SetChi2Par(double n) { chi2_distro.param(std::chi_squared_distribution<double>::param_type(n)); }
  inline static double Chi2(double n) { SetChi2Par(n); return chi2_distro(theGenerator); }

public:  // Shoot a direction
  static const vector Direction();

  static const vector Sphere(double r, const vector& center);
  inline static const vector Sphere(double r) { return std::move(Sphere(r,{})); }

  static const vector Cylinder(double rho, double hh, const vector& center, Random<vector>::Axis axis=Z);
  inline static const vector Cylinder(double rho, double hh, Random<vector>::Axis axis=Z) { return std::move(Cylinder(rho,hh,axis,{})); }

  static const vector Cuboid(double a, double b, double c, const vector& center);
  inline static const vector Cuboid(double a, double b, double c) { return Cuboid(a,b,c,{}); }

private: // the different implementations wrapping the different methods to set the vectors
  /// Spherical coordinates
  // With setRThetaPhi
  template <typename T=vector>
  static const auto SetSphericalCoordinate(T& v)
    -> typename std::enable_if<detail_rand::has_setRThetaPhi<T>{}>::type {
    v.setRThetaPhi(1.,std::acos(1.-2.*Uniform()),Math::TwoPi()*Uniform());
  }

  // With SetMagThetaPhi
  template <typename T=vector>
  static const auto SetSphericalCoordinate(T& v)
    -> typename std::enable_if<detail_rand::has_SetMagThetaPhi<T>{}>::type {
    v.SetMagThetaPhi(1.,std::acos(1.-2.*Uniform()),Math::TwoPi()*Uniform());
  }

  // None of the above, provoque a compilation error with a meaningful message
  template <typename T=vector>
  static const auto SetSphericalCoordinate(T& v)
    -> typename std::enable_if< (not detail_rand::has_setRThetaPhi<T>{})
                            and (not detail_rand::has_SetMagThetaPhi<T>{})
			    >::type {
    static_assert( (detail_rand::has_setRThetaPhi<T>::value)
		or (detail_rand::has_SetMagThetaPhi<T>::value),
		"No method found to define a vector in spherical coordinates");
  }

  /// Cylindrical coordinates
  // With setRhoPhiZ
  template <typename T=vector>
  static const auto SetCylindricalCoordinate(T& v, double rho, double hh, const Random<vector>::Axis& axis)
    -> typename std::enable_if<detail_rand::has_setRhoPhiZ<T>{}>::type {
    v.setRhoPhiZ(rho*Uniform(),Math::TwoPi()*Uniform(),hh*(2.*Uniform()-1.));
    SetRotation(v);
  }

  // With SetRhoPhiZ
  template <typename T=vector>
  static const auto SetCylindricalCoordinate(T& v, double rho, double hh, const Random<vector>::Axis& axis)
    -> typename std::enable_if<detail_rand::has_SetRhoPhiZ<T>{}>::type {
    v.SetRhoPhiZ(rho*Uniform(),Math::TwoPi()*Uniform(),hh*(2.*Uniform()-1.));
    SetRotation(v);
  }

  // None of the above, provoque a compilation error with a meaningful message
  template <typename T=vector>
  static const auto SetCylindricalCoordinate(T& v, double rho, double hh, const Random<vector>::Axis& axis)
    -> typename std::enable_if< not detail_rand::has_setRhoPhiZ<T>{}
                            and not detail_rand::has_SetRhoPhiZ<T>{}
                            >::type {
    static_assert( detail_rand::has_setRhoPhiZ<T>{}
		or detail_rand::has_SetRhoPhiZ<T>{},
		"No method found to define a vector in cylindrical coordinates");
  }

  /// Rotation
  // With rotateX
  template <typename T=vector>
  static const auto SetRotation(T& v, const Random<vector>::Axis& axis)
    -> typename std::enable_if<detail_rand::has_rotateX<T>{}>::type {
    switch(axis) {
      case X: v.rotateY(Math::PiOver2()); break;
      case Y: v.rotateX(Math::PiOver2()); break;
      default: break;
    }
  }

  // With RotateX
  template <typename T=vector>
  static const auto SetRotation(T& v, const Random<vector>::Axis& axis)
    -> typename std::enable_if<detail_rand::has_RotateX<T>{}>::type {
    switch(axis) {
      case X: v.RotateY(Math::PiOver2()); break;
      case Y: v.RotateX(Math::PiOver2()); break;
      default: break;
    }
  }

  // None of the above, provoque a compilation error with a meaningful message
  template <typename T=vector>
  static const auto SetRotation(T& v, const Random<vector>::Axis& axis)
    -> typename std::enable_if< not detail_rand::has_rotateX<T>{}
                            and not detail_rand::has_RotateX<T>{}
                            >::type {
    static_assert( detail_rand::has_rotateX<T>{}
		or detail_rand::has_RotateX<T>{},
		"No method found to rotate a vector");
  }

  /// Cartesian coordinates
  // With setX
  template <typename T=vector>
  static const auto SetCartesianCoordinate(T& v, double a, double b, double c)
    -> typename std::enable_if<detail_rand::has_setX<T>{}>::type {
    v.setX(a*(Uniform()-0.5));
    v.setY(b*(Uniform()-0.5));
    v.setZ(c*(Uniform()-0.5));
  }

  // With SetX
  template <typename T=vector>
  static const auto SetCartesianCoordinate(T& v, double a, double b, double c)
    -> typename std::enable_if<detail_rand::has_SetX<T>{}>::type {
    v.SetX(a*(Uniform()-0.5));
    v.SetY(b*(Uniform()-0.5));
    v.SetZ(c*(Uniform()-0.5));
    }

  // None of the above, provoque a compilation error with a meaningful message
  template <typename T=vector>
  static const auto SetCartesianCoordinate(T& v, double a, double b, double c)
    -> typename std::enable_if< not detail_rand::has_setX<T>{}
                            and not detail_rand::has_SetX<T>{}
                            >::type {
    static_assert( detail_rand::has_setX<T>{}
		or detail_rand::has_SetX<T>{},
		"No method found to define a vector in Cartesian coordinates");
  }
};

/// initialisation of members (static but template so in header)
template<class vector> typename Random<vector>::clock_count_ts
Random<vector>::theSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
template<class vector> 
std::default_random_engine Random<vector>::theGenerator(theSeed);

template<class vector>
std::uniform_real_distribution<double> Random<vector>::uniform_real_distro(0.,1.);
template<class vector>
std::normal_distribution<double>       Random<vector>::gaussian_distro(0.,1.);
template<class vector>
std::bernoulli_distribution            Random<vector>::bernouilli_distro(0.5);
template<class vector>
std::binomial_distribution<int>        Random<vector>::binomial_distro(1l,0.5);
template<class vector>
std::geometric_distribution<int>       Random<vector>::geometric_distro(0.5);
template<class vector>
std::poisson_distribution<int>         Random<vector>::poisson_distro(1.);
template<class vector>
std::exponential_distribution<double>  Random<vector>::exponential_distro(1.);
template<class vector>
std::gamma_distribution<double>        Random<vector>::gamma_distro(1.,1.);
template<class vector>
std::chi_squared_distribution<double>  Random<vector>::chi2_distro(1.);

// Bindings of the distributions and the generator, for convenient use,
// but std::function is slow, prefer distro(theGenerator) if performance is a real concern
template<class vector>
std::function<double(void)> Random<vector>::UniformRand     = std::bind( uniform_real_distro, theGenerator );
template<class vector>
std::function<double(void)> Random<vector>::GaussianRand    = std::bind( gaussian_distro, theGenerator );
template<class vector>
std::function<bool(void)>   Random<vector>::BernouilliRand  = std::bind( bernouilli_distro, theGenerator );
template<class vector>
std::function<int(void)>    Random<vector>::BinomialRand    = std::bind( binomial_distro, theGenerator );
template<class vector>
std::function<int(void)>    Random<vector>::GeometricRand   = std::bind( geometric_distro, theGenerator );
template<class vector>
std::function<int(void)>    Random<vector>::PoissonRand     = std::bind( poisson_distro, theGenerator );
template<class vector>
std::function<double(void)> Random<vector>::ExponentialRand = std::bind( exponential_distro, theGenerator );
template<class vector>
std::function<double(void)> Random<vector>::GammaRand       = std::bind( gamma_distro, theGenerator );
template<class vector>
std::function<double(void)> Random<vector>::Chi2Rand        = std::bind( chi2_distro, theGenerator );

/// definition of method non depending on the actual type of vector
// Direction
template<class vector>
const vector Random<vector>::Direction()
{
  vector v;
  SetSphericalCoordinate(v);
  return std::move(v);
}

// Sphere
template<class vector>
inline const vector Random<vector>::Sphere(double r, const vector& center)
{
  constexpr double one_third = 1./3.;
  vector v(center);
  v += std::pow(uniform_real_distro(theGenerator)*r*r*r,one_third) * Direction();
  return std::move(v);
}

// Cylinder
template<class vector>
inline const vector Random<vector>::Cylinder(double rho, double hh, const vector& center, Random<vector>::Axis axis)
{
  vector v();
  SetCylindricalCoordinate(v, rho, hh, axis);
  v += center;
  return std::move(v);
}

// Cuboid
template<class vector>
inline const vector Random<vector>::Cuboid(double a, double b, double c, const vector& center)
{
  vector v();
  SetCartesianCoordinate(v, a, b, c);
  v += center;
  return std::move(v);
}


} // namespace tools

#endif // RAND_HH
