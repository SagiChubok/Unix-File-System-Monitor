<h1>File System Monitor</h1>
<br />
<br />
  <a href="https://github.com/SagiChubok/Unix-File-System-Monitor">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/35/Tux.svg/150px-Tux.svg.png" alt="Logo" width="80" height="80">
  </a>
<br />
<br />


<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
* [Getting Started](#getting-started)
* [Programming Languages](#programming-languages)
* [License](#license)
* [Contact Us](#contact-us)


<!-- ABOUT THE PROJECT -->
## About The Project

The program will monitor any penetration to the system, using "inotify"
it will report immediately to HTTP server and also to the NETCAT.

**1. Reports to WEB SERVER (APACHE SERVER)**<br>
The program updating the index.html file of the WEB SERVER in real time
<br>and will display a list of files that were accessed, at which time and what is the access type to each file
<br>
(you can refresh the WEBSERVER manually to see the results immediately or to wait for the auto refresh).

Install Apache:
```sh
sudo apt-get update

sudo apt install apache2
```

**2. Reporting on the net by NETCAT**<br>
The udp client sends textual information to the targeted ip, the information will be in the following configuration like the web server.<br><br>
**FILE ACCESSED:**&nbsp;&nbsp;FILE NAME
<br>
**ACCESS:**&nbsp;&nbsp;NO_WRITE / WRITE
<br>
**TIME OF ACCESS:**&nbsp;&nbsp;dd/MM/yyyy HH:mm:ss

```sh
netcat -l -u 127.0.0.1 8888
```

**3. Backtrace feature using Telnetd server and &quot;[libcli](https://github.com/dparrish/libcli)&quot;**<br> Execute telnet command 'backtrace' that will allow backtrace of some thread into the program (the backtrace performed using instrumentation only).<br>
For more commands type 'help'.

```sh
telnet 127.0.0.1 8888
```

<!-- Getting Started EXAMPLES -->
## Getting Started

Compile:
```sh
gcc -g -finstrument-functions -pthread -rdynamic myFileSystemMonitor.c -o myFileSystemMonitor -lcli
```
Execute:
```sh
./myFileSystemMonitor -d test -i 127.0.0.1
```

<!-- PROGRAMMING LANGUAGES -->
## Programming Languages

<img src="https://imgur.com/K2dSIsS.jpg" title="C" /> <img src="https://imgur.com/W5eCDYW.jpg" title="HTML5" /> <img src="https://imgur.com/jzqD88O.jpg" title="CSS3" /> <img src="https://imgur.com/V6olpCS.jpg" title="JavaScript" />
<br />

<!-- LICENSE -->
## License

Distributed under the MIT License.

<!-- CONTACT -->
## Contact Us

Linoy Chubok - linoychubok@gmail.com 
<br>
<br>
Sagi Chubok - chuboksagi@gmail.com 
