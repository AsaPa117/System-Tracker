# System-Tracker

Project Overview and Goals: 
The goal of our project is to create a program that monitors and livestreams a local computer system to multiple clients via a user interface. Most of the design is built entirely in C, which handles Linux filesystem and networking. The other portion is built in HTML, so users can visually see the results on their own computers. The project's architecture is divided into two parts: the front end and the back end. The backend relies on a server that directly reads the Linux filesystem to obtain CPU usage, memory information, and network interfaces. The information is then formatted and sent over a TCP socket. It then handles transferred and requested data to the server and the clients. On the front end, they create the user interface that the clients can use. They also ensure the user and server are properly connected. They ensure the data being sent is processed and updated continuously, and can be accessed by multiple users. 

Description of Themes Used:
There are three main lessons from the course used for the project. The first is System-Level I/O, since we use a virtual Linux OS to directly access a filesystem (/proc) to obtain and understand specific details and specs about our computer system. The second course is Network Programming. We had to set up a TCP server to run and send data through a port so the HTML page and our server can connect. The other course is Process and Execution. Since the project involves multiple clients, we needed a way to distribute livestreamed content from a single system to them. My partner and I decided to use select() to manage it and allow our program to run concurrently. The last theme is Computer Memory Systems. Our project reads /proc/meminfo to obtain Memtotal and Memfree, which are accessed by the kernel to indicate how much physical memory the system has and how much is currently available. We use these two values to calculate memory usage and display it in real time on the frontend as a pie chart.

Design Choices 
Data: We decide to choose three type of data to monitor, cpu usage, memory usage, and network stat. These data are the basic need to monitor a clients computer status and simplier to transfer data from the kernel level to our built dashboard. 
Network/Connection: In the network level, we decide to use mobile hotspot to easily allow communication between the devices without facing security protocol, proxy or outside access that may come when using a public wifi router. 

Tradeoff
Our dashboard is only design to fetch data to known ip address. It cant dynamically obtain nor determine the address of any new device that connect to the same hotspot. It has be added to our list of ips in the html file for it to fetch data from that device. That device must also have the same OS system (LINUX) and program to obtain data from the computer and send it to our dashboard via http format. 

Challenge/Lesson 
Setting up the two devices to communicate each other were difficult to do. Despite knowing the ips of each device and being connected to the same public wifi router, it would not send any communication. After learning the lecture on network, we concluded that the router may had proxy that keep device from communicating and knowing each other ip. To combat that, we used mobile hotspot for communication and assigning ip. It turns out mobile hotspot has no proxy nor restriction that keep device from communicating. 

*Note
A live demo presents laptop1 as the computer that's running the program. Laptop2 livestream and monitors laptop1 data. 
