# pam\_stormpath
This is a pam module for authenticating against the Stormpath API. This isn't UBER useful yet as Stormpath is still working on arbitrary user fields. When that's done, it'll really be nice (think GECOS stuff).

For now you can use it to auth existing accounts on the system.

## Usage
Some really quick notes.

### Create a new application in Stormpath
All of this can be done with the free tier in Stormpath in the [dashboard](https://api.stormpath.com/ui/dashboard)

- Create a new application. We'll call it `pam_auth`. The defaults are fine.
- Click on the link to your new application. Make note of the `REST URL`. Specifically we'll need the last part of the path. Something like `1pZBI8Kh8aC4yBETADFDADFADF`
- Click on the `Directories` heading and go to your new `pam_auth Directory`
- From within here, you can create a new account in the `pam_auth` directory. You'll want to stick to standard UNIX login style usernames.

Once you've got that done, take the application id and your Stormpath API creds and stick them in `/etc/pam_stormpath.conf` like so:

```
SPID=XXXXXXXXXXX
SPSECRET=YYYYYYYYYYYY
SPAPPLICATION=ZZZZZZZZZZZ
```

`SPID` and `SPSECRET` are your Stormpath API key and API secret.

At this point let's open up a second terminal JUST IN CASE. You are do this in a vagrant instances, right? RIGHT?

Start tailing `/var/log/syslog` and `/var/log/auth`

### Building the module
You'll need a few packages. On Ubuntu:
- `sudo apt-get install libpam-dev libjson0-dev libcurl4-openssl-dev`

Then checkout the source:

- `git clone https://github.com/lusis/pam_stormpath.git`

Then do the autoconf dance:

```
./autogen.sh
./configure --prefix=/usr
sudo make install
# On 64-bit systems
cd /usr/lib/x86_64-linux-gnu/security/
ln -s /usr/lib/security/pam_stormpath.so
```

### Adding the module to your pam configs
At the **TOP** of `/etc/pam.d/common-auth`, add the following line:

`auth       sufficient     pam_stormpath.so`

### Create a test user
`useradd -m -d /home/<username> <username>`

### Testing the login
Now let's test ssh:

`ssh <username>@localhost`

In the logging window, you should see the debug output for the pam module. You should also get your prompt as the user you created in Stormpath:

```
Jul  1 01:56:48 precise64 pam_stormpath: curl debug_data: Connection #0 to host api.stormpath.com left intact
Jul  1 01:56:48 precise64 pam_stormpath: curl debug_data: Closing connection #0
Jul  1 01:56:48 precise64 pam_stormpath: curl debug_data: SSLv3, TLS alert, Client hello (1):
Jul  1 01:56:48 precise64 pam_stormpath: curl debug_data: #001
Jul  1 01:56:48 precise64 pam_stormpath: rc: 0
Jul  1 01:56:48 precise64 pam_stormpath: Successful auth
Jul  1 01:56:48 precise64 sshd[19570]: Accepted password for jvincent from 127.0.0.1 port 46926 ssh2
Jul  1 01:56:48 precise64 sshd[19570]: pam_unix(sshd:session): session opened for user jvincent by (uid=0)
```

## Not ready for primetime
This isn't TOTALLY ready for primetime. This is more a POC than anything. I'd like to PERSONALLY thank Jason Day (@jlogday) for fixing my busted ass C. I'm learning a boatload from what he did. He SAYS he enjoyed getting back into C for this. I'll take his word for it.

Below is the original README contents for posterity.

# Original README contents
This is supposed to eventually be a pam module for the Stormpath API. It doesn't work yet. It ALMOST works though.

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

