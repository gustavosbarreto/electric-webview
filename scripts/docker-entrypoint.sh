#!/bin/bash

[ -z "$RESOLUTION" ] && RESOLUTION=1024x768

TRANSPORT=$*
TRANSPORT_NAME=${TRANSPORT/%:*}

echo "unixsocket tcp websocket http" | grep -q $TRANSPORT_NAME || (echo "Invalid transport layer" && exit 1) || exit 1

if [ "$TRANSPORT_NAME" = "http" ]; then
    xvfb-run -a -s "-screen 0 ${RESOLUTION}x16" /usr/local/bin/instant-webview -t unixsocket:/tmp/instant-webview &
    INSTANT_WEBVIEW_PID=$!

    /usr/src/instant-webview/scripts/httpserver.py -t unixsocket:/tmp/instant-webview -p 80 &

    echo "http: Listening on port 80"
else
    xvfb-run -a -s "-screen 0 ${RESOLUTION}x16" /usr/local/bin/instant-webview -t $TRANSPORT &
    INSTANT_WEBVIEW_PID=$!
fi

wait $INSTANT_WEBVIEW_PID
