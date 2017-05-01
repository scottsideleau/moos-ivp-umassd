/****************************************************************************/
/* WolfDecrypt.cpp */
/****************************************************************************/

// WolfDecrypt Includes
#include "WolfDecrypt.h"

// WolfSSL Includes
extern "C"
{
  #include "crypto-aes.h"
}

// Standard Includes
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>


/*************************************************************************//**
 * A constructor for the WolfDecrypt object.
 */
CMOOSWolfDecrypt::CMOOSWolfDecrypt()
{   
  // Initialization
  nKeyLength = 0;
  sPassword = "";
  sSecretMessage = "";
  bNewSecret = false;

  // Character classification and case convention reset to POSIX standard
  setlocale(LC_CTYPE, "");
}


/*************************************************************************//**
 * A destructor for the WolfDecrypt object.
 */
CMOOSWolfDecrypt::~CMOOSWolfDecrypt()
{
  // No dynamic memory to deallocate
}


/*************************************************************************//**
 * Overloaded function that is run once, when WolfDecrypt first connects
 * to the MOOSdb.  Registration for MOOSdb variables we want to track takes 
 * place here and in OnStartUp(), as per the MOOS documentation.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSWolfDecrypt::OnConnectToServer()
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
bool CMOOSWolfDecrypt::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  dfTimeNow = MOOSTime();

  // Process the new mail
  for(p = NewMail.begin(); p != NewMail.end(); p++)
  {
    CMOOSMsg &Message = *p;
    if (Message.m_sKey == "SECRET_MESSAGE")
    {
      sSecretMessage = Message.m_sVal;
      bNewSecret = true;
    }
  }

  /* Success */
  return true;
}                                      


/*************************************************************************//**
 * Overloaded function called every 1/Apptick to process data and do work.
 */
bool CMOOSWolfDecrypt::Iterate()
{   
  if(bNewSecret)
  {
		///////////////////////
		// DECRYPTION (reverse)
		///////////////////////

		// Create the ciphertext container and fill it
		std::ofstream fCiphertext;
		fCiphertext.open( ".decrypt/ciphertext", 
        std::ofstream::out | std::ofstream::trunc);
		fCiphertext << sSecretMessage ;
		fCiphertext.close();

		// Open the Ciphertext for reading
	  FILE* ciphertext;
		ciphertext = fopen( ".decrypt/ciphertext", "r" );

    // Create the Cleartext container
		FILE* cleartext;
		cleartext = fopen( ".decrypt/cleartext", "w" );

    // Debugging: Print the desired key length
    MOOSTrace("AES Key Length = %d \n", nKeyLength);

    // Debugging: Print the user supplied password
    MOOSTrace("Password = " + sPassword + "\n");

    // Generate the decryption key
    //
    //  How to copy a std::string to an unsigned char array
    //    Accessed on: 29-APR-2017
    //    URL: http://stackoverflow.com/questions/35322150/
    //         how-to-copy-a-stdstring-to-unsigned-char-array
		unsigned char decKey[ sPassword.length() ];
		copy( sPassword.begin(), sPassword.end(), decKey );
		decKey[ sPassword.length() ] = 0;
    MOOSTrace("Created the password.\n");

		// Fill the Cleartext with the decoded Ciphertext
		int dec = AesDecrypt(&aes, decKey, nKeyLength, ciphertext, cleartext);
		MOOSTrace("Decryption Errors = %d \n", dec);

    // Reset
    bNewSecret = false;
    std::stringstream s;
    sSecretMessage = s.str(); // Because it gets mangled by binary data.
  }
  else
  {
    MOOSTrace("Waiting for a secret message to decrypt...\n");
  }

  // Debug: Newline for terminal neatness
  MOOSTrace("\n");

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
bool CMOOSWolfDecrypt::OnStartUp()
{
  // Useful temporary variables
  std::string sVal;

  /*
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
  */

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
void CMOOSWolfDecrypt::DoRegistrations()
{   
  m_Comms.Register("SECRET_MESSAGE", 0);
}

