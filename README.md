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
        map<string, string> increment(1:string key, 2:string num, 3:optional map<string, string> inmap)
        map<string, string> increment_double(1:string key, 2:string num, 3:optional map<string, string> inmap)
        map<string, string> match_prefix(1:string prefix, 2:optional map<string, string> inmap)
        map<string, string> match_regex(1:string regexp, 2:optional map<string, string> inmap)
        map<string, string> set_bulk(1:optional map<string, string> inmap)
        map<string, string> remove_bulk(1:optional map<string, string> inmap)
        map<string, string> get_bulk(1:optional map<string, string> inmap)
        void vacuum (1:optional map<string, string> inmap)
        void synchronize (1:optional map<string, string> inmap)
        map<string, string> play_script(1:string name, 2:optional map<string, string> inmap)
        void tune_replication(1:optional map<string, string> inmap)
        void cur_jump(1:string CUR, 2:optional map<string, string> inmap)
        void cur_jump_back(1:string CUR, 2:optional map<string, string> inmap)
        void cur_step(1:string CUR)
        void cur_step_back(1:string CUR)
        void cur_set_value(1:string CUR, 2:string value, 3:optional map<string, string> inmap)
        void cur_remove(1:string CUR)
        void cur_get_key(1:string CUR, 2:optional map<string, string> inmap)
        void cur_get_value(1:string CUR, 2:optional map<string, string> inmap)
        void cur_get(1:string CUR, 2:optional map<string, string> inmap)
        void cur_seize(1:string CUR, 2:optional map<string, string> inmap)
        void cur_delete(1:string CUR)
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


### report (not implemented)
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


### increment
    map<string, string> increment(1:string key, 2:string num, 3:optional map<string, string> inmap)

Add a number to the numeric integer value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- num: the additional number. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- orig: the origin number. If it is omitted, 0 is specified. "try" means INT64MIN. "set" means INT64MAX. (optional)
- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
returns the map data that contains the following keys.

- num: the result value.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.
- 38: existing record was not compatible.


### increment\_double
    map<string, string> increment_double(1:string key, 2:string num, 3:optional map<string, string> inmap)

Add a number to the numeric double value of a record.

#### parameters
specify the following parameters.

- key: the key of the record. (required)
- num: the additional number. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- orig: the origin number. If it is omitted, 0 is specified. "try" means negative infinity. "set" means positive infinity.
- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)

#### return
returns the map data that contains the following keys.

- num: the result value.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.
- 38: existing record was not compatible.


### match\_prefix
    map<string, string> match_prefix(1:string prefix, 2:optional map<string, string> inmap)

Get keys matching a prefix string.

#### parameters
specify the following parameters.

- prefix: the prefix string. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- max: the maximum number to retrieve. If it is omitted or negative, no limit is specified.
- DB: the database identifier. (optional)

#### return
returns the map data that contains the following keys.

- num: the number of retrieved keys.
- \_xxx: arbitrary keys which trail the character "\_". (optional)

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.


### match\_regex
    map<string, string> match_regex(1:string regexp, 2:optional map<string, string> inmap)

Get keys matching a ragular expression string.

#### parameters
specify the following parameters.

- regexp: the regular expression string. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- max: the maximum number to retrieve. If it is omitted or negative, no limit is specified.
- DB: the database identifier. (optional)

#### return
returns the map data that contains the following keys.

- num: the number of retrieved keys.
- \_xxxx(optional): arbitrary keys which trail the character "\_".

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 36: invalid parameters.


### set\_bulk
    map<string, string> set_bulk(1:optional map<string, string> inmap)

Store records at once.

#### parameters
specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- xt: the expiration time from now in seconds. If it is negative, the absolute value is treated as the epoch time. If it is omitted, no expiration time is specified. (optional)
- atomic: to perform all operations atomically. If it is omitted, non-atomic operations are performed. (optional)
- \_xxx(optional): arbitrary records whose keys trail the character "\_".

#### return
returns the map data that contains the following keys.

- num: the number of stored reocrds.

#### error
the following error codes returned in the response.

- 34: not exist a database.


### remove\_bulk
    map<string, string> remove_bulk(1:optional map<string, string> inmap)

Remove records at once.

#### parameters
specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- atomic: to perform all operations atomically. If it is omitted, non-atomic operations are performed. (optional)
- \_xxx: arbitrary records which trail the character "\_". (optional)

#### return
returns the map data that contains the following keys.

- num: the number of removed reocrds.

#### error
the following error codes returned in the response.

- 34: not exist a database.


### get\_bulk
    map<string, string> get_bulk(1:optional map<string, string> inmap)

Retrieve records at once.

#### parameters
specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- atomic: to perform all operations atomically. If it is omitted, non-atomic operations are performed. (optional)
- \_xxx: arbitrary records whose keys trail the character "\_". (optional)

#### return
returns the map data that contains the following keys.

- num: the number of removed reocrds.
- \_xxx: arbitrary records whose keys trail the character "\_". (optional)

#### error
the following error codes returned in the response.

- 34: not exist a database.


### vacuum
    void vacuum (1:optional map<string, string> inmap)

Scan the database and eliminate regions of expired records.

#### parameters
specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- step: the number of steps. If it is omitted or not more than 0, the whole region is scanned. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.


### synchronize
    void synchronize (1:optional map<string, string> inmap)

Synchronize updated contents with the file and the device.

#### parameters
specify the following keys in `inmap` parameter.

- DB: the database identifier. (optional)
- hard: for physical synchronization with the device. (optional)
- command: the command name to process the database file. (optional)

#### return
none.

#### error
the following error codes returned in the response.

- 34: not exist a database.
- 40: failed postprocessing command.


### play\_script (not implemented)
    map<string, string> play_script(1:string name, 2:optional map<string, string> inmap);

Call a procedure of the script language extension.

#### parameters
specify the following parameters.

- name: the name of the procedure to call. (required)

also, if necessary, specify the following keys in `inmap` parameter.

- \_xxx: arbitrary key/value pairs whose keys trail the character "\_". (optional)

#### return
returns the map data that contains the following keys.

- \_xxx: arbitrary key/value pairs which keys trail the character "\_". (optional)

#### error
the following error codes returned in the response.

- 41: arbitrary logical error.


### tune\_replication (not implemented)
    void tune_replication(1:optional map<string, string> inmap)


### cur\_jump (not implemented)
    void cur_jump(1:string CUR, 2:optional map<string, string> inmap)


### cur\_jump\_back (not implemented)
    void cur_jump_back(1:string CUR, 2:optional map<string, string> inmap)


### cur\_step (not implemented)
    void cur_step(1:string CUR)


### cur\_step\_back (not implemented)
    void cur_step_back(1:string CUR)


### cur\_set\_value (not implemented)
    void cur_set_value(1:string CUR, 2:string value, 3:optional map<string, string> inmap)


### cur\_remove (not implemented)
    void cur_remove(1:string CUR)


### cur\_get\_key (not implemented)
    void cur_get_key(1:string CUR, 2:optional map<string, string> inmap)


### cur\_get\_value (not implemented)
    void cur_get_value(1:string CUR, 2:optional map<string, string> inmap)


### cur\_get (not implemented)
    void cur_get(1:string CUR, 2:optional map<string, string> inmap)


### cur\_seize (not implemented)
    void cur_seize(1:string CUR, 2:optional map<string, string> inmap)


### cur\_delete (not implemented)
    void cur_delete(1:string CUR)



## Error codes

- 32: unexpected error.
- 33: existing record was detected.
- 34: no exist a database.
- 35: no record was found.
- 36: invalid parameters.
- 37: old value assumption was failed.
- 38: existing record was not compatible.
- 39: invalid cursor.
- 40: failed postprocessing command.
- 41: arbitrary logical error.
- 42: not implemented.



## Testing

1. Invoke the KyotoTycoon with the following.

    $ ktserver -cmd ./test -plsv /opt/local/libexec/libktmsgpack.dylib -plex 'port=18801#cmd=./test' casket1.kct casket2.kct casket3.kch

2. Run the tests.

    $ python test/test-kt-msgpack.py


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

