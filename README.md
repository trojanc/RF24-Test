# RF24-Test
A simple Arduino project that sends a receives messages from one radio node to another, and indicates how many packets are lost in the process

It does so by sending packets at a fixed rate.
Each node knows how many packets it should expect in a time window.
If the number of expected packets are not received, they are assumed to have been lost.

## Compiling the code for your 2 nodes
You have to swop the NODE\_ID and PEER\_ID for each node so that they have each other's IDs
You might also want to look at the platformio.ini and update the settings to match your devices. (I swop the 2 settings in the ini file between my uno and nano for my two testing nodes)

## Prerequisites
You need to have platformio installed to run this code (If you feel like it, you can adapt it to run in the usual Arduino IDE)
Initialise the project with
```platformio init```

## Running the code
```platformio run```
