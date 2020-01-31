#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "display.h"
#include "utf8.h"
#include "stringbuilder.h"


void clear_screen()
{
	size_t slen = u8_strlen("\\u001b[2J\\u001b[H");
	char* buf = calloc(slen + 1, sizeof(char));
	u8_unescape(buf, slen + 1, "\\u001b[2J\\u001b[H");
	printf("%s", buf);
	free(buf);
}

char* boxed_text(const char* content)
{
	if(content)
	{
		stringbuilder lines = create_stringbuilder(), result = create_stringbuilder();

		char* buf = calloc(strlen(content) + 1, sizeof(char));
		if(!buf) exit(1);

		buf = memcpy(buf, content, strlen(content));

		char *start = buf, *c = buf, found = 0;
		for(c = buf; *c; c++) if(*c == '\n')
		{
			found = 1;
			*c = 0;
			if(start != c)
			{
				sb_append_ch(lines, start);
				start = c + 1;
			}
		}

		if(!found) sb_append_ch(lines, start);
		free(buf);

		size_t max_width = 0;
		for(sb_node sc  = lines->head; sc; sc = sc->next) if(strlen(sc->value) > max_width) max_width = strlen(sc->value);

		max_width += 2;

		for(int i = 0; i < max_width + 2; i++)
			if(i == 0 || i == max_width + 1) sb_append_ch(result, (i) ? "\\u2510\n" : "\\u250c");
			else sb_append_ch(result, "\\u2500");

		for(sb_node line = lines->head; line; line = line->next)
		{
			sb_append_ch(result, "\\u2502 ");
			sb_append_ch(result, line->value);

			size_t slen = strlen(line->value);
			size_t diff = max_width - slen - 1;
			for(int i = 0; i < diff; i++) sb_append_ch(result, " ");
			sb_append_ch(result, "\\u2502\n");
		}

		for(int i = 0; i < max_width + 2; i++)
			if(i == 0 || i == max_width + 1) sb_append_ch(result, (i) ? "\\u2518\n" : "\\u2514");
			else sb_append_ch(result, "\\u2500");

		destroy_stringbuilder(lines);

		buf = sb_concat(result);
		destroy_stringbuilder(result);

		char* newbuf = calloc(u8_strlen(buf) + 1, sizeof(char));
		if(!newbuf) exit(1);

		u8_unescape(newbuf, u8_strlen(buf), buf);
		free(buf);

		return newbuf;
	}
}

void pause()
{
	char* l = readline("Press any key to continue");
	free(l);
}
