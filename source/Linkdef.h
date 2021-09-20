#ifdef __CINT__

#    pragma link off all globals;
#    pragma link off all classes;
#    pragma link off all functions;
#    pragma link off all namespace;

#    pragma link C++ nestedclasses;
#    pragma link C++ nestedtypedef;
#    pragma link C++ namespace tools;
#    pragma link C++ namespace tools::Math;
#    pragma link C++ defined_in tools;

#    pragma link C++ class tools::DataBase;
#    pragma link C++ class tools::DataTable;
#    pragma link C++ class tools::DataBaseError;
#    pragma link C++ class tools::DualStream;
#    pragma link C++ class tools::DualStreamBuf;
#    pragma link C++ class tools::Error;
#    pragma link C++ class tools::ProgressBar;
#    pragma link C++ class tools::Rand;
#    pragma link C++ class tools::Random;
#    pragma link C++ class tools::Regex;
#    pragma link C++ class tools::Vector3 + ;
#    pragma link C++ typedef tools::hash_t;
#    pragma link C++ function tools::StringHash;
#    pragma link C++ function tools::cat < char>;
#    pragma link C++ function tools::scat < char>;
#    pragma link C++ function tools::Math::Pi;
#    pragma link C++ function tools::Math::TwoPi;
#    pragma link C++ function tools::Math::PiSq;
#    pragma link C++ function tools::Math::PiOver2;
#    pragma link C++ function tools::Math::PiOver4;
#    pragma link C++ function tools::Math::InvPi;
#    pragma link C++ function tools::Math::RadToDeg;
#    pragma link C++ function tools::Math::DegToRad;
#    pragma link C++ function tools::Math::Sqrt2;
#    pragma link C++ function tools::Math::sq < int>;
#    pragma link C++ function tools::Math::square < int>;
#    pragma link C++ function tools::Math::cube < int>;
#    pragma link C++ function tools::Math::pow_n < int>;
#    pragma link C++ function tools::Math::sum2 < int>;
#    pragma link C++ function tools::Math::norm2 < int>;
#    pragma link C++ function tools::Math::diff2 < int>;

#    pragma link off namespace* detail*;
#    pragma link off defined_in* detail*;

#endif  // __CINT__
