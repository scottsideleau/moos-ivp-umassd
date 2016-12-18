/****************************************************************************/
/* MoosKiller.cpp                                                           */
/****************************************************************************/

// MoosKiller Includes
#include "MoosKiller.h"

// Standard C Includes
#include <cstdio>
#include <cstdlib>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

/*************************************************************************//**
 * A constructor for the MoosKiller object.
 */
CMOOSMoosKiller::CMOOSMoosKiller()
{   
  // Initialization
  sKillVar = "";
  sMoosBin = "";
  bKillIt = false;

  // Character classification and case convention reset to POSIX standard
  setlocale(LC_CTYPE, "");
}


/*************************************************************************//**
 * A destructor for the MoosKiller object.
 */
CMOOSMoosKiller::~CMOOSMoosKiller()
{
  // No dynamic memory to deallocate
}


/*************************************************************************//**
 * Overloaded function that is run once, when MoosKiller first connects
 * to the MOOSdb.  Registration for MOOSdb variables we want to track takes 
 * place here and in OnStartUp(), as per the MOOS documentation.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSMoosKiller::OnConnectToServer()
{
  DoRegistrations();

  /* Success */
  return true;
}


/*************************************************************************//**
 * Overloaded function called each time a MOOSdb variable that we are
 * subscribed to is updated.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSMoosKiller::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  dfTimeNow = MOOSTime();

  // Process the new mail
  for(p = NewMail.begin(); p != NewMail.end(); p++)
  {
    CMOOSMsg &Message = *p;
    if (Message.m_sKey == sKillVar)
    {
      bKillIt = true;
    }
  }

  /* Success */
  return true;
}                                      


/*************************************************************************//**
 * Overloaded function called Every 1/Apptick to collect all sensor data from
 * and issue directions the Ocean-Server Frontseat Driver.
 */
bool CMOOSMoosKiller::Iterate()
{   
  // Main
  if (bKillIt)
  {
    std::string ktm = sMoosBin + "/ktm";

    // Spawn process to Kill The Moos (KTM)
    pid_t pid;
    char *argv[] = {(char*)"last.moos", NULL, NULL, NULL};
    int status;
    status = posix_spawn(&pid, ktm.c_str(), NULL, NULL, argv, environ);
    if (status == 0) {
      printf("Child pid: %i\n", pid);
      if (waitpid(pid, &status, 0) != -1) {
        printf("Child exited with status %i\n", status);
      } else {
        perror("waitpid");
      }
    } else {
      printf("posix_spawn: %s\n", strerror(status));
    }
  }

  /* Success */
  return true;        
}


/*************************************************************************//**
 * Overloaded function where we initialize the Local UTM coordinate system,
 * perform initializations based on the contents of the .moos file, and 
 * subscribe to any relevant MOOSdb variables.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSMoosKiller::OnStartUp()
{
  // Useful temporary variables
  std::string sVal;

  // Get the killer variable that we should monitor
  if (m_MissionReader.GetConfigurationParam("kill_var", sVal))
  {
    std::transform(sVal.begin(), sVal.end(), sVal.begin(), \
        (int(*)(int)) ::toupper);    //Force upper case.
    sKillVar = sVal;
  }

  // Get the user-defined MOOS/bin path
  if (m_MissionReader.GetConfigurationParam("moos_bin", sVal))
  {
    std::transform(sVal.begin(), sVal.end(), sVal.begin(), \
        (int(*)(int)) ::tolower);    //Force lower case.
    sMoosBin = sVal;
  }

  // Register for relevant MOOSdb variables
  DoRegistrations(); 

  /* Success */
  return true;
}


/*************************************************************************//**
 * Register for the variables of interest in the MOOSdb.
 */
void CMOOSMoosKiller::DoRegistrations()
{   
  m_Comms.Register(sKillVar, 0);
}

