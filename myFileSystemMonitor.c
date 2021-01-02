#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <ctype.h>
#include <time.h>
#include <getopt.h>
#include <errno.h>
#include <execinfo.h>
#include <pthread.h>
#include <libcli.h>

// String sub-functions
char **str_splitter(char *str, size_t *size)
{
    // split string to array of strings, seperator is ' '

    char **array = NULL;
    char *p;
    size_t items = 0, q;
    const char *sepa = " ";

    p = str;
    for (;;)
    {
        p += strspn(p, sepa);
        if (!(q = strcspn(p, sepa)))
            break;
        if (q)
        {
            array = realloc(array, (items + 1) * sizeof(char *));
            if (array == NULL)
                exit(EXIT_FAILURE);

            array[items] = malloc(q + 1);
            if (array[items] == NULL)
                exit(EXIT_FAILURE);

            strncpy(array[items], p, q);
            array[items][q] = 0;
            items++;
            p += q;
        }
    }
    *size = items;
    return array;
}

void str_concat(char **s1, const char *s2)
{
    //Add two strings together

    int s1_len = strlen(*s1);
    int s2_len = strlen(s2);
    int len = s1_len + s2_len + 1;
    char *new_str = (char *)realloc(*s1, len); // +1 for the null-terminator
    if (new_str != NULL)
    {
        for (int i = 0; i < s2_len; i++)
        {
            new_str[s1_len++] = s2[i];
        }
        new_str[s1_len] = '\0';
        *s1 = new_str;
    }
    else
        exit(EXIT_FAILURE);
}

char *createCommand(char *html_data)
{
    // Create command that make system call of html page

    char start[] = "<!DOCTYPE html><html lang='en'>  <head>    <meta charset='UTF-8' />    <meta name='viewport' content='width=device-width, initial-scale=1.0' />    <meta http-equiv='refresh' content='10' />    <title>File System Monitor</title>     <style>      * {        margin: 0;        padding: 0;        box-sizing: border-box;      }      header,      main,      footer,      nav,      div {        display: block;      }      body {        margin: 0;        background: black;      }      #wrapper{        padding: 5vh 5vw;      }      ul {        font-family: monospace;        font-weight: bold;        font-size: 3.5vh;        margin: 0 0 5vh 0;        padding: 0;        line-height: 1;        color: limegreen;        text-shadow: 0px 0px 10px limegreen;        list-style-type:none;      }      #message {        position: fixed;        font-family: monospace;        font-weight: bold;        text-transform: uppercase;        font-size: 4vh;        background: red;        box-shadow: 0 0 30px red;        text-shadow: 0 0 20px white;        color: white;        width: 20vw;        height: 15vh;        top: 50%;        left: 50%;        margin-right: -50%;        transform: translate(-50%, -50%);        text-align: center;        min-width: 200px;        animation-name: blink;        animation-duration: 3.0s;        animation-iteration-count: infinite;        animation-direction: alternate;        animation-timing-function: linear;      }      @keyframes blink {        0% {          opacity: 0;        }        100% {          opacity: 1;        }      }      p{        font-size: 2.3vh;         position: relative;        top:50%;        left: 50%;        transform: translate(-50%, -50%);      }      footer {        background-color: rgb(0, 0, 0);        position: fixed;        box-shadow: 0px 0px 5px limegreen;        border-top: 1px solid limegreen;        width: 100%;        bottom: 0px;        padding:5px 0px 5px 5px;        color: limegreen;        font-family: monospace;        font-weight: bold;        font-size: 3vh; }    </style>      </head>  <body>      <div id='wrapper'>        <div id='message'>            <p>Scanning...<br>Fetching more data in <span id='countdowntimer'>10 </span> Seconds</p>        </div>        <div id='console'> ";
    char end[] = "</div>    </div>    <footer>Sagi Chubok and Linoy Chubok, Unix System Programming Course.</footer>        <script type='text/javascript'>        var timeleft = 10;        var downloadTimer = setInterval(function(){        timeleft--;        document.getElementById('countdowntimer').textContent = timeleft;        if(timeleft <= 0)            clearInterval(downloadTimer);        },1000);    </script></body></html>";

    int data_len = strlen(html_data);
    int fsize = data_len + strlen(start) + strlen(end) + 2 + 5 + 27 + 1;

    // Allocate dynamic memory and implement the data
    char *copy = malloc(fsize);
    if (copy != NULL)
    {
        size_t i = 0;
        char *ptr = NULL;

        copy[i++] = 'e';
        copy[i++] = 'c';
        copy[i++] = 'h';
        copy[i++] = 'o';
        copy[i++] = ' ';
        copy[i++] = '"';

        ptr = start;
        while (*ptr)
        {
            copy[i++] = *ptr;
            ptr++;
        }

        ptr = html_data;
        while (*ptr)
        {
            copy[i++] = *ptr;
            ptr++;
        }

        ptr = end;
        while (*ptr)
        {
            copy[i++] = *ptr;
            ptr++;
        }
        copy[i++] = '"';
        copy[i++] = ' ';
        copy[i++] = '>';
        copy[i++] = ' ';
        copy[i++] = '/';
        copy[i++] = 'v';
        copy[i++] = 'a';
        copy[i++] = 'r';
        copy[i++] = '/';
        copy[i++] = 'w';
        copy[i++] = 'w';
        copy[i++] = 'w';
        copy[i++] = '/';
        copy[i++] = 'h';
        copy[i++] = 't';
        copy[i++] = 'm';
        copy[i++] = 'l';
        copy[i++] = '/';
        copy[i++] = 'i';
        copy[i++] = 'n';
        copy[i++] = 'd';
        copy[i++] = 'e';
        copy[i++] = 'x';
        copy[i++] = '.';
        copy[i++] = 'h';
        copy[i++] = 't';
        copy[i++] = 'm';
        copy[i++] = 'l';
        copy[i] = '\0';
        return copy;
    }
    else
        exit(EXIT_FAILURE);
}

void inotify(int argc, char **argv, char *address)
{
    /* Read all available inotify events from the file descriptor 'fd'.
          wd is the table of watch descriptors for the directories in argv.
          argc is the length of wd and argv.
          argv is the list of watched directories.
          Entry 0 of wd and argv is unused. */

    char buf;
    int fd, i, poll_num;
    int *wd;
    nfds_t nfds;
    struct pollfd fds[2];

    printf("Press ENTER key to terminate.\n");

    /* Create the file descriptor for accessing the inotify API */

    fd = inotify_init1(IN_NONBLOCK);
    if (fd == -1)
    {
        perror("inotify_init1");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for watch descriptors */

    wd = (int *)calloc(argc + 1, sizeof(int));
    if (wd == NULL)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    /* Mark directories for events
	   - file was opened
	   - file was closed */

    for (i = 0; i < argc; i++)
    {
        wd[i] = inotify_add_watch(fd, argv[i], IN_OPEN | IN_CLOSE);
        if (wd[i] == -1)
        {
            fprintf(stderr, "Cannot watch '%s'\n", argv[i]);
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }

    /* Prepare for polling */

    nfds = 2;

    /* Console input */

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* Inotify input */

    fds[1].fd = fd;
    fds[1].events = POLLIN;

    printf("Listening for events.\n");

    while (1)
    {
        poll_num = poll(fds, nfds, -1);
        if (poll_num == -1)
        {
            if (errno == EINTR)
                continue;
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (poll_num > 0)
        {
            if (fds[0].revents & POLLIN)
            {
                /* Console input is available. Empty stdin and quit */

                while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
                    continue;
                break;
            }

            if (fds[1].revents & POLLIN)
            {
                /* Inotify events are available */

                //handle_events(fd, wd, argc, argv);
            }
        }
    }

    printf("Listening for events stopped.\n");

    /* free splitted directories */
    for (int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
    free(argv);

    /* Close inotify file descriptor */
    close(fd);
    free(wd);
}

int main(int argc, char **argv)
{
    if (argc == 5)
    {
        int option_index = 0;
        char *directory = NULL;
        char *address = NULL;
        while ((option_index = getopt(argc, argv, "d:i:")) != -1)
        {
            switch (option_index)
            {
            case 'd':
                directory = optarg;
                break;
            case 'i':
                address = optarg;
                break;
            }
        }

        if (directory != NULL && address != NULL)
        {
            size_t cnt = 0;
            char **directories = str_splitter(directory, &cnt);
            inotify((int)cnt, directories, address);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printf("Usage: %s -d PATH -i IP\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}