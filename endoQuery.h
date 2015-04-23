/*****************************************

File: endoQuery.h
Author: Steve Roys
        3/8/2013

This module implements the class used to interface with a Robin Medical
Endoscout and provide position information.  It will connect to the 
Endoscout on a given IP address and port, and will query and return
the position string from the EndoScout when requested.

*****************************************/
#ifndef ENDOQUERY_H
#define ENDOQUERY_H

class endoQuery {
  // Private variables:
  int verbose;               // - Verbose flag for debugging
  int endoSock;              // - Socket for connection to endoscout
 public:
  // Public methods:
  ~endoQuery();
  endoQuery();
  int endoConnect(const char *ipAddress, const int ipPort);
  int endoDisconnect();
  void setVerbose(int verbose);
  char *getEndoPos();
};

#endif
