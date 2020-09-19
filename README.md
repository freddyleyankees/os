# Nadia Operating System
# How to build this OS

This method watch in linux
 
Install virtual machine such as qemu or virtualbox

install mkdosfs utility

Go to root project in terminal

$ make build

Automatically os is compiled and launght with qemu
to launght os with virtualbox or other VM take nadia.img building and 
insert in your new project.

Take care during the creation process of your new project, add controller' Floppy device then insert path of nadia.img location

