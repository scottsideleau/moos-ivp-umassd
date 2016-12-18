/****************************************************************************/
/* MoosKiller.h                                                             */
/****************************************************************************/

#ifndef MOOS_APP_MoosKiller_H_
#define MOOS_APP_MoosKiller_H_

// MOOS Includes
#include "MOOS/libMOOS/App/MOOSApp.h"

/*************************************************************************//**
 * Class that extends CMOOSApp to produce pMoosKiller.
 */
class CMOOSMoosKiller : public CMOOSApp
{
  public:
    CMOOSMoosKiller();
    virtual ~CMOOSMoosKiller();

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
    // Global variables filled by the .moos file
    std::string sKillVar;
    std::string sMoosBin;

    // Gobal variables
    double dfTimeNow; // Refreshed to current MOOSTime before updating
    bool bKillIt;

    // Functions
    void DoRegistrations();
};

#endif // #ifndef MOOS_APP_MoosKiller_H_

