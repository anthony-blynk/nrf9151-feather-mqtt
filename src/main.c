/*
 * Copyright (c) 2020 Circuit Dojo LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

/* nRF Libraries */
#include <modem/nrf_modem_lib.h>
#include <modem/lte_lc.h>
#include <modem/modem_info.h>

/* Local */
#include "cloud/cloud.h"
#include "credentials/credentials.h"

/* ── Blynk firmware binary tag ───────────────────────────────────────────── */

#define BLYNK_PARAM_KV(k, v)  k "\0" v "\0"

volatile const char firmwareTag[] __attribute__((used)) = "blnkinf\0"
	BLYNK_PARAM_KV("mcu"    , CONFIG_FIRMWARE_VERSION)
	BLYNK_PARAM_KV("fw-type", CONFIG_BLYNK_TEMPLATE_ID)
	BLYNK_PARAM_KV("build"  , __DATE__ " " __TIME__)
	BLYNK_PARAM_KV("blynk"  , "0.1.0")
	BLYNK_PARAM_KV("hw"     , CONFIG_BOARD)
	"\0";


/* Timer */
static void timeout_handler(struct k_timer *timer_id);
K_TIMER_DEFINE(timer, timeout_handler, NULL);

/* Thread control */
K_SEM_DEFINE(thread_sem, 0, 1);

/* Variables */
static struct device_data data = {
    .do_something = true,
};

void cloud_cb(struct device_data *p_data)
{
    LOG_INF("Cloud callback");

    /* TODO: handle data here */
}

static void timeout_handler(struct k_timer *timer_id)
{
    LOG_INF("Publish interval");
    k_sem_give(&thread_sem);
}

int main(void)
{
    int err;

    LOG_INF("Blynk MQTT Sample. Board: %s  FW: %s  Build: %s %s",
            CONFIG_BOARD, CONFIG_FIRMWARE_VERSION, __DATE__, __TIME__);
    (void)firmwareTag[0]; /* retain firmware tag in binary */

    /* Load credentials from NVS */
    err = credentials_init();
    if (err == -ENOENT)
    {
        /* No auth token – shell is still running, wait for provisioning */
        while (1) {
            k_sleep(K_FOREVER);
        }
    }
    else if (err < 0)
    {
        LOG_ERR("Failed to init credentials storage. (err: %i)", err);
        return err;
    }

    /* Init modem lib */
    err = nrf_modem_lib_init();
    if (err < 0)
    {
        LOG_ERR("Failed to init modem lib. (err: %i)", err);
        return err;
    }

    /* Cloud init */
    err = cloud_init(cloud_cb);
    if (err < 0)
    {
        LOG_ERR("Unable to set callback. Err: %i", err);
        return err;
    }

    /* Connect to cellular network */
    LOG_INF("Connecting to cellular network…");
    err = lte_lc_connect();
    if (err < 0)
    {
        LOG_ERR("Failed to connect. Err: %i", err);
        return err;
    }

    /* LTE is up – start MQTT and wait for first connection */
    cloud_start();
    cloud_wait_connected();

    /* Start timer and trigger immediate first publish */
    k_timer_start(&timer, K_MINUTES(CONFIG_DEFAULT_DELAY), K_MINUTES(CONFIG_DEFAULT_DELAY));
    k_sem_give(&thread_sem);

    while (1)
    {
        k_sem_take(&thread_sem, K_FOREVER);

        /* Publish and sleep .. */
        err = cloud_publish(&data);
        if (err < 0)
            LOG_ERR("Unable to publish. Err: %i", err);
    }
}
