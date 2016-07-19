# HTTP Server Documentation

An HTTP interface for Electric WebView.

### Usage

```
httpserver.py [-h] -t TRANSPORT -p PORT
```

## API

**`GET`** `/load`

Loads the specified url.

**Parameters**

* **url** - The url to load.

***

**`GET`** `/stop`

Stops loading the document.

***

**`GET`** `/reload`

Reloads the current document.

***

**`GET`** `/back`

Loads the previous document in the list of documents built by navigating links.

***

**`GET`** `/forward`

Loads the next document in the list of documents built by navigating links.

***

**`GET`** `/open`

Open the window.

**Parameters**

* **state** - The window state can be `maximized` or `fullscreen`.

***

**`GET`** `/close`

Closes the window.

***

**`GET`** `/current_url`

Returns the URL of the web page currently viewed.

***

**`GET`** `/current_title`

Returns the title of the web page currently viewed.

***

**`GET`** `/screenshot`

Take a screenshot of the web page currenly viewed and send base64 encoded JPG.

**Parameters**

* **region** -  Restrict the screenshot by the given region `(x,y,width,height)`.

***

**`GET`** `/get_html`

Retrieve the page's content formated as html or text.

**Parameters**

* **format** - The format can be `html` or `text`.

***

**`GET`** `/set_html`

Sets the content of the web page.

**Parameters**

* **file** - A path to a file to load.

***

**`POST`** `/set_html`

Sets the content of the web page with the request body data.

***

**`GET`** `/exec_js`

Run JavaScript code.

**Parameters**

* **file** - A path to a file to load.

***

**`POST`** `/exec_js`

Run JavaScript code from the request body data.

***

**`GET`** `/subscribe`

Subscribe to specified event.

**Parameters**

* **event** - An event.

***

**`GET`** `/idle_time`

Returns the idle time from the last user activity in milliseconds.

***

**`GET`** `/block_user_activity`

Prevents the user activity with the page.

**Parameters**

* **block** - If true, prevents the user acitivity until `false` is given.

***

**`GET`** `/exec_cmd`

Run a command in the system.

**Parameters**

* **cmd** - The command
* **execution** - The execution behaviour `sync` or `async`.
* **args** - The arguments
