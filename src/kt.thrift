// TODO: should be change the newest msgpack-idl generator

const int64 C_INT64_MAX = 9223372036854775807

service KyotoTycoonService {
    void ping()
    map<bytes, bytes> echo(1:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
    map<bytes, bytes> report()
    map<bytes, bytes> status(1:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
    void add(1:bytes key, 2:bytes value, 3:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
    void set(1:bytes key, 2:bytes value, 3:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
    map<bytes, bytes> get(1:bytes key, 2:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
    void remove(1:bytes key, 2:optional map<bytes, bytes> inmap = "") // HACK: map optional generating
}
