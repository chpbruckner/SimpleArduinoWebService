#include "WebService.h"

WebService::WebService(WebResource*(*resourceFinderFunction)(String&))
{
	requestLine = "";
	resourceFinder = resourceFinderFunction;
}

String WebService::getRequestLine()
{
	return requestLine;
}

bool WebService::setRequest(EthernetClient& client)
{
	Serial.println("New client");

	char c_str[256];
	
	int c = '\0';
	int i = 0;
	
	while (client.connected()) 
	{
		if (client.available()) 
		{
			c = client.read();
			c_str[i] = c;  
			i++;
			if(c == '\n') break;
			if(i > 255) // request 255 chars max
			{
				Serial.println("Oversized URI");
				client.stop();
				return false;	
			}
			
		}
	}
	if(i==0) return false;

	c_str[i] = '\0';
	requestLine = c_str;

	Serial.print("Request: ");
	Serial.print(requestLine);

	return true;
}	

String WebService::getResourcePath()
{
	if(requestLine.indexOf('?') != -1) // in case there are parameters in the URI
	{
		return requestLine.substring(requestLine.indexOf('/'), requestLine.indexOf('?'));   
	}
	else
	{
		int start_index = requestLine.indexOf('/');
		return requestLine.substring(start_index, requestLine.indexOf(' ', start_index)); 
	}  
}

WebMethod WebService::getWebMethod()
{
	String verbStr = requestLine.substring(0, 4);
	if (verbStr == "GET ") return GET;  
	if (verbStr == "POST") return POST; 
	if (verbStr == "PUT ") return PUT;  
	if (verbStr == "DELE") return DELETE;  
	if (verbStr == "OPTI") return OPTIONS; 
	return UNKNOWN;  
}

void WebService::answerClient(EthernetClient& client)
{
	WebMethod WebMethod = getWebMethod();
	
	String resourcePath = getResourcePath();
	Serial.print("Resource: ");
	Serial.println(resourcePath);
	
	// Header, browser data and other useful info should be extracted from here.

	WebResource* resource = resourceFinder(resourcePath);
	
	//FINISH READING ANYTHING LEFT FROM CLIENT: 
	char c = '\n';
	char b;
	int request_size = 0;
	while (client.connected()) 
	{ 
		if (client.available()) 
		{
			b = c;
			c = client.read();
			request_size++;
			Serial.write(c);
			if ( (c == '\n' || c=='\r') && b=='\n') //Web request ends with a new newline alone
			{
				if (c=='\r' && client.available() > 1) client.read(); // last \n before payload
				break;	
			}
			if(request_size > 1024)
			{
				Serial.println("\nOversized request (>1KB)");
				break;   
			}
		}
	}
	/* --- */ 

	//READING IS DONE, START RESPONSE:
	while (client.connected()) 
	{
		if (client.available()) 
		{
			if(WebMethod == GET)
			{
				resource->GET(client);
			}
			else if(WebMethod == PUT)
			{
				resource->PUT(client);
			}
			else if(WebMethod == POST)
			{
				resource->POST(client);
			}
			else if(WebMethod == DELETE)
			{
				resource->DELETE(client);
			}
			else if(WebMethod == OPTIONS)
			{
				resource->OPTIONS(client);
			}
			else
			{
				resource->respondWithCode(client, 400);
			}

			// give the web browser time to receive the data
			delay(1); // TODO: Better way?
			client.stop();
			Serial.println("Client disonnected");
		}
	}
	delete resource;
	/* --- */
}

void WebService::communicateWithClient(EthernetClient& client)
{
	if(setRequest(client)) // See if we can read client's request.
	{
		answerClient(client); // Answer
	}
	else
	{
		Serial.println("Unable to communicate with client");
	}				
}

