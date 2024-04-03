#include "twiliohelper.h"

TwilioHelper::TwilioHelper()
{

}

void TwilioHelper::sendTwilioMessage() {
    QString accountSid = "ACa18405085019ebc311b26b8ade0edd23";
    QString authToken = "[AuthToken]";
    QString fromNumber = "+12185146708";
    QString toNumber = "+21656623537";
    QString message = "Your message here";

    TwilioHelper::sendMessage(fromNumber, toNumber, message, accountSid, authToken);
}
