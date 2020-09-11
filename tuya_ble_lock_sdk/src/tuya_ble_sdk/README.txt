
1、tuya_ble_type.h 数据结构定义

2、tuya_ble_port.h  需要移植实现的所有接口定义及说明。

3、tuya_ble_api.h  application 可直接调用的api。

4、tuya_ble_config.h  sdk配置文件。

5、tuya_ble_port.c   port api弱实现，编译时可被用户实现的函数覆盖，无须更改；用户移植实现tuya_ble_port.h 定义的函数时需新建一个c文件。

6、application通过调用tuya_ble_api.h里的函数向sdk发送数据，通过注册的callback获取SDK的数据，当使用os时，application需要新建一个tuya_ble_cb_evt_param_t结构类型
的queue,并注册queue到ble sdk，在处理完一条数据后，需要调用一次tuya_ble_event_response()函数。

7、application在启动时需要调用tuya_ble_sdk_init()函数初始化sdk，在所有platform相关的函数init之后调用。

