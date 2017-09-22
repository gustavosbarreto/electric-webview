shopt -s expand_aliases

if [ -z "$ELECTRIC_WEBVIEW_TRANSPORT" ]; then
  echo "You need to set ELECTRIC_WEBVIEW_TRANSPORT"
  exit 1
fi

function __electric_webview_send() {
  local COMMAND=$1
  local SINGLE_SHOT=$2
  local ARGUMENTS

  shift 2

  for arg in "$@"; do
      ARGUMENTS="$ARGUMENTS $arg"
  done

  ARGUMENTS=$(echo $ARGUMENTS | sed 's/^ *//')

  if $SINGLE_SHOT; then
    echo "@$COMMAND $ARGUMENTS" | electric-webview-ctl -t $ELECTRIC_WEBVIEW_TRANSPORT -
  else
    echo "$COMMAND $ARGUMENTS" | electric-webview-ctl -t $ELECTRIC_WEBVIEW_TRANSPORT -
  fi
}

function __electric_webview_decode_response() {
    echo $@ | perl -ne "use URI::Escape; print(uri_unescape(\$_));"
}

function __electric_webview_get_html {
    __electric_webview_decode_response $(__electric_webview_send get_html "$@")
}

alias load="__electric_webview_send load false $@"
alias stop="__electric_webview_send stop false"
alias reload="__electric_webview_send reload false"
alias back="__electric_webview_send back false"
alias forward="__electric_webview_send forward false"

alias open="__electric_webview_send open false $@"
alias close="__electric_webview_send close false"

alias current_url="__electric_webview_send current_url false"
alias @current_url="__electric_webview_send current_url true"
alias current_title="__electric_webview_send current_title false"
alias @current_title="__electric_webview_send current_title true"
alias screenshot="__electric_webview_send screenshot false"
alias @screenshot="__electric_webview_send screenshot true"

alias set_html="__electric_webview_send set_html false $@"
alias @set_html"=__electric_webview_send set_html true $@"
alias exec_js="__electric_webview_send exec_js false $@"
alias @exec_js="__electric_webview_send exec_js true $@"

alias subscribe="__electric_webview_send subscribe false $@"
alias @subscribe="__electric_webview_send subscribe true $@"

alias idle_time="__electric_webview_send idle_time false"
alias @idle_time="__electric_webview_send idle_time true"

alias block_user_activity="__electric_webview_send block_user_activity false $@"
alias @block_user_activity="__electric_webview_send block_user_activity true $@"

alias get_html="__electric_webview_get_html false $@"
alias @get_html="__electric_webview_get_html true $@"

alias accept_feature_request="__electric_webview_send accept_feature_request false $@"
alias @accept_feature_request="__electric_webview_send accept_feature_request true $@"

alias reject_feature_request="__electric_webview_send reject_feature_request false $@"
alias @reject_feature_request="__electric_webview_send reject_feature_request true $@"

alias quit="__electric_webview_send quit false $@"
alias @quit="__electric_webview_send quit true $@"
