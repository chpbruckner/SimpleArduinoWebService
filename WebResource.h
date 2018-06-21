#ifndef _WEBRESOURCE_H_
#define _WEBRESOURCE_H_

#include <SPI.h>
#include <Ethernet.h>

class WebResource
{
  private:
	String headers = "Connection: close\n";
	bool headersSet = false;

  public:
	void setHeader(String header, String value); // Set HTTP response header

	// Useful helper functions to create a hierarchical path of resources:
	static String getNodeId(String &resourcePath);   // Use this to get the first thing between '/'s from the URI;
	static String getNextPath(String &resourcePath); // Use this to get the rest of the URI after the former.
	/* -- */

	// You should always call this function with the corresponding HTTP response code before sending your reply to the client;
	void respondWithCode(EthernetClient &client, unsigned short code);

	// The default response for the following methods is 404 because you're supposed to
	// extend this class and implement the functions below to do the the real logic
	virtual void GET(EthernetClient &client);
	virtual void POST(EthernetClient &client);
	virtual void PUT(EthernetClient &client);
	virtual void DELETE(EthernetClient &client);

	static WebResource *notFoundResource(); // returns a plain WebResource object (only answers 404 NotFound.)
};

#endif
