#ifndef COOLFluiD_Numerics_FluctSplit_STM_NSchemeScalar_hh
#define COOLFluiD_Numerics_FluctSplit_STM_NSchemeScalar_hh

//////////////////////////////////////////////////////////////////////////////

#include "STM_SplitterScalar.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {



    namespace FluctSplit {

//////////////////////////////////////////////////////////////////////////////

/**
 * This class represents the SpaceTimeN scheme for RDS space discretization
 *
 * @author Thomas Wuilbaut
 *
 */
class STM_NSchemeScalar : public STM_SplitterScalar {
public:

  /**
   * Default constructor.
   */
  STM_NSchemeScalar(const std::string& name);

  /**
   * Default destructor
   */
  ~STM_NSchemeScalar();

  /**
   * Set up
   */
  void setup();

  /**
   * Distribute the residual
   */
  void distribute(std::vector<RealVector>& residual);

  /**
   * Distribute the residual (contribution from past states)
   */
  void distributePast(const std::vector<Framework::State*>& tStates);

private:

  RealVector _sumKmin;

  RealVector _sumKplus;

  RealVector _sumKminU;

  RealVector _uInflow;

  RealVector _uMin;

}; // end of class STM_NSchemeScalar

//////////////////////////////////////////////////////////////////////////////

    } // namespace FluctSplit



} // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif // COOLFluiD_Numerics_FluctSplit_NSchemeScalar_hh
