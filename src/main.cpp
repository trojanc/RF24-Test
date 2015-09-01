
#include <Arduino.h>
#include <SPI.h>

// Radio dependencies
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#include "printf.h"

RF24 radio(9, 10);
RF24Network network(radio);

// ID of the node, swop the below lines for your 2 nodes
#define NODE_ID 00
#define PEER_ID 01

// Amount of miliseconds between messages
#define MESSAGE_TIMEOUT 100

// Milliseconds to wait before printing our the statistics
#define STATISTICS_TIMEOUT 1000

// ID we use for our messages
#define MESSAGE_ID 1

// Time we last sent a message
unsigned long msg_sent_t = 0;

// Time we last received a message
unsigned long msg_recv_t = 0;

// Number of messages we sent
unsigned long msgs_sent = 0;

// Number of messages we received
unsigned long msgs_received = 0;

// Number of messages we received
unsigned long msgs_lost = 0;

// Time we last update our stats
unsigned long stat_update_t = 0;

void setup(void){
	Serial.begin(115200);
	printf_begin();

	Serial.print("Starting! ID:"); Serial.println(NODE_ID);

	radio.begin();
	radio.printDetails();
	network.begin(/*channel*/ 90, /*node address*/NODE_ID);
	radio.setPALevel(RF24_PA_MIN);
	radio.setDataRate(RF24_250KBPS);
}

/**
 * Send a message to the other node
 */
void send_message(void){

	RF24NetworkHeader message_header(/*to node*/ PEER_ID, /*type*/ MESSAGE_ID);
	msgs_sent++;
	network.write(message_header, &msgs_sent, sizeof(msgs_sent));
}

/**
 * Read any incomming messages
 */
void read_message(void){
	// Check if there is any messages available
	if(network.available()){

		RF24NetworkHeader header;
		network.peek(header);

		Serial.println("Got message");
		// The message type is our message ID
		if(header.type == MESSAGE_ID){
			unsigned long message;
			network.read(header, &message, sizeof(message));
			msgs_received++;
		}
		// This is not a known message
		else{
			// Just read the header off the buffer
			network.read(header,NULL, 0);
			Serial.println("Got unknown message");
		}
		msg_recv_t = millis();
	}
}

/**
 * Prints the current statistics of our test
 */
void print_statistics(void){
	Serial.print("Sent:");Serial.print(msgs_sent);
	Serial.print(" Received:");Serial.print(msgs_received);
	Serial.print(" Lost:");Serial.println(msgs_lost);
}

void loop(void){
	network.update();			// Check the network regularly
	read_message();

	// Check if it's time to send the next message
	if(msg_sent_t + MESSAGE_TIMEOUT <= millis()){
		msg_sent_t = millis();
		send_message();
	}

	// Check if we have failed to receive a message in time, give a extra 25ms
	if(msg_recv_t + MESSAGE_TIMEOUT + 25 <= millis()){
		msg_recv_t = millis();
		msgs_lost++;
	}

	// Check if we need to update our statistics
	if(stat_update_t + STATISTICS_TIMEOUT <= millis()){
		stat_update_t = millis();
		print_statistics();
	}
} // loop(void)
