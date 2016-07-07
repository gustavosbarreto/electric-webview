#!/bin/bash

trap 'kill $INSTANT_WEBVIEW_PID; exit 1' SIGTERM

function install() {
    echo "#!/bin/sh
/usr/bin/cat > instant-webview-container-launcher <<-EOF
$(cat /usr/src/instant-webview/scripts/container-launcher.py)
EOF
chmod +x instant-webview-container-launcher"
}

function setup() {
    useradd --shell /bin/bash -u $LOCAL_USER -o -c "" -m instant-webview

    if [ -n "$XAUTH" ]; then
        export XAUTHORITY=/tmp/.Xauthority
        touch $XAUTHORITY
        chmod 755 $XAUTHORITY
        echo $XAUTH | xauth nmerge -
        X11_UNIX=$(find /tmp/host -name .X11-unix -print -quit)
        ln -sf $X11_UNIX /tmp/.X11-unix
    fi
}

function run() {
    [ -z "$RESOLUTION" ] && RESOLUTION="1024x768"

    TRANSPORT=$*
    TRANSPORT_PROTOCOL=$(echo $TRANSPORT | cut -f1 -d:)

    echo "unixsocket tcp websocket http" | grep -q $TRANSPORT_PROTOCOL || (echo "Invalid transport layer" && exit 1) || exit 1

    if [ "$TRANSPORT_PROTOCOL" = "unixsocket" ]; then
        cd $VOLUME
    fi

    if [ "$TRANSPORT_PROTOCOL" = "http" ]; then
        ADDRESS=$(echo $TRANSPORT | cut -f2 -d:)
        PORT=$(echo $TRANSPORT | cut -f3 -d:)

        INSTANT_WEBVIEW_COMMAND=(/usr/local/bin/instant-webview -t unixsocket:/tmp/instant-webview)

        /usr/src/instant-webview/scripts/httpserver.py -t unixsocket:/tmp/instant-webview -a $ADDRESS -p $PORT &
        echo "http: Listening on port $PORT"
    else
        INSTANT_WEBVIEW_COMMAND=(/usr/local/bin/instant-webview -t $TRANSPORT)
    fi

    if [ -z "$XAUTH" ]; then
        INSTANT_WEBVIEW_COMMAND=(xvfb-run -a -s "-screen 0 ${RESOLUTION}x16" ${INSTANT_WEBVIEW_COMMAND[@]})
    fi

    gosu instant-webview "${INSTANT_WEBVIEW_COMMAND[@]}" &
    INSTANT_WEBVIEW_PID=$!
    wait $INSTANT_WEBVIEW_PID
}

case "$1" in
    run)
        shift
        setup
        run "$*"
        ;;
    "")
        install
        exit 0
        ;;
esac
