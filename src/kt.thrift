// TODO: should be change the newest msgpack-idl generator

const int64 C_INT64_MAX = 9223372036854775807

service KyotoTycoonService {
    void ping()
    map<string, string> echo(1:optional map<string, string> inmap = "") // HACK: map optional generating
    map<string, string> report()
    map<string, string> status(1:optional map<string, string> inmap = "") // HACK: map optional generating
    void add(1:string key, 2:string value, 3:optional map<string, string> inmap = "") // HACK: map optional generating
    void set(1:string key, 2:string value, 3:optional map<string, string> inmap = "") // HACK: map optional generating
    map<string, string> get(1:string key, 2:optional map<string, string> inmap = "") // HACK: map optional generating
    void remove(1:string key, 2:optional map<string, string> inmap = "") // HACK: map optional generating
    void append(1:string key, 2:string value, 3:optional map<string, string> inmap = "") // HACK: map optional generating
    map<string, string> seize(1:string key, 2:optional map<string, string> inmap = "") // HACK: map optional generating
    void clear(1:optional map<string, string> inmap = "") // HACK: map optional generating
    void replace(1:string key, 2:string value, 3:optional map<string, string> inmap = "") // HACK: map optional generating
}
