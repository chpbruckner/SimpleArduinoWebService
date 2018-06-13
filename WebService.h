#ifndef _SERVER_H_
#define _SERVER_H_

#include "WebResource.h"

enum WebMethod { HEAD, GET, POST, PUT, DELETE, UNKNOWN};

String getRequestLine(EthernetClient& client)
{
	String ret = "";
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
				Serial.println("Oversized request");
				break;	
			}
			
		}
	}
	c_str[i] = '\0';
	ret += c_str;
	return  ret;
}	

String getResourcePath(String requestLine)
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

WebMethod getWebMethod(String& requestLine)
{
  String verbStr = requestLine.substring(0, 3);
  if (verbStr == "GET" ) return GET;  
  if (verbStr == "POST") return POST; 
  if (verbStr == "PUT" ) return PUT;  
  if (verbStr == "HEAD") return DELETE;  
  return UNKNOWN;  
}

void answerClient(EthernetClient& client, WebResource*(*resourceFinder)(String&))
{
	Serial.println("New client");
	
	String requestLine = getRequestLine(client);
	Serial.print("Request: ");
	Serial.println(requestLine);
		
	WebMethod WebMethod = getWebMethod(requestLine);
	
	String resourcePath = getResourcePath(requestLine);
	Serial.print("Resource: ");
	Serial.println(resourcePath);
	
	WebResource* resource = resourceFinder(resourcePath);
	
	//FINISH READING FROM CLIENT: Header, browser and other useful info should be extracted from here:
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
			if (c == '\n' && b=='\n' || b=='\r') //Web request ends with a new newline alone
			{
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

#endif
