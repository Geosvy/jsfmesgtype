/*************************************
 *
 *
 *	jsfmesgtype
 *
 *	List the ET jsf file message types and number of occurances
 *	Reflects jsf format 0004824_REV_1.18
 *
 *	Tom O'Brien
 *	USGS
 *	Woods Hole, MA
 *
 *	September 11, 2017
 *
 *	tobrien@usgs.gov
 *	tforideshd@gmail.com
 *
 *************************************/

#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "jsf2.h"

int
main (int argc, char *argv[])
{				/* START MAIN */

  int in_fd;			/* file descriptor */
  int JSFmsgSize;
  int inbytes;
  int msg;
  int Sonar = 0;
  int Sidescan = 0;
  int Pitch = 0;
  int NMEA = 0;
  int Press = 0;
  int Analog = 0;
  int Doppler = 0;
  int Situation = 0;
  int SitII = 0;
  int SAS = 0;
  int S_info = 0;
  int Cable = 0;

  off_t offset;			/* offset for seeking starting record */
  off_t where;

  unsigned char *JSFmsg;

  JSFmsgSize = 16;

  char *progname;

  progname = argv[0];

  if ((argc - optind) < 1)
    usage ();

  /*
   * open the input jsf file
   */

  if ((in_fd = open (argv[optind], O_RDONLY)) == -1)
    {
      fprintf (stderr, "%s: cannot open %s\n", argv[optind], progname);
      perror ("open");
      err_exit ();
    }

  /*
   *      Get some memory for JSF header
   */

  JSFmsg =
    (unsigned char *) calloc ((size_t) JSFmsgSize, sizeof (unsigned char));
  if (JSFmsg == 0)
    fprintf (stdout, "Error callocing\n");

  while (1)
    {
      inbytes = read (in_fd, JSFmsg, (size_t) JSFmsgSize);
      if (inbytes == ZERO)
	{
	  fprintf (stdout, "End of File reached in input file\n");
	  if (Sonar)
	    fprintf (stdout, "Sonar = %d ", Sonar);
	  if (Sidescan)
	    fprintf (stdout, " Sidescan = %d ", Sidescan);
	  if (Pitch)
	    fprintf (stdout, " Pitch = %d ", Pitch);
	  if (NMEA)
	    fprintf (stdout, " NMEA = %d ", NMEA);
	  if (Press)
	    fprintf (stdout, " Pressure = %d ", Press);
	  if (Analog)
	    fprintf (stdout, " Analog = %d ", Analog);
	  if (Doppler)
	    fprintf (stdout, " Doppler = %d ", Doppler);
	  if (Situation)
	    fprintf (stdout, " Situation = %d ", Situation);
	  if (SAS)
	    fprintf (stdout, " SAS = %d ", SAS);
	  if (S_info)
	    fprintf (stdout, " S_info = %d ", S_info);
	  if (Cable)
	    fprintf (stdout, " Cable = %d ", Cable);
	  if (SitII)
	    fprintf (stdout, " DII Situation = %d ", SitII);

	  fprintf (stdout, "\n");
	  exit (EXIT_SUCCESS);
	} // End if

      if (inbytes != (int) JSFmsgSize)
	{
	  fprintf (stderr, "%s: error reading JSF message header\n",
		   progname);
	  perror ("read");
	  err_exit ();
	}

      if (get_short (JSFmsg, 0) != 0x1601)
	{
	  fprintf (stdout, "Invalid file format\n");
	  err_exit ();
	}

      msg = (int) get_short (JSFmsg, 4);
      switch (msg)
	{
	case 80:
	  Sonar++;
	  break;
	case 82:
	  Sidescan++;
	  break;
	case 2020:
	  Pitch++;
	  break;
	case 2002:
	  NMEA++;
	  break;
	case 2060:
	  Press++;
	  break;
	case 2040:
	  Analog++;
	  break;
	case 2080:
	  Doppler++;
	  break;
	case 2091:
	  Situation++;
	  break;
	case 86:
	  SAS++;
	  break;
	case 182:
	  S_info++;
	  break;
	case 2100:
	  Cable++;
	  break;
	case 9002:
	  SitII++;
	  break;
	case '?':
	  err_exit ();
	  break;
	} // End switch

      /*
       *  Skip to next JSF Headeer
       */

      offset = (off_t) get_int (JSFmsg, 12);
      where = lseek (in_fd, offset, SEEK_CUR);
    }   // End while()
}	// End Main()

void
err_exit (void)
{
  fprintf (stdout, "An error occured \n");
  exit (EXIT_FAILURE);
}

void
usage (void)
{

  fprintf (stdout,
	   "jsfmesgtype  ... Lists Edgetech JSF message types within a file\n");
  fprintf (stdout, "Usage:    jsfmesgtype  full path to input file name\n");
  exit (EXIT_FAILURE);
}
