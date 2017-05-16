
/*
 * Copyright (c) 2014, Alper Akcan <alper.akcan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <Alper Akcan> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define MBUS_DEBUG_NAME	"app-event"

#include "mbus/debug.h"
#include "mbus/json.h"
#include "mbus/method.h"
#include "mbus/client.h"
#include "mbus/server.h"
#include "event.h"

#define OPTION_HELP		0x100
#define OPTION_DESTINATION	0x101
#define OPTION_EVENT		0x102
#define OPTION_PAYLOAD		0x103
static struct option longopts[] = {
	{ "help",			no_argument,		NULL,	OPTION_HELP },
	{ "destination",		required_argument,	NULL,	OPTION_DESTINATION },
	{ "event",			required_argument,	NULL,	OPTION_EVENT },
	{ "payload",			required_argument,	NULL,	OPTION_PAYLOAD },
	{ NULL,				0,			NULL,	0 },
};

static void usage (void)
{
	fprintf(stdout, "mbus event arguments:\n");
	fprintf(stdout, "  --destination            : destination identifier\n");
	fprintf(stdout, "  --event                  : event identifier\n");
	fprintf(stdout, "  --payload                : payload json\n");
	fprintf(stdout, "  --help                   : this text\n");
	fprintf(stdout, "  --mbus-help              : mbus help text\n");
}

struct arg {
	const char *destination;
	const char *event;
	struct mbus_json *payload;
	int finished;
	int result;
};

static void event_status_server_connected (struct mbus_client *client, const char *source, const char *status, struct mbus_json *payload, void *data)
{
	int rc;
	struct arg *arg = data;
	(void) source;
	(void) status;
	(void) payload;
	rc = mbus_client_event_to(client, arg->destination, arg->event, arg->payload);
	if (rc != 0) {
		mbus_errorf("can not call event");
	}
	arg->result = rc;
	arg->finished = 1;
}

int main (int argc, char *argv[])
{
	int ch;
	int rc;
	struct arg arg;
	struct mbus_client *client;
	client = NULL;
	memset(&arg, 0, sizeof(struct arg));
	client = mbus_client_create(MBUS_APP_EVENT_NAME, argc, argv);
	if (client == NULL) {
		mbus_errorf("can not create client");
		goto bail;
	}
	arg.destination = MBUS_METHOD_EVENT_DESTINATION_SUBSCRIBERS;
	optind = 1;
	while ((ch = getopt_long(argc, argv, ":", longopts, NULL)) != -1) {
		switch (ch) {
			case OPTION_DESTINATION:
				arg.destination = optarg;
				break;
			case OPTION_EVENT:
				arg.event = optarg;
				break;
			case OPTION_PAYLOAD:
				arg.payload = mbus_json_parse(optarg);
				if (arg.payload == NULL) {
					mbus_errorf("invalid payload");
					goto bail;
				}
				break;
			case OPTION_HELP:
				usage();
				goto bail;
		}
	}
	if (arg.destination == NULL) {
		mbus_errorf("destination is null");
		goto bail;
	}
	if (arg.event == NULL) {
		mbus_errorf("event is null");
		goto bail;
	}
	rc = mbus_client_subscribe(client, MBUS_SERVER_NAME, MBUS_SERVER_STATUS_CONNECTED, event_status_server_connected, &arg);
	if (rc != 0) {
		mbus_errorf("can not subscribe to events");
		goto bail;
	}
	while (1) {
		rc = mbus_client_run_timeout(client, MBUS_CLIENT_DEFAULT_TIMEOUT);
		if (rc != 0) {
			mbus_errorf("client run failed");
			goto bail;
		}
		if (arg.finished != 0) {
			break;
		}
	}
	mbus_json_delete(arg.payload);
	mbus_client_destroy(client);
	return arg.result;
bail:	if (client != NULL) {
		mbus_client_destroy(client);
	}
	if (arg.payload != NULL) {
		mbus_json_delete(arg.payload);
	}
	return -1;
}