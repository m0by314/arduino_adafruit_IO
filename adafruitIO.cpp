#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <WiFiClientSecure.h>

#include "adafruitIO.h"

#define HOST "io.adafruit.com"
#define SSL_PORT 443
#define DEBUG 0   // 1 to active debug 

const char* _adafruit_root_certificate = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEizCCA3OgAwIBAgIQBUb+GCP34ZQdo5/OFMRhczANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0xNzExMDYxMjIzNDVaFw0yNzExMDYxMjIzNDVaMF4xCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xHTAbBgNVBAMTFEdlb1RydXN0IFJTQSBDQSAyMDE4MIIBIjANBgkqhkiG9w0B
AQEFAAOCAQ8AMIIBCgKCAQEAv4rRY03hGOqHXegWPI9/tr6HFzekDPgxP59FVEAh
150Hm8oDI0q9m+2FAmM/n4W57Cjv8oYi2/hNVEHFtEJ/zzMXAQ6CkFLTxzSkwaEB
2jKgQK0fWeQz/KDDlqxobNPomXOMJhB3y7c/OTLo0lko7geG4gk7hfiqafapa59Y
rXLIW4dmrgjgdPstU0Nigz2PhUwRl9we/FAwuIMIMl5cXMThdSBK66XWdS3cLX18
4ND+fHWhTkAChJrZDVouoKzzNYoq6tZaWmyOLKv23v14RyZ5eqoi6qnmcRID0/i6
U9J5nL1krPYbY7tNjzgC+PBXXcWqJVoMXcUw/iBTGWzpwwIDAQABo4IBQDCCATww
HQYDVR0OBBYEFJBY/7CcdahRVHex7fKjQxY4nmzFMB8GA1UdIwQYMBaAFAPeUDVW
0Uy7ZvCj4hsbw5eyPdFVMA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEF
BQcDAQYIKwYBBQUHAwIwEgYDVR0TAQH/BAgwBgEB/wIBADA0BggrBgEFBQcBAQQo
MCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBCBgNVHR8E
OzA5MDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRHbG9i
YWxSb290Q0EuY3JsMD0GA1UdIAQ2MDQwMgYEVR0gADAqMCgGCCsGAQUFBwIBFhxo
dHRwczovL3d3dy5kaWdpY2VydC5jb20vQ1BTMA0GCSqGSIb3DQEBCwUAA4IBAQAw
8YdVPYQI/C5earp80s3VLOO+AtpdiXft9OlWwJLwKlUtRfccKj8QW/Pp4b7h6QAl
ufejwQMb455OjpIbCZVS+awY/R8pAYsXCnM09GcSVe4ivMswyoCZP/vPEn/LPRhH
hdgUPk8MlD979RGoUWz7qGAwqJChi28uRds3thx+vRZZIbEyZ62No0tJPzsSGSz8
nQ//jP8BIwrzBAUH5WcBAbmvgWfrKcuv+PyGPqRcc4T55TlzrBnzAzZ3oClo9fTv
O9PuiHMKrC6V6mgi0s2sa/gbXlPCD9Z24XUMxJElwIVTDuKB0Q4YMMlnpN/QChJ4
B0AFsQ+DU0NCO+f78Xf7
-----END CERTIFICATE-----
)EOF";

/********************************************************************************************
 * Constructor
*/
AdafruitIO::AdafruitIO (const char* adafruit_key, const char* userName) {
    this->_key = adafruit_key;
    this->_userName = userName;
    this->_client = new WiFiClientSecure();
    _client->setCACert(_adafruit_root_certificate);
}

/********************************************************************************************
 * Destructor
*/
AdafruitIO::~AdafruitIO () {
    delete _client;
    _client = NULL;
}

/********************************************************************************************
 *  Update adafruit IO feed
 * 
 * @param feedName 
 * @param value
 * 
 * @return true if succes 
 */ 
bool AdafruitIO::updateFeed(const char* feedName , const char* value) {
    bool _ret;
    if (!_client->connect(HOST, SSL_PORT)) {
        if (DEBUG) printf("Connection failed!\r\n");
        _client->stop();
        _ret = false;
    }
    else {
        if (DEBUG) printf("Connected to server!\r\n");
        
        // Make payload
        int len_payload = strlen("{\"value\":\"") + strlen(value) + strlen("\"}");
        char payload[len_payload + 1]; 
        strcpy(payload, "{\"value\":\"");
        strcat(payload, value);
        strcat(payload, "\"}");
        if (DEBUG) {
            printf("Payload:\r\n");
            printf("%s\r\n", payload);
        }
        
        // Make a HTTP request:
        int len_request = strlen("POST /api/v2/") + strlen(_userName) + strlen("/feeds/") + strlen(feedName) + strlen("/data HTTP/1.1");
        char request[len_request + 1];
        strcpy(request, "POST /api/v2/");
        strcat(request, _userName);
        strcat(request, "/feeds/");
        strcat(request, feedName);
        strcat(request, "/data HTTP/1.1");

        _client->println(request);
        
        // Headers
        _client->print("Host:"); _client->println(HOST);
        _client->print("X-AIO-Key: "); _client->println(_key);
        _client->println("Content-Type: application/json");
        _client->print("Content-Length: "); _client->println(len_payload);
        _client->println("Connection: Close");
        _client->println();

        // POST message body
        _client->println(payload);
        _client->println();
        
        // Wait for server response
        while (_client->available() == 0);
        
        char * response = (char *)malloc(sizeof(char));
	    while (_client->available()) {
		    char c = _client->read();
            response = (char *)realloc(response, (strlen(response) +  strlen(&c) + 1 ) * sizeof(char));
            strncat(response, &c, sizeof(char));
	    }
        if (DEBUG){
            printf("Response;\r\n");
            printf("%s\r\n", response);
        }
        _ret =  _check_response_success(response);
        free(response);
    }
    return _ret;
}

/********************************************************************************************
 * Verify response success
 * 
 * @param response provide by the client
 * @return true in case of sucess
 */
bool AdafruitIO::_check_response_success(const char* response) {
    const char * word = "HTTP/1.1 200 OK";
    return strstr(response, word);
}