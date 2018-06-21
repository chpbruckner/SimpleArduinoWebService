#include "WebResource.h"

String WebResource::getNodeId(String &resourcePath)
{
	int slash_position = resourcePath.indexOf('/', 1);
	if (slash_position != -1)
		return resourcePath.substring(0, slash_position);
	else
		return resourcePath;
}

String WebResource::getNextPath(String &resourcePath)
{
	int slash_position = resourcePath.indexOf('/', 1);
	if (slash_position != -1)
		return resourcePath.substring(slash_position);
	else
		return NULL;
}

void WebResource::setHeader(String header, String value)
{
	headersSet = true;
	headers += header + ": " + value + "\n";
}

void WebResource::respondWithCode(EthernetClient &client, unsigned short code)
{
	Serial.print("Response: ");
	Serial.println(code);
	if (code == 200)
	{
		client.println(F("HTTP/1.1 200 OK"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("200\nServer: Your request was succesfully carried out but no Content-type was set for the response."));
		}
		client.println();
	}
	else if (code == 201)
	{
		client.println(F("HTTP/1.1 201 Created"));
		client.println(headers); //The Location header should contain the URI to the new resource
		if (!headersSet)		 // this implies there is no custom message.
		{
			client.println();
			client.println(F("201\nServer: The requested resource was created."));
		}
		client.println();
	}
	else if (code == 204)
	{
		client.println(F("HTTP/1.1 204 No Content"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("204\nServer: The request was accepted and there is no content to send back to you."));
		}
		client.println();
	}
	else if (code == 205)
	{
		client.println(F("HTTP/1.1 205 Reset content"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("205\nServer: The request was accepted and you should reset your content."));
		}
		client.println();
	}
	else if (code == 400)
	{
		client.println(F("HTTP/1.1 400 Bad request"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("400\nServer: I could not understand your request."));
		}
		client.println();
	}
	else if (code == 403)
	{
		client.println(F("HTTP/1.1 403 Forbidden"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("403\nServer: I cannot allow you to perform the current request."));
		}
		client.println();
	}
	else if (code == 404)
	{
		client.println(F("HTTP/1.1 404 Not Found"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("404\nServer: I could not find the resource you are looking for."));
		}
		client.println();
	}
	else if (code == 405)
	{
		client.println(F("HTTP/1.1 405 Method not allowed."));
		client.println(headers);
		if (!headersSet)
		{
			client.println();
			client.println(F("405\nServer: I can't perform the requested action for the specified resource."));
		}
		client.println();
	}
	else if (code == 406)
	{
		client.println(F("HTTP/1.1 406 Not Acceptable"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("406\nServer: I do not accept the content format you are asking for."));
		}
		client.println();
	}
	else if (code == 409)
	{
		client.println(F("HTTP/1.1 409 Conflict"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("409\nServer: Performing the requested action would break the system state."));
		}
		client.println();
	}
	else if (code == 500)
	{
		client.println(F("HTTP/1.1 500 Internal Server Error"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("500\nServer: There's something wrong with me and I couldn't fulfill your request."));
		}
		client.println();
	}
	else if (code == 503)
	{
		client.println(F("HTTP/1.1 503 Service Unavailable"));
		client.println(headers);
		if (!headersSet) // this implies there is no custom message.
		{
			client.println();
			client.println(F("503\nServer: I cannot fulfill your request right now."));
		}
		client.println();
	}
	else
	{
		client.print(F("HTTP/1.1 "));
		client.print(code);
		client.println(F(" Unknown"));
		client.println(headers);
		if (!headersSet)
		{
			client.println();
			client.print(code);
			client.println(F("\nServer: I've got this code but I don't know what it means."));
		}
		client.println();
	}
}

void WebResource::GET(EthernetClient &client) //The default is 404 because you're supposed to extend this class and implement a GET function to get the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::PUT(EthernetClient &client) //The default is 404 because you're supposed to extend this class and implement a PUT function for the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::POST(EthernetClient &client) //The default is 404 because you're supposed to extend this class and implement a POST function for the real stuff
{
	respondWithCode(client, 404);
}

void WebResource::DELETE(EthernetClient &client) //The default is 404 because you're supposed to extend this class and implement a DELETE function for the real stuff
{
	respondWithCode(client, 404);
}

WebResource *WebResource::notFoundResource()
{
	return new WebResource();
}
