#!/usr/bin/env python2

import BaseHTTPServer
import StringIO
import urlparse
import socket
import SocketServer
import re
import argparse

from electricwebview import WebView

events = [
    'url_changed',
    'title_changed',
    'load_started',
    'load_finished',
    'info_message_raised',
    'warning_message_raised',
    'error_message_raised',
    'user_activity'
]

parser = argparse.ArgumentParser(description='Electric WebView HTTP Interface')
parser.add_argument('-t', '--transport', type=str, required=True, help='Transport layer')
parser.add_argument('-a', '--address', type=str, required=True, help='Address to listen')
parser.add_argument('-p', '--port', type=int, required=True, help='Port to listen')
args = parser.parse_args()

class CommandsHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def send_reply(self, data = ''):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(data if data is not None else '')

    def send_error(self, error, status=500):
        self.send_response(status)
        self.end_headers()
        self.wfile.write(error)

    def do_GET(self):
        url = urlparse.urlparse(self.path)
        query = urlparse.parse_qs(url.query)
        webview = WebView(args.transport)

        if url.path == '/load':
            url_param = query.get('url', [''])[0]

            if not url_param:
                self.send_error('missing url param')
            else:
                webview.load(url_param)
                self.send_reply()

            return

        if url.path == '/stop':
            webview.stop()
            self.send_reply()
            return

        if url.path == '/reload':
            webview.reload()
            self.send_reply()
            return

        if url.path == '/back':
            webview.back()
            self.send_reply()
            return

        if url.path == '/forward':
            webview.forward()
            self.send_reply()
            return

        if url.path == '/open':
            state_param = query.get('state', [''])[0]

            if not state_param:
                self.send_error('missing state param')
            else:
                webview.open(state_param)
                self.send_reply()

            return

        if url.path == '/close':
            webview.close()
            self.send_reply()
            return

        if url.path == '/current_url':
            reply = webview.current_url()
            self.send_reply(reply)
            return

        if url.path == '/current_title':
            reply = webview.current_title()
            self.send_reply(reply)
            return

        if url.path == '/screenshot':
            region_param = query.get('region', [''])[0]
            reply = webview.screenshot(region_param)
            self.send_reply(reply)

        if url.path == '/get_html':
            format_param = query.get('format', [''])[0]

            if not format_param:
                self.send_error('missing format param')
            else:
                reply = webview.get_html(format_param)
                self.send_reply(reply)

            return

        if url.path == '/set_html':
            file_param = query.get('file', [''])[0]

            if not file_param:
                self.send_error('missing file param')
            else:
                webview.set_html('file', file_param)
                self.send_reply()

            return

        if url.path == '/exec_js':
            file_param = query.get('file', [''])[0]

            if not file_param:
                self.send_error('missing file param')
            else:
                reply = webview.exec_js('file', file_param)
                self.send_reply(reply)

            return

        if url.path == '/subscribe':
            event_param = query.get('event', [''])[0]

            if not event_param:
                self.send_error('missing event param')
            else:
                reply = webview.subscribe(event_param)
                reply = re.sub(r'^%s\s' % event_param, '', reply)
                self.send_reply(reply)

            return

        if url.path == '/idle_time':
            reply = webview.idle_time()
            self.send_reply(reply)
            return

        if url.path == '/block_user_activity':
            block_param = query.get('block', [''])[0]

            if not block_param:
                self.send_error('missing block param')
            else:
                webview.block_user_activity(block_param)
                self.send_reply()

            return

        if url.path == '/exec_cmd':
            cmd_param = query.get('cmd', [''])[0]
            execution_param = query.get('execution', [''])[0]
            args_param = query.get('args', [''])[0]

            if not cmd_param:
                self.send_error('missing cmd param')
                return

            if not execution_param:
                self.send_error('missing execution param')
                return

            reply = webview.exec_cmd(execution_param, cmd_param, args=args_param.split())
            self.send_reply(reply)

        if url.path == '/quit':
            code_param = query.get('code', [''])[0]
            webview.quit(code_param)
            self.send_reply()

    def do_POST(self):
        url = urlparse.urlparse(self.path)
        webview = WebView(args.transport)
        content_length = self.headers.get('Content-Length')

        if url.path == '/set_html':
            if not content_length:
                self.send_reply()
                return

            data = self.rfile.read(int(content_length))
            webview.set_html('string', data)
            self.send_reply()
            return

        if url.path == '/exec_js':
            if not content_length:
                self.send_reply()
                return

            data = self.rfile.read(int(content_length))
            webview.exec_js('string', data)
            self.send_reply()
            return

class ThreadedHTTPServer(SocketServer.ThreadingMixIn, BaseHTTPServer.HTTPServer):
    pass

httpd = ThreadedHTTPServer((args.address, args.port), CommandsHandler)
httpd.serve_forever()
