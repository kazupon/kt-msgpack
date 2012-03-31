kt-msgpack
==========
MessagePack-RPC Server Plugin for Kyoto Tycoon



## Requirements

Following programs are requred to build:

  - gcc >= 4.1 with C++ support
  - [MessagePack for C++](http://msgpack.org/) >= 0.5.2
  - [mpio](http://github.com/frsyuki/mpio) >= 0.3.5
  - [MessagePack-RPC for C++](http://msgpack.org/) >= 0.3.0
  - [Kyoto Tycoon](http://fallabs.com/kyototycoon/)



## Installation

Configure and install in the usual way:

    $ ./bootstrap  # if needed
    $ ./configure
    $ make
    $ sudo make install



## Example

    $ ktserver -plsv /usr/local/libexec/libktmsgpack.so -plex 'port=18801'



## Procedures

### ping
Do nothing, just for testing.

#### parameters
none

#### return
true on success


### echo
Echo back the input data as the output data, just for testing.

#### parameters
- inmap: arbitrary parameters.(optional) 

#### return
corresponding parameters to the input data.


### report
Get the report of the server information.

#### parameters
none

#### return
arbitrary records.


### status
Get the miscellaneous status information of a database.

#### parameters
- DB: the database identifier. (optional)

#### return
Returns the type of map data that contains information status.
Contains the following data.

- count: the number of records.
- size: the size of the database file.
- (optional): arbitrary records for other information.


### add
Add a record.

#### parameters
- key: the key of the record.
- value: the value of the record.
- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
Return `nil` of msgpack.

#### error
The following error codes returned in the response.
- 33: existing record was detected.
- 34: not exist a database.


### set
Set the value of a record.

#### parameters
- key: the key of the record.
- value: the value of the record.
- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
Return `nil` of msgpack.

#### error
The following error codes returned in the response.
- 34: not exist a database.

### get
Retrieve the value of a record.

#### parameters
- key: the key of the record.
- DB: the database identifier. (optional)

#### return
Returns the type map that contains the following keys.
- value: the value of the record. 
- xt: the absolute expiration time. If it is omitted, there is no expiration time.

#### error
The following error codes returned in the response.
- 34: no exist a database.
- 35: no record was found.


## Error codes
- 32: unexpected error.
- 33: existing record was detected.
- 34: no exist a database.
- 35: no record was found.



## License

    Copyright (C) 2010 FURUHASHI Sadayuki
    
       Licensed under the Apache License, Version 2.0 (the "License");
       you may not use this file except in compliance with the License.
       You may obtain a copy of the License at
    
           http://www.apache.org/licenses/LICENSE-2.0
    
       Unless required by applicable law or agreed to in writing, software
       distributed under the License is distributed on an "AS IS" BASIS,
       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
       See the License for the specific language governing permissions and
       limitations under the License.

See also NOTICE file.

Note that Kyoto Tycoon is licensed under GPL. On the other hand, a commercial
license is also provided.

If you link kt-msgpack with GPL-licensed software, the whole software will
be licensed under GPL.

