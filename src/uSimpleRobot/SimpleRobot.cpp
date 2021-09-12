/****************************************************************************/
/* SimpleRobot.cpp */
/****************************************************************************/

// Standard Includes
#define _USE_MATH_DEFINES
#include <cmath>

// SimpleRobot Includes
#include "SimpleRobot.h"


/*************************************************************************//**
 * A constructor for the SimpleRobot object.
 */
CMOOSSimpleRobot::CMOOSSimpleRobot()
{   
  // Initialization
  nTimeWarp = 0;
  nAppTick = 0;
  dfTimeNow = -1.0;
  dfStartHeading = 0.0;
  dfStartSpeed = 0.0;
  dfStartDepth = 0.0;
  dfStartX = 0.0;
  dfPrevX = 0.0;
  dfNextX = 0.0;
  dfStartY = 0.0;
  dfPrevY = 0.0;
  dfNextY = 0.0;
  dfRateHeading = 0.0;
  dfRateSpeed = 0.0;
  dfRateDepth = 0.0;
  dfDesiredHeading = 0.0;
  dfPrevHeading = 0.0;
  dfNextHeading = 0.0;
  dfIncHeading = 0.0;
  dfDesiredSpeed = 0.0;
  dfPrevSpeed = 0.0;
  dfNextSpeed = 0.0;
  dfIncSpeed = 0.0;
  dfDesiredDepth = 0.0;
  dfPrevDepth = 0.0;
  dfNextDepth = 0.0;
  dfIncDepth = 0.0;
  bHeadingIsCurrent = true;
  bDepthIsCurrent = true;
  bSpeedIsCurrent = true;

  // Character classification and case convention reset to POSIX standard
  setlocale(LC_CTYPE, "");
}


/*************************************************************************//**
 * A destructor for the SimpleRobot object.
 */
CMOOSSimpleRobot::~CMOOSSimpleRobot()
{
  // No dynamic memory to deallocate
}


/*************************************************************************//**
 * Overloaded function that is run once, when SimpleRobot first connects
 * to the MOOSdb.  Registration for MOOSdb variables we want to track takes 
 * place here and in OnStartUp(), as per the MOOS documentation.
 *
 * @return  a boolean indicating success or failure
 */
bool CMOOSSimpleRobot::OnConnectToServer()
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
bool CMOOSSimpleRobot::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
  dfTimeNow = MOOSTime();

  // Process the new mail
  for(p = NewMail.begin(); p != NewMail.end(); p++)
  {
    CMOOSMsg &Message = *p;
    if (Message.m_sKey == "DESIRED_HEADING")
    {
      dfDesiredHeading = Message.m_dfVal;
      bHeadingIsCurrent = false;
    }
    else if (Message.m_sKey == "DESIRED_SPEED")
    {
      dfDesiredSpeed = Message.m_dfVal;
      bSpeedIsCurrent = false;
    }
    else if (Message.m_sKey == "DESIRED_DEPTH")
    {
      dfDesiredDepth = Message.m_dfVal;
      bDepthIsCurrent = false;
    }
  }

  /* Success */
  return true;
}                                      


/*************************************************************************//**
 * Overloaded function called every 1/Apptick to process data and do work.
 */
bool CMOOSSimpleRobot::Iterate()
{   
  // Main
  dfTimeNow = MOOSTime();

  if ( !bHeadingIsCurrent )
  {
    if ( dfPrevHeading <= dfDesiredHeading )
    {
      dfNextHeading = dfPrevHeading + dfIncHeading;
      if ( dfNextHeading >= dfDesiredHeading )
      {
        dfNextHeading = dfDesiredHeading;
      }
    }
    else
    {
      dfNextHeading = dfPrevHeading - dfIncHeading;
      if ( dfNextHeading <= dfDesiredHeading )
      {
        dfNextHeading = dfDesiredHeading;
      }
    }
    m_Comms.Notify( "NAV_HEADING", dfNextHeading, dfTimeNow );
    std::cout << "NAV_HEADING: " << dfNextHeading << std::endl;
    bHeadingIsCurrent = true;
  }

  if ( !bSpeedIsCurrent )
  {
    if ( dfPrevSpeed <= dfDesiredSpeed )
    {
      dfNextSpeed = dfPrevSpeed + dfIncSpeed;
      if ( dfNextSpeed >= dfDesiredSpeed )
      {
        dfNextSpeed = dfDesiredSpeed;
      }
    }
    else
    {
      dfNextSpeed = dfPrevSpeed - dfIncSpeed;
      if ( dfNextSpeed <= dfDesiredSpeed )
      {
        dfNextSpeed = dfDesiredSpeed;
      }
    }
    m_Comms.Notify( "NAV_SPEED", dfNextSpeed, dfTimeNow );
    std::cout << "NAV_SPEED: " << dfNextSpeed << std::endl;
    bSpeedIsCurrent = true;
  }

  if ( !bDepthIsCurrent )
  {
    if ( dfPrevDepth <= dfDesiredDepth )
    {
      dfNextDepth = dfPrevDepth + dfIncDepth;
      if ( dfNextDepth >= dfDesiredDepth )
      {
        dfNextDepth = dfDesiredDepth;
      }
    }
    else
    {
      dfNextDepth = dfPrevDepth - dfIncDepth;
      if ( dfNextDepth <= dfDesiredDepth )
      {
        dfNextDepth = dfDesiredDepth;
      }
    }
    m_Comms.Notify( "NAV_DEPTH", dfNextDepth, dfTimeNow );
    std::cout << "NAV_DEPTH: " << dfNextDepth << std::endl;
    bDepthIsCurrent = true;
  }

  if ( bHeadingIsCurrent && bSpeedIsCurrent )
  {
    double distance = dfNextSpeed * dfIncTime;
    std::cout << "Time Inc: " << dfIncTime << std::endl;
    std::cout << "Distance: " << distance << std::endl;

    dfNextX = dfPrevX + 5 + ( distance * \
        cos( ( dfNextHeading * M_PI_2 )  ) );
    m_Comms.Notify( "NAV_X", dfNextX, dfTimeNow );

    dfNextY = dfPrevY + 5 + (distance * \
        sin( ( dfNextHeading * M_PI_2 ) ) );
    m_Comms.Notify( "NAV_Y", dfNextY, dfTimeNow );

    dfPrevHeading = dfNextHeading;
    dfPrevSpeed = dfNextSpeed;
    dfPrevDepth = dfNextDepth;
    dfPrevX = dfNextX;
    dfPrevY = dfNextY;
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
bool CMOOSSimpleRobot::OnStartUp()
{
  // Useful temporary variables
  dfTimeNow = MOOSTime();
  std::string sVal;

  if (m_MissionReader.GetValue("MOOSTimeWarp", sVal)) 
  {
    nTimeWarp = std::stoi( sVal.c_str() );
  }
  else 
  {
    nTimeWarp = 1;
  } 

  // Retrieve application-specific configuration parameters
  //   start_heading, start_speed, start_depth, start_x, start_y
  //   rate_heading, rate_speed, rate_depth
  if ( m_MissionReader.GetConfigurationParam( "AppTick", sVal ) )
  {
    nAppTick = std::stoi( sVal );
    dfIncTime = ( 1 / static_cast<double>( nAppTick ) ) \
          * static_cast<double>( nTimeWarp );
  }
  else if ( m_MissionReader.GetConfigurationParam( "start_heading", sVal ) )
  {
    char* pEnd;
    dfStartHeading = std::strtod( sVal.c_str(), &pEnd );
    dfPrevHeading = dfStartHeading;
  }
  else if ( m_MissionReader.GetConfigurationParam( "start_speed", sVal ) )
  {
    char* pEnd;
    dfStartSpeed = std::strtod( sVal.c_str(), &pEnd );
    dfPrevSpeed = dfStartSpeed;
  }
  else if ( m_MissionReader.GetConfigurationParam( "start_depth", sVal ) )
  {
    char* pEnd;
    dfStartDepth = std::strtod( sVal.c_str(), &pEnd );
    dfPrevDepth = dfStartDepth;
  }
  else if ( m_MissionReader.GetConfigurationParam( "start_x", sVal ) )
  {
    char* pEnd;
    dfStartX = std::strtod( sVal.c_str(), &pEnd );
    dfPrevX = dfStartX;
    m_Comms.Notify( "NAV_X", dfStartX, dfTimeNow );
  }
  else if ( m_MissionReader.GetConfigurationParam( "start_y", sVal ) )
  {
    char* pEnd;
    dfStartY = std::strtod( sVal.c_str(), &pEnd );
    dfPrevY = dfStartY;
    m_Comms.Notify( "NAV_Y", dfStartY, dfTimeNow );
  }
  else if ( m_MissionReader.GetConfigurationParam( "rate_heading", sVal ) )
  {
    char* pEnd;
    dfRateHeading = std::strtod( sVal.c_str(), &pEnd );
    dfIncHeading = dfRateDepth * dfIncTime;
  }
  else if ( m_MissionReader.GetConfigurationParam( "rate_speed", sVal ) )
  {
    char* pEnd;
    dfRateSpeed = std::strtod( sVal.c_str(), &pEnd );
    dfIncSpeed = dfRateSpeed * dfIncTime;
  }
  else if ( m_MissionReader.GetConfigurationParam( "rate_depth", sVal ) )
  {
    char* pEnd;
    dfRateDepth = std::strtod( sVal.c_str(), &pEnd );
    dfIncDepth = dfRateDepth * dfIncTime;

  }

  // Register for relevant MOOSdb variables
  DoRegistrations(); 

  /* Success */
  return true;
}


/*************************************************************************//**
 * Register for the variables of interest in the MOOSdb.
 */
void CMOOSSimpleRobot::DoRegistrations()
{   
  // EXAMPLE:
  m_Comms.Register("DESIRED_HEADING", 0);
  m_Comms.Register("DESIRED_SPEED", 0);
  m_Comms.Register("DESIRED_DEPTH", 0);
}

