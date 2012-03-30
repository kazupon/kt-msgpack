
const int64 C_INT64_MAX = 9223372036854775807

service KyotoTycoonService {
    bool ping()
    map<bytes, bytes> echo(1:optional map<bytes, bytes> inmap)
    map<bytes, bytes> report()
    map<bytes, bytes> status(1:optional bytes DB = "")
}
