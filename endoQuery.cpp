#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "endoQuery.h"

#define ENDO_VECT_REQUEST "DATA 1"
#define ENDO_BAD_REQUEST "NADA_NADA"
#define ENDO_QUATERNION_REQUEST "\r"

// Destructor to close any communications nicely...
endoQuery::~endoQuery()
{
  endoDisconnect();
}

// Default constructor to initalize some things...
endoQuery::endoQuery()
{
  // Initialize some variables...
  verbose = 0;
  endoSock = -1;
}

// Attempt to open a TCP connection to an endoscout...
//   Returns 0|-1.. = Connected|Not connected
int endoQuery::endoConnect(const char *endoIP, const int endoPort) {
  char buffer[512];
  struct sockaddr_in servAddr;
  // Create a TCP socket...
  endoSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (endoSock < 0) {
    if (verbose) std::cout << "endoQuery::endoConnect - 1/4 ERROR creating TCP socket\n";
    return(-1);
  }
  // Set up to timeout after 5 seconds on a 'recv'...
  struct timeval send_timeout = {5, 0};
  setsockopt (endoSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&send_timeout, sizeof (struct timeval));
  // Construct the server address structure...
  bzero((char *) &servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(endoIP);
  servAddr.sin_port = htons(endoPort);
  // Attempt to connect to the endoscout...
  if (connect(endoSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    if (verbose) std::cout << "endoQuery::endoConnect - 2/4 ERROR connecting to Endoscout" << std::endl;
    close(endoSock);
    endoSock = -1;
    return(-2);
  }
  // Send a request and see if we get the response we're expecting...
  send(endoSock, ENDO_VECT_REQUEST, strlen(ENDO_VECT_REQUEST), 0);
  bzero(buffer, sizeof(buffer));
  int bytesRcvd = recv(endoSock, buffer, 512, 0);
  if (bytesRcvd == -1) {
    if (verbose) std::cout << "endoQuery::endoConnect - 3/4 Initial Endoscout response timeout.\n";
    close(endoSock);
    endoSock = -1;
    return(-3);
  }
  if (strncmp("#Server Ready", buffer, 13) != 0) {
    if (verbose) {
      std::cout << "endoQuery::endoConnect - 4/4 Invalid initial response from Endoscout:\n";
      std::cout << "endoQuery::endoConnect -     ???:" << buffer << std::endl;
    }
    close(endoSock);
    endoSock = -1;
    return(-4);
  }
  // Success!
  if (verbose) std::cout << "endoQuery:endoConnect - Endoscout connected and responding.\n";
  return(0);
}

// Disconnect from the Endoscout if we're connected...
int endoQuery::endoDisconnect()
{
  if (endoSock != -1) {
    close(endoSock);
    endoSock = -1;
    if (verbose) {
      std::cout << "endoQuery: Endoscout disconnected.\n";
    }
  }
  return(0);
}

// Set verbosity...
void endoQuery::setVerbose(int lverbose)
{
  verbose = lverbose;
}

// Query the Endoscout and return the position string...
char *endoQuery::getEndoPos()
{
  char *p_buffer;
  ssize_t nBytes;
  if (endoSock == -1) return(NULL);
  // Send a query request to the endoscout...
  nBytes = send(endoSock, ENDO_VECT_REQUEST, strlen(ENDO_VECT_REQUEST), 0);
  if (nBytes == -1) {
    std::cout << "endoQuery::getEndoPos - Endoscout send error: " << errno << "\n";
    return(NULL);
  }
  // Read the response from the endoscout...
  p_buffer = new char[512];
  memset(p_buffer, 0, 512);
  int bytesRcvd = recv(endoSock, p_buffer, 512, 0);
  if (bytesRcvd == -1) {
    std::cout << "endoQuery::getEndoPos - Recv error: " << errno << "\n";
    delete(p_buffer);
    return(NULL);
  }
  if (verbose == 2) {
    std::cout << "Endoscout response " <<  
      bytesRcvd << " bytes = -" << p_buffer << "-\n";
  }
  return(p_buffer);
}
