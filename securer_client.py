import socket, ssl, pprint

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

ssl_sock = ssl.wrap_socket(s,
                           #ca_certs = "/etc/ssl/certs/Verisign_Class_3_Public_Primary_Certification_Authority.pem",
                           ca_certs = "cert.public",
                           cert_reqs = ssl.CERT_REQUIRED,
                           ssl_version = ssl.PROTOCOL_SSLv3
                           )
ssl_sock.connect(('localhost', 10023))
#ssl_sock.connect(('www.verisign.com', 443))

print repr(ssl_sock.getpeername())
print ssl_sock.cipher()
print pprint.pformat(ssl_sock.getpeercert())
#print ssl_sock.getpeercert()

#ssl_sock.write("""GET / HTTP/1.0\r
#Host: www.verisign.com\r\n\r\n""")
import time
while True:
    print "write new line"
    ssl_sock.write("""AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n""")
    time.sleep(5)


data = ssl_sock.read()

ssl_sock.close()
