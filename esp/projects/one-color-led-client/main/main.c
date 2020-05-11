/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "stdlib.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_https_server.h"
#include "driver/ledc.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#define WIFI_SSID CONFIG_ESP_WIFI_SSID
#define WIFI_PASS CONFIG_ESP_WIFI_PASS
#define WIFI_MAX_RETRY CONFIG_ESP_WIFI_MAX_RETRY

#define RED1Pin CONFIG_ESP_PIN_RED1
#define GREEN1Pin CONFIG_ESP_PIN_GREEN1
#define BLUE1Pin CONFIG_ESP_PIN_BLUE1
#define RED2Pin CONFIG_ESP_PIN_RED2
#define GREEN2Pin CONFIG_ESP_PIN_GREEN2
#define BLUE2Pin CONFIG_ESP_PIN_BLUE2
#define NUMBER_RGB CONFIG_SECOND_LED_AVAILABLE

#define TAG "LED-Server"


#define RED1 0
#define GREEN1 1
#define BLUE1 2
#define RED2 3
#define GREEN2 4
#define BLUE2 5

static const int ledcNum = (NUMBER_RGB == 2 ? 2 : 1) * 3;

ledc_channel_config_t ledcChannelConfig[6];

void app_main();

void startWifi();

void inits();

static esp_err_t wifiCallback(void *ctx, system_event_t *event);

httpd_handle_t startHttpdServer();

void registerUrlHandler(httpd_handle_t server);

ip4_addr_t *ip4Addr;

esp_err_t get_handler_root(httpd_req_t *req);

esp_err_t get_handler_set(httpd_req_t *req);

void fadeLed(int led, int brightness, int fadeTime);

const httpd_uri_t root = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = get_handler_root
};

const httpd_uri_t set = {
        .uri = "/set",
        .method = HTTP_GET,
        .handler = get_handler_set
};

esp_err_t get_handler_root(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req,
                    "<h1>LED-Client</h1><p>You may access this led-controller with /on?r=xxx&g=xxx&b=xxx where xxx is a number between 0 and 255.</p>",
                    -1);
    return ESP_OK;
}

esp_err_t get_handler_set(httpd_req_t *req) {
    char *buffer;
    size_t bufferLength = httpd_req_get_url_query_len(req) + 1;
    httpd_resp_set_type(req, "text/plain");
    httpd_resp_send(req, "thanks", -1);
    if (bufferLength > 1) {
        buffer = malloc(bufferLength);
        if (httpd_req_get_url_query_str(req, buffer, bufferLength) == ESP_OK) {
            ESP_LOGD(TAG, "Found URL-query => %s", buffer);
            char param[32];
            if (httpd_query_key_value(buffer, "r", param, sizeof(param)) == ESP_OK) {
                ESP_LOGD(TAG, "Found URL-query parameter => r=%s", param);
                int red = atoi(param);
                fadeLed(RED1, red, 1000);
                //fadeLed(RED2, red, 1000);
                ESP_LOGD(TAG, "RED = %i", red);
            }
            if (httpd_query_key_value(buffer, "g", param, sizeof(param)) == ESP_OK) {
                ESP_LOGD(TAG, "Found URL-query parameter => g=%s", param);
                int green = atoi(param);
                fadeLed(GREEN1, green, 1000);
                //fadeLed(GREEN2, green, 1000);
                ESP_LOGD(TAG, "GREEN = %i", green);
            }
            if (httpd_query_key_value(buffer, "b", param, sizeof(param)) == ESP_OK) {
                ESP_LOGD(TAG, "Found URL-query parameter => b=%s", param);
                int blue = atoi(param);
                fadeLed(BLUE1, blue, 1000);
                //fadeLed(BLUE2, blue, 1000);
                ESP_LOGD(TAG, "BLUE = %i", blue);
            }
        }
        free(buffer);
    }
    return ESP_OK;
}


httpd_handle_t start_webserver() {
    httpd_handle_t server = startHttpdServer();
    if (server != NULL) {
        registerUrlHandler(server);
        ESP_LOGI(TAG, "Server started at https://%s", ip4addr_ntoa(ip4Addr));
    }
    return server;
}

void stop_webserver(httpd_handle_t server) {
    httpd_ssl_stop(server);
}

void registerUrlHandler(httpd_handle_t server) {
    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &set);
}

httpd_handle_t startHttpdServer() {
    ESP_LOGI(TAG, "Starting web-server");
    httpd_handle_t server = NULL;

    httpd_ssl_config_t config = HTTPD_SSL_CONFIG_DEFAULT();

    //load cacert
    extern const unsigned char cacert_pem_start[] asm("_binary_cert_pem_start");
    extern const unsigned char cacert_pem_end[] asm("_binary_cert_pem_end");
    config.cacert_pem = cacert_pem_start;
    config.cacert_len = cacert_pem_end - cacert_pem_start;

    //load privateKey
    extern const unsigned char privateKey_pem_start[] asm("_binary_private_pem_start");
    extern const unsigned char privateKey_pem_end[] asm("_binary_private_pem_end");
    config.prvtkey_pem = privateKey_pem_start;
    config.prvtkey_len = privateKey_pem_end - privateKey_pem_start;

    esp_err_t ret = httpd_ssl_start(&server, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "error starting web-server!");
        return NULL;
    }
    return server;
}

//------------ technical methods:

void initNVS();

void initWifiClient();

void configWifiClient();

void startWifiClient();

void initLedc();

void fadeLed(int led, int brightness, int fadetime);

void app_main() {
    inits();
}

void startWifi(void *server) {
    ESP_LOGI(TAG, "Trying to connect to wifi...");
    ESP_LOGD(TAG, "Wifi-data... SSID: %s, PASS: %s, MAX-RETRY: %i", WIFI_SSID, WIFI_PASS, WIFI_MAX_RETRY);

    initNVS();
    initWifiClient(server);
    configWifiClient();
    startWifiClient();
    ESP_LOGI(TAG, "wifi-setup finished.");
}

void startWifiClient() {
    ESP_LOGI(TAG, "starting wifi...");
    ESP_ERROR_CHECK(esp_wifi_start());
}

void configWifiClient() {
    ESP_LOGI(TAG, "configure wifi adapter...");
    wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&initConfig));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifiConfig = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASS
            }
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
}

void initWifiClient(void *server) {
    ESP_LOGI(TAG, "init wifi adapter...");
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(wifiCallback, server));
}

void initNVS() {
    ESP_LOGD(TAG, "init nvs flash...");
    esp_err_t nvsResult = nvs_flash_init();
    if (nvsResult == ESP_ERR_NVS_NO_FREE_PAGES || nvsResult == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_LOGD(TAG, "nvs has no free pages... erasing nvs-flash and re-init...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvsResult = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvsResult);
    ESP_LOGD(TAG, "nvs-flash initialized.");
}

static esp_err_t wifiCallback(void *ctx, system_event_t *event) {
    ESP_LOGD(TAG, "wifiCallback accessed");
    httpd_handle_t *server = (httpd_handle_t *) ctx;
    switch (event->event_id) {
        case SYSTEM_EVENT_WIFI_READY:
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            break;
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "Wifi started... Connecting...");
            esp_wifi_connect();
            ESP_LOGI(TAG, "connected.");
            break;
        case SYSTEM_EVENT_STA_STOP:
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "retry to connect to AP");
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "Got IP: %s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            ip4Addr = &event->event_info.got_ip.ip_info.ip;
            if (*server == NULL) {
                *server = start_webserver();
            }
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            break;
        case SYSTEM_EVENT_AP_START:
            break;
        case SYSTEM_EVENT_AP_STOP:
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            break;
        case SYSTEM_EVENT_GOT_IP6:
            break;
        case SYSTEM_EVENT_ETH_START:
            break;
        case SYSTEM_EVENT_ETH_STOP:
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            break;
        case SYSTEM_EVENT_MAX:
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:
            break;
    }
    return ESP_OK;
}

void inits() {
    printf("-------------------------------------------\n\n\n");
    ESP_LOGI(TAG, "Initializing server...");
    static httpd_handle_t server = NULL;
    startWifi(&server);
    initLedc();
}

void initLedc() {
    ledc_timer_config_t ledcTimerConfig = {
            .duty_resolution = LEDC_TIMER_8_BIT,
            .freq_hz = 5000,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_1
    };
    ledc_timer_config(&ledcTimerConfig);

    if (NUMBER_RGB == 1) {
        ledc_channel_config_t myConfig[3] = {
                {
                        .channel = LEDC_CHANNEL_0,
                        .duty = 0,
                        .gpio_num = RED1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_1,
                        .duty = 0,
                        .gpio_num = GREEN1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_2,
                        .duty = 0,
                        .gpio_num = BLUE1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
        };
        memcpy(ledcChannelConfig, myConfig, sizeof(ledcChannelConfig));
    } else {
        ledc_channel_config_t myConfig[6] = {
                {
                        .channel = LEDC_CHANNEL_0,
                        .duty = 0,
                        .gpio_num = RED1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_1,
                        .duty = 0,
                        .gpio_num = GREEN1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_2,
                        .duty = 0,
                        .gpio_num = BLUE1Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_3,
                        .duty = 0,
                        .gpio_num = RED2Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_4,
                        .duty = 0,
                        .gpio_num = GREEN2Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                },
                {
                        .channel = LEDC_CHANNEL_5,
                        .duty = 0,
                        .gpio_num = BLUE2Pin,
                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                        .hpoint = 0,
                        .timer_sel = LEDC_TIMER_1
                }
        };
        memcpy(ledcChannelConfig, myConfig, sizeof(ledcChannelConfig));
    }

    for (int i = 0; i < ledcNum; ++i) {
        ledc_channel_config(&ledcChannelConfig[i]);
    }

    ledc_fade_func_install(0);

    for (int i = 0; i < ledcNum; ++i) {
        fadeLed(i, 255, 2000);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    for (int i = 0; i < ledcNum; ++i) {
        fadeLed(i, 0, 2000);
    }

    fadeLed(3, 255, 0);
}

void fadeLed(int led, int brightness, int fadeTime) {
    ESP_LOGD(TAG, "SET LED %i to brightness %i in %ims", led, brightness, fadeTime);

    if (fadeTime < 10) {
        ledc_set_duty(ledcChannelConfig[led].speed_mode, ledcChannelConfig[led].channel, brightness);
        ledc_update_duty(ledcChannelConfig[led].speed_mode, ledcChannelConfig[led].channel);
        return;
    }

    ledc_set_fade_with_time(ledcChannelConfig[led].speed_mode, ledcChannelConfig[led].channel, brightness, fadeTime);
    ledc_fade_start(ledcChannelConfig[led].speed_mode, ledcChannelConfig[led].channel, LEDC_FADE_NO_WAIT);
}