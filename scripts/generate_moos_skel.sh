#!/bin/bash

##############################################################################
# generate_moos_skel.sh
# ============================================================================
# Generate a skeleton directory for a new MOOS App with starter files based on
# user input for the application name, application prefix, author, and year of
# application creation.
##############################################################################

# Start the output
echo ""
echo "###################################################################"
echo "# generate_moos_skel.sh"
echo "###################################################################"
echo ""

# Check the input parameters
if [ -z "$4" ] ; then
    echo "Incorrect number of arguments"
    echo "Usage: $0 [AppName] [prefix] [author] [year]"
    echo ""
    exit 0
fi

# Make the directory
mkdir $2$1

# Go into the new directory and make skeleton files
cd $2$1

touch ChangeLog

cat >> CMakeLists.txt <<EOF
#--------------------------------------------------------
# The CMakeLists.txt for:               $2$1
# Author(s):                            $3
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE($2$1 \${SRC})

TARGET_LINK_LIBRARIES($2$1
    MOOS
    MOOSGeodesy
    m
    pthread)

EOF


cat >> ${1}.h <<EOF
/****************************************************************************/
/* $1.h */
/****************************************************************************/

#ifndef MOOS_APP_${1^^}_H_
#define MOOS_APP_${1^^}_H_

// MOOS Includes
#include "MOOS/libMOOS/App/MOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"


/*************************************************************************//**
 * Class that extends CMOOSApp to produce $2$1.
 */
class CMOOS$1 : public CMOOSApp
{
  public:
    CMOOS$1();
    virtual ~CMOOS$1();

  protected:
    // Inherited from MOOSApp
    bool OnConnectToServer();               /* overloaded */
    bool Iterate();                         /* overloaded */
    bool OnNewMail(MOOSMSG_LIST &NewMail);  /* overloaded */
    bool OnStartUp();                       /* overloaded */
    //bool MakeStatusString();              /* using default */
    //bool OnCommandMsg(CMOOSMsg CmdMsg);   /* unused */
    //bool ConfigureComms();                /* unused*/
    //bool OnDisconnectFromServer();        /* unused*/

  private:
    // Initialize the MOOS coordinate conversion utility
    CMOOSGeodesy m_Geodesy;

    // Global variables filled by the .moos file

    // Global variables
    double dfLatOrigin;
    double dfLonOrigin;
    double dfTimeNow;
    double dfNavX;
    bool bNavXisCurrent;

    // Functions
    void DoRegistrations();
};

#endif // #ifndef MOOS_APP_${1^^}_H_

EOF

cat >> ${1}Main.cpp <<EOF
/****************************************************************************/
/* $1Main.cpp */
/****************************************************************************/

// $1 Includes
#include "$1.h"


/*************************************************************************//**
 * The main function, which is responsible for starting the $1
 * MOOS Application and properly handing it the .moos file.
 *
 * @return  the number of errors
 */
int main(int argc , char *argv[])
{
  // Set a default, blank mission file
  const char *sMissionFile = "$2$1.moos";

  // Set a default process name
  const char *sMOOSName = "$2$1";

  // Handle command line arguments
  switch(argc)
  {
    case 3:
      sMOOSName = argv[2];    // alternate process name provided
    case 2:
      sMissionFile = argv[1]; // alternate mission file provided
  }

  // Iniitialize and start the extended MOOSApp class
  CMOOS$1 MOOS$1;
  MOOS$1.Run(sMOOSName, sMissionFile);

  return 0;
}

EOF

cat >> $2${1}.moos <<EOF
//----------------------------------------------------------------------------
ProcessConfig = $2$1
{
  AppTick   = 1
  CommsTick = 1

  //Configuration Parameters
}

EOF

cat >> ${1}.cpp <<EOF
/****************************************************************************/
/* $1.cpp */
/****************************************************************************/

// $1 Includes
#include "$1.h"


/*************************************************************************//**
 * A constructor for the $1 object.
 */
CMOOS$1::CMOOS$1()
{   
  // Initialization

  // Character classification and case convention reset to POSIX standard
  setlocale(LC_CTYPE, "");
}


/*************************************************************************//**
 * A destructor for the $1 object.
 */
CMOOS$1::~CMOOS$1()
{
  // No dynamic memory to deallocate
}


/*************************************************************************//**
 * Overloaded function that is run once, when $1 first connects
 * to the MOOSdb.  Registration for MOOSdb variables we want to track takes 
 * place here and in OnStartUp(), as per the MOOS documentation.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOS$1::OnConnectToServer()
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
bool CMOOS$1::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  dfTimeNow = MOOSTime();

  // Process the new mail

  /* EXAMPLE:
  for(p = NewMail.begin(); p != NewMail.end(); p++)
  {
    CMOOSMsg &Message = *p;
    if (Message.m_sKey == "NAV_X")
    {
      dfNavX = Message.m_dfVal;
      bNavXisCurrent = true;
    }
  }
  */

  /* Success */
  return true;
}                                      


/*************************************************************************//**
 * Overloaded function called Every 1/Apptick to collect all sensor data from
 * and issue directions the Ocean-Server Frontseat Driver.
 */
bool CMOOS$1::Iterate()
{   
  // Main

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
bool CMOOS$1::OnStartUp()
{
  // Useful temporary variables
  std::string sVal;

  // Get the latitude origin from the .moos file
  if (m_MissionReader.GetValue("LatOrigin", sVal)) 
  {
    dfLatOrigin = atof(sVal.c_str());
  } 
  else 
  {
    MOOSTrace("LatOrigin not set - FAIL\n");
    MOOSPause(5000);
    exit(1);
  } 

  // Get the longitude origin from the .moos file
  if (m_MissionReader.GetValue("LongOrigin", sVal)) 
  {
    dfLonOrigin = atof(sVal.c_str());
  } 
  else 
  {
    MOOSTrace("LongOrigin not set - FAIL\n");
    MOOSPause(5000);
    exit(1);
  }

  // Initialize our coordinate origins 
  if (!m_Geodesy.Initialise(dfLatOrigin, dfLonOrigin)) 
  {
    MOOSTrace("Geodesy Init failed - FAIL\n");
    MOOSPause(5000);
    exit(1);
  }

  // Register for relevant MOOSdb variables
  DoRegistrations(); 

  /* Success */
  return true;
}


/*************************************************************************//**
 * Register for the variables of interest in the MOOSdb.
 */
void CMOOS$1::DoRegistrations()
{   
  // EXAMPLE:
  //m_Comms.Register("NAV_X", 0);
}

EOF

# Main
echo "AppName: $1"
echo "Prefix:  $2"
echo "Author:  $3"
echo "Year:    $4"
echo ""
echo "Skeleton directory for $2${1} generated successfully."
echo ""

