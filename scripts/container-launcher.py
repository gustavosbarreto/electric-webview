#!/usr/bin/env python

import argparse
import os
import subprocess
import signal

parser = argparse.ArgumentParser(description='Instant WebView Container')
parser.add_argument('-t', '--transport', type=str, required=True, help='Transport layer')
parser.add_argument('-x', '--x11', required=False, action='store_true', help='Enable X11 forwarding')
parser.add_argument('-r', '--resolution', required=False, default='', help='Set headless screen resolution')
args = parser.parse_args()

docker_image = os.environ.get('DOCKER_IMAGE', 'gustavosbarreto/instant-webview')
docker_run_options = ['--rm']

transport = args.transport.split(':')
transport_protocol = transport[0] if transport[0:] else None

if transport_protocol == 'http' or transport_protocol == 'tcp':
    host = transport[1] if transport[1:] else '0.0.0.0'
    port = transport[2] if transport[2:] else 80

    if '0.0.0.0' != host:
        print('WARNING! You must bind on adress 0.0.0.0 to access the server in your host machine.')

    docker_run_options += ['-p', '%s:%s' % (port,port)]
elif transport_protocol == 'unixsocket':
    filename = transport[1] if transport[1:] else ''

    if filename.startswith('/'):
        print('WARNING! Maybe the socket becomes inaccessible in your host machine because you are using an absolute path to socket file.')

    docker_run_options += ['-v', '%s:%s' % (os.getcwd(), os.getcwd())]
    docker_run_options += ['-e', 'VOLUME=%s' % os.getcwd()]

else:
    print('Invalid transport layer')
    exit(1)

if args.x11:
    xauth = os.popen('xauth nextract - :%s' % os.environ.get('DISPLAY')).read()

    docker_run_options += ['-e', 'XAUTH=%s' % xauth]
    docker_run_options += ['-e', 'DISPLAY=%s' % os.environ.get('DISPLAY')]
    docker_run_options += ['-v', '/tmp:/tmp/host']

docker_run_options += ['-e', 'RESOLUTION=%s' %  args.resolution]
docker_run_options += ['-e', 'LOCAL_USER=%s' % os.getuid()]

docker = subprocess.Popen(['docker', 'run'] + docker_run_options + [docker_image, 'run', args.transport])

try:
    docker.wait()
    exit(docker.returncode)
except KeyboardInterrupt:
    os.kill(docker.pid, signal.SIGTERM)
    exit(docker.returncode)
