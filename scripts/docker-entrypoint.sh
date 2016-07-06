#!/bin/sh

[ -z "$RESOLUTION" ] && RESOLUTION=1024x768

xvfb-run -a -s "-screen 0 ${RESOLUTION}x16" /usr/local/bin/instant-webview $* &

INSTANT_WEBVIEW_PID=$!

/usr/src/instant-webview/scripts/httpserver.py $* -p 80 &

wait $INSTANT_WEBVIEW_PID
