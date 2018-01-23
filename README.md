This project contains a set of files to let one understand how linux namespaces work under the hood.
Each file can be executed with certain parameters as an example for IPC namespaces
1. Create a message queue on host(ipcmk -Q)
2. ./ipc ipcs -q will list the message queues within the namespace. The created queue on host will not get listed as we get into a different namespace
all together
