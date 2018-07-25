#include <SPI.h>
#include <Ethernet.h>
#include "WebService.h"


// Create an EthernetServer like you would normally do.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 11);
EthernetServer server(80);

// Suppose we want to represent an apple tree.
String apples[5];
int num_apples = 0;

// Extend the class WebResource in one or more classes to represent your resources
class AppleTreeResources : public WebResource
{
	void GET(EthernetClient& client)
	{
		setHeader("Content-type", "text/html"); // Define the Content-Type http header to determine the content format.
		respondWithCode(client, 200); 		// Start the HTTP response with its code before sending other stuff to the client
		client.println("<html><head></head><body>This is an apple tree.<br>It has ");
		client.print(num_apples);
		client.println(" apples.<br>");
		for(int i=0; i<num_apples; i++)
		{
			client.print(apples[i]);
			client.println(" apple.<br>");	
		}
		client.println("<form action='/tree' method='POST'>Color:<input type='text' name='color'>");
		client.println("<input type='submit' value='add apple'></form></body></html>");
	}
	void POST(EthernetClient& client)
	{
		// convert POST data into a String
		Serial.println("POST Content:");
		char c_str[256];
		int i=0;
		while(client.available())
		{
			c_str[i] = client.read();
			Serial.write(c_str[i]);
			i++;
		}
		c_str[i] = '\0';  
		Serial.println(); 
		String post_data = c_str;
		// --- 

		int equals_position = post_data.indexOf('=');
		if(equals_position == -1)
		{
			respondWithCode(client, 400); // Bad request: POST payload is not in the format 'color=red'
			return;
		}
		String color = post_data.substring(equals_position+1); // Value of the 'color' attribute sent with the form

		if(num_apples < 5)
		{
			apples[num_apples] = color;
			num_apples++;
			setHeader("Content-type", "text/html");
			respondWithCode(client, 200); 
			client.println();
			client.println(" Apple created");
		}
		else
		{
			setHeader("Content-type", "text/html");
			respondWithCode(client, 500); 
			client.println(" Too many apples.");
		}
	}
	void PUT(EthernetClient& client)
	{
		respondWithCode(client, 405); // Method not allowed
	}
	void DELETE(EthernetClient& client)
	{
		respondWithCode(client, 405); // Method not allowed
	}
};

// Create a function that takes a URI String and returns a WebResource pointer. 
// Check WebResource::getNodeId and WebResource::getNextPath for useful methods for splitting your URI.
WebResource* URIParser(String& URI)
{    
	if (URI == "/tree")
		return new AppleTreeResources();
	else
		return WebResource::notFoundResource();
}

// Then we can create our service passing our parser function to it.
WebService service(URIParser);

void setup() 
{
	Serial.begin(9600);
	while (!Serial) ; // Needed for Leonardo only

	// Start the Ethernet connection and the server:
	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
}


void loop() 
{
	// Listen for incoming clients
	EthernetClient client = server.available();
	if (client) service.communicateWithClient(client);

	// ...do other stuff...
}
