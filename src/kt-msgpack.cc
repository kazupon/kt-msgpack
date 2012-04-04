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
#define ERR_INVALIDE_CURSOR                   (7 + ERROR_CODE_OFFSET)
#define ERR_FAILED_POST_PROCESSING_COMMAND    (8 + ERROR_CODE_OFFSET)
#define ERR_ARBITRARY_LOGICAL_ERROR           (9 + ERROR_CODE_OFFSET)
#define ERR_NOT_IMPLEMENTED                   (10 + ERROR_CODE_OFFSET)


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
  void log(Logger::Kind kind, const char* format, ...) {
    if (!(kind & m_kind)) {
      return;
    }
    std::string msg;
    va_list ap;
    va_start(ap, format);
    kc::vstrprintf(&msg, format, ap);
    va_end(ap);
    m_logger->log(kind, msg.c_str());
  }

public:
  void configure(kt::TimedDB* dbary, size_t dbnum,
      Logger* logger, uint32_t logkinds,
      const char* expr) {
    m_cfg.parse(logger, expr);
    m_dbary = dbary;
    m_dbnum = dbnum;

    m_logger = logger;
    m_kind = (Logger::Kind)logkinds;

    log(Logger::SYSTEM, LOG_PREFIX " dbnum: %d", m_dbnum);
    log(Logger::SYSTEM, LOG_PREFIX " configured expr:%s", expr);
    log(Logger::DEBUG, LOG_PREFIX " host=%s", m_cfg.host.c_str());
    log(Logger::DEBUG, LOG_PREFIX " port=%d", m_cfg.port);
    log(Logger::DEBUG, LOG_PREFIX " thread=%d", m_cfg.thread);
    log(Logger::DEBUG, LOG_PREFIX " mhost=%s", m_cfg.master_host.c_str());
    log(Logger::DEBUG, LOG_PREFIX " mport=%d", m_cfg.master_port);
    log(Logger::DEBUG, LOG_PREFIX " cmd=%s", m_cfg.cmd.c_str());

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
      log(Logger::ERROR, LOG_PREFIX "listen failed: %s", e.what());
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
      master_port(kt::DEFPORT),
      cmd("./") {
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
      } else if (key == "cmd") {
        cmd = value;
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
    std::string cmd;
  private:
    Logger* m_logger;
	};

  Config m_cfg;
  Logger* m_logger;
  Logger::Kind m_kind;
  kt::TimedDB* m_dbary;
  size_t m_dbnum;
  std::map<std::string, int32_t> m_dbmap;

  struct timeval m_start_time;

private:
  void ping(msgpack::rpc::request::type<void> req, KyotoTycoonService::ping& params) {
		log(Logger::INFO, LOG_PREFIX " ping");

    req.result();
  }

  void echo(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::echo& params) {
		log(Logger::INFO, LOG_PREFIX " echo");

    map_t outmap;
    for (map_t::const_iterator it(params.inmap.begin()), it_end(params.inmap.end()); it != it_end; ++it) {
      outmap.insert(std::make_pair(std::string(it->first), std::string(it->second)));
    }

    req.result(outmap);
  }

  void report(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::report& params) {
		log(Logger::INFO, LOG_PREFIX " report");
    req.error(ERR_NOT_IMPLEMENTED);
    /*
    double tout = 0;
    kt::RemoteDB db;
    if (!db.open(m_cfg.master_host.c_str(), m_cfg.master_port, tout)) {
      kt::RemoteDB::Error e = db.error();
      log(Logger::ERROR, LOG_PREFIX " report db open error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    bool err = false;
    map_t status;
    if (!db.report(&status)) {
      kt::RemoteDB::Error e = db.error();
      log(Logger::ERROR, LOG_PREFIX " report db report error: %d: %s: %s", e.code(), e.name(), e.message());
      err = true;
    }

    if (!db.close()) {
      kt::RemoteDB::Error e = db.error();
      log(Logger::ERROR, LOG_PREFIX " report db close error: %d: %s: %s", e.code(), e.name(), e.message());
      err = true;
    }

    if (err) {
      req.error(ERR_UNEXPECTED_ERROR);
    } else {
      req.result(status);
    }
    */
  }

  void status(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::status& params) {
		log(Logger::INFO, LOG_PREFIX " status");

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
      log(Logger::ERROR, LOG_PREFIX " status procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result(status);
  }

  void add(msgpack::rpc::request::type<void> req, KyotoTycoonService::add& params) {
		log(Logger::INFO, LOG_PREFIX " add");

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
        log(Logger::ERROR, LOG_PREFIX " add procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
    }
  }

  void set(msgpack::rpc::request::type<void> req, KyotoTycoonService::set& params) {
		log(Logger::INFO, LOG_PREFIX " set");

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
        log(Logger::ERROR, LOG_PREFIX " set procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
    }

    req.result();
  }

  void get(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::get& params) {
		log(Logger::INFO, LOG_PREFIX " get");

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
        log(Logger::ERROR, LOG_PREFIX " get procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
		} else {
      //log(Logger::DEBUG, LOG_PREFIX " get: value = %s, xt = %lld", vbuf, xt);
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
		log(Logger::INFO, LOG_PREFIX " remove");

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
        log(Logger::ERROR, LOG_PREFIX " remove procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
		} else {
			req.result();
		}
  }

  void append(msgpack::rpc::request::type<void> req, KyotoTycoonService::append& params) {
		log(Logger::INFO, LOG_PREFIX " append");

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
        log(Logger::ERROR, LOG_PREFIX " append procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void seize(msgpack::rpc::request::type<std::map<std::string, std::string> > req, KyotoTycoonService::seize& params) {
    log(Logger::INFO, LOG_PREFIX " seize");

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
        log(Logger::ERROR, LOG_PREFIX " seize procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
		} else {
      //log(Logger::DEBUG, LOG_PREFIX " seize: value = %s, xt = %lld", vbuf, xt);
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
    log(Logger::INFO, LOG_PREFIX " clear");

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
      log(Logger::ERROR, LOG_PREFIX " clear procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result();
  }
  
  void replace(msgpack::rpc::request::type<void> req, KyotoTycoonService::replace& params) {
    log(Logger::INFO, LOG_PREFIX " replace");

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
        log(Logger::ERROR, LOG_PREFIX " replace procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void cas(msgpack::rpc::request::type<void> req, KyotoTycoonService::cas& params) {
    log(Logger::INFO, LOG_PREFIX " cas");

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
        log(Logger::ERROR, LOG_PREFIX " cas procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }

    req.result();
  }

  void increment(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment& params) {
    log(Logger::INFO, LOG_PREFIX " increment");

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
    //log(Logger::DEBUG, LOG_PREFIX " increment: orig = %lld", (long long)orig);

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;

    num = db->increment(params.key.c_str(), params.key.size(), num, orig, xt);
    //log(Logger::DEBUG, LOG_PREFIX " increment: num = %lld", (long long)num);
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
        log(Logger::ERROR, LOG_PREFIX " increment procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void increment_double(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment_double& params) {
    log(Logger::INFO, LOG_PREFIX " increment_double");

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
    //log(Logger::DEBUG, LOG_PREFIX " increment_double: orig = %f", orig);

    const char* s_xt = kt::strmapget(params.inmap, "xt");
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;

    num = db->increment_double(params.key.c_str(), params.key.size(), num, orig, xt);
    //log(Logger::DEBUG, LOG_PREFIX " increment_double: num = %f", num);
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
        log(Logger::ERROR, LOG_PREFIX " increment_double procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void match_prefix(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::match_prefix& params) {
    log(Logger::INFO, LOG_PREFIX " match_prefix");

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
        log(Logger::ERROR, LOG_PREFIX " match_prefix procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void match_regex(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::match_regex& params) {
    log(Logger::INFO, LOG_PREFIX " match_regex");

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
    int64_t num = db->match_regex(params.regex, &keys, max);
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
        log(Logger::ERROR, LOG_PREFIX " match_regex procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void set_bulk(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::set_bulk& params) {
    log(Logger::INFO, LOG_PREFIX " set_bulk");

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

    const char* xt_ptr = kt::strmapget(params.inmap, "xt");
    int64_t xt = xt_ptr ? kc::atoi(xt_ptr) : kc::INT64MAX;

    const char* atomic_ptr = kt::strmapget(params.inmap, "atomic");
    bool atomic = atomic_ptr ? true : false;
                    
    map_t records;
    map_t::const_iterator it = params.inmap.begin();
    map_t::const_iterator itend = params.inmap.end();
    while (it != itend) {
      const char* kbuf = it->first.data();
      size_t ksiz = it->first.size();
      if (ksiz > 0 && *kbuf == '_') {
        std::string key(kbuf + 1, ksiz - 1);
        std::string value(it->second.data(), it->second.size());
        records[key] = value;
      }
      ++it;
    }

    int64_t num = db->set_bulk(records, xt, atomic);
    if (num >= 0) {
      map_t outmap;
      insert_to_map(outmap, "num", "%lld", (long long)num);
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(Logger::ERROR, LOG_PREFIX " set_bulk procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void remove_bulk(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::remove_bulk& params) {
    log(Logger::INFO, LOG_PREFIX " remove_bulk");

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

    const char* atomic_ptr = kt::strmapget(params.inmap, "atomic");
    bool atomic = atomic_ptr ? true : false;
                    
    std::vector<std::string> keys;
    keys.reserve(params.inmap.size());
    map_t::const_iterator it = params.inmap.begin();
    map_t::const_iterator itend = params.inmap.end();
    while (it != itend) {
      const char* kbuf = it->first.data();
      size_t ksiz = it->first.size();
      if (ksiz > 0 && *kbuf == '_') {
        std::string key(kbuf + 1, ksiz - 1);
        keys.push_back(key);
      }
      ++it;
    }

    int64_t num = db->remove_bulk(keys, atomic);
    if (num >= 0) {
      map_t outmap;
      insert_to_map(outmap, "num", "%lld", (long long)num);
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(Logger::ERROR, LOG_PREFIX " remove_bulk procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void get_bulk(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::get_bulk& params) {
    log(Logger::INFO, LOG_PREFIX " get_bulk");

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

    const char* atomic_ptr = kt::strmapget(params.inmap, "atomic");
    bool atomic = atomic_ptr ? true : false;
                    
    std::vector<std::string> keys;
    keys.reserve(params.inmap.size());
    map_t::const_iterator it = params.inmap.begin();
    map_t::const_iterator itend = params.inmap.end();
    while (it != itend) {
      const char* kbuf = it->first.data();
      size_t ksiz = it->first.size();
      if (ksiz > 0 && *kbuf == '_') {
        std::string key(kbuf + 1, ksiz - 1);
        keys.push_back(key);
      }
      ++it;
    }

    map_t records;
    int64_t num = db->get_bulk(keys, &records, atomic);
    if (num >= 0) {
      map_t outmap;
      insert_to_map(outmap, "num", "%lld", (long long)num);
      map_t::iterator it = records.begin();
      map_t::iterator itend = records.end();
      while (it != itend) {
        std::string key("_");
        key.append(it->first);
        outmap[key] = it->second;
        ++it;
      }
      req.result(outmap);
    } else {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(Logger::ERROR, LOG_PREFIX " get_bulk procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
        return;
      }
    }
  }

  void vacuum(msgpack::rpc::request::type<void> req, KyotoTycoonService::vacuum& params) {
    log(Logger::INFO, LOG_PREFIX " vacuum");

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

    const char* step_ptr = kt::strmapget(params.inmap, "step");
    int64_t step = step_ptr ? kc::atoi(step_ptr) : 0;
                    
    if (!db->vacuum(step)) {
      const kc::BasicDB::Error& e = db->error();
      log(Logger::ERROR, LOG_PREFIX "  vacuum procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result();
  }

  void synchronize(msgpack::rpc::request::type<void> req, KyotoTycoonService::synchronize& params) {
    log(Logger::INFO, LOG_PREFIX " synchronize");

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

    const char* hard_ptr = kt::strmapget(params.inmap, "hard");
    bool hard = hard_ptr ? true : false;

    size_t cmd_size;
    const char* cmd_ptr = kt::strmapget(params.inmap, "command", &cmd_size);
    std::string cmd("");
    if (cmd_ptr != NULL) {
      cmd = std::string(cmd_ptr, cmd_size);
    }

    class Visitor : public kc::BasicDB::FileProcessor {
      public:
        Visitor(Logger* logger, std::string& command_path, const std::string& command) :
          logger_(logger), command_path_(command_path), command_(command) { }
      private:
        bool process(const std::string& path, int64_t count, int64_t size) {
          if (command_.size() < 1) {
            return true;
          }
          const char* cmd = command_.c_str();
          if (std::strchr(cmd, kc::File::PATHCHR) || !std::strcmp(cmd, kc::File::CDIRSTR) ||
              !std::strcmp(cmd, kc::File::PDIRSTR)) {
            log(logger_, Logger::ERROR, LOG_PREFIX "  synchronize invalid command name: %s", cmd);
            return false;
          }
          std::string cmdpath;
          kc::strprintf(&cmdpath, "%s%c%s", command_path_.c_str(), kc::File::PATHCHR, cmd);
          std::vector<std::string> args;
          args.push_back(cmdpath);
          args.push_back(path);
          std::string tsstr;
          uint64_t cc = kt::UpdateLogger::clock_pure();
          kc::strprintf(&tsstr, "%020llu", (unsigned long long)cc);
          args.push_back(tsstr);
          log(logger_, Logger::SYSTEM, LOG_PREFIX "  synchronize executing: %s \"%s\"", cmd, path.c_str());
          if (kt::executecommand(args) != 0) {
            log(logger_, Logger::ERROR, LOG_PREFIX "  synchronize execution failed: %s \"%s\"", cmd, path.c_str());
            return false;
          }
          return true;
        }
        Logger* logger_; 
        std::string command_path_;
        std::string command_;
    };
    Visitor visitor(m_logger, m_cfg.cmd, cmd);

    if (!db->synchronize(hard, &visitor)) {
      const kc::BasicDB::Error& e = db->error();
      log(Logger::ERROR, LOG_PREFIX "  vacuum procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      if (e == kc::BasicDB::Error::LOGIC) {
        req.error(ERR_FAILED_POST_PROCESSING_COMMAND);
      } else {
        req.error(ERR_UNEXPECTED_ERROR);
      }
      return;
    }

    req.result();
  }

  void play_script(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::play_script& params) {
    log(Logger::INFO, LOG_PREFIX " play_script");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void tune_replication(msgpack::rpc::request::type<void> req, KyotoTycoonService::tune_replication& params) {
    log(Logger::INFO, LOG_PREFIX " tune_replication");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_jump(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_jump& params) {
    log(Logger::INFO, LOG_PREFIX " cur_jump");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_jump_back(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_jump_back& params) {
    log(Logger::INFO, LOG_PREFIX " cur_jump_back");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_step(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_step& params) {
    log(Logger::INFO, LOG_PREFIX " cur_step");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_step_back(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_step_back& params) {
    log(Logger::INFO, LOG_PREFIX " cur_step_back");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_set_value(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_set_value& params) {
    log(Logger::INFO, LOG_PREFIX " cur_set_value");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_remove(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_remove& params) {
    log(Logger::INFO, LOG_PREFIX " cur_remove");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_get_key(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_get_key& params) {
    log(Logger::INFO, LOG_PREFIX " cur_get_key");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_get_value(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_get_value& params) {
    log(Logger::INFO, LOG_PREFIX " cur_get_value");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_get(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_get& params) {
    log(Logger::INFO, LOG_PREFIX " cur_get");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_seize(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_seize& params) {
    log(Logger::INFO, LOG_PREFIX " cur_seize");
    req.error(ERR_NOT_IMPLEMENTED);
  }

  void cur_delete(msgpack::rpc::request::type<void> req, KyotoTycoonService::cur_delete& params) {
    log(Logger::INFO, LOG_PREFIX " cur_delete");
    req.error(ERR_NOT_IMPLEMENTED);
  }

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

