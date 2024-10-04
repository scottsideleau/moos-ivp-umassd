/****************************************************************************/
/* SimpleRobot.h */
/****************************************************************************/

#ifndef MOOS_APP_SimpleRobot_H_
#define MOOS_APP_SimpleRobot_H_

// MOOS Includes
#include "MOOS/libMOOS/App/MOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"


/*************************************************************************//**
 * Class that extends CMOOSApp to produce uSimpleRobot.
 */
class CMOOSSimpleRobot : public CMOOSApp
{
  public:
    CMOOSSimpleRobot();
    virtual ~CMOOSSimpleRobot();

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
    CMOOSGeodesy m_geodesy;
    bool bGeoOk;
    // Global variables filled by the .moos file
    std::string sPrefix;
    double dfStartHeading;
    double dfStartSpeed;
    double dfStartDepth;
    double dfStartX;
    double dfStartY;

    double dfRateHeading;
    double dfRateSpeed;
    double dfRateDepth;

    // Global variables
    unsigned int nAppTick;
    unsigned int nTimeWarp;

    double dfTimeNow;
    double dfIncTime;
    double dfDesiredHeading;
    double dfPrevHeading;
    double dfNextHeading;
    double dfIncHeading;
    double dfDesiredSpeed;
    double dfPrevSpeed;
    double dfNextSpeed;
    double dfIncSpeed;
    double dfDesiredDepth;
    double dfPrevDepth;
    double dfNextDepth;
    double dfIncDepth;
    double dfPrevX;
    double dfNextX;
    double dfPrevY;
    double dfNextY;

    bool bHeadingIsCurrent;
    bool bSpeedIsCurrent;
    bool bDepthIsCurrent;

    // Functions
    void DoRegistrations();
};

#endif // #ifndef MOOS_APP_SimpleRobot_H_

