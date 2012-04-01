#include <ktplugserv.h>
#include "gen/server.hpp"
#include <stdarg.h>
#include <sys/time.h>

#define MODNAME "msgpack-rpc"
#define LOG_PREFIX MODNAME ": "

#define ERROR_CODE_OFFSET 32
#define ERR_UNEXPECTED_ERROR                  (0 + ERROR_CODE_OFFSET)
#define ERR_EXSISTING_RECORD                  (1 + ERROR_CODE_OFFSET)
#define ERR_NOT_FOUND_DATABASE                (2 + ERROR_CODE_OFFSET)
#define ERR_NO_RECORD                         (3 + ERROR_CODE_OFFSET)
#define ERR_INVALID_ARGUMENT                  (4 + ERROR_CODE_OFFSET)
#define ERR_OLD_VALUE_ASSUMPTION              (5 + ERROR_CODE_OFFSET)
#define ERR_EXSISTING_RECORD_NOT_COMPATIBLE   (6 + ERROR_CODE_OFFSET)


namespace {


namespace kc = kyotocabinet;
namespace kt = kyototycoon;

namespace rpc = msgpack::rpc;

class MessagePackServer : public kt::PluggableServer, public KyotoTycoonService::server {
public:
  MessagePackServer() { }
  ~MessagePackServer() { }

private:
  typedef kt::ThreadedServer::Logger Logger;
  typedef std::map<std::string, std::string> map_t;

private:
  static void log(Logger* logger, Logger::Kind kind, const char* format, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, format);
    int len = vsnprintf(buf, sizeof(buf)-1, format, ap);
    va_end(ap);
    buf[len] = '\0';
    logger->log(kind, buf);
  }

public:
  void configure(kt::TimedDB* dbary, size_t dbnum,
      Logger* logger, uint32_t logkinds,
      const char* expr) {
    m_cfg.parse(logger, expr);
    m_dbary = dbary;
    m_dbnum = dbnum;

    m_logger = logger;
    log(m_logger, Logger::SYSTEM, LOG_PREFIX " dbnum: %d", m_dbnum);
    log(m_logger, Logger::SYSTEM, LOG_PREFIX " configured expr:%s", expr);
    log(m_logger, Logger::DEBUG, LOG_PREFIX " host=%s", m_cfg.host.c_str());
    log(m_logger, Logger::DEBUG, LOG_PREFIX " port=%d", m_cfg.port);
    log(m_logger, Logger::DEBUG, LOG_PREFIX " thread=%d", m_cfg.thread);
    log(m_logger, Logger::DEBUG, LOG_PREFIX " mhost=%s", m_cfg.master_host.c_str());
    log(m_logger, Logger::DEBUG, LOG_PREFIX " mport=%d", m_cfg.master_port);

    for (int32_t i = 0; i < m_dbnum; i++) {
      std::string path = m_dbary[i].path();
      const char* rp = path.c_str();
      const char* pv = std::strrchr(rp, kc::File::PATHCHR);
      if (pv) {
        rp = pv + 1;
      }
      m_dbmap[std::string(rp)] = i;
    }
  }

  bool start() {
    try {
      gettimeofday(&m_start_time, NULL);
      instance.listen(m_cfg.host, m_cfg.port);
      instance.get_loop()->start(m_cfg.thread);
      return true;
    } catch (std::exception& e) {
      log(m_logger, Logger::ERROR, LOG_PREFIX "listen failed: %s", e.what());
      return false;
    }
  }

  bool stop() {
    instance.get_loop()->end();
    instance.close();
    return true;
  }

  bool finish() {
    instance.get_loop()->join();
    return true;
  }

private:
  class Config {
  public:
    Config() :
      host("0.0.0.0"),
      port(0),
      thread(8),
      master_host("127.0.0.1"),
      master_port(kt::DEFPORT) {
    }

  private:
    void option(Logger* logger,
        const std::string& key, const std::string& value) {
      if(key == "host") {
        host = value;
      } else if(key == "port") {
        port = kc::atoi(value.c_str());
      } else if(key == "thread") {
        thread = kc::atoi(value.c_str());
      } else if (key == "mhost") {
        master_host = value;
      } else if (key == "mport") {
        master_host = kc::atoi(value.c_str());
      } else {
        log(logger, Logger::SYSTEM,
            LOG_PREFIX "unknown option: %s", key.c_str());
      }
    }

  public:
    void parse(Logger* logger, const char* expr) {
      std::vector<std::string> elems;
      kc::strsplit(expr, '#', &elems);
      for(std::vector<std::string>::const_iterator it(elems.begin()),
          it_end(elems.end()); it != it_end; ++it) {
        size_t poseq = it->find('=');
        if(poseq != std::string::npos) {
          option(logger, it->substr(0,poseq), it->substr(poseq+1));
        } else {
          option(logger, *it, "");
        }
      }
    }

  public:
    std::string host;
    uint16_t port;
    uint16_t thread;
    std::string master_host;
    uint16_t master_port;
  private:
    Logger* m_logger;
	};

  Config m_cfg;
  Logger* m_logger;
  kt::TimedDB* m_dbary;
  size_t m_dbnum;
  std::map<std::string, int32_t> m_dbmap;

  struct timeval m_start_time;

private:
  void ping(msgpack::rpc::request::type<void> req, KyotoTycoonService::ping& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " ping");

    req.result();
  }

  void echo(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::echo& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " echo");

    map_t outmap;
    for (map_t::const_iterator it(params.inmap.begin()), it_end(params.inmap.end()); it != it_end; ++it) {
      outmap.insert(std::make_pair(std::string(it->first), std::string(it->second)));
    }

    req.result(outmap);
  }

  void report(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::report& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " report");

    double tout = 0;
    kt::RemoteDB db;
    if (!db.open(m_cfg.master_host.c_str(), m_cfg.master_port, tout)) {
      kt::RemoteDB::Error e = db.error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " report db open error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    bool err = false;
    map_t status;
    if (!db.report(&status)) {
      kt::RemoteDB::Error e = db.error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " report db report error: %d: %s: %s", e.code(), e.name(), e.message());
      err = true;
    }

    if (!db.close()) {
      kt::RemoteDB::Error e = db.error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " report db close error: %d: %s: %s", e.code(), e.name(), e.message());
      err = true;
    }

    if (err) {
      req.error(ERR_UNEXPECTED_ERROR);
    } else {
      req.result(status);
    }
  }

  void status(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::status& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " status");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    map_t status;
    if (!db->status(&status)) {
      const kc::BasicDB::Error& e = db->error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " status procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result(status);
  }

  void add(msgpack::rpc::request::type<void> req, KyotoTycoonService::add& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " add");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    if (db->add(params.key.c_str(), params.key.size(), params.value.c_str(), params.value.size(), xt)) {
      req.result();
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::DUPREC) {
        req.error(ERR_EXSISTING_RECORD);
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " add procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
    }
  }

  void set(msgpack::rpc::request::type<void> req, KyotoTycoonService::set& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " set");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    if (!db->set(params.key.c_str(), params.key.size(), params.value.c_str(), params.value.size(), xt)) {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(m_logger, Logger::ERROR, LOG_PREFIX " set procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
    }

    req.result();
  }

  void get(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::get& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " get");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

		size_t vsiz;
    int64_t xt;
		char* vbuf = db->get(params.key.c_str(), params.key.size(), &vsiz, &xt);
		if (vbuf == NULL) {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::NOREC) {
        req.error(ERR_NO_RECORD);
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " get procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
		} else {
      log(m_logger, Logger::DEBUG, LOG_PREFIX " get: value = %s, xt = %lld", vbuf, xt);
      map_t outmap;
      insert_to_map(outmap, "value", "%s", vbuf);
      if (xt < kt::TimedDB::XTMAX) {
        insert_to_map(outmap, "xt", "%lld", (long long)xt);
      }
			req.zone()->push_finalizer(&::free, vbuf);
			req.result(outmap);
		}
  }

  void remove(msgpack::rpc::request::type<void> req, KyotoTycoonService::remove& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " remove");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

		if (!db->remove(params.key.c_str(), params.key.size())) {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::NOREC) {
        req.error(ERR_NO_RECORD);
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " remove procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
		} else {
			req.result();
		}
  }

  void append(msgpack::rpc::request::type<void> req, KyotoTycoonService::append& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " append");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    if (!db->append(params.key.c_str(), params.key.size(), params.value.c_str(), params.value.size(), xt)) {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(m_logger, Logger::ERROR, LOG_PREFIX " append procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void seize(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::seize& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " seize");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

		size_t vsiz;
    int64_t xt;
		char* vbuf = db->seize(params.key.c_str(), params.key.size(), &vsiz, &xt);
		if (vbuf == NULL) {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::NOREC) {
        req.error(ERR_NO_RECORD);
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " seize procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
		} else {
      log(m_logger, Logger::DEBUG, LOG_PREFIX " seize: value = %s, xt = %lld", vbuf, xt);
      map_t outmap;
      insert_to_map(outmap, "value", "%s", vbuf);
      if (xt < kt::TimedDB::XTMAX) {
        insert_to_map(outmap, "xt", "%lld", (long long)xt);
      }
			req.zone()->push_finalizer(&::free, vbuf);
			req.result(outmap);
		}
  }

  void clear(msgpack::rpc::request::type<void> req, KyotoTycoonService::clear& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " clear");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    if (!db->clear()) {
      const kc::BasicDB::Error& e = db->error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " clear procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result();
  }
  
  void replace(msgpack::rpc::request::type<void> req, KyotoTycoonService::replace& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " replace");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    if (!db->replace(params.key.c_str(), params.key.size(), params.value.c_str(), params.value.size(), xt)) {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(m_logger, Logger::ERROR, LOG_PREFIX " replace procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void cas(msgpack::rpc::request::type<void> req, KyotoTycoonService::cas& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " cas");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    size_t ovsiz;
    const char* ovbuf = kt::strmapget(params.inmap, "oval", &ovsiz);
    size_t nvsiz;
    const char* nvbuf = kt::strmapget(params.inmap, "nval", &nvsiz);
    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    if (!db->cas(params.key.c_str(), params.key.size(), ovbuf, ovsiz, nvbuf, nvsiz, xt)) {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::LOGIC) {
        req.error(ERR_OLD_VALUE_ASSUMPTION);
        return;
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " cas procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void increment(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " increment");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    int64_t num = kc::atoi(params.num.c_str());

    const char* orig_ptr = kt::strmapget(params.inmap, "orig");
    int64_t orig;
    if (orig_ptr) {
      if (!std::strcmp(orig_ptr, "try")) {
        orig = kc::INT64MIN;
      } else if (!std::strcmp(orig_ptr, "set")) {
        orig = kc::INT64MAX;
      } else {
        orig = kc::atoi(orig_ptr);
      }
    } else {
      orig = 0;
    }
    log(m_logger, Logger::DEBUG, LOG_PREFIX " increment: orig = %lld", (long long)orig);

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;

    num = db->increment(params.key.c_str(), params.key.size(), num, orig, xt);
    log(m_logger, Logger::DEBUG, LOG_PREFIX " increment: num = %lld", (long long)num);
    if (num != kc::INT64MIN) {
      map_t outmap;
      insert_to_map(outmap, "num", "%lld", (long long)num);
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::LOGIC) {
        req.error(ERR_EXSISTING_RECORD_NOT_COMPATIBLE);
        return;
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " increment procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void increment_double(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment_double& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " increment_double");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    double num = kc::atof(params.num.c_str());

    const char* orig_ptr = kt::strmapget(params.inmap, "orig");
    double orig;
    if (orig_ptr) {
      if (!std::strcmp(orig_ptr, "try")) {
        orig = -kc::inf();
      } else if (!std::strcmp(orig_ptr, "set")) {
        orig = kc::inf();
      } else {
        orig = kc::atof(orig_ptr);
      }
    } else {
      orig = 0;
    }
    log(m_logger, Logger::DEBUG, LOG_PREFIX " increment_double: orig = %f", orig);

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;

    num = db->increment_double(params.key.c_str(), params.key.size(), num, orig, xt);
    log(m_logger, Logger::DEBUG, LOG_PREFIX " increment_double: num = %f", num);
    if (!kc::chknan(num)) {
      map_t outmap;
      insert_to_map(outmap, "num", "%f", num);
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::LOGIC) {
        req.error(ERR_EXSISTING_RECORD_NOT_COMPATIBLE);
        return;
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " increment_double procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void match_prefix(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::match_prefix& params) {
    log(m_logger, Logger::INFO, LOG_PREFIX " match_prefix");

    kt::TimedDB* db = NULL;
    size_t db_name_size;
    const char* db_name = kt::strmapget(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    const char* max_ptr = kt::strmapget(params.inmap, "max");
    int64_t max = max_ptr ? kc::atoi(max_ptr) : -1;

    std::vector<std::string> keys;
    int64_t num = db->match_prefix(params.prefix, &keys, max);
    if (num >= 0) {
      map_t outmap;
      insert_to_map(outmap, "num", "%lld", (long long)num);
      std::vector<std::string>::iterator it = keys.begin();
      std::vector<std::string>::iterator itend = keys.end();
      while (it != itend) {
        std::string key = "_";
        key.append(*it);
        outmap[key] = *it;
        ++it;
      }
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(m_logger, Logger::ERROR, LOG_PREFIX " match_prefix procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }
  /*
	void match_regex(msgpack::rpc::request::type<std::vector<msgpack::type::raw_ref> > req, KyotoTyrantService::match_regex& params) {
		std::vector<std::string>* strvec = req.zone()->allocate<std::vector<std::string> >();
		std::string regex(params.regex.ptr, params.regex.size);
		if(get_db()->match_regex(regex, strvec, params.max) < 0) {
			throw_error("match_regex failed");
		}

		std::vector<msgpack::type::raw_ref> refvec;
		string_vec_to_raw_ref_vec_nocopy(strvec, &refvec);
		req.result(refvec);
	}

	void uptime(msgpack::rpc::request::type<double> req, KyotoTyrantService::uptime& params) {
		struct timeval now;
		gettimeofday(&now, NULL);
		struct timeval t;
		timersub(&now, &m_start_time, &t);
		double sec = t.tv_sec + (double)t.tv_usec*1e-6;
		req.result(sec);
	}
  */


private:
  kt::TimedDB* get_db() {
    return &m_dbary[0];
  }

  kt::TimedDB* get_db(const std::string& name) {
    int32_t index = -1;
    std::map<std::string, int32_t>::const_iterator it = m_dbmap.find(name);
    if (it != m_dbmap.end()) {
      index = it->second;
    }
    return (index != -1) ? &m_dbary[index] : NULL;
  }

  const char* get_c_str_from_map(std::map<std::string, std::string>& map, const char* key, uint32_t *ret_size) {
    std::string str_key(key);
    std::map<std::string, std::string>::const_iterator it = map.find(str_key);
    if (it == map.end()) {
      if (ret_size) {
        *ret_size = 0;
      }
      return NULL;
    }
    if (ret_size) {
      *ret_size = it->second.size();
    }
    return it->second.c_str();
  }

  void insert_to_map(map_t& map, const char* key, const char* format, ...) {
    std::string msg;
    va_list ap;
    va_start(ap, format);
    kc::vstrprintf(&msg, format, ap);
    va_end(ap);
    map.insert(std::make_pair(std::string(key), msg));
  }

  void throw_error(const std::string& msg) {
    throw_error(msg, get_db()->error());
  }

  void throw_error(const std::string& msg, const kc::BasicDB::Error& err) {
    std::ostringstream s;
    s << msg;
    s << ": " << err.message();
    s << " (" << kc::BasicDB::Error::codename(err.code()) << ")";
    throw std::runtime_error(s.str());
  }

  void string_vec_to_raw_ref_vec_nocopy(
      const std::vector<std::string>* src,
      std::vector<msgpack::type::raw_ref>* dst) {
    for(std::vector<std::string>::const_iterator it(src->begin()),
        it_end(src->end()); it != it_end; ++it) {
      dst->push_back(msgpack::type::raw_ref(it->data(), it->size()));
    }
  }

private:
  MessagePackServer(const MessagePackServer&);
};

}  // noname namespace


extern "C"
void* ktservinit(void) {
  return new MessagePackServer();
}

