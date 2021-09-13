
#ifndef BHV_SIMPLE_DEPTH_HEADER
#define BHV_SIMPLE_DEPTH_HEADER

#include "IvPBehavior.h"

class BHV_SimpleDepth : public IvPBehavior {
public:
  BHV_SimpleDepth(IvPDomain);
  ~BHV_SimpleDepth() {}
  
  void         onIdleState() {updateInfoIn();}
  IvPFunction* onRunState();
  bool         setParam(std::string, std::string);

 protected:
  bool         updateInfoIn();

 protected: // Configuration variables
  double      m_desired_depth;
  double      m_peakwidth;
  double      m_basewidth;
  double      m_summitdelta;
  std::string m_depth_mismatch_var;

 protected: // State variables
  double      m_osd;
};

#ifdef WIN32
	// Windows needs to explicitly specify functions to export from a dll
   #define IVP_EXPORT_FUNCTION __declspec(dllexport) 
#else
   #define IVP_EXPORT_FUNCTION
#endif

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_SimpleDepth(domain);}
}
#endif

