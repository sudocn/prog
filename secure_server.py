import socket, ssl

def start_server():
    bindsocket = socket.socket()
    bindsocket.bind(('localhost', 10023))
    bindsocket.listen(5)

    while True:
        newsocket, fromaddr = bindsocket.accept()
        connstream = ssl.wrap_socket(newsocket,
                                     server_side = True,
                                     certfile = "cert1.pem",
                                     #keyfile = "mykeyfile",
                                     ssl_version = ssl.PROTOCOL_SSLv3)
        deal_with_client(connstream)


def deal_with_client(connstream):
    data = connstream.read()
    while data:
        if not receive_data(connstream, data):
            break
        print "read new"
        data = connstream.read()
    connstream.close()

def receive_data(connstream, data):
    print data
    return 1

if __name__ == "__main__":
    start_server()


        
                                 
