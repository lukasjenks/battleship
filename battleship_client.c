#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>

void *getXAndY(char *bbuffer, char *xval, char *yval) {
  int offset = 8 - strlen(xval);
  int i = 0;
  for (i = 0; i < offset; i++) {
    strcat(bbuffer, "0");
  }
  strcat(bbuffer, xval);
  offset = 8 - strlen(yval);
  i = 0;
  for (i = 0; i < offset; i++) {
    strcat(bbuffer, "0");
  }
  strcat(bbuffer, yval);
  return bbuffer;
}

int main(int argc, char **argv)
{
  // Values used for parsing and interpreting cmdline args
  int iflag = 0;
  int jflag = 0;
  int bflag = 0;
  int xflag = 0;
  int yflag = 0;
  int o;
  int cli;
  int testval;
  int size;
  char path[19] = "./cli_socket_";	//unique client identifier will be
  //concatenated to this value
  char *identifier = NULL;
  char *xval = NULL;
  char *yval = NULL;
  char bbuffer[21];	//4 byte JOIN/BOMB/STAT + 8 byte number + 8 byte number
  // + null terminator
  struct sockaddr_un srvaddr;
  struct sockaddr_un cliaddr;

  // ------------------------------------------------------------------------
  // PARSE CMD LINE ARGS
  // ------------------------------------------------------------------------

  // quick error check to see if call is within expected number of arguments
  // to make the getopt command arg parsing easier
  if (!(argc >= 3)) {
    fprintf(stderr,
	    "Error: too few arguments given to num_client\n");
    return 1;
  } else if (!(argc <= 8)) {
    fprintf(stderr,
	    "Error: too many arguments given to num_client\n");
    return 1;
  }
  // parse command line options
  while ((o = getopt(argc, argv, "i:jbx:y:")) != -1) {

    switch (o) {
    case 'i':
      iflag = 1;
      identifier = optarg;
      break;

    case 'j':
      jflag = 1;
      break;

    case 'b':
      bflag = 1;
      break;

    case 'x':
      xflag = 1;
      xval = optarg;
      break;

    case 'y':
      yflag = 1;
      yval = optarg;
      break;

      // Stretch goal: Add s flag and optarg to specify server path

    case '?':
      if (optopt == 'o')
	fprintf(stderr, "-%c needs an argument\n",
		optopt);
      else if (isprint(optopt))
	fprintf(stderr, "Invalid option: -%c\n",
		optopt);
      else
	fprintf(stderr,
		"Error parsing cmdline arguments given\n");
	    return 1;
	  }
	}			// end of while loop to parse cmdline args

	if (!iflag) {
	  fprintf(stderr,
		  "Error: must specify your unique identifier with -i flag and \
		  optarg\n");
	} else if (jflag && bflag) {
	  fprintf(stderr,
		  "Error: cannot use both -j and -b simultaneously\n");
	  return 1;
	} else if (bflag && (!xflag || !yflag)) {
	  fprintf(stderr,
		  "Error: cannot use -b without specifying both the -x and -y \
		  argument and their optargs\n");
	}
	//command line arguments are now parsed and invalid usage has
	//been handled.

	// ------------------------------------------------------------------------
	// SOCKET FUNCTIONALITY
	// ------------------------------------------------------------------------

	// Initialize client socket and setup struct

	cli = socket(AF_UNIX, SOCK_STREAM, 0);
	cliaddr.sun_family = AF_UNIX;
	strcpy(cliaddr.sun_path, strcat(path, identifier));
	size = sizeof(cliaddr);

	// Bind client socket to ./cli_socket
	unlink(cliaddr.sun_path);
	testval = bind(cli, (struct sockaddr *)&cliaddr, size);
	if (testval == -1) {
	  fprintf(stderr, "Error binding client socket\n");
	  close(cli);
	  return 1;
	}
	// Setup srvaddr struct
	srvaddr.sun_family = AF_UNIX;
	strcpy(srvaddr.sun_path, "./srv_socket");

	// Connect to server socket
	testval = connect(cli, (struct sockaddr *)&srvaddr, size);
	if (testval == -1) {
	  fprintf(stderr, "Error connecting to server socket\n");
	  close(cli);
	  return 1;
	}
	// Send data to server
	if (jflag == 1) {
	  strcpy(bbuffer, "JOIN");

	  // Check if optional x and y solution for joining player was specified
	  if (xval != NULL && yval != NULL) {
	    getXAndY(bbuffer, xval, yval);

	    // If optional x and y solution for joining player wasn't specified, set to 0.
	    // server will then assign player a random solution.
	  } else {
	    int i = 0;
	    for (i = 0; i < 16; i++) {
	      strcat(bbuffer, "0");
	    }
	  }

	} else if (bflag == 1) {
	  getXAndY(bbuffer, xval, yval);
	}
	// Send request to server
	testval = send(cli, bbuffer, strlen(bbuffer), 0);
	if (testval == -1) {
	  fprintf(stderr, "Error sending request to server socket\n");
	  close(cli);
	  return 1;
	}
	// Finished interacting with server; close client socket
	close(cli);

	return 0;
}
