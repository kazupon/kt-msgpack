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

### IDL

    service KyotoTycoonService {
        void ping()
        map<string, string> echo(1:optional map<string, string> inmap)
        map<string, string> report()
        map<string, string> status(1:optional map<string, string> inmap)
        void add(1:string key, 2:string value, 3:optional map<string, string> inmap)
        void set(1:string key, 2:string value, 3:optional map<string, string> inmap)
        map<string, string> get(1:string key, 2:optional map<string, string> inmap)
        void remove(1:string key, 2:optional map<string, string> inmap)
        void append(1:string key, 2:string value, 3:optional map<string, string> inmap)
        map<string, string> seize(1:string key, 2:optional map<string, string> inmap)
        void clear(1:optional map<string, string> inmap)
        void replace(1:string key, 2:string value, 3:optional map<string, string> inmap)
        void cas(1:string key, 3:optional map<string, string> inmap)
    }


### ping
    void ping()

Do nothing, just for testing.
This procedure is an alias of HTTP protocol `/rpc/void`.

#### parameters
none.

#### return
none.


### echo
    map<string, string> echo(1:optional map<string, string> inmap)

Echo back the input data as the output data, just for testing.

#### parameters
if necessary, specify the arbitrary keys in `inmap` parameter.

- inmap: arbitrary keys. (optional)

#### return
corresponding parameters to the input data.


### report
    map<string, string> report()

Get the report of the database server information.

#### parameters
none.

#### return
report of the database server information.


### status
    map<string, string> status(1:optional map<string, string> inmap)

Get the miscellaneous status information of a database.

#### parameters
specify the following keys in `inmap`.

- DB: the database identifier. (optional)

#### return
returns the map data that contains the following keys.

- count: the number of records.
- size: the size of the database file.
- (optional): arbitrary records for other information.

#### error
the following error codes returned in the response.

- 34: not exist a database.


### add
    void add(1:string key, 2:string value, 3:optional map<string, string> inmap)

Add a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- value: the value of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 33: existing record was detected.
- 34: not exist a database.
- 36: invalid parameters.


### set
    void set(1:string key, 2:string value, 3:optional map<string, string> inmap)

Set the value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- value: the value of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.


### get
    map<string, string> get(1:string key, 2:optional map<string, string> inmap)

Retrieve the value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)

#### return
returns the map data that contains the following keys.

- value: the value of the record. 
- xt: the absolute expiration time. If it is omitted, there is no expiration time.

#### error
the following error codes returned in the response.

- 34: no exist a database.
- 35: no record was found.
- 36: invalid parameters.


### remove
    void remove(1:string key, 2:optional map<string, string> inmap)

Remove a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: no exist a database.
- 35: no record was found.
- 36: invalid parameters.


### append
    void append(1:string key, 2:string value, 3:optional map<string, string> inmap)

Append the value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- value: the value of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.


### seize
    map<string, string> seize(1:string key, 2:optional map<string, string> inmap)

Retrieve the value of a record and remove it atomically.

#### parameters
specify the following parameters.

- key: the key of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)

#### return
returns the map data that contains the following keys.

- value: the value of the record. 
- xt: the absolute expiration time. If it is omitted, there is no expiration time.

#### error
the following error codes returned in the response.

- 34: no exist a database.
- 35: no record was found.
- 36: invalid parameters.


### clear
    void clear(1:optional map<string, string> inmap)

Remove all records in a database.

#### parameters
specify the following keys in `inmap`.

- DB: the database identifier. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.


### replace
    void replace(1:string key, 2:string value, 3:optional map<string, string> inmap)

Replace the value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- value: the value of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.


### cas
    void cas(1:string key, 3:optional map<string, string> inmap)

Perform compare-and-swap.

#### parameters
specify the following parameters.

- key: the key of the record. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- oval: the old value. If it is omittted, no record is meant. (optional)
- nval: the new value. If it is omittted, the record is removed. (optional)
- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.
- 37: the old value assumption was failed.


## Error codes

- 32: unexpected error.
- 33: existing record was detected.
- 34: no exist a database.
- 35: no record was found.
- 36: invalid parameters.
- 37: the old value assumption was failed.



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

