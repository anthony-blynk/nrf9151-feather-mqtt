# Blynk MQTT Sample for nRF9151 Feather

Connects a [Circuit Dojo Feather nRF9151](https://www.circuitdojo.com/products/nrf9151-feather) to [Blynk](https://blynk.io) over LTE-M using MQTT. Features:

- Publishes an incrementing counter to Blynk datastream **V1** on a configurable interval
- Publishes the time of the last button press to datastream **V2** when the user button is pressed
- OTA firmware updates via Blynk
- Credentials (auth token, server, template ID) stored in flash — no recompile needed to configure a device

## Blynk Setup

In your Blynk template create two datastreams:
- **V1** — Integer — for the counter
- **V2** — String — for the button press timestamp

## Using the Pre-built Binary

If you want to avoid setting up the build environment you can flash the pre-built binary from the [Releases](../../releases) page.

### First-time flash

1. Download `zephyr.signed.bin` from the latest release
2. Install [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-Desktop) and open the **Programmer** app
3. Connect your Feather via USB
4. Do a full chip erase, then flash `zephyr.signed.bin`

### Provisioning credentials

On first boot the device has no credentials. Connect a serial terminal (e.g. PuTTY or nRF Serial Terminal) at **115200 baud** to the Feather's COM port. Press Enter to get the shell prompt:

```
uart:~$
```

Then enter your Blynk credentials:

```
cred token <your-blynk-auth-token>
cred template <your-template-id>
cred server <your-blynk-server>   (optional, default: blynk.cloud)
kernel reboot
```

Use `cred show` to check what is stored, or `cred clear` to erase and re-provision.

The device reboots and connects automatically. Credentials survive OTA updates.

> **Note:** The pre-built binary contains a hardcoded Blynk template ID in its firmware tag. This means Blynk's OTA firmware shipment UI will not work for your template — you need to build from source (see below) to use Blynk OTA. Alternatively you can trigger OTA by publishing directly to the device's `downlink/ota/json` topic with a URL to a `zephyr.signed.bin`.

## Building from Source

You need VSCode with the Zephyr Tools extension installed.

1. In an empty workspace open Zephyr Tools. It will pop up a window asking to Init Repo or Create Project — choose **Init Repo**.

2. It will ask for a directory. This is the root for the entire SDK and where you will open VSCode from in future. Watch out for Windows path length limits — use a short name, e.g. `c:\nrf91`.

3. It will ask for a repository URL, use the Circuit Dojo repo:
```
https://github.com/circuitdojo/nrf9160-feather-examples-and-drivers.git
```
Hit Enter for the default branch. It will then install and initialise everything — this can take 10+ minutes.

4. Clone this sample into the `nfed\samples` directory:
```
cd c:\nrf91\nfed\samples
git clone https://github.com/anthony-blynk/nrf9151-feather-mqtt.git blynk_mqtt
```

5. Set your Blynk template ID in `prj.conf`:
```
CONFIG_BLYNK_TEMPLATE_ID="TMPLxxxxxxxx"
```
The auth token and server are entered at runtime via the shell — do not put them in `prj.conf`.

6. In the Project Settings panel on the left set:
   - **Board:** `circuitdojo_feather_nrf9151/nrf9151/ns`
   - **Project:** the cloned directory, e.g. `c:\nrf91\nfed\samples\blynk_mqtt`

7. In Quick Actions click **Build**. The first build takes about 10 minutes; subsequent builds are faster.

8. Plug in the Feather USB cable. If you have previously done an OTA update, run a chip erase first:
```
probe-rs erase --chip nRF9151_xxAA
```
Then in Quick Actions click **Flash** and select `nRF9151_xxAA`.

9. Click **Monitor** to view the log output. If you miss the startup messages press the reset button on the Feather.

10. On first boot follow the credential provisioning steps above.

## OTA Firmware Updates

For your own builds you can use Blynk's built-in firmware shipment — upload `zephyr.signed.bin` from `build/circuitdojo_feather_nrf9151/blynk_mqtt/zephyr/zephyr.signed.bin`.

To update the version before building, edit the `VERSION` file:
```
VERSION_MAJOR = 1
VERSION_MINOR = 4
PATCHLEVEL = 0
```
