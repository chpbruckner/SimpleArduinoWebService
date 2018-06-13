#include "WebResource.h"

String WebResource::getNodeId(String& resourcePath)
{
	int slash_position = resourcePath.indexOf('/', 1);
	if(slash_position != -1)   
		return resourcePath.substring(0, slash_position);
	else 
		return resourcePath;
}
	
String WebResource::getNextPath(String& resourcePath)
{
	int slash_position = resourcePath.indexOf('/', 1);
	if(slash_position != -1)   
		return resourcePath.substring(slash_position);
	else 
		return NULL;
}
		   
void WebResource::respondWithCode(EthernetClient& client, int code)
{
	if (code == 200)
	{
		client.println(F("Web/1.1 200 OK"));
		client.println(F("Connection: close"));  // the connection will be closed after completion of the response
		client.println();
	}
	else if (code == 400)
	{
		client.println(F("Web/1.1 400 Bad request"));  
		client.println(F("Content-Type: text/plain"));
		client.println(F("Connection: close"));  // the connection will be closed after completion of the response
		client.println();
		client.println(F("400\nServer: I could not understand your request."));  
		client.println();
	}
	else if (code == 404)
	{
		client.println(F("Web/1.1 404 Not Found"));  
		client.println(F("Content-Type: text/plain"));
		client.println(F("Connection: close"));  // the connection will be closed after completion of the response
		client.println();
		client.println(F("404\nServer: I could not find the resource you are looking for."));  
		client.println();
	}
	else if (code == 405)
	{
		client.println(F("Web/1.1 405 Method not allowed."));  
		client.println(F("Content-Type: text/plain"));
		client.println(F("Connection: close"));  // the connection will be closed after completion of the response
		client.println();
		client.println(F("405\nServer: I can't perform the requested action for the specified resource."));  
		client.println();
	}
	else
	{
		client.print(F("Web/1.1 "));
		client.print(code);
		client.println(F(" Unknown"));  
		client.println(F("Content-Type: text/plain"));
		client.println(F("Connection: close"));  // the connection will be closed after completion of the response
		client.println();
		client.print(code);
		client.println(F("nServer: I've got this code but I don't know what it means."));  
		client.println();
	}
}
	
void WebResource::GET(EthernetClient& client) //The default is 404 because you're supposed to extend this class and implement a GET function to get the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::PUT(EthernetClient& client) //The default is 404 because you're supposed to extend this class and implement a PUT function to get the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::POST(EthernetClient& client) //The default is 404 because you're supposed to extend this class and implement a GET function to get the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::DELETE(EthernetClient& client) //The default is 404 because you're supposed to extend this class and implement a GET function to get the real stuff
{
	respondWithCode(client, 404);
}

