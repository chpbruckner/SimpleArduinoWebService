#ifndef _WEBSERVICE_H_
#define _WEBSERVICe_H_

#include "WebResource.h"

enum WebMethod { HEAD, GET, POST, PUT, DELETE, UNKNOWN };

class WebService
{
	private: 
		String requestLine;
		WebResource* (*resourceFinder)(String&);
	public:
		//Constructor argument: A function that takes a String URI and returns a pointer to WebResource.
		WebService(WebResource*(*resourceFinderFunction)(String&)); 
		
		//Function to do all the communication at once. It actually calls setRequest and then answerClient.
		void communicateWithClient(EthernetClient& client);

		bool setRequest(EthernetClient&); // Reads request and saves requestLine.
		void answerClient(EthernetClient& client); // Calls requested method on WebResource gotten from the resourceFinder function.
		String getRequestLine();
		String getResourcePath();
		WebMethod getWebMethod();
};

#endif

