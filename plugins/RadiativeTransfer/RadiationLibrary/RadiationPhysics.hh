#ifndef COOLFluiD_RadiativeTransfer_RadiationPhysics_hh
#define COOLFluiD_RadiativeTransfer_RadiationPhysics_hh


#include "Common/OwnedObject.hh"
#include "Common/SetupObject.hh"
#include "Common/NonCopyable.hh"
#include "Environment/ConcreteProvider.hh"
#include "RadiativeTransfer/RadiativeTransferModule.hh"
#include "Environment/ObjectProvider.hh"
#include "Radiator.hh"
#include "RadiationPhysicsHandler.hh"
#include "Reflector.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

namespace RadiativeTransfer {

class Radiator;
class Reflector;

namespace RadiativeDistTypes{
  enum TRStypeID {WALL=0, MEDIUM=1, BOUNDARY=2 };
  enum DistPhysicsID {ABSORPTION, EMISSION, SCATTERING, REFLECTION};
}

using namespace RadiativeDistTypes;

//////////////////////////////////////////////////////////////////////////////
class RadiationPhysics : public Common::OwnedObject,
                         public Config::ConfigObject
{
public:

  typedef Environment::ConcreteProvider<RadiationPhysics,1> PROVIDER;
  typedef const std::string& ARG1;

  RadiationPhysics(const std::string& name);
  ~RadiationPhysics(){}
  void setupSectra(CFreal wavMin,CFreal wavMax);
  void setup();
  void configure(Config::ConfigArgs& args);
  static void defineConfigOptions(Config::OptionList& options);
  static std::string getClassName() { return "RadiationPhysics"; }

  inline std::string getTRSname(){return m_TRSname;}
  inline std::string getTRStype(){return m_TRStype;}
  inline TRStypeID getTRStypeID(){return m_TRStypeID;}

  inline void getRadPhysicsPtr(RadiationPhysics* ptr) { ptr = this; }

  void getWallStateIDs(std::vector<CFuint> &statesID,
                       std::vector<CFuint>& wallGeoIdx);

  void getCellStateIDs(std::vector<CFuint> &statesID);

  inline Framework::State* getWallState(CFuint wallTrsIdx){return &(m_interpolatedStates[wallTrsIdx]);}


  inline void setRadPhysicsHandlerPtr( RadiationPhysicsHandler* ptr ) {
    m_radPhysicsHandlerPtr = ptr;
  }
  inline Radiator* const getRadiatorPtr(){return m_radiator.getPtr();}
  inline Reflector* const getReflectorPtr(){return m_reflector.getPtr();}


  void computeInterpolatedStates();
private:
  Common::SelfRegistPtr< Radiator > m_radiator;
  Common::SelfRegistPtr< Reflector > m_reflector;
  RadiationPhysicsHandler* m_radPhysicsHandlerPtr;
  TRStypeID m_TRStypeID;
  std::string m_radiatorName;
  std::string m_reflectorName;
  std::string m_TRSname;
  std::string m_TRStype;
  std::vector<Framework::State> m_interpolatedStates;
  std::vector<CFreal> m_faceAreas;

};

//////////////////////////////////////////////////////////////////////////////
}
}

#endif
