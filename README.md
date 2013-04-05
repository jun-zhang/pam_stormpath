This is supposed to eventually be a pam module for the Stormpath API

It has some....problems. The biggest of which is that I'm attempting to learn C in the process of writing it.
At this point it compiles but I'm pretty sure my problems are related to libcurl not being threadsafe (in addition to manual memory management being an entirely new concept to me).

If you can help me fix it, I'll be your best friend forever. Be gentle though.

# Issues I've seen
- Random strings embedded in the various places where I've used strcat and strcpy. I'm pretty sure this is due to the improper use of malloc and free.
- Segfaults. See previous
- Steps just not completing. Example. I'll get down to setting up curl but nothing else will be logged or happen. If I stick logging around various `curl_easy_setopt` lines, it sometimes doesn't get all the way through. I think this is related to threadsafety issues in libcurl.

# welp
Yes the code is all over the place. The file I'm most focused on right now is `req.c`.

The places where I'm using sprintf were me trying an alternate approach to the malloc, strcpy, strcat dance. I think I'm missing null terminators in several of my strings and thus it was causing those problems (which is why you see obscene crap like `spurl[strlen(spurl) - 1] = '\0';`.

Anyway, I want to have someone help me with this and show me where I'm screwing up/where my lack of foundation is hurting me. If you want to make a PR to fix something, please explain what's happening and why you're doing it a given way. This way I can learn from you. I'll even buy you a beer or send you a logstash sticker. Or something.

