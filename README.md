# Introduction

[![Build Status](https://travis-ci.org/gustavosbarreto/instant-webview.svg?branch=master)](https://travis-ci.org/gustavosbarreto/instant-webview)

Instant WebView is a scriptable WebView for developers.

The WebView uses the QtWebEngine (which is based on Chromium) to render the HTML
content. There is also a simple protocol to get data, send commands and
listen for events: through Unix Socket, TCP or WebSocket. The window of WebView
does not have any UI component what you see in a standard web browser like an
address bar, status bar, navigation buttons, etc.

# Use Cases

* Web testing
  - Browser automated testing done easy.
* Page automation
  - Access and manipulate web pages using DOM API.
* Digital Signage
  - Display Web content without distractions.
* Kiosk Web Browser
  - The users can only interact with a single web application.

# Building

Before building Instant WebView you need to install GCC 6, Qt and QtWebEngine 5.6 on the system.

```sh
qmake PREFIX=/usr
make
make install
```

# Usage

### instant-webview

The WebView itself.

```
Usage: instant-webview [options]
Instant WebView is a scriptable WebView for developers.

Options:
  -h, --help                                  Displays this help.
  -v, --version                               Displays version information.
  -t, --transport <tcp|unixsocket|websocket>  IPC Transport Layer to use.
  -r, --reverse <ID>                          Enable reverse mode. The ID is
                                              used to identify your session in
                                              the server.
  -s, --script <path>                         Script to run.
```

**Example:**

```sh
instant-webview -t unixsocket:/tmp/instant-webview
```

### instant-webview-ctl

A utility to interact with the WebView.

```
Usage: instant-webview-ctl [options] command
Instant WebView is a scriptable WebView for developers.

Options:
  -h, --help                                  Displays this help.
  -v, --version                               Displays version information.
  -t, --transport <tcp|unixsocket|websocket>  IPC Transport Layer to use.

Arguments:
  command                                     Command to execute. Pass "-" to
                                              read from stdin.
```

**Example:**

```sh
echo "open maximized" | instant-webview-ctl -t unixsocket:/tmp/instant-webview -
```

# Commands

Instant WebView reads commands from TCP, Unix Socket or WebSocket. Each command starts
with the name of a command and is terminated by a newline. Empty line are interpreted
as end of connection.

If the command starts with `@` the command is marked as single-shot.

Due to the simplicity of the protocol, it is possible to interact with the WebView
using the command-line utility GNU Netcat. However, there is `instant-webview-ctl`
utility that can be used to interact with the WebView, so you don't need extra
tools to interact with the WebView.

Simple example using the GNU Netcat utility to demonstrate how simple is the protocol:

```sh
echo "open maximized" | nc -U /tmp/instant-webview
echo "load http://google.com" | nc -U /tmp/instant-webview
```

In the below example a maximized window is open and the http://google.com is loaded.

## Single-shot commands

Single-shot commands might be used to wait until command response is received.
It might be useful to get data from the WebView using both `instant-webview-ctl`
and GNU Netcat utility.

```sh
URL=$(echo "@current_url" | instant-webview-ctl -t unixsocket:/tmp/instant-webview)
echo "The current URL is $URL"
```

## Command response

The response of a command may vary according the type of command. If the command is
marked as single-shot, the response only contains the returned data from the command,
otherwise it is not a single-shot command, the response is composed of the command
name and returned data, separated by space.

**Note that not all commands have a response.**

## Navigation

* `load <URL>`
  - Loads the specified `URL`.

* `stop`
  - Stops loading the document.

* `reload`
  - Reloads the current document.

* `back`
  - Loads the previous document in the list of documents built by navigating links.

* `forward`
  - Loads the next document in the list of documents built by navigating links.

## Window

* `open <maximized|fullscreen>`
  - Open the window. If `maximized` is given, the window is open in maximized
    state, otherwise `fullscreen` is given, the window is open in fullscreen mode.

* `close`
  - Closes the window.

## Page

* `current_url`
  - Returns the URL of the web page currently viewed.
* `current_title`
  - Returns the title of the web page currently viewed.
* `screenshot [REGION]`
  - Take a screenshot of the web page currenly viewed and send base64 encoded JPG.
    If `[REGION]` is given, restrict the screenshot by the given region `(x,y,width,height)`.

## Content

* `set_html <string|file> <VALUE>`
  - Sets the content of the web page. If `file` is given, the `VALUE`
    is interpreted as a path to a file to load, otherwise `string` is given,
    the `VALUE` is used to set the content of the web page.

* `get_html <text|html>`
  - Retrieve the page's content formated as `html` or `text`.

* `exec_js <string|file> <VALUE>`
  - Run JavaScript code. If `file` is given, the `VALUE` is interpreted as
    a path to a file to load, otherwise `string` is given, the value `VALUE`
    is executed as JavaScript code.

## Events

* `subscribe <VALUE>`
  - Subscribe to specified event in `VALUE`.

### Available events

* `url_changed`
  - This event is fired when the url chages.
* `title_changed`
  - This event is fired whenever the title changes.
* `load_started`
  - This event is fired when a page starts loading content.
* `load_finished`
  - This event is fired when a load of the page has finished.
* `info_message_raised <MESSAGE>`
  - This event is fired when a JavaScript program tries to print a info
    `MESSAGE` to the console.
* `warning_message_raised <MESSAGE>`
  - This event is fired when a JavaScript program tries to print a warning
    `MESSAGE` to the console.
* `error_message_raised <MESSAGE>`
  - This event is fired when a JavaScript program tries to print a error
    `MESSAGE` to the console.
* `user_activity <IDLE_TIME>`
  - This event is fired when the user interacts with the input system through
    keystrokes or mouse clicks. The `IDLE_TIME` is the milliseconds since the
    last user activity.

## Instant WebView

* `idle_time`
  - Returns the idle time from the last user activity in milliseconds.

* `block_user_activity <true|false>`
  - If `true` is given, prevents the user activity with the page until `false` is given.

* `exec_cmd <sync|async> <COMMAND> [ARGUMENTS]`
  - Run a `COMMAND` on the system with the given `ARGUMENTS`. If `async` is given,
    the command will be started detached and the output will be ignored,
    otherwise `sync` is given, the execution is blocked until the command has
    finished and the output will be send in the response message.
