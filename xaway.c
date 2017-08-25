/*
 * Copyright (c) [2017] by Christophe Bacara <christophe.bacara@live.fr>
 *
 * This file is part of xaway.
 *
 * Some open source application is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Some open source application is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xaway. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <sys/stat.h>

#include <unistd.h>
#include <syslog.h>
#include <getopt.h>
#include <errno.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <X11/extensions/scrnsaver.h>

#define PROGRAM_NAME		"xaway"

#define MONTH_IN_SECONDS	2592000

/*
 * Allowed options
 */
#define OPTION_STRING "ht:i:"

/*
 * Long option strings
 */
static struct option long_options[] =
{
	{ "help",         no_argument,       NULL, 'h' },
	{ "timeout",      required_argument, NULL, 't' },
	{ "interval",     required_argument, NULL, 'i' },
};

void usage(int status)
{
  fprintf(stderr, "\
Usage: %s [OPTIONS] idle_script [active_script]\n\
Set up a daemon watching for X11 inactivity, executing commands when user is\n\
inactive/back.\n\
\n\
Parameter 'idle_script' is mandatory. This is the script to execute when idle\n\
timer occurs.\n\
\n\
Parameter 'active_script' is the script to execute when going out of idle.\n\
\n\
Options: \n\
  -h, --help          Print this help and exit.\n\
  -t, --timeout=N     Set idle timer to N (in seconds, default: 60).\n\
  -i, --interval=N    Set inactivity checks interval to N (in milliseconds,\n\
                      default: 500).\n\
", PROGRAM_NAME);

  exit(status);
}

static void loop(unsigned int timeout, unsigned int interval, char *idle_script, char *active_script)
{
	Display				*display	   = NULL;
	XScreenSaverInfo	*scrnsaverinfo = NULL;

	bool				 idle		   = false;

	/* Get display information */
	display = XOpenDisplay(NULL);

	if ( display == NULL )
	{
		syslog(LOG_ERR, "cannot open X display");
		return;
	}

	/* Allocate screensaving information struct */
	scrnsaverinfo = XScreenSaverAllocInfo();

	/* Main loop */
	while ( true )
	{
		/* Refresh screensaving information */
		XScreenSaverQueryInfo(display, DefaultRootWindow(display), scrnsaverinfo);

		if ( scrnsaverinfo->idle > timeout )
		{
			if ( !idle )
			{
				syslog(LOG_NOTICE, "idle mode set");
				idle = true;

				if ( system(idle_script) != 0 )
					syslog(LOG_ERR, "error while executing script");
			}
		}
		else if ( idle )
		{
			syslog(LOG_NOTICE, "active mode set");
			idle = false;

			if ( system(active_script) != 0 )
				syslog(LOG_ERR, "error while executing script");
		}

		usleep(interval);
	}
}

static void daemonize()
{
	pid_t	pid = 0;
	int		x   = 0;

	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);

	if (setsid() < 0)
		exit(EXIT_FAILURE);

	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid > 0)
		exit(EXIT_SUCCESS);

	umask(0);

	chdir("/");

	for ( x = sysconf(_SC_OPEN_MAX); x >= 0; --x )
		close(x);

	openlog("xaway", LOG_PID, LOG_DAEMON);
}

int main(int argc, char *argv[])
{
	unsigned int	 timeout		  = 60000;	/* One minute in milliseconds */
	unsigned int	 interval		  = 500000;	/* Half a second in microseconds */

	long int		 tmp			  = 0;

	int				 option_index	  = 0;

	char			 c				  = 0;

	char			*idle_script	  = NULL;
	char			*active_script	  = NULL;
	char			*endptr			  = NULL;

	/*
	 * Options processing
	 */
	while ( (c = getopt_long(argc, argv, OPTION_STRING, long_options, &option_index)) != -1 )
	{
		switch ( c )
		{
			/* flag options */
			case 0:
				break;

			/* help: -h, --help */
			case 'h':
				usage(EXIT_SUCCESS);
				break;

			/* timeout: -t, --timeout */
			case 't':
				tmp = strtol(optarg, &endptr, 10);
				if ( *endptr != '\0' || tmp < 0 || tmp > MONTH_IN_SECONDS )
				{
					fprintf(stderr, "%s: invalid value for timeout: %s\n", PROGRAM_NAME, optarg);
					exit(EXIT_FAILURE);
				}
				timeout = (unsigned int) tmp * 1000; /* seconds to milliseconds */
				break;

			/* interval: -i, --interval */
			case 'i':
				tmp = strtol(optarg, &endptr, 10);
				if ( *endptr != '\0' || tmp < 0 || tmp > MONTH_IN_SECONDS )
				{
					fprintf(stderr, "%s: invalid value for interval: %s\n", PROGRAM_NAME, optarg);
					exit(EXIT_FAILURE);
				}
				interval = (unsigned int) tmp * 1000; /* milliseconds to microseconds */
				break;

			/* unknown option */
			case '?':
				usage(EXIT_FAILURE);
				break;

			/* program shall definitely NOT come here */
			default:
				fprintf(stderr, "An option ended the world: %c (integer: %i)\n", c, (int) c);
				abort();
		}
	}

	/* No argument left: usage ! */
	if ( optind == argc )
		usage(EXIT_FAILURE);

	/* Check path to idle script */
	idle_script = argv[optind++];
	if ( access(idle_script, F_OK) == -1 )
	{
		fprintf(stderr, "%s: no such file\n", idle_script);
		exit(EXIT_FAILURE);
	}

	/* Check path to active script if needed */
	if ( optind < argc )
	{
		active_script = argv[optind];
		if ( access(active_script, F_OK) == -1 )
		{
			fprintf(stderr, "%s: no such file\n", active_script);
			exit(EXIT_FAILURE);
		}
	}

	/* Daemonize */
	daemonize();

	/* Loop */
	syslog(LOG_NOTICE, "daemon started");
	loop(timeout, interval, idle_script, active_script);
	syslog(LOG_NOTICE, "daemon stopped");

	return 0;
}
