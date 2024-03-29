#include "tos_k.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"

#include "cJSON.h"
#include "stdbool.h"

// 物联网开发平台设备信息
#define PRODUCT_ID "SJB32726CH"
#define DEVICE_NAME "dev001"
#define DEVICE_KEY ""

// WIFI信息
#define YOUR_WIFI_SSID "iPhone SE"
#define YOUR_WIFI_PWD ""

// 数据模板
//#define REPORT_LX_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"lx\\\":%.0f}}"
#define REPORT_BEDROOM_LIGHT_SWITCH_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"bedroom_light_switch\\\":%d}}"
#define REPORT_SUNSET_LIGHT_SWITCH_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"sunset_light_switch\\\":%d}}"
//#define REPORT_BRIGHTNESS_LEVEL_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"brightness_level\\\":%d}}"
#define CONTROL_REPLY_DATA_TEMPLATE "{\\\"method\\\":\\\"control_reply\\\"\\,\\\"clientToken\\\":\\\"%s\\\"\\,\\\"code\\\":0\\,\\\"status\\\":\\\"ok\\\"}"

//#define REPORT_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"RGB_brightness\\\":%d\\,\\\"name\\\":\\\"bedroom\\\"}}"

#define OPEN_SUNSET_LIGHT GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define CLOSE_SUNSET_LIGHT GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define SUNSET_LIGHT_IS_OPEN GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_10)

char payload[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};

static bool is_bedroom_light_switch_changed = true;
static bool is_sunset_light_switch_changed = true;
// static bool is_brightness_level_changed = true;
static bool is_client_token_received = false;

int bedroom_light_switch_cache = 0;
int sunset_light_switch_cache = 0;
int rgb_light_type_cache = 0;
// int brightness_level_cache = 2;
char client_token_cache[128] = {0};

extern rgb_data_msg_q;

// /***************************************************************
// * 函数名称: key1_handler_callback
// * 说    明: 按键处理回调
// ***************************************************************/
// void key1_handler_callback(void)
// {
//     if (power_switch_cache)
//     {
//         power_switch_cache = 0;
//         LED_CLOSE;
//     }
//     else
//     {
//         power_switch_cache = 1;
//         LED_OPEN;
//     }

//     is_power_switch_changed = true;
// }

static void iot_explorer_handle_bedroom_light_switch(int bedroom_light_switch)
{
    bedroom_light_switch_cache = bedroom_light_switch;

    if (bedroom_light_switch == 1)
    {
        turn_on_bedroom_light();
    }
    else
    {
        turn_off_bedroom_light();
    }
    is_bedroom_light_switch_changed = true;
}

static void iot_explorer_handle_sunset_light_switch(int sunset_light_switch)
{
    sunset_light_switch_cache = sunset_light_switch;

    if (sunset_light_switch == 1)
    {
        if (SUNSET_LIGHT_IS_OPEN)
        {
            printf("the light already open\r\n");
        }
        else
        {
            printf("iot-explorer open the light\r\n");
            OPEN_SUNSET_LIGHT;
            is_sunset_light_switch_changed = true;
        }
    }
    else
    {
        if (SUNSET_LIGHT_IS_OPEN)
        {
            printf("iot-explorer close the light\r\n");
            CLOSE_SUNSET_LIGHT;
            is_sunset_light_switch_changed = true;
        }
        else
        {
            printf("the light already closed\r\n");
        }
    }
    is_sunset_light_switch_changed = true;
}

static void iot_explorer_handle_rgb_light_type(int rgb_light_type)
{
    rgb_light_type_cache = rgb_light_type;
    if (rgb_light_type == 0)
    {
        char *msg_to_one_receiver = "color_wipe";
        tos_msg_q_post(&rgb_data_msg_q, msg_to_one_receiver);
    }
    else if (rgb_light_type == 1)
    {
        char *msg_to_one_receiver = "theater_chase";
        tos_msg_q_post(&rgb_data_msg_q, msg_to_one_receiver);
    }
    else if (rgb_light_type == 2)
    {
        char *msg_to_one_receiver = "rainbow";
        tos_msg_q_post(&rgb_data_msg_q, msg_to_one_receiver);
    }
    else if (rgb_light_type == 3)
    {
        char *msg_to_one_receiver = "rainbow_cycle";
        tos_msg_q_post(&rgb_data_msg_q, msg_to_one_receiver);
    }
    else if (rgb_light_type == 4)
    {
        char *msg_to_one_receiver = "theater_chase_rain";
        tos_msg_q_post(&rgb_data_msg_q, msg_to_one_receiver);
    }
    else
    {
        printf("Unknown type\r\n");
    }
}

/***************************************************************
 * 函数名称: default_message_handler
 * 说    明: IoT Explorer下行数据处理
 ***************************************************************/
static void default_message_handler(mqtt_message_t *msg)
{
    cJSON *root;
    cJSON *params;
    cJSON *token;
    cJSON *method;
    cJSON *bedroom_light_switch;
    cJSON *sunset_light_switch;
    cJSON *rgb_light_type;
    cJSON *RGB_brightness;

    printf("callback:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("\ttopic:%s\r\n", msg->topic);
    printf("\tpayload:%s\r\n", msg->payload);
    printf("---------------------------------------------------------\r\n");

    // 1. 解析从云端收到的控制信息，示例控制信息为
    //"{"method":"control","clientToken":"clientToken-Xx-N_enad","params":{"power_switch":0}}"
    root = cJSON_Parse(msg->payload + 1);
    if (!root)
    {
        printf("Invalid json root\r\n");
        return;
    }

    // 2. 解析出method
    method = cJSON_GetObjectItem(root, "method");
    if (!method)
    {
        printf("Invalid json method\r\n");
        cJSON_Delete(root);
        return;
    }

    // 3. 仅处理云端下发的 control 数据，report_reply暂不处理
    if (0 != strncmp(method->valuestring, "control", sizeof("control") - 1))
    {
        cJSON_Delete(root);
        return;
    }

    // 4. 解析出params
    params = cJSON_GetObjectItem(root, "params");
    if (!params)
    {
        printf("Invalid json params\r\n");
        cJSON_Delete(root);
        return;
    }

    // 1. sunset_light_switch
    sunset_light_switch = cJSON_GetObjectItem(params, "sunset_light_switch");
    if (sunset_light_switch)
    {
        iot_explorer_handle_sunset_light_switch(sunset_light_switch->valueint);
    }

    // 2. bedroom_light_switch
    bedroom_light_switch = cJSON_GetObjectItem(params, "bedroom_light_switch");
    if (bedroom_light_switch)
    {
        iot_explorer_handle_bedroom_light_switch(bedroom_light_switch->valueint);
    }

    // 3. rgb_light_type
    rgb_light_type = cJSON_GetObjectItem(params, "rgb_light_type");
    if (rgb_light_type)
    {
        iot_explorer_handle_rgb_light_type(rgb_light_type->valueint);
    }

    //    RGB_brightness = cJSON_GetObjectItem(params, "RGB_brightness");
    //    if (RGB_brightness)
    //    {
    //        printf("RGB_brightness: %d", RGB_brightness);
    //        // e53_sc1_set_ligth_level(brightness_level->valueint);
    //        //        brightness_level_cache = brightness_level->valueint;
    //        is_brightness_level_changed = true;
    //    }

    // 4. 设置clientToken回复
    token = cJSON_GetObjectItem(root, "clientToken");
    if (token)
    {
        strcpy(client_token_cache, token->valuestring);
        is_client_token_received = true;
    }

    cJSON_Delete(root);
}

// void default_message_handler(mqtt_message_t *msg)
// {
//     printf("callback:\r\n");
//     printf("---------------------------------------------------------\r\n");
//     printf("\ttopic:%s\r\n", msg->topic);
//     printf("\tpayload:%s\r\n", msg->payload);
//     printf("---------------------------------------------------------\r\n");
// }

extern void mqtt_task(void)
{
    int ret = 0;
    int size = 0;
//    int RGB_brightness = 0;
    mqtt_state_t state;

    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;

    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));

    /**
     * Please Choose your AT Port first, default is HAL_UART_2(USART2)
     */
    // 1. 初始化ESP8266
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);

    if (ret < 0)
    {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }

    // 2. 连接wifi，如果需要自行配置热点或者wifi，请打开注释
    esp8266_tencent_firmware_join_ap(YOUR_WIFI_SSID, YOUR_WIFI_PWD);

    // 3. 设置设备信息：产品ID，设备名，设备密钥
    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

    // 4. 连接IoT Explorer
    mqtt_param_t init_params = DEFAULT_MQTT_PARAMS;
    if (tos_tf_module_mqtt_conn(init_params) != 0)
    {
        printf("module mqtt conn fail\n");
    }
    else
    {
        printf("module mqtt conn success\n");
    }

    if (tos_tf_module_mqtt_state_get(&state) != -1)
    {
        printf("MQTT: %s\n", state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    }

    // 5. 订阅数据模板 topic
    size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/down/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_reply_topic_name) - 1)
    {
        printf("sub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_reply_topic_name));
    }
    if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0, default_message_handler) != 0)
    {
        printf("module mqtt sub fail\n");
    }
    else
    {
        printf("module mqtt sub success\n");
    }

    memset(report_topic_name, 0, sizeof(report_topic_name));
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1)
    {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }

    while (1)
    {
        tos_sleep_ms(5000);

        // 判断当前IoT Explorer连接状态
        mqtt_state_t state;
        tos_tf_module_mqtt_state_get(&state);
        if (state == MQTT_STATE_DISCONNECTED)
        {
            printf("mqtt is disconnected\r\n");
            tos_sleep_ms(5000);
            continue;
        }

        // // 读取光照强度，数据存放在 e53_value
        // e53_value = e53_scl_read_data();
        // printf("e53 value %.0f lx\r\n", e53_value);

        // memset(e53_str, 0, sizeof(e53_str));
        // sprintf(e53_str, "%.0f lx(lm/m2)", e53_value);
        // OLED_ShowString(0, 2, (uint8_t *)e53_str, 16);

        // 如果开关状态改变，需要上报给平台以同步
        if (is_sunset_light_switch_changed)
        {
            is_sunset_light_switch_changed = false;
            memset(payload, 0, sizeof(payload));
            snprintf(payload, sizeof(payload), REPORT_SUNSET_LIGHT_SWITCH_DATA_TEMPLATE, sunset_light_switch_cache);

            if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
            {
                printf("module mqtt pub fail\n");
            }
            else
            {
                printf("module mqtt pub success\n");
            }
        }

        if (is_bedroom_light_switch_changed)
        {
            is_bedroom_light_switch_changed = false;
            memset(payload, 0, sizeof(payload));
            snprintf(payload, sizeof(payload), REPORT_BEDROOM_LIGHT_SWITCH_DATA_TEMPLATE, bedroom_light_switch_cache);

            if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
            {
                printf("module mqtt pub fail\n");
            }
            else
            {
                printf("module mqtt pub success\n");
            }
        }

        // // 扩展实验2：请在此处添加亮度等级状态上报物联网开发平台的代码
        // if (is_brightness_level_changed)
        // {
        //     is_brightness_level_changed = false;
        //     //----请在空白处填写代码

        //     //----
        // }

        // // 接受到下行control消息，回复最近一条
        // if (is_client_token_received)
        // {
        //     is_client_token_received = false;
        //     memset(payload, 0, sizeof(payload));
        //     snprintf(payload, sizeof(payload), CONTROL_REPLY_DATA_TEMPLATE, client_token_cache);
        //     if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
        //     {
        //         printf("module mqtt pub fail\n");
        //     }
        //     else
        //     {
        //         printf("module mqtt pub success\n");
        //     }
        // }

        /* use AT+PUB AT command */

        //        memset(payload, 0, sizeof(payload));
        //        snprintf(payload, sizeof(payload), REPORT_DATA_TEMPLATE, RGB_brightness++);
        //
        //        if (RGB_brightness > 100)
        //        {
        //            RGB_brightness = 0;
        //        }
        //
        if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
        {
            printf("module mqtt pub fail\n");
            break;
        }
        else
        {
            printf("module mqtt pub success\n");
        }

        // // 2. 向数据模板topic发布光照强度信息
        // memset(payload, 0, sizeof(payload));
        // snprintf(payload, sizeof(payload), REPORT_BRIGHTNESS_LEVEL_DATA_TEMPLATE,
        //          brightness_level_cache);
        // if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload) != 0)
        // {
        //     printf("module mqtt pub fail\n");
        // }
        // else
        // {
        //     printf("module mqtt pub success\n");
        // }
    }
}

// void application_entry(void *arg)
//{
//     init_e53_sc1_light();
//     mqtt_demo_task();
//     while (1)
//     {
//         printf("This is a mqtt demo!\r\n");
//         tos_task_delay(1000);
//     }
// }
