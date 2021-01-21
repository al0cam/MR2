from socket import *
srvName = '127.0.0.1'
srvPort = 8000

clientSocket = socket(AF_INET, SOCK_DGRAM)

message = input('Unesite tekst:')
clientSocket.sendto(message.encode(),(srvName, srvPort))
modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
print(modifiedMessage.decode())
clientSocket.close()