# Introduction

[![Build Status](https://travis-ci.org/gustavosbarreto/instant-webview.svg?branch=master)](https://travis-ci.org/gustavosbarreto/instant-webview)

Instant WebView is a web browser which has no UI controls. You can interact with
them through inter-process communication (IPC).

Currently, we support the following IPC transport layers:

* TCP
* Unix Socket
* WebSocket

You can implement another IPC transport layer inheriting the
`IpcTransportLayer` abstract class. See the existing IPC transport
implementations to get some clue.

# Use Cases

* Web testing
  - Browser automated testing done easy.
* Page automation
  - Access and manipulate web pages using DOM API.
* Digital Signage
  - Display Web content without distractions.
* Kiosk Web Browser
  - The users can only interact with a single web application.

# Reverse IPC

With reverse IPC, the Instant WebView accept commands from remote IPC server
instead of accepting commands from IPC clients. This is useful for controlling
the browser remotely. To enable reverse IPC pass `"--reverse"` in the cmdline.

# Building

Before building Instant WebView you need Qt and QtWebEngine 5.6 installed on the system.

```
$ qmake PREFIX=/usr
$ make
$ make install
```

# Running

To run Instant WebView you need to choose which IPC transport layer should be used
by IPC mechanism, use the `--transport` option to that.

Example:

```
$ instant-webview --transport unixsocket
$ echo -e "open maximized\nload http://google.com" | socat - unix-connect:/tmp/instant-webview
```

# Commands

Instant WebView will read commands via TCP, Unix Socket or WebSocket. Each command command
starts with the name of a command and is terminated by a newline. Empty line are interpreted
as end of connection.

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

* `js <string|file> <VALUE>`
  - Run JavaScript code. If `file` is given, the `VALUE` is interpreted as
    a path to a file to load, otherwise `string` is given, the value `VALUE`
    is executed as JavaScript code.

## Events

* `subscribe <VALUE>`
  - Subscribe to specified event in `VALUE`. If `VALUE` starts with `@` the
    subscription is marked as single-shot.

### Single-shot event subscription

The single-shot event subscription might be used to wait until an event is fired.

### Available events

* `url_changed`
  - This event is fired when the url chages.
* `title_changed`
  - This event is fired whenever the title changes.
* `load_started`
  - This event is fired when a page starts loading content.
* `load_finished`
  - This event is fired when a load of the page has finished.

# Examples

**Basic Example**

Open a maximized window and load google.com:

```sh
echo "open maximized" | nc -U /tmp/instant-webview
echo "load http://google.com" | nc -U /tmp/instant-webview
```

**Screenshot Example**

Open a maximized window, load google.com and wait until load is finished
to take a screenshot:

```sh
echo "open maximized" | nc -U /tmp/instant-webview
echo -e "subscribe @load_finished\nload http://google.com" | nc -q -1 -U /tmp/instant-webview
echo -e "screenshot\n" | nc -U /tmp/instant-webview | base64 --decode > screenshot.jpg
```

**Browser History Example**

Open a maximized window, subscribe to url_changed event, wait until it
is fired then append the URL to /tmp/history.txt

```sh
echo "open maximized" | nc -U /tmp/instant-webview
while true; do
  URL=$(echo -e "subscribe @url_changed" | nc -q -1 -U /tmp/instant-webview)
  echo "$URL" >> /tmp/history.txt
done
```
