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
		bool setRequest(EthernetClient&);
		String getRequestLine();
		String getResourcePath();
		WebMethod getWebMethod();
		void answerClient(EthernetClient& client);
};

#endif

