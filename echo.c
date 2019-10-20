/*
 * Copyright 2013, 2017, Jernej Kovacic
 * Copyright 2019 Jan Klötzke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>

#include "echo.h"

/* This string is displayed first when Enter is pressed: */
#define MSG_PREFIX "You entered: \""
#define MSG_SUFFIX "\"\r\n"


int echo_init(struct echo *self, echo_reply_cb_t callback)
{
	self->callback = callback;
	self->pos = strlen(MSG_PREFIX);
	strcpy(self->buf, MSG_PREFIX);

	return 0;
}

void echo_putchar(struct echo *self, char ch)
{
	if (ch >= 0x20 && ch < 0x7f) {
		/* keep space for final MSG_SUFFIX */
		if (self->pos < sizeof(self->buf) - sizeof(MSG_SUFFIX))
			self->buf[self->pos++] = ch;
	} else {
		switch (ch) {
			case 0x08: /* backspace - ^H */
			case 0x7f: /* DEL */
				if (self->pos > strlen(MSG_PREFIX))
					self->pos--;
				break;
			case '\r':
			case '\n':
				strcpy(self->buf + self->pos, MSG_SUFFIX);
				self->callback(self->buf);
				self->pos = strlen(MSG_PREFIX);
				break;
		}
	}
}
