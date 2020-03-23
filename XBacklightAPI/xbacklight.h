/*
 * Copyright © 2007 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>
#include <xcb/randr.h>

#include <ctype.h>
#include <string.h>

static xcb_atom_t backlight;
static xcb_connection_t* conn;
static xcb_randr_output_t output;

static long
backlight_get ()
{
	xcb_generic_error_t *error;
	xcb_randr_get_output_property_reply_t *prop_reply = NULL;
	xcb_randr_get_output_property_cookie_t prop_cookie;
	long value;

	prop_cookie = xcb_randr_get_output_property (conn, output,
			backlight, XCB_ATOM_NONE,
			0, 4, 0, 0);
	prop_reply = xcb_randr_get_output_property_reply (conn, prop_cookie, &error);
	if (error != NULL || prop_reply == NULL) {
		return -1;
	}

	if (prop_reply == NULL ||
			prop_reply->type != XCB_ATOM_INTEGER ||
			prop_reply->num_items != 1 ||
			prop_reply->format != 32) {
		value = -1;
	} else {
		value = *((int32_t *) xcb_randr_get_output_property_data (prop_reply));
	}

	free (prop_reply);
	return value;
}

static void
backlight_set (long value)
{
	xcb_randr_change_output_property (conn, output, backlight, XCB_ATOM_INTEGER,
			32, XCB_PROP_MODE_REPLACE,
			1, (unsigned char *)&value);
	xcb_flush (conn);
}

static void
find_output ()
{
	xcb_generic_error_t *error;

	xcb_screen_iterator_t iter = xcb_setup_roots_iterator (xcb_get_setup (conn));
	while (iter.rem) {
		xcb_screen_t *screen = iter.data;
		xcb_window_t root = screen->root;
		xcb_randr_output_t *outputs;

		xcb_randr_get_screen_resources_current_cookie_t resources_cookie;
		xcb_randr_get_screen_resources_current_reply_t *resources_reply;

		resources_cookie = xcb_randr_get_screen_resources_current (conn, root);
		resources_reply = xcb_randr_get_screen_resources_current_reply (conn, resources_cookie, &error);
		if (error != NULL || resources_reply == NULL) {
			int ec = error ? error->error_code : -1;
			fprintf (stderr, "RANDR Get Screen Resources returned error %d\n", ec);
			continue;
		}

		outputs = xcb_randr_get_screen_resources_current_outputs (resources_reply);
		for (int o = 0; o < resources_reply->num_outputs; o++)
		{
			output = outputs[o];
			double cur;

			cur = backlight_get ();
			if (cur != -1)
			{
				break;
			}
		}

		free (resources_reply);
		xcb_screen_next (&iter);
	}
}

static void
backlight_setup ()
{
	xcb_generic_error_t *error;

	xcb_intern_atom_cookie_t backlight_cookie;
	xcb_intern_atom_reply_t *backlight_reply;

	conn = xcb_connect (NULL, NULL);

	backlight_cookie = xcb_intern_atom (conn, 1, strlen("Backlight"), "Backlight");

	backlight_reply = xcb_intern_atom_reply (conn, backlight_cookie, &error);
	if (error != NULL || backlight_reply == NULL) {
		int ec = error ? error->error_code : -1;
		fprintf (stderr, "Intern Atom returned error %d\n", ec);
		exit (1);
	}

	backlight = backlight_reply->atom;
	free (backlight_reply);
	find_output ();
}
