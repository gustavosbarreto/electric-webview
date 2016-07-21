#!/bin/sh

# Detect if is being sourced from Electric WebView
if [ -n "$ELECTRIC_WEBVIEW_TRANSPORT" ]
then
    # Open a maximized window
    open maximized

    # Subscribe for load_finished event
    @subscribe load_finished &
    LOAD_FINISHED=$1

    load http://google.com

    # Wait for load finished
    wait $LOAD_FINISHED

    sleep 1 # ugly hack to wait until page is rendered

    # Take a screenshot and save to "/tmp/screenshot.jpg"
    @screenshot | base64 --decode > /tmp/screenshot.jpg

    # Tells the Electric WebView to exit
    quit
else
    ELECTRIC_SOCKET=$(mktemp)
    SCRIPT=$(readlink -f $0)

    # Starts a instance of Electric WebView using a unixsocket file as transport layer
    electric-webview -t unixsocket:$ELECTRIC_SOCKET -s $SCRIPT

    rm $ELECTRIC_SOCKET
fi
