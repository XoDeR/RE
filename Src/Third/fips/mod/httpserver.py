"""wrap SimpleHTTPServer and prevent Ctrl-C stack trace output"""

import SimpleHTTPServer
import SocketServer

import log

try :
    log.colored(log.GREEN, 'serving on http://localhost:8000 (Ctrl-C to quit)')
    SocketServer.TCPServer.allow_reuse_address = True
    httpd = SocketServer.TCPServer(('localhost', 8000), SimpleHTTPServer.SimpleHTTPRequestHandler)
    httpd.serve_forever()
except KeyboardInterrupt:
    httpd.shutdown()
    httpd.server_close()
    log.colored(log.GREEN, '\nhttp server stopped')
    exit(0)

