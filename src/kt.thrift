
const int64 C_INT64_MAX = 9223372036854775807


service KyotoTycoonService {
    bool ping()
    map<bytes, bytes> echo(1:optional map<bytes, bytes> inmap)
    map<bytes, bytes> report()
    map<bytes, bytes> status(1:optional bytes DB = "")
    void add(1:bytes key, 2:bytes value, 3:optional bytes DB = "", 4:optional int64 xt = C_INT64_MAX)
    void set(1:bytes key, 2:bytes value, 3:optional bytes DB = "", 4:optional int64 xt = C_INT64_MAX)
}
