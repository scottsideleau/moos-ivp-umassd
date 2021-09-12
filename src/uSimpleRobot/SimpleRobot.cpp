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
  dfNextX = 0.0;
  dfNextY = 0.0;
  dfRateHeading = 0.0;
  dfRateSpeed = 0.0;
  dfRateDepth = 0.0;
  dfDesiredHeading = 0.0;
  dfNextHeading = 0.0;
  dfIncHeading = 0.0;
  dfDesiredSpeed = 0.0;
  dfNextSpeed = 0.0;
  dfIncSpeed = 0.0;
  dfDesiredDepth = 0.0;
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

  // Heading
  if ( ( dfPrevHeading <= dfDesiredHeading ) && \
      ( abs( dfPrevHeading - dfDesiredHeading ) < 180.0 ) )
  {
    // Turn Right (Starboard)
    dfNextHeading = dfPrevHeading + dfIncHeading;
    if ( dfNextHeading >= dfDesiredHeading )
    {
      dfNextHeading = dfDesiredHeading;
    }
  }
  else
  {
    // Turn Left (Port)
    dfNextHeading = dfPrevHeading - dfIncHeading;
    if ( dfNextHeading <= dfDesiredHeading )
    {
      dfNextHeading = dfDesiredHeading;
    }
  }
  m_Comms.Notify( "NAV_HEADING", dfNextHeading, dfTimeNow );
  std::cout << "HEADING (prev, inc, next, desired): " \
    << dfPrevHeading    << "  " \
    << dfIncHeading     << "  " \
    << dfNextHeading    << "  " \
    << dfDesiredHeading << "  " \
    << std::endl;
  bHeadingIsCurrent = true;

  // Speed
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
  std::cout << "SPEED (prev, inc, next, desired): " \
    << dfPrevSpeed    << "  " \
    << dfIncSpeed     << "  " \
    << dfNextSpeed    << "  " \
    << dfDesiredSpeed << "  " \
    << std::endl;
  bSpeedIsCurrent = true;

  // Depth
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
  std::cout << "DEPTH (prev, inc, next, desired): " \
    << dfPrevDepth    << "  " \
    << dfIncDepth     << "  " \
    << dfNextDepth    << "  " \
    << dfDesiredDepth << "  " \
    << std::endl;
  bDepthIsCurrent = true;

  // X, Y
  double distance = dfNextSpeed * dfIncTime;

  double dfStandardAngle = 90.0 - dfNextHeading;
  if ( dfStandardAngle < 0.0 )
  {
    dfStandardAngle += 360.0;
  }
  if ( dfStandardAngle >= 360.0 )
  {
    dfStandardAngle -= 360.0;
  }
  double dfStandardAngleInRadians = dfStandardAngle * ( M_PI / 180.0 );

  dfNextX = dfPrevX + ( distance * cos( dfStandardAngleInRadians ) );
  m_Comms.Notify( "NAV_X", dfNextX, dfTimeNow );

  dfNextY = dfPrevY + ( distance * sin( dfStandardAngleInRadians ) );
  m_Comms.Notify( "NAV_Y", dfNextY, dfTimeNow );

  std::cout << "X, Y (time step, distance, x, y): " \
    << dfIncTime << "  " \
    << distance  << "  " \
    << dfNextX   << "  " \
    << dfNextY   << "  " \
    << std::endl;

  // Prepare for Next Time Step
  dfPrevHeading = dfNextHeading;
  dfPrevSpeed = dfNextSpeed;
  dfPrevDepth = dfNextDepth;
  dfPrevX = dfNextX;
  dfPrevY = dfNextY;
  std::cout << "--" << std::endl;

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
  
  // Retrieve global .moos configuration parameters
  if ( m_MissionReader.GetValue( "MOOSTimeWarp", sVal ) ) 
  {
    nTimeWarp = std::stoi( sVal );
  }
  else 
  {
    nTimeWarp = 1;
  } 

  // Retrieve application-specific .moos configuration parameters
  if ( m_MissionReader.GetConfigurationParam( "AppTick", sVal ) )
  {
    nAppTick = std::stoi( sVal );
    dfIncTime = ( 1 / static_cast<double>( nAppTick ) ) \
          * static_cast<double>( nTimeWarp );
  }

  if ( m_MissionReader.GetConfigurationParam( "start_heading", sVal ) )
  {
    dfStartHeading = std::stod( sVal );
    dfPrevHeading = dfStartHeading;
  }

  if ( m_MissionReader.GetConfigurationParam( "start_speed", sVal ) )
  {
    dfStartSpeed = std::stod( sVal );
    dfPrevSpeed = dfStartSpeed;
  }

  if ( m_MissionReader.GetConfigurationParam( "start_depth", sVal ) )
  {
    dfStartDepth = std::stod( sVal );
    dfPrevDepth = dfStartDepth;
  }

  if ( m_MissionReader.GetConfigurationParam( "start_x", sVal ) )
  {
    dfStartX = std::stod( sVal );
    dfPrevX = dfStartX;
    m_Comms.Notify( "NAV_X", dfStartX, dfTimeNow );
  }

  if ( m_MissionReader.GetConfigurationParam( "start_y", sVal ) )
  {
    dfStartY = std::stod( sVal );
    dfPrevY = dfStartY;
    m_Comms.Notify( "NAV_Y", dfStartY, dfTimeNow );
  }

  if ( m_MissionReader.GetConfigurationParam( "rate_heading", sVal ) )
  {
    dfRateHeading = std::stod( sVal );
    dfIncHeading = dfRateHeading * dfIncTime;
  }

  if ( m_MissionReader.GetConfigurationParam( "rate_speed", sVal ) )
  {
    dfRateSpeed = std::stod( sVal );
    dfIncSpeed = dfRateSpeed * dfIncTime;
  }

  if ( m_MissionReader.GetConfigurationParam( "rate_depth", sVal ) )
  {
    dfRateDepth = std::stod( sVal );
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

