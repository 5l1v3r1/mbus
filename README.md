# mBus #

mBus is a lightweight messaging system.

1. <a href="#1-overview">overview</a>
2. <a href="#2-download">download</a>
3. <a href="#3-build">download</a>
4. <a href="#4-execute">execute</a>
  - <a href="#41-controller">controller</a>
  - <a href="#42-listener">listener</a>
  - <a href="#43-cli">cli</a>
  - <a href="#44-launcher">launcher</a>
5. <a href="#5-api">api</a>
6. <a href="#6-protocol">protocol</a>
7. <a href="#7-contact">contact</a>
8. <a href="#8-license">license</a>

## 1. overview ##

mBus is a lightweight messaging system providing communication between daemons 
and applications. enabling development of component based distributed
applications.

every daemon registers with a unique namespace and a set of procedures with
various number of arguments under the namespace. procedure request and responce
are in JSON format. any daemon can post an event under registered namespace
with various number of arguments in JSON format. any daemon or application can
subscribe to events with namespaces and can call procedures from other
namespaces.

## 2. download ##

    git clone git@github.com:alperakcan/mbus.git

## 3. build ##

    cd mbus
    make

## 4. execute ##

### 4.1 controller ###

#### 4.1.1 command line options ####

  - --mbus-help
  
    print available parameters list and exit

  - --mbus-debug-level

    set debug level, available options: debug, info, error. default: error
  
  - --mbus-server-protocol
  
    set communication protocol, available options: tcp, uds. default: uds

  - --mbus-server-address
  
    set server address:

    - tcp: default is 127.0.0.1
    - uds: default is /tmp/mbus-server
  
  - --mbus-server-port
  
    set server port:

    - tcp: default is 8000
    - uds: unused

### 4.2 listener ###

### 4.3 cli ###

### 4.4 launcher ###

## 5. api ##

### 5.1 server ###

  - struct mbus_server * mbus_server_create (int argc, char *argv[]);
  - void mbus_server_destroy (struct mbus_server *server);

  - int mbus_server_run (struct mbus_server *server);
  - int mbus_server_run_timeout (struct mbus_server *server, int milliseconds);

### 5.2 client ###

  - struct mbus_client * mbus_client_create (const char *name, int argc, char *argv[]);
  - void mbus_client_destroy (struct mbus_client *client);

  - int mbus_client_subscribe (struct mbus_client *client, const char *source, const char *event, void (*function) (struct mbus_client *client, const char *source, const char *event, cJSON *payload, void *data), void *data);
  - int mbus_client_register (struct mbus_client *client, const char *command, int (*function) (struct mbus_client *client, const char *source, const char *command, cJSON *payload, cJSON *result, void *data), void *data);
  - int mbus_client_event (struct mbus_client *client, const char *event, cJSON *payload);
  - int mbus_client_event_to (struct mbus_client *client, const char *to, const char *identifier, cJSON *event);
  - int mbus_client_command (struct mbus_client *client, const char *destination, const char *command, cJSON *call, cJSON **result);

  - int mbus_client_run (struct mbus_client *client);
  - int mbus_client_run_timeout (struct mbus_client *client, int milliseconds);

## 6. protocol ##

## 7. contact ##

if you are using the software and/or have any questions, suggestions, etc. please contact with me at alper.akcan@gmail.com

## 8. license ##

### mBus Copyright (c) 2014, Alper Akcan <alper.akcan@gmail.com> ###

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  - Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  - Neither the name of the developer nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### cJSON Copyright (c) 2009 Dave Gamble ###
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

