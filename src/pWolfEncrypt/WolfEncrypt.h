/****************************************************************************/
/* WolfEncrypt.h */
/****************************************************************************/

#ifndef MOOS_APP_WolfEncrypt_H_
#define MOOS_APP_WolfEncrypt_H_

// MOOS Includes
#include "MOOS/libMOOS/MOOSLib.h"

// WolfSSL Includes
#include <wolfssl/wolfcrypt/aes.h>


/*************************************************************************//**
 * Class that extends CMOOSApp to produce pWolfEncrypt.
 */
class CMOOSWolfEncrypt : public CMOOSApp
{
  public:
    CMOOSWolfEncrypt();
    virtual ~CMOOSWolfEncrypt();

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
    std::string sSecretMessage;
    bool bNewSecret;

    // Global variables
    double dfTimeNow;

    // WolfSSL variables
    Aes aes;

    // Functions
    void DoRegistrations();
};

#endif // #ifndef MOOS_APP_WolfEncrypt_H_

