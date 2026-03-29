# Blynk MQTT Sample for nRF9151 Feather

Sends an incrementing counter to a Blynk Datastream. Supports OTA firmware updates.
Uses LTE-M.

## Installation and Building

You need VSCode, and install the "Zephyr Tools" Extension

1) In an empty workspace on Zephyr Tools. It will popup a window asking to Init Repo or Create Project, choose Init Repo. 

2) Next, it will ask for a Directory. This is the root directory for the project and where you will open VSCode from in the future. Watch out for Windows file path length limitations - use a short name in the root directory, eg c:\nrf91

3) It will then ask for a repository url, use this Circuitdojo repo: 
```
https://github.com/circuitdojo/nrf9160-feather-examples-and-drivers.git
```
It will then ask for a branch, just hit enter for the default.

It will then start installing and initializing things which can take a while, at least 10 minutes. At the end the terminal shows:
```
. . .
[notice] A new release of pip is available: 24.3.1 -> 26.0.1
[notice] To update, run: python.exe -m pip install --upgrade pip
 *  Terminal will be reused by tasks, press any key to close it. 
 ```

4) Clone the Blynk sample into the ```nfed\samples``` directory:
```
C:\nRF91Sdk\nfed\samples>git clone https://github.com/anthony-blynk/nrf9151-feather-https.git blynk_https
Cloning into 'blynk_https'...
remote: Enumerating objects: 29, done.
remote: Counting objects: 100% (29/29), done.
remote: Compressing objects: 100% (18/18), done.
remote: Total 29 (delta 7), reused 29 (delta 7), pack-reused 0 (from 0)
Receiving objects: 100% (29/29), 9.00 KiB | 460.00 KiB/s, done.
Resolving deltas: 100% (7/7), done.
```

5) In the top of file ```prj.conf``` set your Blynk Server and Auth Token

6) In the Project Settings on the left set the Board and Project:
- The Board should be circuitdojo_feather_nrf9151/nrf9151/ns 
- The Project is the repo you just cloned, eg c:\nRF91Sdk\nfed\samples\blynk_https

7) In Quick Actions click Build.

This is also slow and takes about 10 minutes for the first build, subsequent ones can be faster.

8) Plug in your Feather USB cable and in Quick Actions click Flash.

- choose the target chip: nRF9151_xxAA

9) Click Monitor, if you're slow and miss the initial messages press the reset button on the Feather to see from the begining



