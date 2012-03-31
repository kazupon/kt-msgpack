#include <ktplugserv.h>
#include "gen/server.hpp"
#include <stdarg.h>
#include <sys/time.h>

#define MODNAME "msgpack-rpc"
#define LOG_PREFIX MODNAME ": "

#define ERROR_CODE_OFFSET 32
#define ERR_UNEXPECTED_ERROR    (0 + ERROR_CODE_OFFSET)
#define ERR_EXSISTING_RECORD    (1 + ERROR_CODE_OFFSET)
#define ERR_NOT_FOUND_DATABASE  (2 + ERROR_CODE_OFFSET)
#define ERR_NO_RECORD           (3 + ERROR_CODE_OFFSET)


namespace {


namespace kc = kyotocabinet;
namespace kt = kyototycoon;

namespace rpc = msgpack::rpc;

//class MessagePackServer : public kt::PluggableServer, public KyotoTyrantService::server {
class MessagePackServer : public kt::PluggableServer, public KyotoTycoonService::server {
public:
  MessagePackServer() { }
  ~MessagePackServer() { }

private:
  typedef kt::ThreadedServer::Logger Logger;

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
    log(m_logger, Logger::DEBUG, LOG_PREFIX "  host=%s", m_cfg.host.c_str());
    log(m_logger, Logger::DEBUG, LOG_PREFIX "  port=%d", m_cfg.port);
    log(m_logger, Logger::DEBUG, LOG_PREFIX "  thread=%d", m_cfg.thread);
    log(m_logger, Logger::DEBUG, LOG_PREFIX "  mhost=%s", m_cfg.master_host.c_str());
    log(m_logger, Logger::DEBUG, LOG_PREFIX "  mport=%d", m_cfg.master_port);

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

  void echo(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref, msgpack::type::raw_ref> > req, KyotoTycoonService::echo& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " echo");

    std::map<msgpack::type::raw_ref, msgpack::type::raw_ref> refoutmap;
    for (std::map<msgpack::type::raw_ref, msgpack::type::raw_ref>::const_iterator it(params.inmap.begin()), it_end(params.inmap.end()); it != it_end; ++it) {
      msgpack::type::raw_ref key(it->first.ptr, it->first.size);
      msgpack::type::raw_ref value(it->second.ptr, it->second.size);
      refoutmap.insert(std::make_pair(key, value));
    }

    req.result(refoutmap);
  }

  void report(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > req, KyotoTycoonService::report& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " report");

    std::map<msgpack::type::raw_ref, msgpack::type::raw_ref> refreport;
    double tout = 0;
    kt::RemoteDB db;
    if (!db.open(m_cfg.master_host.c_str(), m_cfg.master_port, tout)) {
      req.error(1); // TODO: error 
      return;
    }
    bool err = false;
    std::map<std::string, std::string> status;
    if (db.report(&status)) {
      std::map<std::string, std::string>::iterator it = status.begin();
      std::map<std::string, std::string>::iterator itend = status.end();
      while (it != itend) {
        msgpack::type::raw_ref key(it->first.c_str(), it->first.size());
        msgpack::type::raw_ref value(it->second.c_str(), it->second.size());
        refreport.insert(std::make_pair(key, value));
        ++it;
      }
    } else {
      req.error(2); // TODO: error
      err = true;
    }
    if (!db.close()) {
      req.error(3); // TODO: error
      err = true;
    }
    req.result(refreport);
  }

  void status(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > req, KyotoTycoonService::status& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " status");

    std::map<msgpack::type::raw_ref, msgpack::type::raw_ref> refstatus;

    kt::TimedDB* db = NULL;
    uint32_t db_name_size;
    const char* db_name = get_c_str_from_map(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    std::map<std::string, std::string> status;
    if (db->status(&status)) {
      for (std::map<std::string, std::string>::const_iterator it(status.begin()), it_end(status.end()); it != it_end; ++it) {
        msgpack::type::raw_ref key(it->first.c_str(), it->first.size());
        msgpack::type::raw_ref value(it->second.c_str(), it->second.size());
        refstatus.insert(std::make_pair(key, value));
      }
    } else {
      const kc::BasicDB::Error& e = db->error();
      log(m_logger, Logger::ERROR, LOG_PREFIX " status procedure error: %d: %s: %s", e.code(), e.name(), e.message());
      req.error(ERR_UNEXPECTED_ERROR);
      return;
    }

    req.result(refstatus);
  }

  void add(msgpack::rpc::request::type<void> req, KyotoTycoonService::add& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " add");

    kt::TimedDB* db = NULL;
    uint32_t db_name_size;
    const char* db_name = get_c_str_from_map(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    uint32_t s_xt_size;
    const char* s_xt = get_c_str_from_map(params.inmap, "xt", &s_xt_size);
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
		bool success = db->add(params.key.ptr, params.key.size, params.value.ptr, params.value.size, xt);
    if (success) {
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
    uint32_t db_name_size;
    const char* db_name = get_c_str_from_map(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    uint32_t s_xt_size;
    const char* s_xt = get_c_str_from_map(params.inmap, "xt", &s_xt_size);
    int64_t xt = s_xt ? kc::atoi(s_xt) : kc::INT64MAX;
    bool success = db->set(params.key.ptr, params.key.size, params.value.ptr, params.value.size, xt);
    if (!success) {
      const kc::BasicDB::Error& e = db->error();
      if (e) {
        log(m_logger, Logger::ERROR, LOG_PREFIX " set procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
    }

    req.result();
  }

  void get(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > req, KyotoTycoonService::get& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " get");

    kt::TimedDB* db = NULL;
    uint32_t db_name_size;
    const char* db_name = get_c_str_from_map(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

    std::map<msgpack::type::raw_ref, msgpack::type::raw_ref> refoutmap;
		size_t vsiz;
    int64_t xt;
		char* vbuf = db->get(params.key.ptr, params.key.size, &vsiz, &xt);
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
      insert_to_map(refoutmap, "value", "%s", vbuf);
      if (xt < kt::TimedDB::XTMAX) {
        insert_to_map(refoutmap, "xt", "%lld", (long long)xt);
      }
			req.zone()->push_finalizer(&::free, vbuf);
			req.result(refoutmap);
		}
  }

  void remove(msgpack::rpc::request::type<void> req, KyotoTycoonService::remove& params) {
		log(m_logger, Logger::INFO, LOG_PREFIX " remove");

    kt::TimedDB* db = NULL;
    uint32_t db_name_size;
    const char* db_name = get_c_str_from_map(params.inmap, "DB", &db_name_size);
    if (db_name != NULL) {
      db = get_db(std::string(db_name, db_name_size));
    } else {
      db = get_db();
    }
    if (db == NULL) {
      req.error(ERR_NOT_FOUND_DATABASE);
      return;
    }

		if (!db->remove(params.key.ptr, params.key.size)) {
      const kc::BasicDB::Error& e = db->error();
      if (e == kc::BasicDB::Error::NOREC) {
        req.error(ERR_NO_RECORD);
      } else {
        log(m_logger, Logger::ERROR, LOG_PREFIX " remove procedure error: %d: %s: %s", e.code(), e.name(), e.message());
        req.error(ERR_UNEXPECTED_ERROR);
      }
		} else {
			req.result();
		}
  }
  /*
	void replace(msgpack::rpc::request::type<bool> req, KyotoTyrantService::replace& params) {
		bool success = get_db()->replace(params.key.ptr, params.key.size,
				params.value.ptr, params.value.size, params.xt);
		req.result(success);
	}

	void append(msgpack::rpc::request::type<void> req, KyotoTyrantService::append& params) {
		bool success = get_db()->append(params.key.ptr, params.key.size,
				params.value.ptr, params.value.size, params.xt);
		if(!success) {
			throw_error("append failed");
		}
		req.result();
	}

	void cas(msgpack::rpc::request::type<bool> req, KyotoTyrantService::cas& params) {
		bool success = get_db()->cas(params.key.ptr, params.key.size,
				params.ovalue.ptr, params.ovalue.size,
				params.nvalue.ptr, params.nvalue.size,
				params.xt);
		req.result(success);
	}

	void remove(msgpack::rpc::request::type<bool> req, KyotoTyrantService::remove& params) {
		bool success = get_db()->remove(params.key.ptr, params.key.size);
		req.result(success);
	}

	void get(msgpack::rpc::request::type<msgpack::type::raw_ref> req, KyotoTyrantService::get& params) {
		size_t vsiz;
		char* vbuf = get_db()->get(params.key.ptr, params.key.size, &vsiz);
		if(vbuf == NULL) {
			req.result_nil();
		} else {
			req.zone()->push_finalizer(&::free, vbuf);
			req.result(msgpack::type::raw_ref(vbuf, vsiz));
		}
	}

	void match_prefix(msgpack::rpc::request::type<std::vector<msgpack::type::raw_ref> > req, KyotoTyrantService::match_prefix& params) {
		std::vector<std::string>* strvec = req.zone()->allocate<std::vector<std::string> >();
		std::string prefix(params.prefix.ptr, params.prefix.size);
		if(get_db()->match_prefix(prefix, strvec, params.max) < 0) {
			throw_error("match_prefix failed");
		}

		std::vector<msgpack::type::raw_ref> refvec;
		string_vec_to_raw_ref_vec_nocopy(strvec, &refvec);
		req.result(refvec);
	}

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

  const char* get_c_str_from_map(std::map<msgpack::type::raw_ref, msgpack::type::raw_ref>& map, const char* key, uint32_t *ret_size) {
    std::string str_key(key);
    std::map<msgpack::type::raw_ref, msgpack::type::raw_ref>::const_iterator it = map.find(msgpack::type::raw_ref(str_key.c_str(), str_key.size()));
    if (it == map.end()) {
      if (ret_size) {
        *ret_size = 0;
      }
      return NULL;
    }
    if (ret_size) {
      *ret_size = it->second.size;
    }
    return it->second.ptr;
  }

  void insert_to_map(
      std::map<msgpack::type::raw_ref, msgpack::type::raw_ref>& map, 
      const char* key,
      const char* format, ...) {
    std::string msg;
    std::string str_key(key);
    va_list ap;
    va_start(ap, format);
    kc::vstrprintf(&msg, format, ap);
    va_end(ap);
    msgpack::type::raw_ref refkey(str_key.c_str(), str_key.size());
    msgpack::type::raw_ref refmsg(msg.c_str(), msg.size());
    map.insert(std::make_pair(refkey, refmsg));
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

