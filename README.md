# RadioLib 7.2.0 Port to ESP-IDF 5.4.1 — OTAA Join Issue

I have been experimenting with **RadioLib 7.2.0** inside the **ESP-IDF 5.4.1** framework by porting the Arduino LoRa OTAA example to ESP-IDF.

Although the code compiles and runs, I am encountering the following error during the OTAA join process:

RADIOLIB_ERR_NO_JOIN_ACCEPT

At the same time, I also see this warning/error message:
gpio_install_isr_service(502): GPIO isr service already installed

It appears that the GPIO ISR service might be installed multiple times, which could be causing conflicts or be related to the join failure.

I am sharing this project to document my porting efforts and to seek advice or solutions regarding these issues.
