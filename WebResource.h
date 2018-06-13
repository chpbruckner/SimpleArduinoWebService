#ifndef _WEBRESOURCE_H_
#define _WEBRESOURCE_H_

#include <SPI.h>
#include <Ethernet.h>

class WebResource
{
	public:

		static String getNodeId(String& resourcePath);
        	static String getNextPath(String& resourcePath);
		void respondWithCode(EthernetClient& client, int code);

		// The default response for the following methods is 404 because you're supposed to 
		// extend this class and implement the functions below to do the the real logic
		virtual void GET(EthernetClient& client); 
		virtual void POST(EthernetClient& client); 
		virtual void PUT(EthernetClient& client); 
		virtual void DELETE(EthernetClient& client); 
};
    
#endif
