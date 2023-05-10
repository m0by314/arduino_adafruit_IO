/** 
 * Class to update an AdafruitIo feed with an ESP32
 * 
 * Exemple: 
 *      #define IO_KEY "zzzzzzzzzzzzzzzzzzzzzzz"
 *      #define IO_KEY "user name"
 *      
 *      AdafruitIO adafruit_feed(IO_KEY, IO_KEY);
 *
 *      adafruit_feed.updateFeed(IO_FEED_NAME, value); //returns true if successful
*/
#ifndef _ADAFRUITIO_H_
#define _ADAFRUITIO_H_
#include <WiFiClientSecure.h>

class AdafruitIO {
    public:
        AdafruitIO(const char* adafruit_key, const char* userName);
        ~AdafruitIO();
        bool updateFeed(const char* feedName , const char* value);

    private:
        const char*         _key;
        const char*         _userName;
        WiFiClientSecure*   _client;
        bool _check_response_success(const char* response);
        
};

#endif