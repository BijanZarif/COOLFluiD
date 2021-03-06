// Copyright (C) 2012 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <sstream>
#include <boost/filesystem/path.hpp>

#include "Common/EventHandler.hh"
#include "Common/FilesystemException.hh"

#include "Config/ConfigOptionException.hh"

#include "Environment/FileHandlerOutput.hh"
#include "Environment/DirPaths.hh"
#include "Environment/CFEnv.hh"
#include "Environment/CFEnvVars.hh"

#include "Framework/Simulator.hh"
#include "Framework/SubSystem.hh"

//////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace COOLFluiD::Common;
using namespace COOLFluiD::Environment;

namespace COOLFluiD {

  namespace Framework {

//////////////////////////////////////////////////////////////////////////////

void SimulatorPaths::defineConfigOptions(Config::OptionList& options)
{
   options.addConfigOption< std::vector< std::string > >("ModulesDir","Module directories paths.");
   options.addConfigOption< std::string >("WorkingDir","Working directory path.");
   options.addConfigOption< std::string >("ResultsDir","Results directory path.");
   options.addConfigOption< std::string >("RepositoryURL","URL of repository of remote files");
}

//////////////////////////////////////////////////////////////////////////////

SimulatorPaths::SimulatorPaths(const std::string& name) : ConfigObject(name)
{
  CFAUTOTRACE;

  addConfigOptionsTo(this);

  setParameter("ModulesDir",&m_modules_dirs);

  m_working_dir = "";
  setParameter("WorkingDir",&m_working_dir);

  m_results_dir = "";
  setParameter("ResultsDir",&m_results_dir);

  m_repository_url = "http://coolfluidsrv.vki.ac.be/webfiles/coolfluid/repository/";
  setParameter("RepositoryURL",&m_repository_url);
}

//////////////////////////////////////////////////////////////////////////////

void SimulatorPaths::configure ( Config::ConfigArgs& args )
{
  CFAUTOTRACE;

  ConfigObject::configure(args);

  Environment::DirPaths& dirpaths = Environment::DirPaths::getInstance();

  dirpaths.setWorkingDir(m_working_dir);
  dirpaths.setResultsDir(m_results_dir);
  dirpaths.setRepositoryURL(m_repository_url);

  dirpaths.addModuleDirs(m_modules_dirs);
}

//////////////////////////////////////////////////////////////////////////////

void Simulator::defineConfigOptions(Config::OptionList& options)
{
   options.addConfigOption< std::vector<std::string> >("SubSystems","The SubSystems present in this simulation.");
   options.addConfigOption< std::vector<std::string> >("SubSystemTypes","The type of the SubSystems present in this simulation.");
}

//////////////////////////////////////////////////////////////////////////////

Simulator::Simulator(const std::string& name) :
  ConfigObject(name),
  m_moduleLoader(),
  m_subSystemNames(),
  m_subSystemTypes(),
  m_subSys(),
  m_sim_args()
{
  CFAUTOTRACE;

  addConfigOptionsTo(this);
  registActionListeners();

  m_subSystemNames.push_back("SubSystem");
  setParameter("SubSystems",&m_subSystemNames);

  m_subSystemTypes.push_back("StandardSubSystem");
  setParameter("SubSystemTypes",&m_subSystemTypes);

  m_paths.reset( new SimulatorPaths("Paths") );
}

//////////////////////////////////////////////////////////////////////////////

Simulator::~Simulator()
{
  CFAUTOTRACE;
}

//////////////////////////////////////////////////////////////////////////////

void Simulator::registActionListeners()
{
  CFAUTOTRACE;

  Common::SafePtr<EventHandler> event_handler = Environment::CFEnv::getInstance().getEventHandler();
  
  event_handler->addListener(event_handler->key("","CF_ON_MAESTRO_BUILDSUBSYSTEM"), 
			     this, &Simulator::buildSubSystem);
  event_handler->addListener(event_handler->key("","CF_ON_MAESTRO_DESTROYSUBSYSTEM"),
			     this, &Simulator::destroySubSystem);
  event_handler->addListener(event_handler->key("","CF_ON_MAESTRO_CONFIGSUBSYSTEM"), 
			     this, &Simulator::configSubSystem);
}
    
//////////////////////////////////////////////////////////////////////////////

void Simulator::openCaseFile ( const std::string& sCFcaseFile )
{
  CFAUTOTRACE;
  setConfigFile ( sCFcaseFile );
  configure ( m_sim_args );
}

//////////////////////////////////////////////////////////////////////////////

void Simulator::processOptions ( Config::ConfigArgs& args )
{
  CFAUTOTRACE;

  processConfigFile ( args );

  processParentArgs ( args );

  processEnvironmentVariables ( args );

  // filter the options that are not for Simulator nested objects
  // avoids errors with unused options for CFEnv
  args.pass_filter ( getNestName() );

  getOptionList().doValidation();
  getOptionList().doComplete();
}

//////////////////////////////////////////////////////////////////////////////

void Simulator::configure ( Config::ConfigArgs& args )
{
  CFAUTOTRACE;

  CFLog(NOTICE,"-------------------------------------------------------------\n");

  CFLog(NOTICE,"Configuration of Simulator\n");
  ConfigObject::configure(args);

  CFLog(NOTICE,"-------------------------------------------------------------\n");

  configureNested ( m_paths.getPtr(), args );

  CFLog(NOTICE,"-------------------------------------------------------------\n");

  // configure ModuleLoader must be after dir config
  // and before all other configs
  CFLog(NOTICE,"-------------------------------------------------------------\n");

  CFLog(NOTICE,"Loading external modules\n");
  configureNested ( m_moduleLoader, args );
  m_moduleLoader.loadExternalModules();
  CFLog(NOTICE,"Initiating environment of loaded modules\n");
  Environment::CFEnv::getInstance().initiateModules();

  CFLog(NOTICE,"-------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////

Common::Signal::return_t Simulator::buildSubSystem(Common::Signal::arg_t eBuild)
{
  CFAUTOTRACE;

  /// @todo will make a check here for processor rank to allocate the sub system in the correct processors
  cf_assert(m_subSys.isNull());

  std::string subSystemName;
  std::string subSystemType;

  stringstream ss;
  ss << eBuild;

  ss >> subSystemName;
  ss >> subSystemType;

  CFLog(NOTICE,"-------------------------------------------------------------\n");
  CFLog(NOTICE,"Building SubSystem\n");
  CFLog(NOTICE,"Name : " << subSystemName << "\n");
  CFLog(NOTICE,"Type : " << subSystemType << "\n");
  CFLog(NOTICE,"-------------------------------------------------------------\n");

  Common::SafePtr<SubSystem::PROVIDER> prov = Environment::Factory<SubSystem>::getInstance().getProvider(subSystemType);
  cf_assert(prov.isNotNull());

  m_subSys.reset(prov->create(subSystemName));
  return Common::Signal::return_t ();
}

//////////////////////////////////////////////////////////////////////////////

Common::Signal::return_t Simulator::destroySubSystem(Common::Signal::arg_t eDestroy)
{
  CFAUTOTRACE;

  std::string name;
  stringstream ss;
  ss << eDestroy;
  ss >> name;

  if (name == m_subSys->getName())  m_subSys.release();

  return Common::Signal::return_t ();
}

//////////////////////////////////////////////////////////////////////////////

Common::Signal::return_t Simulator::configSubSystem(Common::Signal::arg_t eConfig)
{
  CFAUTOTRACE;

  cf_assert(m_subSys.isNotNull());

  Config::ConfigArgs local_args = m_sim_args;

  configureNested ( m_subSys.getPtr(), local_args );

  if ( local_args.size() > 0 ) // some configurations where not used
  {
    std::string msg ( "Unused User Configuration Arguments:\n" );
    msg += local_args.str();
    CFLog ( WARN , "WARNING : " << msg << "\n" );

    if ( CFEnv::getInstance().getVars()->ErrorOnUnusedConfig ) {
      throw Config::ConfigOptionException ( FromHere(), msg );
    }
  }

  return Common::Signal::return_t ();
}

//////////////////////////////////////////////////////////////////////////////

std::vector< std::string > Simulator::getSubSystemNames() const
{
  return m_subSystemNames;
}

//////////////////////////////////////////////////////////////////////////////

std::vector< std::string > Simulator::getSubSystemTypes() const
{
  return m_subSystemTypes;
}

//////////////////////////////////////////////////////////////////////////////

  } // namespace Framework

} // namespace COOLFluiD
