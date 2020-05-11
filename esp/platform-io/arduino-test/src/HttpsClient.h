#include "SSD1306Wire.h"

class HttpsClient
{
private:
    void makeRequest(String host, String path, String body, String additionalHeader, String method);
    int retry;
public:
    HttpsClient();
    ~HttpsClient();
    void doGet(String host, String path);
    void doGet(String host, String path, String additionalHeader);
    void doPost(String host, String path, String body, String additionalHeader);
};

IPAddress initWifiClient(String ssid, String password);