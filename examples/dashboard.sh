#!/bin/sh

# This example loads a dashboard application and prevent the user activity on page.
# Perfect for digital signage applications.

# Detect if is being sourced from Electric WebView
if [ -n "$ELECTRIC_WEBVIEW_TRANSPORT" ]
then
    # Open a maximized window
    open maximized

    load http://dashingdemo.herokuapp.com/sample

    # Prevent the user activity for 10 seconds
    block_user_activity true; sleep 10

    # Allow the user to answer the question
    block_user_activity false

    # Displays a confirmation dialog
    QUIT=$(@exec_js string "confirm('Are you sure you want to quit?')")

    # If the user choose to quit
    if $QUIT
    then
        quit
    else
        # If not, prevent the user activity
        block_user_activity true
    fi
else
    ELECTRIC_SOCKET=$(mktemp)
    SCRIPT=$(readlink -f $0)

    # Starts a instance of Electric WebView using a unixsocket file as transport layer
    electric-webview -t unixsocket:$ELECTRIC_SOCKET -s $SCRIPT

    rm $ELECTRIC_SOCKET
fi
