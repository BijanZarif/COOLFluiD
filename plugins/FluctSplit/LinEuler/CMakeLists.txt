LIST ( APPEND OPTIONAL_dirfiles 
  SaveSourceData.cxx
  LoadSourceData3Dcomp.cxx
  Acoustic_ArtDiffStrategy_HO.cxx
  Acoustic_ArtDiffStrategy_HO.hh
  SaveSourceData.hh
  LoadSourceData3Dcomp.hh
)

LIST ( APPEND FluctSplitLinEuler_files
FluctSplitLinEuler.hh

Acoustic_ArtDiffStrategy.hh
Acoustic_ArtDiffStrategy.cxx

# boundary conditions
StrongReflectiveWallLinearizedEuler2DCons.hh
StrongReflectiveWallLinearizedEuler2DCons.cxx
StrongReflectiveWallLinearizedEuler2DConsImpl.hh
StrongReflectiveWallLinearizedEuler2DConsImpl.cxx
StrongReflectiveWallLinearizedEuler3DCons.hh
StrongReflectiveWallLinearizedEuler3DCons.cxx

StrongSubOutletHedstrom2DCons.hh
StrongSubOutletHedstrom2DCons.cxx
StrongSubInletHedstrom2DCons.hh
StrongSubInletHedstrom2DCons.cxx
StrongSubInletHedstrom2DConsImpl.hh
StrongSubInletHedstrom2DConsImpl.cxx
WeakSubOutletHedsrtom2DCons.cxx
WeakSubOutletHedsrtom2DCons.hh
StrongSubOutletHedstrom3DCons.hh
StrongSubOutletHedstrom3DCons.cxx
StrongSubInletHedstrom3DCons.hh
StrongSubInletHedstrom3DCons.cxx

StrongSubOutletLinEuler2DCons.hh
StrongSubOutletLinEuler2DCons.cxx
StrongSubOutletLinEuler2DConsImpl.hh
StrongSubOutletLinEuler2DConsImpl.cxx
StrongSubOutletLinEuler3DCons.hh
StrongSubOutletLinEuler3DCons.cxx
StrongSubInletLinEuler2DCons.hh
StrongSubInletLinEuler2DCons.cxx
StrongSubInletLinEuler3DCons.hh
StrongSubInletLinEuler3DCons.cxx
StrongSubFunctionInletLinEuler2DCons.cxx
StrongSubFunctionInletLinEuler2DCons.hh
StrongSubFunctionInletLinEuler2DConsImpl.cxx
StrongSubFunctionInletLinEuler2DConsImpl.hh
StrongSubFunctionInletLinEuler3DCons.cxx
StrongSubFunctionInletLinEuler3DCons.hh

# sources
LinearizedEuler2DSourceTermDampingZone.hh
LinearizedEuler2DSourceTermDampingZone.cxx
LinearizedEuler2DSourceTermDampingMonopole.hh
LinearizedEuler2DSourceTermDampingMonopole.cxx
LinearizedEuler2DSourceTermMeanMonopole.hh
LinearizedEuler2DSourceTermMeanMonopole.cxx

LinearizedEuler2DSourceTermMonopole.hh
LinearizedEuler2DSourceTermMonopole.cxx
LinearizedEuler3DSourceTermMonopole.hh
LinearizedEuler3DSourceTermMonopole.cxx

LinearizedEuler2DSourceTermDipole.hh
LinearizedEuler2DSourceTermDipole.cxx
LinearizedEuler2DSourceTermQuadrupole.hh
LinearizedEuler2DSourceTermQuadrupole.cxx

LinearizedEuler2DSourceTermValiant.hh
LinearizedEuler2DSourceTermValiant.cxx
LinearizedEuler3DSourceTermValiant.hh
LinearizedEuler3DSourceTermValiant.cxx

LinearizedEuler2DSourceTermMeanFlow.hh
LinearizedEuler2DSourceTermMeanFlow.cxx
LinearizedEuler3DSourceTermMeanFlow.hh
LinearizedEuler3DSourceTermMeanFlow.cxx

LinearizedEuler2DSourceTermFlap.cxx
LinearizedEuler2DSourceTermFlap.hh

LinearizedEuler2DSourceLEScomp.hh
LinearizedEuler2DSourceLEScomp.cxx
LinearizedEuler2DSourceLESincomp.hh
LinearizedEuler2DSourceLESincomp.cxx
LinearizedEuler3DSourceLES.hh
LinearizedEuler3DSourceLES.cxx
LinearizedEuler3DSourceMeanLES.hh
LinearizedEuler3DSourceMeanLES.cxx
SaveSourceData2D.hh
SaveSourceData2D.cxx
LoadSourceData2Dcomp.hh
LoadSourceData2Dcomp.cxx
LoadSourceData2Dincomp.hh
LoadSourceData2Dincomp.cxx
LoadSourceData3Dincomp.hh
LoadSourceData3Dincomp.cxx

LinearizedEuler2DSourceTermWall.hh
LinearizedEuler2DSourceTermWall.cxx
SavePrms.cxx
SavePrms.hh

# High order
LinearizedEuler2DSourceTermMonopoleHO.cxx
LinearizedEuler2DSourceTermMonopoleHO.hh
)


LIST ( APPEND FluctSplitLinEuler_requires_mods LinEuler )

LIST ( APPEND FluctSplitLinEuler_cflibs LinEuler FluctSplit )

CF_ADD_PLUGIN_LIBRARY ( FluctSplitLinEuler )

CF_WARN_ORPHAN_FILES()

