/****************************************************************************/
/* WolfEncrypt.cpp */
/****************************************************************************/

// WolfEncrypt Includes
#include "WolfEncrypt.h"

// WolfSSL Includes
extern "C"
{
  #include "crypto-aes.h"
}

// Standard Includes
#include <cstdio>

// MacOS Includes
#ifdef __APPLE__
  extern "C"
  {
    #include "MacOS/fmemopen.h"
  }
#endif


/*************************************************************************//**
 * A constructor for the WolfEncrypt object.
 */
CMOOSWolfEncrypt::CMOOSWolfEncrypt()
{   
  // Initialization
  nKeyLength = 0;
  sPassword = "";

  // Character classification and case convention reset to POSIX standard
  setlocale(LC_CTYPE, "");
}


/*************************************************************************//**
 * A destructor for the WolfEncrypt object.
 */
CMOOSWolfEncrypt::~CMOOSWolfEncrypt()
{
  // No dynamic memory to deallocate
}


/*************************************************************************//**
 * Overloaded function that is run once, when WolfEncrypt first connects
 * to the MOOSdb.  Registration for MOOSdb variables we want to track takes 
 * place here and in OnStartUp(), as per the MOOS documentation.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSWolfEncrypt::OnConnectToServer()
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
bool CMOOSWolfEncrypt::OnNewMail(MOOSMSG_LIST &NewMail)
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
 * Overloaded function called Every 1/Apptick to process data and do work.
 */
bool CMOOSWolfEncrypt::Iterate()
{   
  FILE* ciphertext;
  FILE* plaintext;

  std::string sPlaintext = "Hello, World!";
  plaintext = fmemopen(static_cast<void*>(&sPlaintext), sPlaintext.length(), "r");

  AesEncrypt(&aes, (unsigned char*)sPassword.c_str(), nKeyLength, plaintext, 
      ciphertext);


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
bool CMOOSWolfEncrypt::OnStartUp()
{
  // Useful temporary variables
  std::string sVal;

  // Get the latitude origin from the .moos file
  if (m_MissionReader.GetValue("LatOrigin", sVal)) 
  {
    char* end;
    dfLatOrigin = std::strtod(sVal.c_str(), &end);
    if (!(*end == '\0'))
    {
      MOOSTrace("LatOrigin not set; unexpected end of string - FAIL\n");
      MOOSPause(5000);
      exit(1);
    } 
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
    char* end;
    dfLonOrigin = std::strtod(sVal.c_str(), &end);
    if (!(*end == '\0'))
    {
      MOOSTrace("LonOrigin not set; unexpected end of string - FAIL\n");
      MOOSPause(5000);
      exit(1);
    } 
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

  // Retrieve application-specific configuration parameters
  if (m_MissionReader.GetConfigurationParam("key_length", sVal)) 
  {
    nKeyLength = std::stoi(sVal);;
    if(SizeCheck(nKeyLength) != 0)
    {
      MOOSTrace("key_length not one of 128, 192, or 256 - FAIL\n");
      MOOSPause(5000);
      exit(1);
    }
  } 
  else 
  {
    MOOSTrace("key_length not set - FAIL\n");
    MOOSPause(5000);
    exit(1);
  }

  if (m_MissionReader.GetConfigurationParam("password", sVal))
  {
    sPassword = sVal;
  }
  else
  {
    MOOSTrace("password not set - FAIL\n");
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
void CMOOSWolfEncrypt::DoRegistrations()
{   
  // EXAMPLE:
  //m_Comms.Register("NAV_X", 0);
}

