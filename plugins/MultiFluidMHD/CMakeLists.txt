LIST ( APPEND MultiFluidMHD_files
MultiFluidMHD.hh
EulerMFMHDTerm.cxx
EulerMFMHDTerm.hh
DiffMFMHD2DRhoiViTi.hh
DiffMFMHD2DRhoiViTi.cxx
DiffMFMHD2DVarSet.hh
DiffMFMHD2DVarSet.cxx
DiffMFMHD3DRhoiViTi.cxx
DiffMFMHD3DRhoiViTi.hh
DiffMFMHD3DVarSet.hh
DiffMFMHD3DVarSet.cxx
DiffMFMHDTerm.cxx
DiffMFMHDTerm.hh
DiffMFMHDVarSet.cxx
DiffMFMHDVarSet.hh
Euler2DMFMHDConsToRhoiViTiInRhoiViTi.cxx
Euler2DMFMHDConsToRhoiViTiInRhoiViTi.hh
EulerMFMHD2DCons.cxx
EulerMFMHD2DCons.hh
EulerMFMHD2DRhoiViTi.cxx
EulerMFMHD2DRhoiViTi.hh
EulerMFMHD2DConsToRhoiViTi.cxx
EulerMFMHD2DConsToRhoiViTi.hh
EulerMFMHD2DRhoiViTiToCons.cxx
EulerMFMHD2DRhoiViTiToCons.hh
EulerMFMHD3DRhoiViTi.cxx
EulerMFMHD3DRhoiViTi.hh
EulerMFMHD3DRhoiViTiToCons.cxx
EulerMFMHD3DRhoiViTiToCons.hh
EulerMFMHD3DConsToRhoiViTi.cxx
EulerMFMHD3DConsToRhoiViTi.hh
Euler3DMFMHDConsToRhoiViTiInRhoiViTi.cxx
Euler3DMFMHDConsToRhoiViTiInRhoiViTi.hh
EulerMFMHD3DCons.cxx
EulerMFMHD3DCons.hh
MultiFluidMHDModel.ci
MultiFluidMHDModel.cxx
MultiFluidMHDModel.hh
MultiFluidMHDVarSet.ci
MultiFluidMHDVarSet.hh
ReacMFMHDTerm.cxx
ReacMFMHDTerm.hh
)

LIST ( APPEND MultiFluidMHD_cflibs Framework Maxwell )
LIST ( APPEND MultiFluidMHD_requires_mods Maxwell )

CF_ADD_PLUGIN_LIBRARY ( MultiFluidMHD )
CF_WARN_ORPHAN_FILES()
#ADD_SUBDIRECTORY ( testcases )
