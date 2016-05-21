# Introduction

[![Build Status](https://travis-ci.org/gustavosbarreto/instant-webview.svg?branch=master)](https://travis-ci.org/gustavosbarreto/instant-webview)

Instant WebView is a web browser which has no UI controls. You can interact with
them through inter-process communication (IPC).

Currently, we support the following IPC transport layers:

* TCP
* Unix Socket

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

# Building

Before building Instant WebView you need Qt 5.5 installed on the system.

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
$ instant-webview --transport tcp
$ echo -e "open maximized\nload http://google.com" | socat - tcp-connect:localhost:38395
```

# Commands

Instant WebView will read commands via TCP or Unix Socket. Each command command starts
with the name of a command and is terminated by a newline. Empty line are interpreted
as end of connection.

## Navigation

* `load <URL>`
  - Loads the specified `URL`.

* `stop`
  - Stops loading the document.

* `reload`
  - Reloads the current document.

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
  - Subscribe to specified event in `VALUE`.

### Available events

* `url_changed`
  - This event is fired when the url chages.
* `title_changed`
  - This event is fired whenever the title changes.
* `load_finished`
  - This event is fired when a load of the page has finished.
