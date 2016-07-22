#!/bin/sh

export LANG=C

HTML_FILE=$(mktemp)
HTML="
<html>
<head>
<link rel='stylesheet' type='text/css' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'>
</head>
<body>
<div class='container'>
<div class='row'>
<h1 class='text-center'>System Monitor</h1>
</div>
<div class='row'>
  <div class='col-md-4 text-center'>
    <h1 class='glyphicon glyphicon-plus glyphicon-tasks'></h1>
    <small class='center-block'>CPU</small>
    <h3>__CPU__</h3>
  </div>
  <div class='col-md-4 text-center'>
    <h1 class='glyphicon glyphicon-plus glyphicon-asterisk'></h1>
    <small class='center-block'>MEM</small>
    <h3>__MEM__</h3>
  </div> 
  <div class='col-md-4 text-center'>
    <h1 class='glyphicon glyphicon-plus glyphicon-hdd'></h1>
    <small class='center-block'>HDD</small>
    <h3>__HDD__</h3>
  </div>
</div>
</div>
</body>
</html>
"

update_stats() {
    CPU=$(awk -v a="$(awk '/cpu /{print $2+$4,$2+$4+$5}' /proc/stat; sleep 1)" '/cpu /{split(a,b," "); printf "%.0f%", 100*($2+$4-b[1])/($2+$4+$5-b[2])}' /proc/stat)
    MEM_FREE=$(free -m | grep Mem)
    MEM_CURRENT=$(echo $MEM_FREE | cut -f3 -d' ')
    MEM_TOTAL=$(echo $MEM_FREE | cut -f2 -d' ')
    MEM=$(printf "%.0f" "$(echo "scale = 2; $MEM_CURRENT/$MEM_TOTAL*100" | bc)")
    HDD=$(df -lh | awk '{if ($6 == "/") { print $5 }}' | head -1 | cut -d'%' -f1)
    
    echo "$HTML" > $HTML_FILE
    sed -i "s,__CPU__,$CPU,g" $HTML_FILE
    sed -i "s,__MEM__,$MEM%,g" $HTML_FILE
    sed -i "s,__HDD__,$HDD%,g" $HTML_FILE
}

# Detect if is being sourced from Electric WebView
if [ -n "$ELECTRIC_WEBVIEW_TRANSPORT" ]
then

    # Open a maximized window
    open maximized

    while true
    do
        update_stats
        set_html file $HTML_FILE
        sleep 1
    done
else
    ELECTRIC_SOCKET=$(mktemp)
    SCRIPT=$(readlink -f $0)

    echo "$HTML" > $HTML_FILE

    # Starts a instance of Electric WebView using a unixsocket file as transport layer
    electric-webview -t unixsocket:$ELECTRIC_SOCKET -s $SCRIPT

    rm $ELECTRIC_SOCKET

    rm $HTML_FILE
fi
