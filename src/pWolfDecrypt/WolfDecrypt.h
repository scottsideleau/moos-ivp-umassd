/****************************************************************************/
/* WolfDecrypt.h */
/****************************************************************************/

#ifndef MOOS_APP_WolfDecrypt_H_
#define MOOS_APP_WolfDecrypt_H_

// MOOS Includes
#include "MOOS/libMOOS/App/MOOSApp.h"

// WolfSSL Includes
#include <wolfssl/wolfcrypt/aes.h>


/*************************************************************************//**
 * Class that extends CMOOSApp to produce pWolfDecrypt.
 */
class CMOOSWolfDecrypt : public CMOOSApp
{
  public:
    CMOOSWolfDecrypt();
    virtual ~CMOOSWolfDecrypt();

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
    int nKeyLength;
    std::string sPassword;

    // Global variables
    double dfTimeNow;
    std::string sSecretMessage;
    bool bNewSecret;

    // WolfSSL variables
    Aes aes;

    // Functions
    void DoRegistrations();
};

#endif // #ifndef MOOS_APP_WolfDecrypt_H_

