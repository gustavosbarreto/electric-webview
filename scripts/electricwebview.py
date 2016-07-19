import socket
import StringIO
import urllib
import re
import sys

class WebView:
    def __init__(self, transport):
        self.transport = transport
        self.transport_type = transport.split(':')[0]
        self.transport_options = transport.split(':')[1:]

    def send_command(self, command, params = [], wait_for_reply=True):
        webview = None

        if self.transport_type == 'unixsocket':
            socket_file = self.transport_options[0]
            webview = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            webview.connect(socket_file)
        elif self.transport_type == 'tcp':
            address = self.transport_options[0]
            port = self.transport_options[1]
            webview = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            webview.connect((address, int(port)))

        webview.send('%s %s\n' % (command, ' '.join(params)))

        if wait_for_reply:
            buffer = StringIO.StringIO()
            while True:
                data = webview.recv(1024)
                buffer.write(data)
                if '\n' in data: break
            return re.sub(r'^%s\s' % command, '', urllib.unquote(buffer.getvalue().splitlines()[0]))

    def load(self, url):
        self.send_command('load', [url], wait_for_reply=False)

    def stop(self):
        self.send_command('stop', wait_for_reply=False)

    def reload(self):
        self.send_command('reload', wait_for_reply=False)

    def back(self):
        self.send_command('back', wait_for_reply=False)

    def forward(self):
        self.send_command('forward', wait_for_reply=False)

    def open(self, state):
        self.send_command('open', [state], wait_for_reply=False)

    def close(self):
        self.send_command('close', wait_for_reply=False)

    def current_url(self):
        return self.send_command('current_url')

    def current_title(self):
        return self.send_command('current_title')

    def screenshot(self, region = ''):
        return self.send_command('screenshot', [region])

    def set_html(self, source, value):
        self.send_command('set_html', [source, value], wait_for_reply=False)

    def get_html(self, format):
        return self.send_command('get_html', [format])

    def exec_js(self, source, value):
        return self.send_command('exec_js', [source, value])

    def subscribe(self, event):
        return self.send_command('subscribe', [event])

    def idle_time(self):
        return self.send_command('idle_time')

    def block_user_activity(self, block):
        self.send_command('block_user_activity', [block], wait_for_reply=False)

    def exec_cmd(self, execution, command, args=[]):
        reply = self.send_command('exec_cmd', ['sync' if execution else 'async', command] + args, wait_for_reply=execution == 'sync')

        if reply is not None:
            return reply

    def quit(self, exit_code):
        self.send_command('quit', [exit_code], wait_for_reply=False)
