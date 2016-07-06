if [ -z "$INSTANT_WEBVIEW_TRANSPORT" ]; then
  echo "You need to set INSTANT_WEBVIEW_TRANSPORT"
  exit 1
fi

function __instant_webview_send() {
  local COMMAND=$1
  local SINGLE_SHOT=$2
  local ARGUMENTS

  shift 2

  for arg in "$@"; do
    if echo $arg | grep -q " "; then
      ARGUMENTS="$ARGUMENTS \"$arg\""
    else
      ARGUMENTS="$ARGUMENTS $arg"
    fi
  done

  ARGUMENTS=$(echo $ARGUMENTS | sed 's/^ *//')

  if $SINGLE_SHOT; then
    echo "@$COMMAND $ARGUMENTS" | instant-webview-ctl -t $INSTANT_WEBVIEW_TRANSPORT -
  else
    echo "$COMMAND $ARGUMENTS" | instant-webview-ctl -t $INSTANT_WEBVIEW_TRANSPORT -
  fi
}

function __instant_webview_decode_response() {
    echo $@ | perl -ne "use URI::Escape; print(uri_unescape(\$_));"
}

function __instant_webview_get_html {
    __instant_webview_decode_response $(__instant_webview_send get_html "$@")
}

alias load="__instant_webview_send load false $@"
alias stop="__instant_webview_send stop false"
alias reload="__instant_webview_send reload false"
alias back="__instant_webview_send back false"
alias forward="__instant_webview_send forward false"

alias open="__instant_webview_send open false $@"
alias close="__instant_webview_send close false"

alias current_url="__instant_webview_send current_url false"
alias @current_url="__instant_webview_send current_url true"
alias current_title="__instant_webview_send current_title false"
alias @current_title="__instant_webview_send current_title true"
alias screenshot="__instant_webview_send screenshot false"
alias @screenshot="__instant_webview_send screenshot true"

alias set_html="__instant_webview_send set_html false $@"
alias @set_html"=__instant_webview_send set_html true $@"
alias exec_js="__instant_webview_send exec_js false $@"
alias @exec_js="__instant_webview_send exec_js true $@"

alias subscribe="__instant_webview_send subscribe false $@"
alias @subscribe="__instant_webview_send subscribe true $@"

alias idle_time="__instant_webview_send idle_time false"
alias @idle_time="__instant_webview_send idle_time true"

alias block_activity="__instant_webview_send block_activity false $@"
alias @block_activity="__instant_webview_send block_activity true $@"

alias get_html="__instant_webview_get_html false $@"
alias @get_html="__instant_webview_get_html true $@"

alias exec_cmd="__instant_webview_send exec_cmd false $@"
alias @exec_cmd="__instant_webview_send exec_cmd true $@"

alias quit="__instant_webview_send quit false $@"
alias @quit="__instant_webview_send quit true $@"
